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
 * @file Tests_StringBuilder.cpp
 * @brief UNit tests for nfx-stringbuilder high-performance string building
 * @details Tests covering StringBuilder functionality including construction, capacity management,
 *          data access, append operations, formatting, string conversion, and iterators
 */

#include <gtest/gtest.h>

#include <nfx/string/StringBuilder.h>

#include <algorithm>
#include <string>
#include <string_view>

namespace nfx::string::test
{
    //=====================================================================
    // StringBuilder tests
    //=====================================================================

    //----------------------------------------------
    // Construction
    //----------------------------------------------

    TEST( StringBuilder, DefaultConstructor )
    {
        StringBuilder builder;
        EXPECT_EQ( builder.size(), 0 );
        EXPECT_GT( builder.capacity(), 0 );
        EXPECT_TRUE( builder.isEmpty() );
    }

    TEST( StringBuilder, ConstructorWithInitialCapacity )
    {
        StringBuilder builder( 512 );
        EXPECT_EQ( builder.size(), 0 );
        EXPECT_GE( builder.capacity(), 512 );
        EXPECT_TRUE( builder.isEmpty() );
    }

    TEST( StringBuilder, CopyConstructor )
    {
        StringBuilder original;
        original.append( "Hello, World!" );

        StringBuilder copy( original );
        EXPECT_EQ( copy.size(), original.size() );
        EXPECT_EQ( copy.toString(), "Hello, World!" );
        EXPECT_EQ( copy.toString(), original.toString() );
    }

    TEST( StringBuilder, MoveConstructor )
    {
        StringBuilder original;
        original.append( "Hello, World!" );
        const size_t originalSize = original.size();

        StringBuilder moved( std::move( original ) );
        EXPECT_EQ( moved.size(), originalSize );
        EXPECT_EQ( moved.toString(), "Hello, World!" );
    }

    //----------------------------------------------
    // Assignment
    //----------------------------------------------

    TEST( StringBuilder, CopyAssignment )
    {
        StringBuilder original;
        original.append( "Hello, World!" );

        StringBuilder copy;
        copy = original;
        EXPECT_EQ( copy.size(), original.size() );
        EXPECT_EQ( copy.toString(), "Hello, World!" );
    }

    TEST( StringBuilder, MoveAssignment )
    {
        StringBuilder original;
        original.append( "Hello, World!" );
        const size_t originalSize = original.size();

        StringBuilder moved;
        moved = std::move( original );
        EXPECT_EQ( moved.size(), originalSize );
        EXPECT_EQ( moved.toString(), "Hello, World!" );
    }

    //----------------------------------------------
    // Capacity
    //----------------------------------------------

    TEST( StringBuilder, SizeAndCapacity )
    {
        StringBuilder builder;
        EXPECT_EQ( builder.size(), 0 );

        builder.append( "Test" );
        EXPECT_EQ( builder.size(), 4 );
        EXPECT_GE( builder.capacity(), 4 );
    }

    TEST( StringBuilder, IsEmpty )
    {
        StringBuilder builder;
        EXPECT_TRUE( builder.isEmpty() );

        builder.append( "x" );
        EXPECT_FALSE( builder.isEmpty() );

        builder.clear();
        EXPECT_TRUE( builder.isEmpty() );
    }

    TEST( StringBuilder, Length )
    {
        StringBuilder builder;
        EXPECT_EQ( builder.size(), 0 );

        builder.append( "Hello" );
        EXPECT_EQ( builder.size(), 5 );
        EXPECT_EQ( builder.size(), builder.size() );
    }

    //----------------------------------------------
    // Buffer management
    //----------------------------------------------

    TEST( StringBuilder, Clear )
    {
        StringBuilder builder;
        builder.append( "Some content" );
        const size_t capacityBefore = builder.capacity();

        builder.clear();
        EXPECT_EQ( builder.size(), 0 );
        EXPECT_TRUE( builder.isEmpty() );
        EXPECT_EQ( builder.capacity(), capacityBefore ); // Capacity preserved
    }

    TEST( StringBuilder, Reserve )
    {
        StringBuilder builder;
        builder.reserve( 1024 );
        EXPECT_GE( builder.capacity(), 1024 );
        EXPECT_EQ( builder.size(), 0 );
    }

