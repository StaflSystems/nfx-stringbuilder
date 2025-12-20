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
 * @brief Implementation of StringBuilder methods with SIMD-optimized string operations
 */

#include "nfx/string/StringBuilder.h"

#include <iostream>

#include <algorithm>
#include <charconv>
#include <cstring>

#ifdef _MSC_VER
#	include <intrin.h>
#elif defined( __GNUC__ ) || defined( __clang__ )
#	include <immintrin.h>
#endif

namespace nfx::string
{
	//=====================================================================
	// Internal constants
	//=====================================================================

	namespace
	{
		constexpr size_t SMALL_COPY_THRESHOLD = 16; // Small strings use simple memcpy (cache-friendly)
#if defined( __AVX2__ )
		constexpr size_t AVX2_VECTOR_SIZE = 32;	 // 256-bit AVX2 register
		constexpr size_t AVX2_UNROLL_SIZE = 128; // 4× AVX2 vectors for optimal unrolling
#elif defined( __SSE2__ ) || ( defined( _MSC_VER ) && ( defined( _M_X64 ) || defined( _M_IX86 ) ) )
		constexpr size_t SSE2_VECTOR_SIZE = 16; // 128-bit SSE2 register
		constexpr size_t SSE2_UNROLL_SIZE = 64; // 4× SSE2 vectors for optimal unrolling
#endif
	} // anonymous namespace

	//=====================================================================
	// StringBuilder class
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	StringBuilder::StringBuilder()
		: m_heapBuffer{ nullptr },
		  m_size{ 0 },
		  m_capacity{ STACK_BUFFER_SIZE },
		  m_onHeap{ false }
	{
	}

	StringBuilder::StringBuilder( size_t initialCapacity )
		: m_heapBuffer{ nullptr },
		  m_size{ 0 },
		  m_capacity{ STACK_BUFFER_SIZE },
		  m_onHeap{ false }
	{
		if ( initialCapacity > STACK_BUFFER_SIZE )
		{
			m_heapBuffer = std::make_unique<char[]>( initialCapacity );
			m_capacity = initialCapacity;
			m_onHeap = true;
		}
	}

	StringBuilder::StringBuilder( const StringBuilder& other )
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

	StringBuilder::StringBuilder( StringBuilder&& other ) noexcept
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

	StringBuilder& StringBuilder::operator=( const StringBuilder& other )
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

	StringBuilder& StringBuilder::operator=( StringBuilder&& other ) noexcept
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
	// Buffer management
	//----------------------------------------------

	void StringBuilder::reserve( size_t newCapacity )
	{
		if ( newCapacity > m_capacity )
		{
			ensureCapacity( newCapacity );
		}
	}

	void StringBuilder::resize( size_t newSize )
	{
		ensureCapacity( newSize );
		m_size = newSize;
	}

	//----------------------------------------------
	// Content manipulation
	//----------------------------------------------

