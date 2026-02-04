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
 * @file StringBuilder.inl
 * @brief Inline method implementations for StringBuilder
 * @details High-performance inline implementations for StringBuilder with SBO
 */

#include <charconv>
#include <cstring>
#include <iterator>
#include <utility>

namespace nfx::string
{
	//=====================================================================
	// Internal constants for numeric conversions
	//=====================================================================

	namespace
	{

		constexpr size_t INT8_MAX_DIGITS = 4;	 // -128 to 127: max 4 chars
		constexpr size_t UINT8_MAX_DIGITS = 3;	 // 0 to 255: max 3 chars
		constexpr size_t INT16_MAX_DIGITS = 6;	 // -32768 to 32767: max 6 chars
		constexpr size_t UINT16_MAX_DIGITS = 5;	 // 0 to 65535: max 5 chars
		constexpr size_t INT32_MAX_DIGITS = 11;	 // -2147483648: max 11 chars
		constexpr size_t UINT32_MAX_DIGITS = 10; // 4294967295: max 10 chars
		constexpr size_t INT64_MAX_DIGITS = 20;	 // -9223372036854775808: max 20 chars
		constexpr size_t UINT64_MAX_DIGITS = 20; // 18446744073709551615: max 20 chars
		constexpr size_t FLOAT_MAX_CHARS = 15;	 // -3.40282347e+38: max 15 chars
		constexpr size_t DOUBLE_MAX_CHARS = 24;	 // -1.7976931348623157e+308: max 24 chars
	} // anonymous namespace

	//=====================================================================
	// StringBuilder class
	//=====================================================================

	//----------------------------------------------
	// Capacity and size information
	//----------------------------------------------

	inline size_t StringBuilder::size() const noexcept
	{
		return m_size;
	}

	inline size_t StringBuilder::capacity() const noexcept
	{
		return m_capacity;
	}

	inline bool StringBuilder::isEmpty() const noexcept
	{
		return m_size == 0;
	}

	//----------------------------------------------
	// Data access
	//----------------------------------------------

	inline char* StringBuilder::data() noexcept
	{
		return m_buffer;
	}

	inline const char* StringBuilder::data() const noexcept
	{
		return m_buffer;
	}

	inline char& StringBuilder::operator[]( size_t index )
	{
		return m_buffer[index];
	}

	inline const char& StringBuilder::operator[]( size_t index ) const
	{
		return m_buffer[index];
	}

	//----------------------------------------------
	// Buffer management
	//----------------------------------------------

	inline void StringBuilder::clear() noexcept
	{
		m_size = 0;
	}

	inline void StringBuilder::reserve( size_t newCapacity )
	{
		if ( newCapacity > m_capacity ) [[likely]]
		{
			ensureCapacity( newCapacity );
		}
	}

	inline void StringBuilder::resize( size_t newSize )
	{
		ensureCapacity( newSize );
		m_size = newSize;
	}

	//----------------------------------------------
	// Content manipulation
	//----------------------------------------------

	inline StringBuilder& StringBuilder::append( const std::string& str )
	{
		return append( std::string_view{ str } );
	}

	inline StringBuilder& StringBuilder::append( std::string_view str )
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

