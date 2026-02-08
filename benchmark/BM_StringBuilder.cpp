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
 * @file BM_StringBuilder.cpp
 * @brief Benchmark StringBuilder performance against std::string, std::stringstream, std::format, fmt, and Abseil
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

#include <format>
#include <sstream>

namespace nfx::string::benchmark
{
    //=====================================================================
    // StringBuilder benchmark suite
    //=====================================================================

    //----------------------------------------------
    // Small strings
    //----------------------------------------------

    static void BM_StdString_SmallStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            std::string result;
            for( const auto& str : testdata::smallStrings() )
            {
                result += str;
                result += " ";
            }
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringStream_SmallStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            std::ostringstream oss;
            for( const auto& str : testdata::smallStrings() )
            {
                oss << str << " ";
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
            fmt::memory_buffer buffer;
            for( const auto& str : testdata::smallStrings() )
            {
                fmt::format_to( std::back_inserter( buffer ), "{} ", str );
            }
            std::string result = fmt::to_string( buffer );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

#ifdef HAS_ABSEIL
    static void BM_Abseil_SmallStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
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
            StringBuilder builder;
            for( const auto& str : testdata::smallStrings() )
            {
                builder.append( str ).append( " " );
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
            std::string result;
            for( const auto& str : testdata::mediumStrings() )
            {
                result += str;
                result += " ";
            }
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringStream_MediumStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            std::ostringstream oss;
            for( const auto& str : testdata::mediumStrings() )
            {
                oss << str << " ";
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
            fmt::memory_buffer buffer;
            for( const auto& str : testdata::mediumStrings() )
            {
                fmt::format_to( std::back_inserter( buffer ), "{} ", str );
            }
            std::string result = fmt::to_string( buffer );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

#ifdef HAS_ABSEIL
    static void BM_Abseil_MediumStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
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
            StringBuilder builder;
            for( const auto& str : testdata::mediumStrings() )
            {
                builder.append( str ).append( " " );
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
            std::string result;
            for( const auto& str : testdata::largeStrings() )
            {
                result += str;
                result += "\n";
            }
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringStream_LargeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            std::ostringstream oss;
            for( const auto& str : testdata::largeStrings() )
            {
                oss << str << "\n";
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
            fmt::memory_buffer buffer;
            for( const auto& str : testdata::largeStrings() )
            {
                fmt::format_to( std::back_inserter( buffer ), "{}\n", str );
            }
            std::string result = fmt::to_string( buffer );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

#ifdef HAS_ABSEIL
    static void BM_Abseil_LargeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            std::string result;

            for( const auto& str : testdata::largeStrings() )
            {
                absl::StrAppend( &result, str, "\n" );
            }

            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    static void BM_StringBuilder_LargeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            StringBuilder builder;
            for( const auto& str : testdata::largeStrings() )
            {
                builder.append( str ).append( "\n" );
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
            std::string result;
            for( const auto& str : testdata::hugeStrings() )
            {
                result += str;
                result += "\n";
            }
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringStream_HugeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            std::ostringstream oss;
            for( const auto& str : testdata::hugeStrings() )
            {
                oss << str << "\n";
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
            fmt::memory_buffer buffer;
            for( const auto& str : testdata::hugeStrings() )
            {
                fmt::format_to( std::back_inserter( buffer ), "{}\n", str );
            }
            std::string result = fmt::to_string( buffer );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

#ifdef HAS_ABSEIL
    static void BM_Abseil_HugeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            std::string result;

            for( const auto& str : testdata::hugeStrings() )
            {
                absl::StrAppend( &result, str, "\n" );
            }

            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    static void BM_StringBuilder_HugeStrings( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            StringBuilder builder;
            for( const auto& str : testdata::hugeStrings() )
            {
                builder.append( str ).append( "\n" );
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
            for( int i = 0; i < 10; ++i )
            {
                std::string result = "Iteration ";
                result += std::to_string( i );
                result += ": ";
                result += testdata::smallStrings()[i % testdata::smallStrings().size()];
                ::benchmark::DoNotOptimize( result );
            }
        }
    }

#ifdef HAS_FMT
    static void BM_FmtMemoryBuffer_RapidCycles( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            for( int i = 0; i < 10; ++i )
            {
                fmt::memory_buffer buffer;
                fmt::format_to(
                    std::back_inserter( buffer ),
                    "Iteration {}: {}",
                    i,
                    testdata::smallStrings()[i % testdata::smallStrings().size()] );
                std::string result = fmt::to_string( buffer );
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
            for( int i = 0; i < 10; ++i )
            {
                std::string result = absl::StrCat(
                    "Iteration ", i, ": ", testdata::smallStrings()[i % testdata::smallStrings().size()] );
                ::benchmark::DoNotOptimize( result );
            }
        }
    }
#endif

    static void BM_StringBuilder_RapidCycles( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            for( int i = 0; i < 10; ++i )
            {
                StringBuilder builder;
                builder.append( "Iteration " )
                    .append( i )
                    .append( ": " )
                    .append( testdata::smallStrings()[i % testdata::smallStrings().size()] );
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
            std::string result = "Header: ";
            result += testdata::mediumStrings()[0];
            result += "\n";

            for( size_t i = 0; i < testdata::smallStrings().size(); ++i )
            {
                result += "Item ";
                result += i;
                result += ": ";
                result += testdata::smallStrings()[i];
                result += "\n";
            }

            result += "Footer: ";
            result += testdata::mediumStrings()[1];

            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringStream_MixedOperations( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            std::ostringstream oss;
            oss << "Header: " << testdata::mediumStrings()[0] << "\n";

            for( size_t i = 0; i < testdata::smallStrings().size(); ++i )
            {
                oss << "Item " << i << ": " << testdata::smallStrings()[i] << "\n";
            }

            oss << "Footer: " << testdata::mediumStrings()[1];

            std::string result = oss.str();
            ::benchmark::DoNotOptimize( result );
        }
    }

#ifdef HAS_FMT
    static void BM_FmtMemoryBuffer_MixedOperations( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            fmt::memory_buffer buffer;
            fmt::format_to( std::back_inserter( buffer ), "Header: {}\n", testdata::mediumStrings()[0] );

            for( size_t i = 0; i < testdata::smallStrings().size(); ++i )
            {
                fmt::format_to( std::back_inserter( buffer ), "Item {}: {}\n", i, testdata::smallStrings()[i] );
            }

            fmt::format_to( std::back_inserter( buffer ), "Footer: {}", testdata::mediumStrings()[1] );

            std::string result = fmt::to_string( buffer );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

#ifdef HAS_ABSEIL
    static void BM_Abseil_MixedOperations( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            std::string result;

            absl::StrAppend( &result, "Header: " );
            absl::StrAppend( &result, testdata::mediumStrings()[0] );
            absl::StrAppend( &result, "\n" );

            for( size_t i = 0; i < testdata::smallStrings().size(); ++i )
            {
                absl::StrAppend( &result, "Item " );
                absl::StrAppend( &result, std::to_string( i ) );
                absl::StrAppend( &result, ": " );
                absl::StrAppend( &result, testdata::smallStrings()[i] );
                absl::StrAppend( &result, "\n" );
            }

            absl::StrAppend( &result, "Footer: " );
            absl::StrAppend( &result, testdata::mediumStrings()[1] );

            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    static void BM_StringBuilder_MixedOperations( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            StringBuilder builder;
            builder.append( "Header: " ).append( testdata::mediumStrings()[0] ).append( "\n" );

            for( size_t i = 0; i < testdata::smallStrings().size(); ++i )
            {
                builder.append( "Item " )
                    .append( i )
                    .append( ": " )
                    .append( testdata::smallStrings()[i] )
                    .append( "\n" );
            }

            builder.append( "Footer: " ).append( testdata::mediumStrings()[1] );

            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Capacity hints
    //----------------------------------------------

    static void BM_StringBuilder_NoHint( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            StringBuilder builder;

            for( int i = 0; i < 50; ++i )
            {
                builder.append( "Item " )
                    .append( i )
                    .append( ": " )
                    .append( testdata::mediumStrings()[i % testdata::mediumStrings().size()] )
                    .append( "\n" );
            }

            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringBuilder_WithHint( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            StringBuilder builder( 4096 );

            for( int i = 0; i < 50; ++i )
            {
                builder.append( "Item " )
                    .append( i )
                    .append( ": " )
                    .append( testdata::mediumStrings()[i % testdata::mediumStrings().size()] )
                    .append( "\n" );
            }

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
            std::string result;

            for( int i = 0; i < 10; ++i )
            {
                result += std::format( "User {} (ID: {:08}) - Score: {:.2f}\n", "Alice", i, 99.5 + i * 0.1 );
            }

            ::benchmark::DoNotOptimize( result );
        }
    }

#ifdef HAS_FMT
    static void BM_FmtFormat_ComplexFormatting( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            std::string result;

            for( int i = 0; i < 10; ++i )
            {
                result += fmt::format( "User {} (ID: {:08}) - Score: {:.2f}\n", "Alice", i, 99.5 + i * 0.1 );
            }

            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

#ifdef HAS_ABSEIL
    static void BM_Abseil_ComplexFormatting( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            std::string result;

            for( int i = 0; i < 10; ++i )
            {
                absl::StrAppend(
                    &result, std::format( "User {} (ID: {:08}) - Score: {:.2f}\n", "Alice", i, 99.5 + i * 0.1 ) );
            }

            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    static void BM_StringBuilder_WithFormat( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            StringBuilder builder;

            for( int i = 0; i < 10; ++i )
            {
                builder.append( std::format( "User {} (ID: {:08}) - Score: {:.2f}\n", "Alice", i, 99.5 + i * 0.1 ) );
            }

            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringBuilder_ManualFormatting( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            StringBuilder builder;

            for( int i = 0; i < 10; ++i )
            {
                builder.append( "User Alice (ID: " )
                    .append( i )
                    .append( ") - Score: " )
                    .append( 99.5 + i * 0.1 )
                    .append( "\n" );
            }

            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Advanced
    //----------------------------------------------

    static void BM_StringBuilder_BufferReuse( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            for( int cycle = 0; cycle < 5; ++cycle )
            {
                StringBuilder builder;

                for( const auto& str : testdata::mediumStrings() )
                {
                    builder.append( "Cycle " ).append( cycle ).append( ": " ).append( str ).append( " " );
                }

                auto view = builder.toStringView();
                ::benchmark::DoNotOptimize( view.data() );
                ::benchmark::DoNotOptimize( view.size() );
            }
        }
    }

    static void BM_StringBuilder_ZeroAlloc( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            StringBuilder builder;

            for( const auto& str : testdata::smallStrings() )
            {
                builder.append( str ).append( " " );
            }

            auto view = builder.toStringView();
            ::benchmark::DoNotOptimize( view );
        }
    }

    static void BM_StringBuilder_MemoryPressure( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            StringBuilder builder;

            for( int i = 0; i < 20; ++i )
            {
                for( const auto& str : testdata::largeStrings() )
                {
                    builder.append( str ).append( " " );
                }
            }

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
// Capacity hints
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_NoHint )->Unit( ::benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_WithHint )->Unit( ::benchmark::kNanosecond );

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
// Advanced
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_BufferReuse )->Unit( ::benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_ZeroAlloc )->Unit( ::benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_MemoryPressure )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Benchmark main entry point
//----------------------------------------------

BENCHMARK_MAIN();