    TEST( StringBuilder, Resize )
    {
        StringBuilder builder;
        builder.append( "Hello" );

        builder.resize( 10 );
        EXPECT_EQ( builder.size(), 10 );

        builder.resize( 3 );
        EXPECT_EQ( builder.size(), 3 );
    }

    //----------------------------------------------
    // Data access
    //----------------------------------------------

    TEST( StringBuilder, DataPointer )
    {
        StringBuilder builder;
        builder.append( "Hello" );

        const char* data = builder.data();
        EXPECT_NE( data, nullptr );
        EXPECT_EQ( data[0], 'H' );
        EXPECT_EQ( data[4], 'o' );
    }

    TEST( StringBuilder, ConstDataPointer )
    {
        StringBuilder builder;
        builder.append( "World" );

        const StringBuilder& constBuilder = builder;
        const char* data = constBuilder.data();
        EXPECT_NE( data, nullptr );
        EXPECT_EQ( data[0], 'W' );
    }

    TEST( StringBuilder, IndexOperator )
    {
        StringBuilder builder;
        builder.append( "Test" );

        EXPECT_EQ( builder[0], 'T' );
        EXPECT_EQ( builder[1], 'e' );
        EXPECT_EQ( builder[2], 's' );
        EXPECT_EQ( builder[3], 't' );

        builder[0] = 'B';
        EXPECT_EQ( builder[0], 'B' );
    }

    TEST( StringBuilder, ConstIndexOperator )
    {
        StringBuilder builder;
        builder.append( "Hello" );

        const StringBuilder& constBuilder = builder;
        EXPECT_EQ( constBuilder[0], 'H' );
        EXPECT_EQ( constBuilder[4], 'o' );
    }

    //----------------------------------------------
    // Append operations
    //----------------------------------------------

    TEST( StringBuilder, AppendStringView )
    {
        StringBuilder builder;
        std::string_view sv = "Hello";
        builder.append( sv );
        EXPECT_EQ( builder.toString(), "Hello" );
    }

    TEST( StringBuilder, AppendStdString )
    {
        StringBuilder builder;
        std::string str = "World";
        builder.append( str );
        EXPECT_EQ( builder.toString(), "World" );
    }

    TEST( StringBuilder, AppendCString )
    {
        StringBuilder builder;
        builder.append( "C-String" );
        EXPECT_EQ( builder.toString(), "C-String" );
    }

    TEST( StringBuilder, AppendNullCString )
    {
        StringBuilder builder;
        builder.append( static_cast<const char*>( nullptr ) );
        EXPECT_EQ( builder.size(), 0 );
    }

    TEST( StringBuilder, AppendChar )
    {
        StringBuilder builder;
        builder.append( 'A' );
        builder.append( 'B' );
        EXPECT_EQ( builder.toString(), "AB" );
    }

    TEST( StringBuilder, AppendChaining )
    {
        StringBuilder builder;
        builder.append( "Hello" ).append( ", " ).append( "World" ).append( '!' );
        EXPECT_EQ( builder.toString(), "Hello, World!" );
    }

    TEST( StringBuilder, AppendInt8 )
    {
        StringBuilder builder;
        builder.append( static_cast<std::int8_t>( -128 ) );
        EXPECT_EQ( builder.toString(), "-128" );

        builder.clear();
        builder.append( static_cast<std::int8_t>( 127 ) );
        EXPECT_EQ( builder.toString(), "127" );

        builder.clear();
        builder.append( static_cast<std::int8_t>( 0 ) );
        EXPECT_EQ( builder.toString(), "0" );
    }

    TEST( StringBuilder, AppendUInt8 )
    {
        StringBuilder builder;
        builder.append( static_cast<std::uint8_t>( 0 ) );
        EXPECT_EQ( builder.toString(), "0" );

        builder.clear();
        builder.append( static_cast<std::uint8_t>( 255 ) );
        EXPECT_EQ( builder.toString(), "255" );
    }

    TEST( StringBuilder, AppendInt16 )
    {
        StringBuilder builder;
        builder.append( static_cast<std::int16_t>( -32768 ) );
        EXPECT_EQ( builder.toString(), "-32768" );

        builder.clear();
        builder.append( static_cast<std::int16_t>( 32767 ) );
        EXPECT_EQ( builder.toString(), "32767" );
    }

