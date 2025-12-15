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
 * @brief Implementation file for StringBuilder methods
 */

#include <algorithm>
#include <cstring>

#ifdef _MSC_VER
#	include <intrin.h>
#elif defined( __GNUC__ ) || defined( __clang__ )
#	include <immintrin.h>
#endif

#include "nfx/string/StringBuilder.h"
#include "DynamicStringBufferPool.h"

namespace nfx::string
{
	//=====================================================================
	// DynamicStringBuffer class
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	DynamicStringBuffer::DynamicStringBuffer()
		: m_heapBuffer{ nullptr },
		  m_size{ 0 },
		  m_capacity{ STACK_BUFFER_SIZE },
		  m_onHeap{ false }
	{
	}

	DynamicStringBuffer::DynamicStringBuffer( size_t initialCapacity )
		: m_heapBuffer{ nullptr },
		  m_size{ 0 }, m_capacity{ STACK_BUFFER_SIZE },
		  m_onHeap{ false }
	{
		if ( initialCapacity > STACK_BUFFER_SIZE )
		{
			m_heapBuffer = std::make_unique<char[]>( initialCapacity );
			m_capacity = initialCapacity;
			m_onHeap = true;
		}
	}

	DynamicStringBuffer::DynamicStringBuffer( const DynamicStringBuffer& other )
		: m_heapBuffer{ nullptr },
		  m_size{ other.m_size },
		  m_capacity{ other.m_capacity },
		  m_onHeap{ other.m_onHeap }
	{
		if ( m_onHeap )
		{
			m_heapBuffer = std::make_unique<char[]>( m_capacity );
			std::memcpy( m_heapBuffer.get(), other.m_heapBuffer.get(), m_size );
		}
		else
		{
			std::memcpy( m_stackBuffer, other.m_stackBuffer, m_size );
		}
	}

	DynamicStringBuffer::DynamicStringBuffer( DynamicStringBuffer&& other ) noexcept
		: m_heapBuffer{ std::move( other.m_heapBuffer ) },
		  m_size{ other.m_size },
		  m_capacity{ other.m_capacity },
		  m_onHeap{ other.m_onHeap }
	{
		if ( !m_onHeap )
		{
			std::memcpy( m_stackBuffer, other.m_stackBuffer, m_size );
		}

		other.m_size = 0;
		other.m_capacity = STACK_BUFFER_SIZE;
		other.m_onHeap = false;
	}

	//----------------------------------------------
	// Assignment
	//----------------------------------------------

	DynamicStringBuffer& DynamicStringBuffer::operator=( const DynamicStringBuffer& other )
	{
		if ( this != &other )
		{
			if ( other.m_onHeap )
			{
				// Other uses heap, we need heap too
				if ( !m_onHeap || m_capacity < other.m_capacity )
				{
					m_heapBuffer = std::make_unique<char[]>( other.m_capacity );
					m_capacity = other.m_capacity;
					m_onHeap = true;
				}
				std::memcpy( m_heapBuffer.get(), other.m_heapBuffer.get(), other.m_size );
			}
			else
			{
				// Other uses stack, we can use stack too
				if ( m_onHeap )
				{
					m_heapBuffer.reset();
					m_capacity = STACK_BUFFER_SIZE;
					m_onHeap = false;
				}
				std::memcpy( m_stackBuffer, other.m_stackBuffer, other.m_size );
			}
			m_size = other.m_size;
		}
		return *this;
	}

	DynamicStringBuffer& DynamicStringBuffer::operator=( DynamicStringBuffer&& other ) noexcept
	{
		if ( this != &other )
		{
			m_heapBuffer = std::move( other.m_heapBuffer );
			m_size = other.m_size;
			m_capacity = other.m_capacity;
			m_onHeap = other.m_onHeap;

			if ( !m_onHeap )
			{
				std::memcpy( m_stackBuffer, other.m_stackBuffer, m_size );
			}

			other.m_size = 0;
			other.m_capacity = STACK_BUFFER_SIZE;
			other.m_onHeap = false;
		}
		return *this;
	}

