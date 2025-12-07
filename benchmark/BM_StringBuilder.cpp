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
 * @brief Benchmark StringBuilder performance vs standard alternatives
 */

#include <benchmark/benchmark.h>

#include <sstream>
#include <string>
#include <vector>

#include <nfx/string/StringBuilder.h>

namespace nfx::string::benchmark
{
	//=====================================================================
	// StringBuilder benchmark suite
	//=====================================================================

	//----------------------------------------------
	// Test data
	//----------------------------------------------

	static const std::vector<std::string_view> small_strings = {
		"Hello", "World", "Test", "String", "Builder" };

	static const std::vector<std::string_view> medium_strings = {
		"This is a medium length string for testing purposes",
		"Another medium string with different content and length",
		"Performance testing requires consistent data patterns",
		"Medium strings help measure real-world usage scenarios" };

	static const std::vector<std::string_view> large_strings = {
		"This is a much longer string that contains significantly more characters and is designed to test the performance characteristics of string building operations when dealing with larger amounts of text content that might be more representative of real-world applications",
		"Large strings like this one are important for benchmarking because they help us understand how the string builder performs when concatenating substantial amounts of text data, which is common in applications that generate reports, logs, or formatted output",
		"Performance analysis of string concatenation with longer strings reveals different behavior patterns compared to small string operations, particularly in terms of memory allocation patterns and buffer management strategies" };

	//----------------------------------------------
	// StringBuilder vs std::string concatenation
	//----------------------------------------------

	//----------------------------
	// Small strings
	//----------------------------

