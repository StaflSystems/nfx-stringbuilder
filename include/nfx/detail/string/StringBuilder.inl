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

	inline StringBuilder& StringBuilder::operator<<( std::int32_t value )
	{
		char temp[16];
		auto [ptr, ec] = std::to_chars( temp, temp + sizeof( temp ), value );
		if ( ec == std::errc() )
		{
			append( std::string_view( temp, ptr - temp ) );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::operator<<( std::uint32_t value )
	{
		char temp[16];
		auto [ptr, ec] = std::to_chars( temp, temp + sizeof( temp ), value );
		if ( ec == std::errc() )
		{
			append( std::string_view( temp, ptr - temp ) );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::operator<<( std::int64_t value )
	{
		char temp[24];
		auto [ptr, ec] = std::to_chars( temp, temp + sizeof( temp ), value );
		if ( ec == std::errc() )
		{
			append( std::string_view( temp, ptr - temp ) );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::operator<<( std::uint64_t value )
	{
		char temp[24];
		auto [ptr, ec] = std::to_chars( temp, temp + sizeof( temp ), value );
		if ( ec == std::errc() )
		{
			append( std::string_view( temp, ptr - temp ) );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::operator<<( float value )
	{
		char temp[32];
		auto [ptr, ec] = std::to_chars( temp, temp + sizeof( temp ), value );
		if ( ec == std::errc() )
		{
			append( std::string_view( temp, ptr - temp ) );
		}
		return *this;
	}

	inline StringBuilder& StringBuilder::operator<<( double value )
	{
		char temp[32];
		auto [ptr, ec] = std::to_chars( temp, temp + sizeof( temp ), value );
		if ( ec == std::errc() )
		{
			append( std::string_view( temp, ptr - temp ) );
		}
		return *this;
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