    TEST( StringBuilder, AppendUInt16 )
    {
        StringBuilder builder;
        builder.append( static_cast<std::uint16_t>( 0 ) );
        EXPECT_EQ( builder.toString(), "0" );

        builder.clear();
        builder.append( static_cast<std::uint16_t>( 65535 ) );
        EXPECT_EQ( builder.toString(), "65535" );
    }

    TEST( StringBuilder, AppendInt32 )
    {
        StringBuilder builder;
        builder.append( static_cast<std::int32_t>( -2147483648 ) );
        EXPECT_EQ( builder.toString(), "-2147483648" );

        builder.clear();
        builder.append( static_cast<std::int32_t>( 2147483647 ) );
        EXPECT_EQ( builder.toString(), "2147483647" );

        builder.clear();
        builder.append( static_cast<std::int32_t>( 0 ) );
        EXPECT_EQ( builder.toString(), "0" );
    }

    TEST( StringBuilder, AppendUInt32 )
    {
        StringBuilder builder;
        builder.append( static_cast<std::uint32_t>( 0 ) );
        EXPECT_EQ( builder.toString(), "0" );

        builder.clear();
        builder.append( static_cast<std::uint32_t>( 4294967295 ) );
        EXPECT_EQ( builder.toString(), "4294967295" );
    }

    TEST( StringBuilder, AppendInt64 )
    {
        StringBuilder builder;
        builder.append( static_cast<std::int64_t>( -9223372036854775807LL - 1 ) );
        EXPECT_EQ( builder.toString(), "-9223372036854775808" );

        builder.clear();
        builder.append( static_cast<std::int64_t>( 9223372036854775807LL ) );
        EXPECT_EQ( builder.toString(), "9223372036854775807" );
    }

    TEST( StringBuilder, AppendUInt64 )
    {
        StringBuilder builder;
        builder.append( static_cast<std::uint64_t>( 0 ) );
        EXPECT_EQ( builder.toString(), "0" );

        builder.clear();
        builder.append( static_cast<std::uint64_t>( 18446744073709551615ULL ) );
        EXPECT_EQ( builder.toString(), "18446744073709551615" );
    }

    TEST( StringBuilder, AppendFloat )
    {
        StringBuilder builder;
        builder.append( 3.14159f );
        std::string result = builder.toString();
        EXPECT_FALSE( result.empty() );
        EXPECT_NE( result.find( "3.14" ), std::string::npos );

        builder.clear();
        builder.append( 0.0f );
        EXPECT_EQ( builder.toString(), "0" );

        builder.clear();
        builder.append( -123.456f );
        result = builder.toString();
        EXPECT_NE( result.find( "-123" ), std::string::npos );
    }

    TEST( StringBuilder, AppendDouble )
    {
        StringBuilder builder;
        builder.append( 2.718281828 );
        std::string result = builder.toString();
        EXPECT_FALSE( result.empty() );
        EXPECT_NE( result.find( "2.718" ), std::string::npos );

        builder.clear();
        builder.append( 0.0 );
        EXPECT_EQ( builder.toString(), "0" );

        builder.clear();
        builder.append( -987.654321 );
        result = builder.toString();
        EXPECT_NE( result.find( "-987" ), std::string::npos );
    }

    TEST( StringBuilder, AppendMixedNumericTypes )
    {
        StringBuilder builder;
        builder.append( "int32: " )
            .append( static_cast<std::int32_t>( 42 ) )
            .append( ", uint64: " )
            .append( static_cast<std::uint64_t>( 123456789 ) )
            .append( ", float: " )
            .append( 3.14f );

        std::string result = builder.toString();
        EXPECT_NE( result.find( "int32: 42" ), std::string::npos );
        EXPECT_NE( result.find( "uint64: 123456789" ), std::string::npos );
        EXPECT_NE( result.find( "float: 3.14" ), std::string::npos );
    }

    TEST( StringBuilder, VariadicAppendMultipleStrings )
    {
        StringBuilder builder;
        builder.append( "Hello", ", ", "World", "!" );
        EXPECT_EQ( builder.toString(), "Hello, World!" );
    }

    TEST( StringBuilder, VariadicAppendMixedTypes )
    {
        StringBuilder builder;
        builder.append( "Value: ", 42, ", Name: ", "Test" );
        EXPECT_EQ( builder.toString(), "Value: 42, Name: Test" );
    }

