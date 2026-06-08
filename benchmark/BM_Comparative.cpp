/*
 * MIT License
 *
 * Copyright (c) 2026 nfx
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
 * @file BM_Comparative.cpp
 * @brief Comparative benchmarks: StringBuilder vs std::string, std::stringstream, fmt, and Abseil
 */

#include <benchmark/benchmark.h>

#include <nfx/string/StringBuilder.h>

#include "BM_Data.h"

#ifdef HAS_FMT
#    include <fmt/format.h>
#endif

#ifdef HAS_ABSEIL
#    include <absl/strings/str_cat.h>
#    include <absl/strings/str_join.h>
#endif

#if defined( NFX_STRINGBUILDER_HAS_STD_FORMAT )
#    include <format>
#endif
#include <sstream>

namespace nfx::string::benchmark
{
    //=====================================================================
    // Comparative benchmarks: StringBuilder vs Competition
    //=====================================================================

    //----------------------------------------------
    // Small strings
    //----------------------------------------------

    static void BM_StdString_SmallStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::string result;
            for( const auto& str : testdata::smallStrings() )
            {
                result += str;
                result += ' ';
            }
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringStream_SmallStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::ostringstream oss;
            for( const auto& str : testdata::smallStrings() )
            {
                oss << str << ' ';
            }
            std::string result = oss.str();
            ::benchmark::DoNotOptimize( result );
        }
    }

#ifdef HAS_FMT
    static void BM_FmtMemoryBuffer_SmallStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            fmt::memory_buffer buf;
            for( const auto& str : testdata::smallStrings() )
            {
                fmt::format_to( std::back_inserter( buf ), "{} ", str );
            }
            std::string result( buf.data(), buf.size() );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

#ifdef HAS_ABSEIL
    static void BM_Abseil_SmallStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::string result;
            for( const auto& str : testdata::smallStrings() )
            {
                absl::StrAppend( &result, str, " " );
            }
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    static void BM_StringBuilder_SmallStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( const auto& str : testdata::smallStrings() )
            {
                builder.append( str ).append( ' ' );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Medium strings
    //----------------------------------------------

    static void BM_StdString_MediumStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::string result;
            for( const auto& str : testdata::mediumStrings() )
            {
                result += str;
                result += ' ';
            }
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringStream_MediumStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::ostringstream oss;
            for( const auto& str : testdata::mediumStrings() )
            {
                oss << str << ' ';
            }
            std::string result = oss.str();
            ::benchmark::DoNotOptimize( result );
        }
    }

#ifdef HAS_FMT
    static void BM_FmtMemoryBuffer_MediumStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            fmt::memory_buffer buf;
            for( const auto& str : testdata::mediumStrings() )
            {
                fmt::format_to( std::back_inserter( buf ), "{} ", str );
            }
            std::string result( buf.data(), buf.size() );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

#ifdef HAS_ABSEIL
    static void BM_Abseil_MediumStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::string result;
            for( const auto& str : testdata::mediumStrings() )
            {
                absl::StrAppend( &result, str, " " );
            }
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    static void BM_StringBuilder_MediumStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( const auto& str : testdata::mediumStrings() )
            {
                builder.append( str ).append( ' ' );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Large strings
    //----------------------------------------------

    static void BM_StdString_LargeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::string result;
            for( const auto& str : testdata::largeStrings() )
            {
                result += str;
                result += ' ';
            }
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringStream_LargeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::ostringstream oss;
            for( const auto& str : testdata::largeStrings() )
            {
                oss << str << ' ';
            }
            std::string result = oss.str();
            ::benchmark::DoNotOptimize( result );
        }
    }

#ifdef HAS_FMT
    static void BM_FmtMemoryBuffer_LargeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            fmt::memory_buffer buf;
            for( const auto& str : testdata::largeStrings() )
            {
                fmt::format_to( std::back_inserter( buf ), "{} ", str );
            }
            std::string result( buf.data(), buf.size() );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