	//----------------------------------------------
	// Capacity and size information
	//----------------------------------------------

	size_t DynamicStringBuffer::size() const noexcept
	{
		return m_size;
	}

	size_t DynamicStringBuffer::capacity() const noexcept
	{
		return m_capacity;
	}

	bool DynamicStringBuffer::isEmpty() const noexcept
	{
		return m_size == 0;
	}

	//----------------------------------------------
	// Buffer management
	//----------------------------------------------

	void DynamicStringBuffer::clear() noexcept
	{
		m_size = 0;
	}

	void DynamicStringBuffer::reserve( size_t newCapacity )
	{
		if ( newCapacity > m_capacity )
		{
			ensureCapacity( newCapacity );
		}
	}

	void DynamicStringBuffer::resize( size_t newSize )
	{
		ensureCapacity( newSize );
		m_size = newSize;
	}

	//----------------------------------------------
	// Data access
	//----------------------------------------------

	char* DynamicStringBuffer::data() noexcept
	{
		return currentBuffer();
	}

	const char* DynamicStringBuffer::data() const noexcept
	{
		return currentBuffer();
	}

	char& DynamicStringBuffer::operator[]( size_t index )
	{
		return currentBuffer()[index];
	}

	const char& DynamicStringBuffer::operator[]( size_t index ) const
	{
		return currentBuffer()[index];
	}

	//----------------------------------------------
	// Content manipulation
	//----------------------------------------------