    //----------------------------------------------
    // Prepend operations
    //----------------------------------------------

    TEST( StringBuilder, PrependStringView )
    {
        StringBuilder builder;
        builder.append( "World" );
        std::string_view sv = "Hello ";
        builder.prepend( sv );
        EXPECT_EQ( builder.toString(), "Hello World" );
    }

    TEST( StringBuilder, PrependStdString )
    {
        StringBuilder builder;
        builder.append( "End" );
        std::string str = "Start ";
        builder.prepend( str );
        EXPECT_EQ( builder.toString(), "Start End" );
    }

    TEST( StringBuilder, PrependCString )
    {
        StringBuilder builder;
        builder.append( "Suffix" );
        builder.prepend( "Prefix " );
        EXPECT_EQ( builder.toString(), "Prefix Suffix" );
    }

    TEST( StringBuilder, PrependNullCString )
    {
        StringBuilder builder;
        builder.append( "Content" );
        builder.prepend( static_cast<const char*>( nullptr ) );
        EXPECT_EQ( builder.toString(), "Content" );
    }

    TEST( StringBuilder, PrependChar )
    {
        StringBuilder builder;
        builder.append( "BC" );
        builder.prepend( 'A' );
        EXPECT_EQ( builder.toString(), "ABC" );
    }

    TEST( StringBuilder, PrependChaining )
    {
        StringBuilder builder;
        builder.append( "!" );
        builder.prepend( "World" ).prepend( " " ).prepend( "Hello" );
        EXPECT_EQ( builder.toString(), "Hello World!" );
    }

    TEST( StringBuilder, PrependInt8 )
    {
        StringBuilder builder;
        builder.append( " value" );
        builder.prepend( static_cast<std::int8_t>( -42 ) );
        EXPECT_EQ( builder.toString(), "-42 value" );
    }

    TEST( StringBuilder, PrependUInt8 )
    {
        StringBuilder builder;
        builder.append( " items" );
        builder.prepend( static_cast<std::uint8_t>( 255 ) );
        EXPECT_EQ( builder.toString(), "255 items" );
    }

    TEST( StringBuilder, PrependInt16 )
    {
        StringBuilder builder;
        builder.append( " degrees" );
        builder.prepend( static_cast<std::int16_t>( -1000 ) );
        EXPECT_EQ( builder.toString(), "-1000 degrees" );
    }

    TEST( StringBuilder, PrependUInt16 )
    {
        StringBuilder builder;
        builder.append( " ports" );
        builder.prepend( static_cast<std::uint16_t>( 8080 ) );
        EXPECT_EQ( builder.toString(), "8080 ports" );
    }

    TEST( StringBuilder, PrependInt32 )
    {
        StringBuilder builder;
        builder.append( " units" );
        builder.prepend( static_cast<std::int32_t>( -123456 ) );
        EXPECT_EQ( builder.toString(), "-123456 units" );
    }

    TEST( StringBuilder, PrependUInt32 )
    {
        StringBuilder builder;
        builder.append( " bytes" );
        builder.prepend( static_cast<std::uint32_t>( 987654321 ) );
        EXPECT_EQ( builder.toString(), "987654321 bytes" );
    }

    TEST( StringBuilder, PrependInt64 )
    {
        StringBuilder builder;
        builder.append( " milliseconds" );
        builder.prepend( static_cast<std::int64_t>( -9876543210LL ) );
        EXPECT_EQ( builder.toString(), "-9876543210 milliseconds" );
    }

    TEST( StringBuilder, PrependUInt64 )
    {
        StringBuilder builder;
        builder.append( " records" );
        builder.prepend( static_cast<std::uint64_t>( 9876543210ULL ) );
        EXPECT_EQ( builder.toString(), "9876543210 records" );
    }

    TEST( StringBuilder, PrependFloat )
    {
        StringBuilder builder;
        builder.append( " meters" );
        builder.prepend( 3.14f );
        std::string result = builder.toString();
        EXPECT_NE( result.find( "3.14" ), std::string::npos );
        EXPECT_NE( result.find( "meters" ), std::string::npos );
    }