	StringBuilder& StringBuilder::append( std::string_view str )
	{
		if ( str.empty() ) [[unlikely]]
		{
			return *this;
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
		if ( len <= SMALL_COPY_THRESHOLD ) [[likely]]
		{
			std::memcpy( dest, src, len );
		}
#if defined( __AVX2__ )

		// For very large copies, use unrolled AVX2 loop
		else if ( len >= AVX2_UNROLL_SIZE )
		{
			size_t remaining = len;

			// Process 128 bytes (4× AVX2 vectors) per iteration
			while ( remaining >= AVX2_UNROLL_SIZE )
			{
				__m256i chunk0 = _mm256_loadu_si256( reinterpret_cast<const __m256i*>( src + 0 ) );
				__m256i chunk1 = _mm256_loadu_si256( reinterpret_cast<const __m256i*>( src + 32 ) );
				__m256i chunk2 = _mm256_loadu_si256( reinterpret_cast<const __m256i*>( src + 64 ) );
				__m256i chunk3 = _mm256_loadu_si256( reinterpret_cast<const __m256i*>( src + 96 ) );

				_mm256_storeu_si256( reinterpret_cast<__m256i*>( dest + 0 ), chunk0 );
				_mm256_storeu_si256( reinterpret_cast<__m256i*>( dest + 32 ), chunk1 );
				_mm256_storeu_si256( reinterpret_cast<__m256i*>( dest + 64 ), chunk2 );
				_mm256_storeu_si256( reinterpret_cast<__m256i*>( dest + 96 ), chunk3 );

				src += AVX2_UNROLL_SIZE;
				dest += AVX2_UNROLL_SIZE;
				remaining -= AVX2_UNROLL_SIZE;
			}

			// Handle remaining 32-byte chunks
			while ( remaining >= AVX2_VECTOR_SIZE )
			{
				__m256i chunk = _mm256_loadu_si256( reinterpret_cast<const __m256i*>( src ) );
				_mm256_storeu_si256( reinterpret_cast<__m256i*>( dest ), chunk );
				src += AVX2_VECTOR_SIZE;
				dest += AVX2_VECTOR_SIZE;
				remaining -= AVX2_VECTOR_SIZE;
			}

			// Handle tail
			if ( remaining > 0 )
			{
				std::memcpy( dest, src, remaining );
			}
		}
		// Medium-sized copies: single AVX2 vector loop
		else if ( len >= AVX2_VECTOR_SIZE )
		{
			size_t remaining = len;
			while ( remaining >= AVX2_VECTOR_SIZE )
			{
				__m256i chunk = _mm256_loadu_si256( reinterpret_cast<const __m256i*>( src ) );
				_mm256_storeu_si256( reinterpret_cast<__m256i*>( dest ), chunk );
				src += AVX2_VECTOR_SIZE;
				dest += AVX2_VECTOR_SIZE;
				remaining -= AVX2_VECTOR_SIZE;
			}
			if ( remaining > 0 )
			{
				std::memcpy( dest, src, remaining );
			}
		}
#elif defined( __SSE2__ ) || ( defined( _MSC_VER ) && ( defined( _M_X64 ) || defined( _M_IX86 ) ) )
		// SSE2 path for systems without AVX2
		else if ( len >= SSE2_UNROLL_SIZE )
		{
			size_t remaining = len;

			// Unrolled SSE2 loop for larger copies
			while ( remaining >= SSE2_UNROLL_SIZE )
			{
				__m128i chunk0 = _mm_loadu_si128( reinterpret_cast<const __m128i*>( src + 0 ) );
				__m128i chunk1 = _mm_loadu_si128( reinterpret_cast<const __m128i*>( src + 16 ) );
				__m128i chunk2 = _mm_loadu_si128( reinterpret_cast<const __m128i*>( src + 32 ) );
				__m128i chunk3 = _mm_loadu_si128( reinterpret_cast<const __m128i*>( src + 48 ) );

				_mm_storeu_si128( reinterpret_cast<__m128i*>( dest + 0 ), chunk0 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>( dest + 16 ), chunk1 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>( dest + 32 ), chunk2 );
				_mm_storeu_si128( reinterpret_cast<__m128i*>( dest + 48 ), chunk3 );

				src += SSE2_UNROLL_SIZE;
				dest += SSE2_UNROLL_SIZE;
				remaining -= SSE2_UNROLL_SIZE;
			}

			// Handle remaining 16-byte chunks
			while ( remaining >= SSE2_VECTOR_SIZE )
			{
				__m128i chunk = _mm_loadu_si128( reinterpret_cast<const __m128i*>( src ) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>( dest ), chunk );
				src += SSE2_VECTOR_SIZE;
				dest += SSE2_VECTOR_SIZE;
				remaining -= SSE2_VECTOR_SIZE;
			}

			if ( remaining > 0 )
			{
				std::memcpy( dest, src, remaining );
			}
		}
		else if ( len >= SSE2_VECTOR_SIZE )
		{
			size_t remaining = len;
			while ( remaining >= SSE2_VECTOR_SIZE )
			{
				__m128i chunk = _mm_loadu_si128( reinterpret_cast<const __m128i*>( src ) );
				_mm_storeu_si128( reinterpret_cast<__m128i*>( dest ), chunk );
				src += SSE2_VECTOR_SIZE;
				dest += SSE2_VECTOR_SIZE;
				remaining -= SSE2_VECTOR_SIZE;
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
		return *this;
	}

	//----------------------------------------------
	// Prepend operations
	//----------------------------------------------

	StringBuilder& StringBuilder::prepend( std::string_view str )
	{
		if ( str.empty() ) [[unlikely]]
		{
			return *this;
		}

		const size_t len = str.size();
		const size_t newSize = m_size + len;

		if ( newSize > m_capacity ) [[unlikely]]
		{
			ensureCapacity( newSize );
		}

		char* buf = currentBuffer();

		// Shift existing content to the right using memmove (handles overlap)
		if ( m_size > 0 )
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
		if ( neededCapacity <= m_capacity )
		{
			return;
		}

		size_t newCapacity;
		if ( m_capacity < GROWTH_THRESHOLD )
		{
			newCapacity = std::max( neededCapacity, static_cast<size_t>( m_capacity * AGGRESSIVE_GROWTH_FACTOR ) );
		}
		else
		{
			newCapacity = std::max( neededCapacity, static_cast<size_t>( m_capacity * STANDARD_GROWTH_FACTOR ) );
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
} // namespace nfx::string
