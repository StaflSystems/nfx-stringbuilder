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
 * @file BM_StringBuilder_Core.cpp
 * @brief Core StringBuilder benchmarks: append, prepend, stream operators, basic operations
 */

#include <benchmark/benchmark.h>

#include <nfx/string/StringBuilder.h>

#include "BM_Data.h"

namespace nfx::string::benchmark
{
    //=====================================================================
    // Core StringBuilder operations
    //=====================================================================

    //----------------------------------------------
    // Basic append operations
    //----------------------------------------------

    static void BM_StringBuilder_AppendChar( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( int i = 0; i < 100; ++i )
            {
                builder.append( 'X' );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringBuilder_AppendCString( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( int i = 0; i < 50; ++i )
            {
                builder.append( "Hello " );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringBuilder_AppendStringView( ::benchmark::State& state )
    {
        std::string_view sv = "StringBuilder";
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( int i = 0; i < 50; ++i )
            {
                builder.append( sv );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringBuilder_AppendString( ::benchmark::State& state )
    {
        std::string str = "std::string";
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( int i = 0; i < 50; ++i )
            {
                builder.append( str );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Numeric append operations
    //----------------------------------------------

    static void BM_StringBuilder_AppendInt( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( int i = 0; i < 100; ++i )
            {
                builder.append( i );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringBuilder_AppendDouble( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( int i = 0; i < 100; ++i )
            {
                builder.append( 3.14159 + i * 0.1 );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_StringBuilder_AppendBool( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( int i = 0; i < 100; ++i )
            {
                builder.append( i % 2 == 0 );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Stream operators
    //----------------------------------------------

    static void BM_StringBuilder_StreamOperator( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            builder << "Name: " << "Alice" << ", Age: " << 30 << ", Score: " << 95.5 << ", Active: " << true;
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Prepend operations
    //----------------------------------------------

    static void BM_StringBuilder_Prepend( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( int i = 0; i < 20; ++i )
            {
                builder.prepend( "[PREFIX] " );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Mixed append/prepend
    //----------------------------------------------

    static void BM_StringBuilder_MixedAppendPrepend( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            for( int i = 0; i < 10; ++i )
            {
                builder.append( " [SUFFIX]" );
                builder.prepend( "[PREFIX] " );
            }
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Chained operations
    //----------------------------------------------

    static void BM_StringBuilder_ChainedAppends( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            builder.append( "Name: " )
                .append( "Alice" )
                .append( ", Age: " )
                .append( 30 )
                .append( ", Score: " )
                .append( 95.5 )
                .append( ", Active: " )
                .append( true );
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Clear and reuse
    //----------------------------------------------

    static void BM_StringBuilder_ClearAndReuse( ::benchmark::State& state )
    {
        StringBuilder builder;
        for( auto _ : state )
        {
            (void)_;
            for( int i = 0; i < 10; ++i )
            {
                builder.append( "Iteration " ).append( i ).append( ": Data" );
                auto view = builder.toStringView();
                ::benchmark::DoNotOptimize( view );
                builder.clear();
            }
        }
    }

    //----------------------------------------------
    // toStringView (zero-copy)
    //----------------------------------------------

    static void BM_StringBuilder_ToStringView( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            builder.append( "Test data for string view access" );
            auto view = builder.toStringView();
            ::benchmark::DoNotOptimize( view );
        }
    }

    //----------------------------------------------
    // toString (copy)
    //----------------------------------------------

    static void BM_StringBuilder_ToString( ::benchmark::State& state )
    {
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            builder.append( "Test data for string copy" );
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

} // namespace nfx::string::benchmark

//=====================================================================
// Benchmarks registration
//=====================================================================

//----------------------------------------------
// Basic append operations
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendChar )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendCString )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendStringView )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendString )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Numeric append operations
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendInt )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendDouble )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_AppendBool )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Stream operators
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_StreamOperator )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Prepend operations
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_Prepend )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Mixed append/prepend
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_MixedAppendPrepend )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Chained operations
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_ChainedAppends )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Clear and reuse
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_ClearAndReuse )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// String access
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_ToStringView )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_ToString )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Benchmark main entry point
//----------------------------------------------

BENCHMARK_MAIN();