    TEST( StringBuilder, PrependDouble )
    {
        StringBuilder builder;
        builder.append( " seconds" );
        builder.prepend( 2.718281828 );
        std::string result = builder.toString();
        EXPECT_NE( result.find( "2.718" ), std::string::npos );
        EXPECT_NE( result.find( "seconds" ), std::string::npos );
    }

    TEST( StringBuilder, PrependMixedNumericTypes )
    {
        StringBuilder builder;
        builder.append( " end" );
        builder.prepend( 3.14f )
            .prepend( ", float: " )
            .prepend( static_cast<std::uint64_t>( 123456789 ) )
            .prepend( ", uint64: " )
            .prepend( static_cast<std::int32_t>( 42 ) )
            .prepend( "int32: " );

        std::string result = builder.toString();
        EXPECT_NE( result.find( "int32: 42" ), std::string::npos );
        EXPECT_NE( result.find( "uint64: 123456789" ), std::string::npos );
        EXPECT_NE( result.find( "float: 3.14" ), std::string::npos );
        EXPECT_NE( result.find( "end" ), std::string::npos );
    }

    TEST( StringBuilder, PrependToEmptyBuilder )
    {
        StringBuilder builder;
        builder.prepend( "First" );
        EXPECT_EQ( builder.toString(), "First" );
    }

    TEST( StringBuilder, PrependAndAppendMixed )
    {
        StringBuilder builder;
        builder.append( "Middle" );
        builder.prepend( "Start " );
        builder.append( " End" );
        EXPECT_EQ( builder.toString(), "Start Middle End" );
    }

    TEST( StringBuilder, PrependLargeString )
    {
        StringBuilder builder;
        builder.append( "Suffix" );
        std::string largePrefix( 1000, 'X' );
        builder.prepend( largePrefix );
        EXPECT_EQ( builder.size(), 1006 ); // 1000 + 6
        EXPECT_EQ( builder.toString().substr( 0, 1000 ), largePrefix );
        EXPECT_EQ( builder.toString().substr( 1000 ), "Suffix" );
    }

    TEST( StringBuilder, PrependMultipleTimes )
    {
        StringBuilder builder;
        for( int i = 5; i > 0; --i )
        {
            builder.prepend( std::to_string( i ) ).prepend( " " );
        }
        std::string result = builder.toString();
        EXPECT_EQ( result, " 1 2 3 4 5" );
    }

    TEST( StringBuilder, PrependAfterClear )
    {
        StringBuilder builder;
        builder.append( "Old" );
        builder.clear();
        builder.prepend( "New" );
        EXPECT_EQ( builder.toString(), "New" );
    }

    //----------------------------------------------
    //  Stream operator
    //----------------------------------------------

    TEST( StringBuilder, StreamOperatorStringView )
    {
        StringBuilder builder;
        std::string_view sv = "Hello";
        builder << sv;
        EXPECT_EQ( builder.toString(), "Hello" );
    }

    TEST( StringBuilder, StreamOperatorStdString )
    {
        StringBuilder builder;
        std::string str = "World";
        builder << str;
        EXPECT_EQ( builder.toString(), "World" );
    }

    TEST( StringBuilder, StreamOperatorCString )
    {
        StringBuilder builder;
        builder << "C-String";
        EXPECT_EQ( builder.toString(), "C-String" );
    }

    TEST( StringBuilder, StreamOperatorChar )
    {
        StringBuilder builder;
        builder << 'A' << 'B' << 'C';
        EXPECT_EQ( builder.toString(), "ABC" );
    }

    TEST( StringBuilder, StreamOperatorInt8 )
    {
        StringBuilder builder;
        builder << static_cast<std::int8_t>( -42 );
        EXPECT_EQ( builder.toString(), "-42" );
    }

    TEST( StringBuilder, StreamOperatorUInt8 )
    {
        StringBuilder builder;
        builder << static_cast<std::uint8_t>( 255 );
        EXPECT_EQ( builder.toString(), "255" );
    }

    TEST( StringBuilder, StreamOperatorInt16 )
    {
        StringBuilder builder;
        builder << static_cast<std::int16_t>( -1000 );
        EXPECT_EQ( builder.toString(), "-1000" );
    }

    TEST( StringBuilder, StreamOperatorUInt16 )
    {
        StringBuilder builder;
        builder << static_cast<std::uint16_t>( 60000 );
        EXPECT_EQ( builder.toString(), "60000" );
    }

