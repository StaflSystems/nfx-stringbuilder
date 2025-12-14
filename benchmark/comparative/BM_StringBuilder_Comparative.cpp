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
 * @file BM_StringBuilder_Comparative.cpp
 * @brief Comparative benchmarks: StringBuilder vs standard library and third-party implementations
 */

#include <benchmark/benchmark.h>

#include <nfx/string/StringBuilder.h>

#ifdef HAS_FMT
#	include <fmt/format.h>
#endif

#ifdef HAS_ABSEIL
#	include <absl/strings/str_cat.h>
#	include <absl/strings/str_join.h>
#endif

#include <sstream>
#include <string>
#include <string_view>
#include <vector>

namespace nfx::string::benchmark
{
	//=====================================================================
	// StringBuilder comparative benchmark suite
	//=====================================================================

	//----------------------------------------------
	// Test data
	//----------------------------------------------

	static const std::vector<std::string_view> smallStrings = {
		"Hello", "World", "Test", "String", "Builder" };

	static const std::vector<std::string_view> mediumStrings = {
		"This is a medium length string for testing purposes",
		"Another medium string with different content and length",
		"Performance testing requires consistent data patterns",
		"Medium strings help measure real-world usage scenarios" };

	static const std::vector<std::string_view> largeStrings = {
		"This is a much longer string that contains significantly more characters and is designed to test the performance characteristics of string building operations when dealing with larger amounts of text content that might be more representative of real-world applications",
		"Large strings like this one are important for benchmarking because they help us understand how the string builder performs when concatenating substantial amounts of text data, which is common in applications that generate reports, logs, or formatted output",
		"Performance analysis of string concatenation with longer strings reveals different behavior patterns compared to small string operations, particularly in terms of memory allocation patterns and buffer management strategies" };

	inline std::string generateBookPage( int pageNumber, std::size_t targetSize )
	{
		std::string page;
		page.reserve( targetSize );

		// Add page header
		page += "=== Page " + std::to_string( pageNumber ) + " ===\n\n";

		const char* paragraph =
			"This is a paragraph from a book page containing substantial text content. "
			"It represents the kind of large string operations that occur in real-world applications "
			"such as document generation, report creation, log aggregation, or web page rendering. "
			"String builders are particularly useful in these scenarios where multiple text fragments "
			"need to be efficiently concatenated without excessive memory allocations. ";

		while ( page.size() < targetSize )
		{
			page += paragraph;
		}

		// Truncate to exact size if we overshoot
		if ( page.size() > targetSize )
		{
			page.resize( targetSize );
		}

		return page;
	}

	static std::vector<std::string> hugeStringsStorage = {
		generateBookPage( 1, 3000 ),
		generateBookPage( 2, 4000 ),
		generateBookPage( 3, 5000 ) };

	static std::vector<std::string_view> hugeStrings = {
		hugeStringsStorage[0],
		hugeStringsStorage[1],
		hugeStringsStorage[2] };

	inline std::size_t calculateTotalSize( const std::vector<std::string_view>& strings )
	{
		std::size_t total = 0;
		for ( const auto& str : strings )
		{
			total += str.size() + 1; // +1 for separator
		}
		return total;
	}

	const std::size_t smallStringsLen = calculateTotalSize( smallStrings );
	const std::size_t mediumStringsLen = calculateTotalSize( mediumStrings );
	const std::size_t largeStringsLen = calculateTotalSize( largeStrings );
	const std::size_t hugeStringsLen = calculateTotalSize( hugeStrings );

	//----------------------------------------------
	// Small strings
	//----------------------------------------------

	static void BM_StdString_SmallStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;