		// Inline fast path for small strings (most common case)
		std::memcpy( m_buffer + m_size, str.data(), len );
		m_size = newSize;
		return *this;
	}

	inline StringBuilder& StringBuilder::append( const char* str )
	{
		if ( str ) [[likely]]
		{
			return append( std::string_view{ str, strlen( str ) } );
		}
		return *this;
	}

	template <size_t N>
	NFX_STRINGBUILDER_FORCE_INLINE StringBuilder& StringBuilder::append( const char ( &str )[N] )
	{
		// N includes null terminator, actual length is N-1
		return append( std::string_view{ str, N - 1 } );
	}

	inline StringBuilder& StringBuilder::append( char c )
	{
		if ( m_size + 1 > m_capacity ) [[unlikely]]
		{
			ensureCapacity( m_size + 1 );
		}
		m_buffer[m_size++] = c;
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::int8_t value )
	{
		if ( m_size + INT8_MAX_DIGITS > m_capacity ) [[unlikely]]
		{
			ensureCapacity( m_size + INT8_MAX_DIGITS );
		}
		auto [ptr, ec] = std::to_chars( m_buffer + m_size, m_buffer + m_capacity, static_cast<int>( value ) );
		if ( ec == std::errc() ) [[likely]]
		{
			m_size = ptr - m_buffer;
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::uint8_t value )
	{
		if ( m_size + UINT8_MAX_DIGITS > m_capacity ) [[unlikely]]
		{
			ensureCapacity( m_size + UINT8_MAX_DIGITS );
		}
		auto [ptr, ec] = std::to_chars( m_buffer + m_size, m_buffer + m_capacity, static_cast<unsigned>( value ) );
		if ( ec == std::errc() ) [[likely]]
		{
			m_size = ptr - m_buffer;
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::int16_t value )
	{
		if ( m_size + INT16_MAX_DIGITS > m_capacity ) [[unlikely]]
		{
			ensureCapacity( m_size + INT16_MAX_DIGITS );
		}
		auto [ptr, ec] = std::to_chars( m_buffer + m_size, m_buffer + m_capacity, value );
		if ( ec == std::errc() ) [[likely]]
		{
			m_size = ptr - m_buffer;
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::uint16_t value )
	{
		if ( m_size + UINT16_MAX_DIGITS > m_capacity ) [[unlikely]]
		{
			ensureCapacity( m_size + UINT16_MAX_DIGITS );
		}
		auto [ptr, ec] = std::to_chars( m_buffer + m_size, m_buffer + m_capacity, value );
		if ( ec == std::errc() ) [[likely]]
		{
			m_size = ptr - m_buffer;
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::int32_t value )
	{
		if ( m_size + INT32_MAX_DIGITS > m_capacity ) [[unlikely]]
		{
			ensureCapacity( m_size + INT32_MAX_DIGITS );
		}
		auto [ptr, ec] = std::to_chars( m_buffer + m_size, m_buffer + m_capacity, value );
		if ( ec == std::errc() ) [[likely]]
		{
			m_size = ptr - m_buffer;
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::uint32_t value )
	{
		if ( m_size + UINT32_MAX_DIGITS > m_capacity ) [[unlikely]]
		{
			ensureCapacity( m_size + UINT32_MAX_DIGITS );
		}
		auto [ptr, ec] = std::to_chars( m_buffer + m_size, m_buffer + m_capacity, value );
		if ( ec == std::errc() ) [[likely]]
		{
			m_size = ptr - m_buffer;
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::int64_t value )
	{
		if ( m_size + INT64_MAX_DIGITS > m_capacity ) [[unlikely]]
		{
			ensureCapacity( m_size + INT64_MAX_DIGITS );
		}
		auto [ptr, ec] = std::to_chars( m_buffer + m_size, m_buffer + m_capacity, value );
		if ( ec == std::errc() ) [[likely]]
		{
			m_size = ptr - m_buffer;
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::uint64_t value )
	{
		if ( m_size + UINT64_MAX_DIGITS > m_capacity ) [[unlikely]]
		{
			ensureCapacity( m_size + UINT64_MAX_DIGITS );
		}
		auto [ptr, ec] = std::to_chars( m_buffer + m_size, m_buffer + m_capacity, value );
		if ( ec == std::errc() ) [[likely]]
		{
			m_size = ptr - m_buffer;
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( float value )
	{
		if ( m_size + FLOAT_MAX_CHARS > m_capacity ) [[unlikely]]
		{
			ensureCapacity( m_size + FLOAT_MAX_CHARS );
		}
		auto [ptr, ec] = std::to_chars( m_buffer + m_size, m_buffer + m_capacity, value );
		if ( ec == std::errc() ) [[likely]]
		{
			m_size = ptr - m_buffer;
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( double value )
	{
		if ( m_size + DOUBLE_MAX_CHARS > m_capacity ) [[unlikely]]
		{
			ensureCapacity( m_size + DOUBLE_MAX_CHARS );
		}
		auto [ptr, ec] = std::to_chars( m_buffer + m_size, m_buffer + m_capacity, value );
		if ( ec == std::errc() ) [[likely]]
		{
			m_size = ptr - m_buffer;
		}
		return *this;
	}

	//----------------------------------------------
	// Prepend operations
	//----------------------------------------------

	inline StringBuilder& StringBuilder::prepend( const std::string& str )
	{
		prepend( std::string_view{ str } );
		return *this;
	}

	inline StringBuilder& StringBuilder::prepend( const char* str )
	{
		if ( str )
		{
			prepend( std::string_view{ str, strlen( str ) } );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::prepend( char c )
	{
		if ( m_size + 1 > m_capacity ) [[unlikely]]
		{
			ensureCapacity( m_size + 1 );
		}

		char* buf = m_buffer;

		// Shift existing content right by 1
		if ( m_size > 0 )
		{
			std::memmove( buf + 1, buf, m_size );
		}

		buf[0] = c;
		++m_size;
		return *this;
	}

	inline StringBuilder& StringBuilder::prepend( std::int8_t value )
	{
		char buffer[INT8_MAX_DIGITS];
		auto [ptr, ec] = std::to_chars( buffer, buffer + INT8_MAX_DIGITS, static_cast<int>( value ) );
		if ( ec == std::errc() ) [[likely]]
		{
			return prepend( std::string_view{ buffer, static_cast<size_t>( ptr - buffer ) } );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::prepend( std::uint8_t value )
	{
		char buffer[UINT8_MAX_DIGITS];
		auto [ptr, ec] = std::to_chars( buffer, buffer + UINT8_MAX_DIGITS, static_cast<unsigned>( value ) );
		if ( ec == std::errc() ) [[likely]]
		{
			return prepend( std::string_view{ buffer, static_cast<size_t>( ptr - buffer ) } );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::prepend( std::int16_t value )
	{
		char buffer[INT16_MAX_DIGITS];
		auto [ptr, ec] = std::to_chars( buffer, buffer + INT16_MAX_DIGITS, value );
		if ( ec == std::errc() ) [[likely]]
		{
			return prepend( std::string_view{ buffer, static_cast<size_t>( ptr - buffer ) } );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::prepend( std::uint16_t value )
	{
		char buffer[UINT16_MAX_DIGITS];
		auto [ptr, ec] = std::to_chars( buffer, buffer + UINT16_MAX_DIGITS, value );
		if ( ec == std::errc() ) [[likely]]
		{
			return prepend( std::string_view{ buffer, static_cast<size_t>( ptr - buffer ) } );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::prepend( std::int32_t value )
	{
		char buffer[INT32_MAX_DIGITS];
		auto [ptr, ec] = std::to_chars( buffer, buffer + INT32_MAX_DIGITS, value );
		if ( ec == std::errc() ) [[likely]]
		{
			return prepend( std::string_view{ buffer, static_cast<size_t>( ptr - buffer ) } );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::prepend( std::uint32_t value )
	{
		char buffer[UINT32_MAX_DIGITS];
		auto [ptr, ec] = std::to_chars( buffer, buffer + UINT32_MAX_DIGITS, value );
		if ( ec == std::errc() ) [[likely]]
		{
			return prepend( std::string_view{ buffer, static_cast<size_t>( ptr - buffer ) } );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::prepend( std::int64_t value )
	{
		char buffer[INT64_MAX_DIGITS];
		auto [ptr, ec] = std::to_chars( buffer, buffer + INT64_MAX_DIGITS, value );
		if ( ec == std::errc() ) [[likely]]
		{
			return prepend( std::string_view{ buffer, static_cast<size_t>( ptr - buffer ) } );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::prepend( std::uint64_t value )
	{
		char buffer[UINT64_MAX_DIGITS];
		auto [ptr, ec] = std::to_chars( buffer, buffer + UINT64_MAX_DIGITS, value );
		if ( ec == std::errc() ) [[likely]]
		{
			return prepend( std::string_view{ buffer, static_cast<size_t>( ptr - buffer ) } );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::prepend( float value )
	{
		char buffer[FLOAT_MAX_CHARS];
		auto [ptr, ec] = std::to_chars( buffer, buffer + FLOAT_MAX_CHARS, value );
		if ( ec == std::errc() ) [[likely]]
		{
			return prepend( std::string_view{ buffer, static_cast<size_t>( ptr - buffer ) } );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::prepend( double value )
	{
		char buffer[DOUBLE_MAX_CHARS];
		auto [ptr, ec] = std::to_chars( buffer, buffer + DOUBLE_MAX_CHARS, value );
		if ( ec == std::errc() ) [[likely]]
		{
			return prepend( std::string_view{ buffer, static_cast<size_t>( ptr - buffer ) } );
		}
		return *this;
	}

	//----------------------------------------------
	// Batch operations
	//----------------------------------------------

	template <typename T, typename... Args, typename>
	inline StringBuilder& StringBuilder::append( T&& first, Args&&... args )
	{
		append( first );
		if constexpr ( sizeof...( Args ) > 0 )
		{
			( append( args ), ... );
		}

		return *this;
	}

	//----------------------------------------------
	// Stream operators
	//----------------------------------------------

	inline StringBuilder& StringBuilder::operator<<( std::string_view str )
	{
		append( str );
		return *this;
	}

	inline StringBuilder& StringBuilder::operator<<( const std::string& str )
	{
		append( str );
		return *this;
	}

	inline StringBuilder& StringBuilder::operator<<( const char* str )
	{
		append( str );
		return *this;
	}

	template <size_t N>
	NFX_STRINGBUILDER_FORCE_INLINE StringBuilder& StringBuilder::operator<<( const char ( &str )[N] )
	{
		return append( str );
	}

	inline StringBuilder& StringBuilder::operator<<( char c )
	{
		append( c );
		return *this;
	}

	inline StringBuilder& StringBuilder::operator<<( std::int8_t value )
	{
		return append( value );
	}

	inline StringBuilder& StringBuilder::operator<<( std::uint8_t value )
	{
		return append( value );
	}

	inline StringBuilder& StringBuilder::operator<<( std::int16_t value )
	{
		return append( value );
	}

	inline StringBuilder& StringBuilder::operator<<( std::uint16_t value )
	{
		return append( value );
	}

	inline StringBuilder& StringBuilder::operator<<( std::int32_t value )
	{
		return append( value );
	}

	inline StringBuilder& StringBuilder::operator<<( std::uint32_t value )
	{
		return append( value );
	}

	inline StringBuilder& StringBuilder::operator<<( std::int64_t value )
	{
		return append( value );
	}

	inline StringBuilder& StringBuilder::operator<<( std::uint64_t value )
	{
		return append( value );
	}

	inline StringBuilder& StringBuilder::operator<<( float value )
	{
		return append( value );
	}

	inline StringBuilder& StringBuilder::operator<<( double value )
	{
		return append( value );
	}

	//----------------------------------------------
	// Formatting operations
	//----------------------------------------------

	template <typename... Args>
	inline StringBuilder& StringBuilder::format( std::format_string<Args...> fmt, Args&&... args )
	{
		std::format_to( std::back_inserter( *this ), fmt, std::forward<Args>( args )... );
		return *this;
	}

	//----------------------------------------------
	// String conversion
	//----------------------------------------------

	inline std::string StringBuilder::toString() const&
	{
		return std::string( m_buffer, m_size );
	}

	inline std::string StringBuilder::toString() &&
	{
		if ( m_onHeap )
		{
			std::string result( m_heapBuffer.get(), m_size );
			m_heapBuffer.reset();
			m_buffer = m_stackBuffer;
			m_size = 0;
			m_capacity = STACK_BUFFER_SIZE;
			m_onHeap = false;
			return result;
		}
		else
		{
			return std::string( m_stackBuffer, m_size );
		}
	}

	inline std::string_view StringBuilder::toStringView() const noexcept
	{
		return std::string_view{ m_buffer, m_size };
	}

	//----------------------------------------------
	// STL support
	//----------------------------------------------

	inline void StringBuilder::push_back( char c )
	{
		append( c );
	}

	//----------------------------------------------
	// Iterator interface
	//----------------------------------------------

	inline StringBuilder::Iterator StringBuilder::begin() noexcept
	{
		return m_buffer;
	}

	inline StringBuilder::ConstIterator StringBuilder::begin() const noexcept
	{
		return m_buffer;
	}

	inline StringBuilder::Iterator StringBuilder::end() noexcept
	{
		return m_buffer + m_size;
	}

	inline StringBuilder::ConstIterator StringBuilder::end() const noexcept
	{
		return m_buffer + m_size;
	}
} // namespace nfx::string

namespace std
{
	//=====================================================================
	// std::formatter specialization
	//=====================================================================

	template <>
	struct formatter<nfx::string::StringBuilder>
	{
		constexpr auto parse( std::format_parse_context& ctx )
		{
			return ctx.begin();
		}

		auto format( const nfx::string::StringBuilder& buf, std::format_context& ctx ) const
		{
			return std::format_to( ctx.out(), "{}", buf.toStringView() );
		}
	};
} // namespace std
