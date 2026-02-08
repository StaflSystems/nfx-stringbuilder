/*
 * MIT License
 *
 * Copyright (c) 2025 nfx
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/**
 * @file StringBuilder.cpp
 * @brief Implementation of StringBuilder cold-path methods (constructors, assignment, prepend)
 */

#include "nfx/string/StringBuilder.h"

#include <algorithm>
#include <cstring>

namespace nfx::string
{

    //=====================================================================
    // StringBuilder class
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    StringBuilder::StringBuilder()
        : m_heapBuffer{ nullptr },
          m_buffer{ m_stackBuffer },
          m_size{ 0 },
          m_capacity{ STACK_BUFFER_SIZE },
          m_onHeap{ false }
    {
    }

    StringBuilder::StringBuilder( size_t initialCapacity )
        : m_heapBuffer{ nullptr },
          m_buffer{ m_stackBuffer },
          m_size{ 0 },
          m_capacity{ STACK_BUFFER_SIZE },
          m_onHeap{ false }
    {
        if( initialCapacity > STACK_BUFFER_SIZE )
        {
            m_heapBuffer = std::make_unique<char[]>( initialCapacity );
            m_buffer = m_heapBuffer.get();
            m_capacity = initialCapacity;
            m_onHeap = true;
        }
    }

    StringBuilder::StringBuilder( const StringBuilder& other )
        : m_heapBuffer{ nullptr },
          m_buffer{ m_stackBuffer },
          m_size{ other.m_size },
          m_capacity{ other.m_capacity },
          m_onHeap{ other.m_onHeap }
    {
        if( m_onHeap ) [[likely]]
        {
            m_heapBuffer = std::make_unique<char[]>( m_capacity );
            m_buffer = m_heapBuffer.get();
            std::memcpy( m_heapBuffer.get(), other.m_heapBuffer.get(), m_size );
        }
        else
        {
            std::memcpy( m_stackBuffer, other.m_stackBuffer, m_size );
        }
    }

    StringBuilder::StringBuilder( StringBuilder&& other ) noexcept
        : m_heapBuffer{ std::move( other.m_heapBuffer ) },
          m_buffer{ other.m_onHeap ? other.m_buffer : m_stackBuffer },
          m_size{ other.m_size },
          m_capacity{ other.m_capacity },
          m_onHeap{ other.m_onHeap }
    {
        if( !m_onHeap ) [[likely]]
        {
            // Fast move: just copy stack buffer content without clearing source
            std::memcpy( m_stackBuffer, other.m_stackBuffer, m_size );
        }

        // Reset source to empty state
        other.m_buffer = other.m_stackBuffer;
        other.m_size = 0;
        other.m_capacity = STACK_BUFFER_SIZE;
        other.m_onHeap = false;
    }

    //----------------------------------------------
    // Assignment
    //----------------------------------------------

    StringBuilder& StringBuilder::operator=( const StringBuilder& other )
    {
        if( this != &other ) [[likely]]
        {
            if( other.m_onHeap )
            {
                // Other uses heap, we need heap too
                if( !m_onHeap || m_capacity < other.m_capacity )
                {
                    m_heapBuffer = std::make_unique<char[]>( other.m_capacity );
                    m_buffer = m_heapBuffer.get();
                    m_capacity = other.m_capacity;
                    m_onHeap = true;
                }
                std::memcpy( m_heapBuffer.get(), other.m_heapBuffer.get(), other.m_size );
            }
            else
            {
                // Other uses stack, we can use stack too
                if( m_onHeap )
                {
                    m_heapBuffer.reset();
                    m_buffer = m_stackBuffer;
                    m_capacity = STACK_BUFFER_SIZE;
                    m_onHeap = false;
                }
                std::memcpy( m_stackBuffer, other.m_stackBuffer, other.m_size );
            }
            m_size = other.m_size;
        }
        return *this;
    }

    StringBuilder& StringBuilder::operator=( StringBuilder&& other ) noexcept
    {
        if( this != &other ) [[likely]]
        {
            m_heapBuffer = std::move( other.m_heapBuffer );
            m_size = other.m_size;
            m_capacity = other.m_capacity;
            m_onHeap = other.m_onHeap;

            if( !m_onHeap )
            {
                std::memcpy( m_stackBuffer, other.m_stackBuffer, m_size );
                m_buffer = m_stackBuffer;
            }
            else
            {
                m_buffer = m_heapBuffer.get();
            }

            other.m_buffer = other.m_stackBuffer;
            other.m_size = 0;
            other.m_capacity = STACK_BUFFER_SIZE;
            other.m_onHeap = false;
        }
        return *this;
    }

    //----------------------------------------------
    // Prepend operations
    //----------------------------------------------

    StringBuilder& StringBuilder::prepend( std::string_view str )
    {
        if( str.empty() ) [[unlikely]]
        {
            return *this;
        }

        const size_t len = str.size();
        const size_t newSize = m_size + len;

        if( newSize > m_capacity ) [[unlikely]]
        {
            ensureCapacity( newSize );
        }

        char* buf = m_buffer;

        // Shift existing content to the right using memmove (handles overlap)
        if( m_size > 0 ) [[likely]]
        {
            std::memmove( buf + len, buf, m_size );
        }

        // Copy new content to the beginning
        std::memcpy( buf, str.data(), len );

        m_size = newSize;
        return *this;
    }

    //----------------------------------------------
    // Private methods
    //----------------------------------------------

    void StringBuilder::ensureCapacity( size_t neededCapacity )
    {
        if( neededCapacity <= m_capacity )
        {
            return;
        }

        size_t newCapacity;
        if( m_capacity < GROWTH_THRESHOLD )
        {
            newCapacity = std::max( neededCapacity, static_cast<size_t>( m_capacity * AGGRESSIVE_GROWTH_FACTOR ) );
        }
        else
        {
            newCapacity = std::max( neededCapacity, static_cast<size_t>( m_capacity * STANDARD_GROWTH_FACTOR ) );
        }

        // Transition from stack to heap or expand heap buffer
        if( !m_onHeap )
        {
            // Transition from stack to heap
            m_heapBuffer = std::make_unique<char[]>( newCapacity );
            if( m_size > 0 ) [[likely]]
            {
                std::memcpy( m_heapBuffer.get(), m_stackBuffer, m_size );
            }
            m_buffer = m_heapBuffer.get();
            m_onHeap = true;
            m_capacity = newCapacity;
        }
        else
        {
            // Expand existing heap buffer
            auto newBuffer = std::make_unique<char[]>( newCapacity );
            if( m_size > 0 ) [[likely]]
            {
                std::memcpy( newBuffer.get(), m_heapBuffer.get(), m_size );
            }
            m_heapBuffer = std::move( newBuffer );
            m_buffer = m_heapBuffer.get();
            m_capacity = newCapacity;
        }
    }
} // namespace nfx::string
