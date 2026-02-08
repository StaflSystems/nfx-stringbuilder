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
 * @file BM_StringBuilder_Lines.cpp
 * @brief Line operation benchmarks: appendLine() / appendLn() vs manual patterns
 */

#include <benchmark/benchmark.h>

#include <nfx/string/StringBuilder.h>

#include "BM_Data.h"

namespace nfx::string::benchmark
{
    //=====================================================================
    // Line operation benchmarks
    //=====================================================================

    //----------------------------------------------
    // appendLine vs manual append + newline
    //----------------------------------------------

    static void BM_StringBuilder_AppendLine_Small( ::benchmark::State& state )
    {
        const auto& strings = testdata::smallStrings();
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( const auto& str : strings )
            {
                builder.appendLine( str );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringBuilder_AppendManual_Small( ::benchmark::State& state )
    {
        const auto& strings = testdata::smallStrings();
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( const auto& str : strings )
            {
                builder.append( str ).append( '\n' );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Medium strings
    //----------------------------------------------

    static void BM_StringBuilder_AppendLine_Medium( ::benchmark::State& state )
    {
        const auto& strings = testdata::mediumStrings();
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( const auto& str : strings )
            {
                builder.appendLine( str );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringBuilder_AppendManual_Medium( ::benchmark::State& state )
    {
        const auto& strings = testdata::mediumStrings();
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( const auto& str : strings )
            {
                builder.append( str ).append( '\n' );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Large strings
    //----------------------------------------------

    static void BM_StringBuilder_AppendLine_Large( ::benchmark::State& state )
    {
        const auto& strings = testdata::largeStrings();
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( const auto& str : strings )
            {
                builder.appendLine( str );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringBuilder_AppendManual_Large( ::benchmark::State& state )
    {
        const auto& strings = testdata::largeStrings();
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( const auto& str : strings )
            {
                builder.append( str ).append( '\n' );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Log-like workload (mixed data)
    //----------------------------------------------

    static void BM_StringBuilder_LogWorkload( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( int i = 0; i < 100; ++i )
            {
                builder.appendLine( "[INFO] Processing item " ).append( i ).appendLine( " with status: OK" );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // CSV-like workload (rows of data)
    //----------------------------------------------

    static void BM_StringBuilder_CsvWorkload( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            builder.appendLine( "id,name,score,active" );
            for( int i = 0; i < 50; ++i )
            {
                builder.append( i )
                    .append( ",User" )
                    .append( i )
                    .append( "," )
                    .append( 95.5 + i * 0.1 )
                    .appendLine( ",true" );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // NMEA-like protocol workload
    //----------------------------------------------

    static void BM_StringBuilder_NmeaWorkload( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( int i = 0; i < 20; ++i )
            {
                builder.append( "$GPGGA," )
                    .append( "123519.00," )
                    .append( "4807.038,N," )
                    .append( "01131.000,E," )
                    .appendLine( "1,08,0.9,545.4,M,46.9,M,,*47" );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Empty line edge case
    //----------------------------------------------

    static void BM_StringBuilder_AppendLine_Empty( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( int i = 0; i < 100; ++i )
            {
                builder.appendLine( "" );
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

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendLine_Small )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendManual_Small )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Medium strings
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendLine_Medium )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendManual_Medium )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Large strings
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendLine_Large )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendManual_Large )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Workloads
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_LogWorkload )->Unit( ::benchmark::kMicrosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_CsvWorkload )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_NmeaWorkload )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Edge cases
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendLine_Empty )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Benchmark main entry point
//----------------------------------------------

BENCHMARK_MAIN();