#ifdef HAS_ABSEIL
    static void BM_Abseil_LargeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::string result;
            for( const auto& str : testdata::largeStrings() )
            {
                absl::StrAppend( &result, str, " " );
            }
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    static void BM_StringBuilder_LargeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( const auto& str : testdata::largeStrings() )
            {
                builder.append( str ).append( ' ' );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Huge strings
    //----------------------------------------------

    static void BM_StdString_HugeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::string result;
            for( const auto& str : testdata::hugeStrings() )
            {
                result += str;
                result += ' ';
            }
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringStream_HugeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::ostringstream oss;
            for( const auto& str : testdata::hugeStrings() )
            {
                oss << str << ' ';
            }
            std::string result = oss.str();
            ::benchmark::DoNotOptimize( result );
        }
    }

#ifdef HAS_FMT
    static void BM_FmtMemoryBuffer_HugeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            fmt::memory_buffer buf;
            for( const auto& str : testdata::hugeStrings() )
            {
                fmt::format_to( std::back_inserter( buf ), "{} ", str );
            }
            std::string result( buf.data(), buf.size() );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

#ifdef HAS_ABSEIL
    static void BM_Abseil_HugeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::string result;
            for( const auto& str : testdata::hugeStrings() )
            {
                absl::StrAppend( &result, str, " " );
            }
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    static void BM_StringBuilder_HugeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( const auto& str : testdata::hugeStrings() )
            {
                builder.append( str ).append( ' ' );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Rapid allocation cycles
    //----------------------------------------------

    static void BM_StdString_RapidCycles( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            for( int i = 0; i < 10; ++i )
            {
                std::string result;
                result += "Cycle ";
                result += std::to_string( i );
                ::benchmark::DoNotOptimize( result );
            }
        }
    }

#ifdef HAS_FMT
    static void BM_FmtMemoryBuffer_RapidCycles( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            for( int i = 0; i < 10; ++i )
            {
                fmt::memory_buffer buf;
                fmt::format_to( std::back_inserter( buf ), "Cycle {}", i );
                std::string result( buf.data(), buf.size() );
                ::benchmark::DoNotOptimize( result );
            }
        }
    }
#endif

#ifdef HAS_ABSEIL
    static void BM_Abseil_RapidCycles( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            for( int i = 0; i < 10; ++i )
            {
                std::string result = absl::StrCat( "Cycle ", i );
                ::benchmark::DoNotOptimize( result );
            }
        }
    }
#endif

    static void BM_StringBuilder_RapidCycles( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            for( int i = 0; i < 10; ++i )
            {
                StringBuilder builder;
                builder.append( "Cycle " ).append( i );
                std::string result = builder.toString();
                ::benchmark::DoNotOptimize( result );
            }
        }
    }

    //----------------------------------------------
    // Mixed operations
    //----------------------------------------------

    static void BM_StdString_MixedOperations( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::string result;
            result += "ID: ";
            result += std::to_string( 12345 );
            result += ", Name: ";
            result += "John";
            result += ", Score: ";
            result += std::to_string( 98.5 );
            result += ", Active: ";
            result += "true";
            result += ", Tags: [";
            result += "A, B, C";
            result += "]";
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringStream_MixedOperations( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::ostringstream oss;
            oss << "ID: " << 12345 << ", Name: " << "John" << ", Score: " << 98.5 << ", Active: " << true
                << ", Tags: [A, B, C]";
            std::string result = oss.str();
            ::benchmark::DoNotOptimize( result );
        }
    }