	static void BM_StdString_SmallStrings( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;

			for ( const auto& str : small_strings )
			{
				result += str;
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringStream_SmallStrings( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::ostringstream oss;

			for ( const auto& str : small_strings )
			{
				oss << str;
			}

			std::string result = oss.str();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringBuilder_SmallStrings( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			for ( const auto& str : small_strings )
			{
				builder << str;
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------
	// Medium strings
	//----------------------------

	static void BM_StdString_MediumStrings( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;

			for ( const auto& str : medium_strings )
			{
				result += str;
				result += " ";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringStream_MediumStrings( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::ostringstream oss;

			for ( const auto& str : medium_strings )
			{
				oss << str << " ";
			}

			std::string result = oss.str();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringBuilder_MediumStrings( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			for ( const auto& str : medium_strings )
			{
				builder << str << " ";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------
	// Large strings
	//----------------------------

	static void BM_StdString_LargeStrings( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;

			for ( const auto& str : large_strings )
			{
				result += str;
				result += "\n";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringStream_LargeStrings( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::ostringstream oss;

			for ( const auto& str : large_strings )
			{
				oss << str << "\n";
			}

			std::string result = oss.str();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringBuilder_LargeStrings( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			for ( const auto& str : large_strings )
			{
				builder << str << "\n";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Pool efficiency - rapid lease/return cycles
	//----------------------------------------------

	static void BM_StdString_RapidCycles( ::benchmark::State& state )
	{
		// Equivalent rapid string building without pooling
		for ( auto _ : state )
		{
			for ( int i = 0; i < 10; ++i )
			{
				std::string result = "Iteration ";
				result += std::to_string( i );
				result += ": ";
				result += small_strings[i % small_strings.size()];
				::benchmark::DoNotOptimize( result );
			}
		}
	}

	static void BM_StringBuilder_PoolEfficiency( ::benchmark::State& state )
	{
		// Test pool efficiency with rapid lease/return cycles
		for ( auto _ : state )
		{
			for ( int i = 0; i < 10; ++i )
			{
				auto lease = StringBuilderPool::lease();
				auto builder = lease.create();
				builder << "Iteration " << i << ": " << small_strings[i % small_strings.size()];

				// Access buffer without string copy to truly test pooling efficiency
				auto view = lease.buffer().toStringView();
				::benchmark::DoNotOptimize( view.data() );
				::benchmark::DoNotOptimize( view.size() );
			}
		}
	}

	//----------------------------------------------
	// Mixed operations - realistic usage patterns
	//----------------------------------------------

	static void BM_StdString_MixedOperations( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result = "Header: ";
			result += medium_strings[0];
			result += "\n";

			for ( size_t i = 0; i < small_strings.size(); ++i )
			{
				result += "Item ";
				result += std::to_string( i );
				result += ": ";
				result += small_strings[i];
				result += "\n";
			}

			result += "Footer: ";
			result += medium_strings[1];

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringStream_MixedOperations( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::ostringstream oss;

			// Simulate realistic string building patterns
			oss << "Header: " << medium_strings[0] << "\n";

			for ( size_t i = 0; i < small_strings.size(); ++i )
			{
				oss << "Item " << std::to_string( i ) << ": " << small_strings[i] << "\n";
			}

			oss << "Footer: " << medium_strings[1];

			std::string result = oss.str();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringBuilder_MixedOperations( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			// Simulate realistic string building patterns
			builder << "Header: " << medium_strings[0] << "\n";

			for ( size_t i = 0; i < small_strings.size(); ++i )
			{
				builder << "Item " << i << ": " << small_strings[i] << "\n";
			}

			builder << "Footer: " << medium_strings[1];

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Capacity hints
	//----------------------------------------------

	static void BM_StringBuilder_NoHint( ::benchmark::State& state )
	{
		// Build large string without capacity hint (multiple reallocations)
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			for ( int i = 0; i < 50; ++i )
			{
				builder << "Item " << i << ": " << medium_strings[i % medium_strings.size()] << "\n";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringBuilder_WithHint( ::benchmark::State& state )
	{
		// Build large string with capacity hint (no reallocations)
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease( 4096 ); // Pre-allocate 4KB
			auto builder = lease.create();

			for ( int i = 0; i < 50; ++i )
			{
				builder << "Item " << i << ": " << medium_strings[i % medium_strings.size()] << "\n";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// format() method
	//----------------------------------------------

	static void BM_StdFormat_ComplexFormatting( ::benchmark::State& state )
	{
		// Baseline: std::format directly
		for ( auto _ : state )
		{
			std::string result;

			for ( int i = 0; i < 10; ++i )
			{
				result += std::format( "User {} (ID: {:08}) - Score: {:.2f}\n",
					"Alice", i, 99.5 + i * 0.1 );
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringBuilder_FormatMethod( ::benchmark::State& state )
	{
		// StringBuilder.format() using std::format_to with back_inserter
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			for ( int i = 0; i < 10; ++i )
			{
				builder.format( "User {} (ID: {:08}) - Score: {:.2f}\n",
					"Alice", i, 99.5 + i * 0.1 );
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringBuilder_ManualFormatting( ::benchmark::State& state )
	{
		// Manual concatenation without format() for comparison
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			for ( int i = 0; i < 10; ++i )
			{
				builder << "User Alice (ID: " << i << ") - Score: " << ( 99.5 + i * 0.1 ) << "\n";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Variadic append operations
	//----------------------------------------------

	static void BM_StringBuilder_MultipleAppends( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			builder.append( "SELECT " )
				.append( "id" )
				.append( ", " )
				.append( "name" )
				.append( ", " )
				.append( "email" )
				.append( " FROM users WHERE status = '" )
				.append( "active" )
				.append( "'" );

			::benchmark::DoNotOptimize( builder.length() );
		}
	}

	static void BM_StringBuilder_VariadicAppend( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			builder.append( "SELECT ", "id", ", ", "name", ", ", "email",
				" FROM users WHERE status = '", "active", "'" );

			::benchmark::DoNotOptimize( builder.length() );
		}
	}

	static void BM_StringBuilder_MultipleAppends_WithNumeric( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			builder.append( "User " )
				.append( 12345 )
				.append( " (" )
				.append( "Alice" )
				.append( ") logged in at " )
				.append( 1735000000 )
				.append( " with score " )
				.append( 99.5 )
				.append( "%" );

			::benchmark::DoNotOptimize( builder.length() );
		}
	}

	static void BM_StringBuilder_VariadicAppend_WithNumeric( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			builder.append( "User ", 12345, " (", "Alice", ") logged in at ",
				1735000000, " with score ", 99.5, "%" );

			::benchmark::DoNotOptimize( builder.length() );
		}
	}

	//----------------------------------------------
	// Advanced
	//----------------------------------------------

	//----------------------------
	// Buffer reuse
	//----------------------------

	static void BM_StringBuilder_BufferReuse( ::benchmark::State& state )
	{
		// Test buffer reuse efficiency - multiple builds reusing same pooled buffer
		for ( auto _ : state )
		{
			for ( int cycle = 0; cycle < 5; ++cycle )
			{
				auto lease = StringBuilderPool::lease();
				auto builder = lease.create();

				for ( const auto& str : medium_strings )
				{
					builder << "Cycle " << cycle << ": " << str << " ";
				}

				// Access buffer without string copy
				auto view = lease.buffer().toStringView();
				::benchmark::DoNotOptimize( view.data() );
				::benchmark::DoNotOptimize( view.size() );
			}
		}
	}

	//----------------------------
	// Zero-allocation
	//----------------------------

	static void BM_StringBuilder_ZeroAlloc( ::benchmark::State& state )
	{
		// Test pure builder operations without final string conversion
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			for ( const auto& str : small_strings )
			{
				builder << str << " ";
			}

			// Just access the buffer without creating string
			auto view = lease.buffer().toStringView();
			::benchmark::DoNotOptimize( view );
		}
	}

	//----------------------------
	// Memory pressure
	//----------------------------

	static void BM_StringBuilder_MemoryPressure( ::benchmark::State& state )
	{
		// Simulate memory pressure with large strings
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			// Build a large result
			for ( int i = 0; i < 20; ++i )
			{
				for ( const auto& str : large_strings )
				{
					builder << str << " ";
				}
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

} // namespace nfx::string::benchmark

//=====================================================================
// Benchmarks registration
//=====================================================================

//----------------------------------------------
// StringBuilderPool vs std::string concatenation
//----------------------------------------------

//----------------------------
// Small strings
//----------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_SmallStrings )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringStream_SmallStrings )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_SmallStrings )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------
// Medium strings
//----------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_MediumStrings )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringStream_MediumStrings )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_MediumStrings )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------
// Large strings
//----------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_LargeStrings )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringStream_LargeStrings )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_LargeStrings )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// Pool efficiency
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_RapidCycles )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_PoolEfficiency )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// Mixed operations
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_MixedOperations )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringStream_MixedOperations )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_MixedOperations )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// Capacity hints
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_NoHint )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_WithHint )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// format() method
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdFormat_ComplexFormatting )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_FormatMethod )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_ManualFormatting )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// Variadic append operations
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_MultipleAppends )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_VariadicAppend )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_MultipleAppends_WithNumeric )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_VariadicAppend_WithNumeric )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// Advanced
//----------------------------------------------

//----------------------------
// Buffer reuse
//----------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_BufferReuse )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------
// Zero-allocation
//----------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_ZeroAlloc )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------
// Memory pressure
//----------------------------

BENCHMARK( nfx::string::benchmark::BM_StringBuilder_MemoryPressure )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// Benchmark main entry point
//----------------------------------------------

BENCHMARK_MAIN();
