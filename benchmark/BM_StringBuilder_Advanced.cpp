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
 * @file BM_StringBuilder_Advanced.cpp
 * @brief Advanced benchmarks: capacity hints, buffer reuse, zero-allocation patterns, memory pressure
 */

#include <benchmark/benchmark.h>

#include <nfx/string/StringBuilder.h>

#include "BM_Data.h"

namespace nfx::string::benchmark
{
    //=====================================================================
    // Advanced StringBuilder benchmarks
    //=====================================================================

    //----------------------------------------------
    // Capacity hints
    //----------------------------------------------

    static void BM_StringBuilder_NoHint( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
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
            (void)_;
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
    // Buffer reuse
    //----------------------------------------------

    static void BM_StringBuilder_BufferReuse( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
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

    //----------------------------------------------
    // Zero-allocation patterns
    //----------------------------------------------

    static void BM_StringBuilder_ZeroAlloc( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;

            for( const auto& str : testdata::smallStrings() )
            {
                builder.append( str ).append( " " );
            }

            auto view = builder.toStringView();
            ::benchmark::DoNotOptimize( view );
        }
    }

    //----------------------------------------------
    // Memory pressure
    //----------------------------------------------

    static void BM_StringBuilder_MemoryPressure( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
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
// Capacity hints
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_NoHint )->Unit( ::benchmark::kMicrosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_WithHint )->Unit( ::benchmark::kMicrosecond );

//----------------------------------------------
// Buffer reuse
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_BufferReuse )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Zero-allocation patterns
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_ZeroAlloc )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Memory pressure
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_MemoryPressure )->Unit( ::benchmark::kMicrosecond );

//----------------------------------------------
// Benchmark main entry point
//----------------------------------------------

BENCHMARK_MAIN();