#ifdef HAS_FMT
    static void BM_FmtMemoryBuffer_MixedOperations( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            fmt::memory_buffer buf;
            fmt::format_to(
                std::back_inserter( buf ),
                "ID: {}, Name: {}, Score: {}, Active: {}, Tags: [A, B, C]",
                12345,
                "John",
                98.5,
                true );
            std::string result( buf.data(), buf.size() );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

#ifdef HAS_ABSEIL
    static void BM_Abseil_MixedOperations( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::string result = absl::StrCat(
                "ID: ", 12345, ", Name: ", "John", ", Score: ", 98.5, ", Active: ", true, ", Tags: [A, B, C]" );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    static void BM_StringBuilder_MixedOperations( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            builder.append( "ID: " )
                .append( 12345 )
                .append( ", Name: " )
                .append( "John" )
                .append( ", Score: " )
                .append( 98.5 )
                .append( ", Active: " )
                .append( true )
                .append( ", Tags: [A, B, C]" );
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Formatting
    //----------------------------------------------

    static void BM_StdFormat_ComplexFormatting( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
#if defined( NFX_STRINGBUILDER_HAS_STD_FORMAT )
            std::string result =
                std::format( "User {} (ID: {}) scored {:.2f} points at {}", "Alice", 42, 95.75, "2024-01-15 10:30:00" );
            ::benchmark::DoNotOptimize( result );
#endif
        }
    }

#ifdef HAS_FMT
    static void BM_FmtFormat_ComplexFormatting( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::string result =
                fmt::format( "User {} (ID: {}) scored {:.2f} points at {}", "Alice", 42, 95.75, "2024-01-15 10:30:00" );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

#ifdef HAS_ABSEIL
    static void BM_Abseil_ComplexFormatting( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            std::string result = absl::StrCat(
                "User ", "Alice", " (ID: ", 42, ") scored ", 95.75, " points at ", "2024-01-15 10:30:00" );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    static void BM_StringBuilder_WithFormat( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
#if defined( NFX_STRINGBUILDER_HAS_STD_FORMAT )
            StringBuilder builder;
            builder.append( std::format(
                "User {} (ID: {}) scored {:.2f} points at {}", "Alice", 42, 95.75, "2024-01-15 10:30:00" ) );
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
#endif
        }
    }

    static void BM_StringBuilder_ManualFormatting( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            builder.append( "User " )
                .append( "Alice" )
                .append( " (ID: " )
                .append( 42 )
                .append( ") scored " )
                .append( 95.75 )
                .append( " points at " )
                .append( "2024-01-15 10:30:00" );
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

} // namespace nfx::string::benchmark

//=====================================================================
// Benchmarks registration
//=====================================================================

//----------------------------------------------
// Small strings
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_SmallStrings )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringStream_SmallStrings )->Unit( ::benchmark::kNanosecond );
#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_SmallStrings )->Unit( ::benchmark::kNanosecond );
#endif
#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_SmallStrings )->Unit( ::benchmark::kNanosecond );
#endif
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_SmallStrings )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Medium strings
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_MediumStrings )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringStream_MediumStrings )->Unit( ::benchmark::kNanosecond );
#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_MediumStrings )->Unit( ::benchmark::kNanosecond );
#endif
#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_MediumStrings )->Unit( ::benchmark::kNanosecond );
#endif
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_MediumStrings )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Large strings
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_LargeStrings )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringStream_LargeStrings )->Unit( ::benchmark::kNanosecond );
#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_LargeStrings )->Unit( ::benchmark::kNanosecond );
#endif
#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_LargeStrings )->Unit( ::benchmark::kNanosecond );
#endif
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_LargeStrings )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Huge strings
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_HugeStrings )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringStream_HugeStrings )->Unit( ::benchmark::kNanosecond );
#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_HugeStrings )->Unit( ::benchmark::kNanosecond );
#endif
#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_HugeStrings )->Unit( ::benchmark::kNanosecond );
#endif
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_HugeStrings )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Rapid allocation cycles
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_RapidCycles )->Unit( ::benchmark::kNanosecond );
#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_RapidCycles )->Unit( ::benchmark::kNanosecond );
#endif
#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_RapidCycles )->Unit( ::benchmark::kNanosecond );
#endif
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_RapidCycles )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Mixed operations
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_MixedOperations )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringStream_MixedOperations )->Unit( ::benchmark::kNanosecond );
#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_MixedOperations )->Unit( ::benchmark::kNanosecond );
#endif
#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_MixedOperations )->Unit( ::benchmark::kNanosecond );
#endif
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_MixedOperations )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Formatting
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdFormat_ComplexFormatting )->Unit( ::benchmark::kNanosecond );
#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtFormat_ComplexFormatting )->Unit( ::benchmark::kNanosecond );
#endif
#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_ComplexFormatting )->Unit( ::benchmark::kNanosecond );
#endif
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_WithFormat )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_ManualFormatting )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Benchmark main entry point
//----------------------------------------------

BENCHMARK_MAIN();
