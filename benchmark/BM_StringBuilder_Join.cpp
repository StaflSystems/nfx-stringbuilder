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
 * @file BM_StringBuilder_Join.cpp
 * @brief Join operation benchmarks: StringBuilder::join() vs alternatives
 */

#include <benchmark/benchmark.h>

#include <nfx/string/StringBuilder.h>

#include "BM_Data.h"

#ifdef HAS_ABSEIL
#    include <absl/strings/str_join.h>
#endif

#include <numeric>
#include <vector>

namespace nfx::string::benchmark
{
    //=====================================================================
    // Join benchmarks
    //=====================================================================

    //----------------------------------------------
    // Small strings join
    //----------------------------------------------

    static void BM_StringBuilder_JoinSmall_Comma( ::benchmark::State& state )
    {
        const auto& strings = testdata::smallStrings();
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            builder.join( strings, ", " );
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_ManualLoop_JoinSmall_Comma( ::benchmark::State& state )
    {
        const auto& strings = testdata::smallStrings();
        for( auto _ : state )
        {
            (void)_;
            std::string result;
            for( size_t i = 0; i < strings.size(); ++i )
            {
                if( i > 0 )
                    result += ", ";
                result += strings[i];
            }
            ::benchmark::DoNotOptimize( result );
        }
    }

#ifdef HAS_ABSEIL
    static void BM_Abseil_JoinSmall_Comma( ::benchmark::State& state )
    {
        const auto& strings = testdata::smallStrings();
        for( auto _ : state )
        {
            (void)_;
            std::string result = absl::StrJoin( strings, ", " );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    //----------------------------------------------
    // Medium strings join
    //----------------------------------------------

    static void BM_StringBuilder_JoinMedium_Pipe( ::benchmark::State& state )
    {
        const auto& strings = testdata::mediumStrings();
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            builder.join( strings, " | " );
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_ManualLoop_JoinMedium_Pipe( ::benchmark::State& state )
    {
        const auto& strings = testdata::mediumStrings();
        for( auto _ : state )
        {
            (void)_;
            std::string result;
            for( size_t i = 0; i < strings.size(); ++i )
            {
                if( i > 0 )
                    result += " | ";
                result += strings[i];
            }
            ::benchmark::DoNotOptimize( result );
        }
    }

#ifdef HAS_ABSEIL
    static void BM_Abseil_JoinMedium_Pipe( ::benchmark::State& state )
    {
        const auto& strings = testdata::mediumStrings();
        for( auto _ : state )
        {
            (void)_;
            std::string result = absl::StrJoin( strings, " | " );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    //----------------------------------------------
    // Large strings join
    //----------------------------------------------

    static void BM_StringBuilder_JoinLarge_Newline( ::benchmark::State& state )
    {
        const auto& strings = testdata::largeStrings();
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            builder.join( strings, '\n' );
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_ManualLoop_JoinLarge_Newline( ::benchmark::State& state )
    {
        const auto& strings = testdata::largeStrings();
        for( auto _ : state )
        {
            (void)_;
            std::string result;
            for( size_t i = 0; i < strings.size(); ++i )
            {
                if( i > 0 )
                    result += '\n';
                result += strings[i];
            }
            ::benchmark::DoNotOptimize( result );
        }
    }

#ifdef HAS_ABSEIL
    static void BM_Abseil_JoinLarge_Newline( ::benchmark::State& state )
    {
        const auto& strings = testdata::largeStrings();
        for( auto _ : state )
        {
            (void)_;
            std::string result = absl::StrJoin( strings, "\n" );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    //----------------------------------------------
    // Numeric array join
    //----------------------------------------------

    static void BM_StringBuilder_JoinNumbers( ::benchmark::State& state )
    {
        std::vector<int> numbers( 100 );
        std::iota( numbers.begin(), numbers.end(), 1 );

        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            builder.join( numbers, ", " );
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    static void BM_ManualLoop_JoinNumbers( ::benchmark::State& state )
    {
        std::vector<int> numbers( 100 );
        std::iota( numbers.begin(), numbers.end(), 1 );

        for( auto _ : state )
        {
            (void)_;
            std::string result;
            for( size_t i = 0; i < numbers.size(); ++i )
            {
                if( i > 0 )
                    result += ", ";
                result += std::to_string( numbers[i] );
            }
            ::benchmark::DoNotOptimize( result );
        }
    }

#ifdef HAS_ABSEIL
    static void BM_Abseil_JoinNumbers( ::benchmark::State& state )
    {
        std::vector<int> numbers( 100 );
        std::iota( numbers.begin(), numbers.end(), 1 );

        for( auto _ : state )
        {
            (void)_;
            std::string result = absl::StrJoin( numbers, ", " );
            ::benchmark::DoNotOptimize( result );
        }
    }
#endif

    //----------------------------------------------
    // Empty container edge case
    //----------------------------------------------

    static void BM_StringBuilder_JoinEmpty( ::benchmark::State& state )
    {
        std::vector<std::string> empty;
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            builder.join( empty, ", " );
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

    //----------------------------------------------
    // Single element edge case
    //----------------------------------------------

    static void BM_StringBuilder_JoinSingle( ::benchmark::State& state )
    {
        std::vector<std::string> single = { "OnlyOne" };
        for( auto _ : state )
        {
            (void)_;
            StringBuilder builder;
            builder.join( single, ", " );
            std::string result = builder.toString();
            ::benchmark::DoNotOptimize( result );
        }
    }

} // namespace nfx::string::benchmark

//=====================================================================
// Benchmarks registration
//=====================================================================

//----------------------------------------------
// Small strings join
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_JoinSmall_Comma )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_ManualLoop_JoinSmall_Comma )->Unit( ::benchmark::kNanosecond );
#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_JoinSmall_Comma )->Unit( ::benchmark::kNanosecond );
#endif

//----------------------------------------------
// Medium strings join
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_JoinMedium_Pipe )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_ManualLoop_JoinMedium_Pipe )->Unit( ::benchmark::kNanosecond );
#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_JoinMedium_Pipe )->Unit( ::benchmark::kNanosecond );
#endif

//----------------------------------------------
// Large strings join
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_JoinLarge_Newline )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_ManualLoop_JoinLarge_Newline )->Unit( ::benchmark::kNanosecond );
#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_JoinLarge_Newline )->Unit( ::benchmark::kNanosecond );
#endif

//----------------------------------------------
// Numeric array join
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_JoinNumbers )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_ManualLoop_JoinNumbers )->Unit( ::benchmark::kNanosecond );
#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_JoinNumbers )->Unit( ::benchmark::kNanosecond );
#endif

//----------------------------------------------
// Edge cases
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_JoinEmpty )->Unit( ::benchmark::kNanosecond );
BENCHMARK( nfx::string::benchmark::BM_StringBuilder_JoinSingle )->Unit( ::benchmark::kNanosecond );

//----------------------------------------------
// Benchmark main entry point
//----------------------------------------------

BENCHMARK_MAIN();