	void DynamicStringBuffer::append( std::string_view str )
	{
		if ( str.empty() ) [[unlikely]]
		{
			return;
		}

		const size_t len = str.size();
		const size_t newSize = m_size + len;

		if ( newSize > m_capacity ) [[unlikely]]
		{
			ensureCapacity( newSize );
		}

		char* dest = currentBuffer() + m_size;
		const char* src = str.data();

		// Fast path for small string
		if ( len <= 16 ) [[likely]]
		{
			std::memcpy( dest, src, len );
		}

#if defined( __AVX2__ ) || ( defined( _MSC_VER ) && defined( __AVX2__ ) )
		// For very large copies, use unrolled AVX2 loop
		else if ( len >= 128 )
		{
			size_t remaining = len;

			// Process 128 bytes (4× AVX2 vectors) per iteration
			while ( remaining >= 128 )
			{
				__m256i chunk0 = _mm256_loadu_si256( reinterpret_cast<const __m256i*>( src + 0 ) );
				__m256i chunk1 = _mm256_loadu_si256( reinterpret_cast<const __m256i*>( src + 32 ) );
				__m256i chunk2 = _mm256_loadu_si256( reinterpret_cast<const __m256i*>( src + 64 ) );
				__m256i chunk3 = _mm256_loadu_si256( reinterpret_cast<const __m256i*>( src + 96 ) );

				_mm256_storeu_si256( reinterpret_cast<__m256i*>( dest + 0 ), chunk0 );
				_mm256_storeu_si256( reinterpret_cast<__m256i*>( dest + 32 ), chunk1 );
				_mm256_storeu_si256( reinterpret_cast<__m256i*>( dest + 64 ), chunk2 );
				_mm256_storeu_si256( reinterpret_cast<__m256i*>( dest + 96 ), chunk3 );

				src += 128;
				dest += 128;
				remaining -= 128;
			}

			// Handle remaining 32-byte chunks
			while ( remaining >= 32 )
			{
				__m256i chunk = _mm256_loadu_si256( reinterpret_cast<const __m256i*>( src ) );
				_mm256_storeu_si256( reinterpret_cast<__m256i*>( dest ), chunk );
				src += 32;
				dest += 32;
				remaining -= 32;
			}

			// Handle tail
			if ( remaining > 0 )
			{
				std::memcpy( dest, src, remaining );
			}
		}
		// Medium-sized copies: single AVX2 vector loop
		else if ( len >= 32 )
		{
			size_t remaining = len;
			while ( remaining >= 32 )
			{
				__m256i chunk = _mm256_loadu_si256( reinterpret_cast<const __m256i*>( src ) );
				_mm256_storeu_si256( reinterpret_cast<__m256i*>( dest ), chunk );
				src += 32;
				dest += 32;
				remaining -= 32;
			}
			if ( remaining > 0 )
			{
				std::memcpy( dest, src, remaining );
			}
		}
#elif defined( __SSE2__ ) || ( defined( _MSC_VER ) && ( defined( _M_X64 ) || defined( _M_IX86 ) ) )
		// SSE2 path for systems without AVX2
		else if ( len >= 64 )
		{
			size_t remaining = len;

			// Unrolled SSE2 loop for larger copies
			while ( remaining >= 64 )
			{
				__m128i chunk0 = _mm_loadu_si128( reinterpret_cast<const __m128i*>( src + 0 ) );
				__m128i chunk1 = _mm_loadu_si128( reinterpret_cast<const __m128i*>( src + 16 ) );
				__m128i chunk2 = _mm_loadu_si128( reinterpret_cast<const __m128i*>( src + 32 ) );
				__m128i chunk3 = _mm_loadu_si128( reinterpret_cast<const __m128i*>( src + 48 ) );

				_mm_storeu_si128( reinterpret_cast<__m128i*>( dest + 0 ), chunk0 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>( dest + 16 ), chunk1 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>( dest + 32 ), chunk2 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>( dest + 48 ), chunk3 );

				src += 64;
				dest += 64;
				remaining -= 64;
			}

			// Handle remaining 16-byte chunks
			while ( remaining >= 16 )
			{
				__m128i chunk = _mm_loadu_si128( reinterpret_cast<const __m128i*>( src ) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>( dest ), chunk );
				src += 16;
				dest += 16;
				remaining -= 16;
			}

			if ( remaining > 0 )
			{
				std::memcpy( dest, src, remaining );
			}
		}
		else if ( len >= 16 )
		{
			size_t remaining = len;
			while ( remaining >= 16 )
			{
				__m128i chunk = _mm_loadu_si128( reinterpret_cast<const __m128i*>( src ) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>( dest ), chunk );
				src += 16;
				dest += 16;
				remaining -= 16;
			}
			if ( remaining > 0 )
			{
				std::memcpy( dest, src, remaining );
			}
		}
#endif
		else
		{
			std::memcpy( dest, src, len );
		}

		m_size = newSize;
	}

	void DynamicStringBuffer::append( const std::string& str )
	{
		append( std::string_view{ str } );
	}

	void DynamicStringBuffer::append( const char* str )
	{
		if ( str )
		{
			append( std::string_view{ str, std::strlen( str ) } );
		}
	}

	void DynamicStringBuffer::append( char c )
	{
		if ( m_size + 1 > m_capacity ) [[unlikely]]
		{
			ensureCapacity( m_size + 1 );
		}
		currentBuffer()[m_size++] = c;
	}

	//----------------------------------------------
	// String conversion
	//----------------------------------------------

	std::string DynamicStringBuffer::toString() const
	{
		return std::string( currentBuffer(), m_size );
	}

	std::string_view DynamicStringBuffer::toStringView() const noexcept
	{
		return std::string_view{ currentBuffer(), m_size };
	}

	//----------------------------------------------
	// Iterator interface
	//----------------------------------------------

	DynamicStringBuffer::Iterator DynamicStringBuffer::begin() noexcept
	{
		return currentBuffer();
	}

	DynamicStringBuffer::ConstIterator DynamicStringBuffer::begin() const noexcept
	{
		return currentBuffer();
	}

	DynamicStringBuffer::Iterator DynamicStringBuffer::end() noexcept
	{
		return currentBuffer() + m_size;
	}

	DynamicStringBuffer::ConstIterator DynamicStringBuffer::end() const noexcept
	{
		return currentBuffer() + m_size;
	}

