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
 * @details High-performance inline implementations for string builder pooling infrastructure
 */

#include <charconv>
#include <iterator>
#include <utility>

namespace nfx::string
{
	//=====================================================================
	// StringBuilder class
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	inline StringBuilder::StringBuilder( DynamicStringBuffer& buffer )
		: m_buffer{ buffer }
	{
	}

	//----------------------------------------------
	// Array access operators
	//----------------------------------------------

	inline char& StringBuilder::operator[]( size_t index )
	{
		return m_buffer[index];
	}

	inline const char& StringBuilder::operator[]( size_t index ) const
	{
		return m_buffer[index];
	}

	//----------------------------------------------
	// String append operations
	//----------------------------------------------

	inline StringBuilder& StringBuilder::append( std::string_view str )
	{
		m_buffer.append( str );
		return *this;
	}

	inline StringBuilder& StringBuilder::append( const std::string& str )
	{
		m_buffer.append( str );
		return *this;
	}

	inline StringBuilder& StringBuilder::append( const char* str )
	{
		if ( str )
		{
			m_buffer.append( str );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( char c )
	{
		m_buffer.append( c );
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::int8_t value )
	{
		// -128 to 127 needs max 4 chars
		size_t oldSize = m_buffer.size();
		m_buffer.resize( oldSize + 4 );

		char* dest = m_buffer.data() + oldSize;
		auto [ptr, ec] = std::to_chars( dest, dest + 4, static_cast<int>( value ) );

		if ( ec == std::errc() )
		{
			m_buffer.resize( oldSize + ( ptr - dest ) );
		}
		else
		{
			m_buffer.resize( oldSize );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::uint8_t value )
	{
		// 0 to 255 needs max 3 chars
		size_t oldSize = m_buffer.size();
		m_buffer.resize( oldSize + 3 );

		char* dest = m_buffer.data() + oldSize;
		auto [ptr, ec] = std::to_chars( dest, dest + 3, static_cast<unsigned>( value ) );

		if ( ec == std::errc() )
		{
			m_buffer.resize( oldSize + ( ptr - dest ) );
		}
		else
		{
			m_buffer.resize( oldSize );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::int16_t value )
	{
		// -32768 to 32767 needs max 6 chars
		size_t oldSize = m_buffer.size();
		m_buffer.resize( oldSize + 6 );

		char* dest = m_buffer.data() + oldSize;
		auto [ptr, ec] = std::to_chars( dest, dest + 6, value );

		if ( ec == std::errc() )
		{
			m_buffer.resize( oldSize + ( ptr - dest ) );
		}
		else
		{
			m_buffer.resize( oldSize );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::uint16_t value )
	{
		// 0 to 65535 needs max 5 chars
		size_t oldSize = m_buffer.size();
		m_buffer.resize( oldSize + 5 );

		char* dest = m_buffer.data() + oldSize;
		auto [ptr, ec] = std::to_chars( dest, dest + 5, value );

		if ( ec == std::errc() )
		{
			m_buffer.resize( oldSize + ( ptr - dest ) );
		}
		else
		{
			m_buffer.resize( oldSize );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::int32_t value )
	{
		// -2147483648 needs max 11 chars
		size_t oldSize = m_buffer.size();
		m_buffer.resize( oldSize + 11 );

		char* dest = m_buffer.data() + oldSize;
		auto [ptr, ec] = std::to_chars( dest, dest + 11, value );

		if ( ec == std::errc() )
		{
			m_buffer.resize( oldSize + ( ptr - dest ) );
		}
		else
		{
			m_buffer.resize( oldSize );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::uint32_t value )
	{
		// 4294967295 needs max 10 chars
		size_t oldSize = m_buffer.size();
		m_buffer.resize( oldSize + 10 );

		char* dest = m_buffer.data() + oldSize;
		auto [ptr, ec] = std::to_chars( dest, dest + 10, value );

		if ( ec == std::errc() )
		{
			m_buffer.resize( oldSize + ( ptr - dest ) );
		}
		else
		{
			m_buffer.resize( oldSize );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::int64_t value )
	{
		// -9223372036854775808 needs max 20 chars
		size_t oldSize = m_buffer.size();
		m_buffer.resize( oldSize + 20 );

		char* dest = m_buffer.data() + oldSize;
		auto [ptr, ec] = std::to_chars( dest, dest + 20, value );

		if ( ec == std::errc() )
		{
			m_buffer.resize( oldSize + ( ptr - dest ) );
		}
		else
		{
			m_buffer.resize( oldSize );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( std::uint64_t value )
	{
		// 18446744073709551615 needs max 20 chars
		size_t oldSize = m_buffer.size();
		m_buffer.resize( oldSize + 20 );

		char* dest = m_buffer.data() + oldSize;
		auto [ptr, ec] = std::to_chars( dest, dest + 20, value );

		if ( ec == std::errc() )
		{
			m_buffer.resize( oldSize + ( ptr - dest ) );
		}
		else
		{
			m_buffer.resize( oldSize );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( float value )
	{
		// Sufficient for scientific notation and special values
		size_t oldSize = m_buffer.size();
		m_buffer.resize( oldSize + 32 );

		char* dest = m_buffer.data() + oldSize;
		auto [ptr, ec] = std::to_chars( dest, dest + 32, value );

		if ( ec == std::errc() )
		{
			m_buffer.resize( oldSize + ( ptr - dest ) );
		}
		else
		{
			m_buffer.resize( oldSize );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::append( double value )
	{
		// Sufficient for scientific notation and special values
		size_t oldSize = m_buffer.size();
		m_buffer.resize( oldSize + 32 );

		char* dest = m_buffer.data() + oldSize;
		auto [ptr, ec] = std::to_chars( dest, dest + 32, value );

		if ( ec == std::errc() )
		{
			m_buffer.resize( oldSize + ( ptr - dest ) );
		}
		else
		{
			m_buffer.resize( oldSize );
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
	// Size and capacity management
	//----------------------------------------------

	inline size_t StringBuilder::length() const noexcept
	{
		return m_buffer.size();
	}

	inline void StringBuilder::reserve( size_t newCapacity )
	{
		m_buffer.reserve( newCapacity );
	}

	inline void StringBuilder::resize( size_t newSize )
	{
		m_buffer.resize( newSize );
	}

	//----------------------------------------------
	// Iterator interface
	//----------------------------------------------

	inline StringBuilder::Iterator StringBuilder::begin()
	{
		return m_buffer.data();
	}

	inline StringBuilder::ConstIterator StringBuilder::begin() const
	{
		return m_buffer.data();
	}

	inline StringBuilder::Iterator StringBuilder::end()
	{
		return m_buffer.end();
	}

	inline StringBuilder::ConstIterator StringBuilder::end() const
	{
		return m_buffer.end();
	}

	//=====================================================================
	// StringBuilderLease class
	//=====================================================================

	//----------------------------------------------
	// Construction
	//----------------------------------------------

	inline StringBuilderLease::StringBuilderLease( DynamicStringBuffer* buffer )
		: m_buffer{ buffer },
		  m_valid{ true }
	{
	}

	inline StringBuilderLease::StringBuilderLease( StringBuilderLease&& other ) noexcept
		: m_buffer{ std::exchange( other.m_buffer, nullptr ) },
		  m_valid{ std::exchange( other.m_valid, false ) }
	{
	}

	//----------------------------------------------
	// Assignment
	//----------------------------------------------

	inline StringBuilderLease& StringBuilderLease::operator=( StringBuilderLease&& other ) noexcept
	{
		if ( this != &other )
		{
			dispose();
			m_buffer = std::exchange( other.m_buffer, nullptr );
			m_valid = std::exchange( other.m_valid, false );
		}

		return *this;
	}

	//----------------------------------------------
	// Public interface implementations
	//----------------------------------------------

	inline StringBuilder StringBuilderLease::create()
	{
		if ( !m_valid )
		{
			throwInvalidOperation();
		}

		return StringBuilder{ *m_buffer };
	}

	inline DynamicStringBuffer& StringBuilderLease::buffer()
	{
		if ( !m_valid )
		{
			throwInvalidOperation();
		}

		return *m_buffer;
	}

	inline std::string StringBuilderLease::toString() const
	{
		if ( !m_valid )
		{
			throwInvalidOperation();
		}

		return m_buffer->toString();
	}
} // namespace nfx::string

namespace std
{
	//=====================================================================
	// std::formatter specialization
	//=====================================================================

	template <>
	struct formatter<nfx::string::DynamicStringBuffer>
	{
		constexpr auto parse( std::format_parse_context& ctx )
		{
			return ctx.begin();
		}

		auto format( const nfx::string::DynamicStringBuffer& buf, std::format_context& ctx ) const
		{
			return std::format_to( ctx.out(), "{}", buf.toStringView() );
		}
	};

	template <>
	struct formatter<nfx::string::StringBuilder>
	{
		constexpr auto parse( std::format_parse_context& ctx )
		{
			return ctx.begin();
		}

		auto format( const nfx::string::StringBuilder& builder, std::format_context& ctx ) const
		{
			return std::format_to( ctx.out(), "{}",
				std::string_view( builder.begin(), builder.length() ) );
		}
	};
} // namespace std