			for ( const auto& str : smallStrings )
			{
				result += str;
				result += " ";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StdString_SmallStrings_WarmStart( ::benchmark::State& state )
	{
		std::string result;

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : smallStrings )
			{
				result += str;
				result += " ";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringStream_SmallStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::ostringstream oss;

			for ( const auto& str : smallStrings )
			{
				oss << str << " ";
			}

			std::string result = oss.str();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringStream_SmallStrings_WarmStart( ::benchmark::State& state )
	{
		std::ostringstream oss;

		for ( auto _ : state )
		{
			oss.str( "" );
			oss.clear();

			for ( const auto& str : smallStrings )
			{
				oss << str << " ";
			}

			std::string result = oss.str();
			::benchmark::DoNotOptimize( result );
		}
	}

#ifdef HAS_FMT
	static void BM_FmtMemoryBuffer_SmallStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			fmt::memory_buffer buffer;

			for ( const auto& str : smallStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{} ", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_FmtMemoryBuffer_SmallStrings_WarmStart( ::benchmark::State& state )
	{
		fmt::memory_buffer buffer;

		for ( auto _ : state )
		{
			buffer.clear();

			for ( const auto& str : smallStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{} ", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}
#endif

#ifdef HAS_ABSEIL
	static void BM_Abseil_SmallStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;

			for ( const auto& str : smallStrings )
			{
				absl::StrAppend( &result, str, " " );
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_Abseil_SmallStrings_WarmStart( ::benchmark::State& state )
	{
		std::string result;

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : smallStrings )
			{
				absl::StrAppend( &result, str, " " );
			}

			::benchmark::DoNotOptimize( result );
		}
	}
#endif

	static void BM_nfx_StringBuilder_SmallStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			for ( const auto& str : smallStrings )
			{
				builder << str << " ";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_nfx_StringBuilder_SmallStrings_WarmStart( ::benchmark::State& state )
	{
		auto lease = StringBuilderPool::lease();
		auto builder = lease.create();

		for ( auto _ : state )
		{
			lease.buffer().clear();

			for ( const auto& str : smallStrings )
			{
				builder << str << " ";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Small strings with reserve
	//----------------------------------------------

	static void BM_StdString_SmallStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;
			result.reserve( smallStringsLen );

			for ( const auto& str : smallStrings )
			{
				result += str;
				result += " ";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StdString_SmallStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		std::string result;
		result.reserve( smallStringsLen );

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : smallStrings )
			{
				result += str;
				result += " ";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

#ifdef HAS_FMT
	static void BM_FmtMemoryBuffer_SmallStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			fmt::memory_buffer buffer;
			buffer.reserve( smallStringsLen );

			for ( const auto& str : smallStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{} ", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_FmtMemoryBuffer_SmallStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		fmt::memory_buffer buffer;
		buffer.reserve( smallStringsLen );

		for ( auto _ : state )
		{
			buffer.clear();

			for ( const auto& str : smallStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{} ", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}
#endif

#ifdef HAS_ABSEIL
	static void BM_Abseil_SmallStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;
			result.reserve( smallStringsLen );

			for ( const auto& str : smallStrings )
			{
				absl::StrAppend( &result, str, " " );
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_Abseil_SmallStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		std::string result;
		result.reserve( smallStringsLen );

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : smallStrings )
			{
				absl::StrAppend( &result, str, " " );
			}

			::benchmark::DoNotOptimize( result );
		}
	}
#endif

	static void BM_nfx_StringBuilder_SmallStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease( smallStringsLen );
			auto builder = lease.create();

			for ( const auto& str : smallStrings )
			{
				builder << str << " ";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_nfx_StringBuilder_SmallStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		auto lease = StringBuilderPool::lease( smallStringsLen );
		auto builder = lease.create();

		for ( auto _ : state )
		{
			lease.buffer().clear();

			for ( const auto& str : smallStrings )
			{
				builder << str << " ";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Medium strings
	//----------------------------------------------

	static void BM_StdString_MediumStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;

			for ( const auto& str : mediumStrings )
			{
				result += str;
				result += " ";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StdString_MediumStrings_WarmStart( ::benchmark::State& state )
	{
		std::string result;

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : mediumStrings )
			{
				result += str;
				result += " ";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringStream_MediumStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::ostringstream oss;

			for ( const auto& str : mediumStrings )
			{
				oss << str << " ";
			}

			std::string result = oss.str();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringStream_MediumStrings_WarmStart( ::benchmark::State& state )
	{
		std::ostringstream oss;

		for ( auto _ : state )
		{
			oss.str( "" );
			oss.clear();

			for ( const auto& str : mediumStrings )
			{
				oss << str << " ";
			}

			std::string result = oss.str();
			::benchmark::DoNotOptimize( result );
		}
	}

#ifdef HAS_FMT
	static void BM_FmtMemoryBuffer_MediumStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			fmt::memory_buffer buffer;

			for ( const auto& str : mediumStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{} ", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_FmtMemoryBuffer_MediumStrings_WarmStart( ::benchmark::State& state )
	{
		fmt::memory_buffer buffer;

		for ( auto _ : state )
		{
			buffer.clear();

			for ( const auto& str : mediumStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{} ", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}
#endif

#ifdef HAS_ABSEIL
	static void BM_Abseil_MediumStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;

			for ( const auto& str : mediumStrings )
			{
				absl::StrAppend( &result, str, " " );
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_Abseil_MediumStrings_WarmStart( ::benchmark::State& state )
	{
		std::string result;

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : mediumStrings )
			{
				absl::StrAppend( &result, str, " " );
			}

			::benchmark::DoNotOptimize( result );
		}
	}
#endif

	static void BM_nfx_StringBuilder_MediumStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			for ( const auto& str : mediumStrings )
			{
				builder << str << " ";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_nfx_StringBuilder_MediumStrings_WarmStart( ::benchmark::State& state )
	{
		auto lease = StringBuilderPool::lease();
		auto builder = lease.create();

		for ( auto _ : state )
		{
			lease.buffer().clear();

			for ( const auto& str : mediumStrings )
			{
				builder << str << " ";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Medium strings with reserve
	//----------------------------------------------

	static void BM_StdString_MediumStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;
			result.reserve( mediumStringsLen );

			for ( const auto& str : mediumStrings )
			{
				result += str;
				result += " ";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StdString_MediumStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		std::string result;
		result.reserve( mediumStringsLen );

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : mediumStrings )
			{
				result += str;
				result += " ";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

#ifdef HAS_FMT
	static void BM_FmtMemoryBuffer_MediumStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			fmt::memory_buffer buffer;
			buffer.reserve( mediumStringsLen );

			for ( const auto& str : mediumStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{} ", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_FmtMemoryBuffer_MediumStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		fmt::memory_buffer buffer;
		buffer.reserve( mediumStringsLen );

		for ( auto _ : state )
		{
			buffer.clear();

			for ( const auto& str : mediumStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{} ", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}
#endif

#ifdef HAS_ABSEIL
	static void BM_Abseil_MediumStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;
			result.reserve( mediumStringsLen );

			for ( const auto& str : mediumStrings )
			{
				absl::StrAppend( &result, str, " " );
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_Abseil_MediumStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		std::string result;
		result.reserve( mediumStringsLen );

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : mediumStrings )
			{
				absl::StrAppend( &result, str, " " );
			}

			::benchmark::DoNotOptimize( result );
		}
	}
#endif

	static void BM_nfx_StringBuilder_MediumStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease( mediumStringsLen );
			auto builder = lease.create();

			for ( const auto& str : mediumStrings )
			{
				builder << str << " ";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_nfx_StringBuilder_MediumStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		auto lease = StringBuilderPool::lease( mediumStringsLen );
		auto builder = lease.create();

		for ( auto _ : state )
		{
			lease.buffer().clear();

			for ( const auto& str : mediumStrings )
			{
				builder << str << " ";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// large strings
	//----------------------------------------------

	static void BM_StdString_LargeStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;

			for ( const auto& str : largeStrings )
			{
				result += str;
				result += "\n";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StdString_LargeStrings_WarmStart( ::benchmark::State& state )
	{
		std::string result;

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : largeStrings )
			{
				result += str;
				result += "\n";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringStream_LargeStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::ostringstream oss;

			for ( const auto& str : largeStrings )
			{
				oss << str << "\n";
			}

			std::string result = oss.str();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringStream_LargeStrings_WarmStart( ::benchmark::State& state )
	{
		std::ostringstream oss;

		for ( auto _ : state )
		{
			oss.str( "" );
			oss.clear();

			for ( const auto& str : largeStrings )
			{
				oss << str << "\n";
			}

			std::string result = oss.str();
			::benchmark::DoNotOptimize( result );
		}
	}

#ifdef HAS_FMT
	static void BM_FmtMemoryBuffer_LargeStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			fmt::memory_buffer buffer;

			for ( const auto& str : largeStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{}\n", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_FmtMemoryBuffer_LargeStrings_WarmStart( ::benchmark::State& state )
	{
		fmt::memory_buffer buffer;

		for ( auto _ : state )
		{
			buffer.clear();

			for ( const auto& str : largeStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{}\n", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}
#endif

#ifdef HAS_ABSEIL
	static void BM_Abseil_LargeStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;

			for ( const auto& str : largeStrings )
			{
				absl::StrAppend( &result, str, "\n" );
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_Abseil_LargeStrings_WarmStart( ::benchmark::State& state )
	{
		std::string result;

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : largeStrings )
			{
				absl::StrAppend( &result, str, "\n" );
			}

			::benchmark::DoNotOptimize( result );
		}
	}
#endif

	static void BM_nfx_StringBuilder_LargeStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			for ( const auto& str : largeStrings )
			{
				builder << str << "\n";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_nfx_StringBuilder_LargeStrings_WarmStart( ::benchmark::State& state )
	{
		auto lease = StringBuilderPool::lease();
		auto builder = lease.create();

		for ( auto _ : state )
		{
			lease.buffer().clear();

			for ( const auto& str : largeStrings )
			{
				builder << str << "\n";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Large strings with reserve
	//----------------------------------------------

	static void BM_StdString_LargeStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;
			result.reserve( largeStringsLen );

			for ( const auto& str : largeStrings )
			{
				result += str;
				result += "\n";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StdString_LargeStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		std::string result;
		result.reserve( largeStringsLen );

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : largeStrings )
			{
				result += str;
				result += "\n";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

#ifdef HAS_FMT
	static void BM_FmtMemoryBuffer_LargeStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			fmt::memory_buffer buffer;
			buffer.reserve( largeStringsLen );

			for ( const auto& str : largeStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{}\n", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_FmtMemoryBuffer_LargeStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		fmt::memory_buffer buffer;
		buffer.reserve( largeStringsLen );

		for ( auto _ : state )
		{
			buffer.clear();

			for ( const auto& str : largeStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{}\n", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}
#endif

#ifdef HAS_ABSEIL
	static void BM_Abseil_LargeStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;
			result.reserve( largeStringsLen );

			for ( const auto& str : largeStrings )
			{
				absl::StrAppend( &result, str, "\n" );
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_Abseil_LargeStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		std::string result;
		result.reserve( largeStringsLen );

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : largeStrings )
			{
				absl::StrAppend( &result, str, "\n" );
			}

			::benchmark::DoNotOptimize( result );
		}
	}
#endif

	static void BM_nfx_StringBuilder_LargeStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease( largeStringsLen );
			auto builder = lease.create();

			for ( const auto& str : largeStrings )
			{
				builder << str << "\n";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_nfx_StringBuilder_LargeStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		auto lease = StringBuilderPool::lease( largeStringsLen );
		auto builder = lease.create();

		for ( auto _ : state )
		{
			lease.buffer().clear();

			for ( const auto& str : largeStrings )
			{
				builder << str << "\n";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Huge strings
	//----------------------------------------------

	static void BM_StdString_HugeStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;

			for ( const auto& str : hugeStrings )
			{
				result += str;
				result += "\n";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StdString_HugeStrings_WarmStart( ::benchmark::State& state )
	{
		std::string result;

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : hugeStrings )
			{
				result += str;
				result += "\n";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringStream_HugeStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::ostringstream oss;

			for ( const auto& str : hugeStrings )
			{
				oss << str << "\n";
			}

			std::string result = oss.str();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StringStream_HugeStrings_WarmStart( ::benchmark::State& state )
	{
		std::ostringstream oss;

		for ( auto _ : state )
		{
			oss.str( "" );
			oss.clear();

			for ( const auto& str : hugeStrings )
			{
				oss << str << "\n";
			}

			std::string result = oss.str();
			::benchmark::DoNotOptimize( result );
		}
	}

#ifdef HAS_FMT
	static void BM_FmtMemoryBuffer_HugeStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			fmt::memory_buffer buffer;

			for ( const auto& str : hugeStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{}\n", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_FmtMemoryBuffer_HugeStrings_WarmStart( ::benchmark::State& state )
	{
		fmt::memory_buffer buffer;

		for ( auto _ : state )
		{
			buffer.clear();

			for ( const auto& str : hugeStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{}\n", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}
#endif

#ifdef HAS_ABSEIL
	static void BM_Abseil_HugeStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;

			for ( const auto& str : hugeStrings )
			{
				absl::StrAppend( &result, str, "\n" );
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_Abseil_HugeStrings_WarmStart( ::benchmark::State& state )
	{
		std::string result;

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : hugeStrings )
			{
				absl::StrAppend( &result, str, "\n" );
			}

			::benchmark::DoNotOptimize( result );
		}
	}
#endif

	static void BM_nfx_StringBuilder_HugeStrings_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			for ( const auto& str : hugeStrings )
			{
				builder << str << "\n";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_nfx_StringBuilder_HugeStrings_WarmStart( ::benchmark::State& state )
	{
		auto lease = StringBuilderPool::lease();
		auto builder = lease.create();

		for ( auto _ : state )
		{
			lease.buffer().clear();

			for ( const auto& str : hugeStrings )
			{
				builder << str << "\n";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	//----------------------------------------------
	// Huge strings with reserve
	//----------------------------------------------

	static void BM_StdString_HugeStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;
			result.reserve( hugeStringsLen );

			for ( const auto& str : hugeStrings )
			{
				result += str;
				result += "\n";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_StdString_HugeStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		std::string result;
		result.reserve( hugeStringsLen );

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : hugeStrings )
			{
				result += str;
				result += "\n";
			}

			::benchmark::DoNotOptimize( result );
		}
	}

#ifdef HAS_FMT
	static void BM_FmtMemoryBuffer_HugeStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			fmt::memory_buffer buffer;
			buffer.reserve( hugeStringsLen );

			for ( const auto& str : hugeStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{}\n", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_FmtMemoryBuffer_HugeStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		fmt::memory_buffer buffer;
		buffer.reserve( hugeStringsLen );

		for ( auto _ : state )
		{
			buffer.clear();

			for ( const auto& str : hugeStrings )
			{
				fmt::format_to( std::back_inserter( buffer ), "{}\n", str );
			}

			std::string result = fmt::to_string( buffer );
			::benchmark::DoNotOptimize( result );
		}
	}
#endif

#ifdef HAS_ABSEIL
	static void BM_Abseil_HugeStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			std::string result;
			result.reserve( hugeStringsLen );

			for ( const auto& str : hugeStrings )
			{
				absl::StrAppend( &result, str, "\n" );
			}

			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_Abseil_HugeStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		std::string result;
		result.reserve( hugeStringsLen );

		for ( auto _ : state )
		{
			result.clear();

			for ( const auto& str : hugeStrings )
			{
				absl::StrAppend( &result, str, "\n" );
			}

			::benchmark::DoNotOptimize( result );
		}
	}
#endif

	static void BM_nfx_StringBuilder_HugeStrings_Reserve_ColdStart( ::benchmark::State& state )
	{
		for ( auto _ : state )
		{
			auto lease = StringBuilderPool::lease( hugeStringsLen );
			auto builder = lease.create();

			for ( const auto& str : hugeStrings )
			{
				builder << str << "\n";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}

	static void BM_nfx_StringBuilder_HugeStrings_Reserve_WarmStart( ::benchmark::State& state )
	{
		auto lease = StringBuilderPool::lease( hugeStringsLen );
		auto builder = lease.create();

		for ( auto _ : state )
		{
			lease.buffer().clear();

			for ( const auto& str : hugeStrings )
			{
				builder << str << "\n";
			}

			std::string result = lease.toString();
			::benchmark::DoNotOptimize( result );
		}
	}
} // namespace nfx::string::benchmark

//=====================================================================
// Benchmarks registration
//=====================================================================

//----------------------------
// Small strings
//----------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_SmallStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StdString_SmallStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringStream_SmallStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringStream_SmallStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_SmallStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_SmallStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_SmallStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_Abseil_SmallStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_SmallStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_SmallStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// Small strings with reserve
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_SmallStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StdString_SmallStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_SmallStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_SmallStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_SmallStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_Abseil_SmallStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_SmallStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_SmallStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// Medium strings
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_MediumStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StdString_MediumStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringStream_MediumStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringStream_MediumStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_MediumStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_MediumStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_MediumStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_Abseil_MediumStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_MediumStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_MediumStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// Medium strings with reserve
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_MediumStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StdString_MediumStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_MediumStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_MediumStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_MediumStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_Abseil_MediumStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_MediumStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_MediumStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// large strings
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_LargeStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StdString_LargeStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringStream_LargeStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringStream_LargeStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_LargeStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_LargeStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_LargeStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_Abseil_LargeStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_LargeStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_LargeStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// Large strings with reserve
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_LargeStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StdString_LargeStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_LargeStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_LargeStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_LargeStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_Abseil_LargeStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_LargeStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_LargeStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// Huge strings
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_HugeStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StdString_HugeStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringStream_HugeStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StringStream_HugeStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_HugeStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_HugeStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_HugeStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_Abseil_HugeStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_HugeStrings_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_HugeStrings_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// Huge strings with reserve
//----------------------------------------------

BENCHMARK( nfx::string::benchmark::BM_StdString_HugeStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_StdString_HugeStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

#ifdef HAS_FMT
BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_HugeStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_FmtMemoryBuffer_HugeStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

#ifdef HAS_ABSEIL
BENCHMARK( nfx::string::benchmark::BM_Abseil_HugeStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_Abseil_HugeStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );
#endif

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_HugeStrings_Reserve_ColdStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

BENCHMARK( nfx::string::benchmark::BM_nfx_StringBuilder_HugeStrings_Reserve_WarmStart )
	->MinTime( 1.0 )
	->Unit( benchmark::kNanosecond );

//----------------------------------------------
// Benchmark main entry point
//----------------------------------------------

BENCHMARK_MAIN();