	//----------------------------------------------
	// Private methods
	//----------------------------------------------

	void DynamicStringBuffer::ensureCapacity( size_t neededCapacity )
	{
		if ( neededCapacity <= m_capacity )
		{
			return;
		}

		size_t newCapacity;
		if ( m_capacity < 8192 )
		{
			newCapacity = std::max( neededCapacity, static_cast<size_t>( m_capacity * GROWTH_FACTOR ) );
		}
		else
		{
			newCapacity = std::max( neededCapacity, static_cast<size_t>( m_capacity + m_capacity / GROWTH_FACTOR ) );
		}

		if ( !m_onHeap && newCapacity <= STACK_BUFFER_SIZE )
		{
			return;
		}

		if ( !m_onHeap )
		{
			// Transition from stack to heap
			m_heapBuffer = std::make_unique<char[]>( newCapacity );
			if ( m_size > 0 )
			{
				std::memcpy( m_heapBuffer.get(), m_stackBuffer, m_size );
			}
			m_onHeap = true;
			m_capacity = newCapacity;
		}
		else
		{
			// Expand existing heap buffer
			auto newBuffer = std::make_unique<char[]>( newCapacity );
			if ( m_size > 0 )
			{
				std::memcpy( newBuffer.get(), m_heapBuffer.get(), m_size );
			}
			m_heapBuffer = std::move( newBuffer );
			m_capacity = newCapacity;
		}
	}

	char* DynamicStringBuffer::currentBuffer() noexcept
	{
		return m_onHeap ? m_heapBuffer.get() : m_stackBuffer;
	}

	const char* DynamicStringBuffer::currentBuffer() const noexcept
	{
		return m_onHeap ? m_heapBuffer.get() : m_stackBuffer;
	}

	//=====================================================================
	// StringBuilderLease class
	//=====================================================================

	//----------------------------------------------
	// Destruction
	//----------------------------------------------

	StringBuilderLease::~StringBuilderLease()
	{
		dispose();
	}

	//----------------------------------------------
	// Private implementation methods
	//----------------------------------------------

	void StringBuilderLease::dispose()
	{
		if ( m_valid )
		{
			dynamicStringBufferPool().returnToPool( m_buffer );
			m_buffer = nullptr;
			m_valid = false;
		}
	}

	void StringBuilderLease::throwInvalidOperation() const
	{
		throw std::runtime_error{ "Tried to access StringBuilder after it was returned to pool" };
	}

	//=====================================================================
	// StringBuilderPool class
	//=====================================================================

	//----------------------------------------------
	// Static factory methods
	//----------------------------------------------

	StringBuilderLease StringBuilderPool::lease()
	{
		return StringBuilderLease{ dynamicStringBufferPool().get() };
	}

	StringBuilderLease StringBuilderPool::lease( size_t capacityHint )
	{
		return StringBuilderLease{ dynamicStringBufferPool().get( capacityHint ) };
	}

	//----------------------------
	// Statistics methods
	//----------------------------

	StringBuilderPool::PoolStatistics StringBuilderPool::stats() noexcept
	{
		const auto& internalStats = dynamicStringBufferPool().stats();
		return PoolStatistics{
			.threadLocalHits = internalStats.threadLocalHits.load(),
			.dynamicStringBufferPoolHits = internalStats.dynamicStringBufferPoolHits.load(),
			.newAllocations = internalStats.newAllocations.load(),
			.totalRequests = internalStats.totalRequests.load(),
			.hitRate = internalStats.hitRate() };
	}

	void StringBuilderPool::resetStats() noexcept
	{
		dynamicStringBufferPool().resetStats();
	}

	//----------------------------
	// Lease management
	//----------------------------

	size_t StringBuilderPool::clear()
	{
		dynamicStringBufferPool().resetStats();

		return dynamicStringBufferPool().clear();
	}

	size_t StringBuilderPool::size() noexcept
	{
		return dynamicStringBufferPool().size();
	}
} // namespace nfx::string