    TEST( StringBuilder, StreamOperatorInt32 )
    {
        StringBuilder builder;
        builder << static_cast<std::int32_t>( -123456 );
        EXPECT_EQ( builder.toString(), "-123456" );
    }

    TEST( StringBuilder, StreamOperatorUInt32 )
    {
        StringBuilder builder;
        builder << static_cast<std::uint32_t>( 987654321 );
        EXPECT_EQ( builder.toString(), "987654321" );
    }

    TEST( StringBuilder, StreamOperatorInt64 )
    {
        StringBuilder builder;
        builder << static_cast<std::int64_t>( -9876543210LL );
        EXPECT_EQ( builder.toString(), "-9876543210" );
    }

    TEST( StringBuilder, StreamOperatorUInt64 )
    {
        StringBuilder builder;
        builder << static_cast<std::uint64_t>( 9876543210ULL );
        EXPECT_EQ( builder.toString(), "9876543210" );
    }

    TEST( StringBuilder, StreamOperatorFloat )
    {
        StringBuilder builder;
        builder << 1.5f;
        EXPECT_EQ( builder.toString(), "1.5" );
    }

    TEST( StringBuilder, StreamOperatorDouble )
    {
        StringBuilder builder;
        builder << 2.5;
        EXPECT_EQ( builder.toString(), "2.5" );
    }

    TEST( StringBuilder, StreamOperatorChaining )
    {
        StringBuilder builder;
        builder << "Answer: " << 42 << ", Pi: " << 3.14159f;
        std::string result = builder.toString();
        EXPECT_NE( result.find( "Answer: 42" ), std::string::npos );
        EXPECT_NE( result.find( "Pi: 3.14" ), std::string::npos );
    }

    //----------------------------------------------
    //  Format operator
    //----------------------------------------------

    TEST( StringBuilder, FormatBasic )
    {
        StringBuilder builder;
        builder.format( "Hello, {}!", "World" );
        EXPECT_EQ( builder.toString(), "Hello, World!" );
    }

    TEST( StringBuilder, FormatMultipleArgs )
    {
        StringBuilder builder;
        builder.format( "User {} (ID: {}) logged in at {}", "Alice", 42, "10:30" );
        EXPECT_EQ( builder.toString(), "User Alice (ID: 42) logged in at 10:30" );
    }

    TEST( StringBuilder, FormatNumericTypes )
    {
        StringBuilder builder;
        builder.format( "int: {}, float: {:.2f}, hex: {:x}", 123, 3.14159, 255 );
        EXPECT_EQ( builder.toString(), "int: 123, float: 3.14, hex: ff" );
    }

    TEST( StringBuilder, FormatChaining )
    {
        StringBuilder builder;
        builder.append( "Start: " ).format( "{}", 100 ).append( " End" );
        EXPECT_EQ( builder.toString(), "Start: 100 End" );
    }

    //----------------------------------------------
    //  String conversion
    //----------------------------------------------

    TEST( StringBuilder, ToString )
    {
        StringBuilder builder;
        builder.append( "Test String" );
        std::string result = builder.toString();
        EXPECT_EQ( result, "Test String" );
        EXPECT_EQ( result.size(), 11 );
    }

    TEST( StringBuilder, ToStringView )
    {
        StringBuilder builder;
        builder.append( "Test View" );
        std::string_view view = builder.toStringView();
        EXPECT_EQ( view, "Test View" );
        EXPECT_EQ( view.size(), 9 );
    }

    TEST( StringBuilder, ToStringViewZeroCopy )
    {
        StringBuilder builder;
        builder.append( "Zero Copy" );
        std::string_view view = builder.toStringView();
        EXPECT_EQ( view.data(), builder.data() );
    }

    //----------------------------------------------
    //  Iterator
    //----------------------------------------------

    TEST( StringBuilder, BeginEndIterators )
    {
        StringBuilder builder;
        builder.append( "ABC" );

        auto it = builder.begin();
        EXPECT_EQ( *it, 'A' );
        ++it;
        EXPECT_EQ( *it, 'B' );
        ++it;
        EXPECT_EQ( *it, 'C' );
        ++it;
        EXPECT_EQ( it, builder.end() );
    }

    TEST( StringBuilder, ConstIterators )
    {
        StringBuilder builder;
        builder.append( "XYZ" );

        const StringBuilder& constBuilder = builder;
        auto it = constBuilder.begin();
        EXPECT_EQ( *it, 'X' );
        ++it;
        EXPECT_EQ( *it, 'Y' );
        ++it;
        EXPECT_EQ( *it, 'Z' );
        ++it;
        EXPECT_EQ( it, constBuilder.end() );
    }

    TEST( StringBuilder, RangeBasedForLoop )
    {
        StringBuilder builder;
        builder.append( "Test" );

        std::string collected;
        for( char c : builder )
        {
            collected += c;
        }
        EXPECT_EQ( collected, "Test" );
    }

    TEST( StringBuilder, StlAlgorithm )
    {
        StringBuilder builder;
        builder.append( "hello" );

        std::transform( builder.begin(), builder.end(), builder.begin(), []( char c ) { return std::toupper( c ); } );

        EXPECT_EQ( builder.toString(), "HELLO" );
    }

    //----------------------------------------------
    //  Edge cases and integration
    //----------------------------------------------

    TEST( StringBuilder, EmptyBuilder )
    {
        StringBuilder builder;
        EXPECT_TRUE( builder.isEmpty() );
        EXPECT_EQ( builder.size(), 0 );
        EXPECT_EQ( builder.toString(), "" );
        EXPECT_EQ( builder.toStringView(), "" );
    }

    TEST( StringBuilder, LargeStringAppend )
    {
        StringBuilder builder;
        std::string largeString( 10000, 'X' );
        builder.append( largeString );
        EXPECT_EQ( builder.size(), 10000 );
        EXPECT_EQ( builder.toString(), largeString );
    }

    TEST( StringBuilder, SmallBufferOptimization )
    {
        StringBuilder builder;
        std::string smallString( 100, 'S' ); // Should fit in stack buffer
        builder.append( smallString );
        EXPECT_EQ( builder.toString(), smallString );
    }

    TEST( StringBuilder, RepeatedClearAndReuse )
    {
        StringBuilder builder;
        for( int i = 0; i < 100; ++i )
        {
            builder.append( "Test" );
            EXPECT_EQ( builder.size(), 4 );
            builder.clear();
            EXPECT_EQ( builder.size(), 0 );
        }
    }

    TEST( StringBuilder, GrowthBehavior )
    {
        StringBuilder builder;
        size_t lastCapacity = builder.capacity();

        for( int i = 0; i < 1000; ++i )
        {
            builder.append( "X" );
            if( builder.capacity() > lastCapacity )
            {
                lastCapacity = builder.capacity();
            }
        }

        EXPECT_EQ( builder.size(), 1000 );
        EXPECT_GT( builder.capacity(), 1000 );
    }

    TEST( StringBuilder, ComplexChaining )
    {
        StringBuilder builder;
        ( ( builder.append( "Start" ).append( " middle " ).append( 42 ).format( " formatted: {}", "text" ) )
              .append( " end" ) );

        std::string result = builder.toString();
        EXPECT_NE( result.find( "Start" ), std::string::npos );
        EXPECT_NE( result.find( "middle" ), std::string::npos );
        EXPECT_NE( result.find( "42" ), std::string::npos );
        EXPECT_NE( result.find( "formatted: text" ), std::string::npos );
        EXPECT_NE( result.find( "end" ), std::string::npos );
    }

    TEST( StringBuilder, MultipleNumericAppends )
    {
        StringBuilder builder;
        for( int i = 0; i < 10; ++i )
        {
            builder.append( i ).append( " " );
        }
        EXPECT_EQ( builder.toString(), "0 1 2 3 4 5 6 7 8 9 " );
    }

    TEST( StringBuilder, AlternatingTypes )
    {
        StringBuilder builder;
        builder.append( "String" ).append( 123 ).append( 'X' ).append( 3.14f ).append( "End" );

        std::string result = builder.toString();
        EXPECT_NE( result.find( "String" ), std::string::npos );
        EXPECT_NE( result.find( "123" ), std::string::npos );
        EXPECT_NE( result.find( "X" ), std::string::npos );
        EXPECT_NE( result.find( "End" ), std::string::npos );
    }
} // namespace nfx::string::test
