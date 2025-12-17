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
 * @file BM_Data.h
 * @brief Shared test data for StringBuilder benchmarks
 */

#pragma once

#include <string>
#include <string_view>
#include <vector>

namespace nfx::string::benchmark::testdata
{
	//=====================================================================
	// StringBuilder benchmark test data
	//=====================================================================

	inline const std::vector<std::string_view>& smallStrings()
	{
		static const std::vector<std::string_view> data = {
			"Hello", "World", "Test", "String", "Builder" };
		return data;
	}

	inline const std::vector<std::string_view>& mediumStrings()
	{
		static const std::vector<std::string_view> data = {
			"This is a medium length string for testing purposes",
			"Another medium string with different content and length",
			"Performance testing requires consistent data patterns",
			"Medium strings help measure real-world usage scenarios" };
		return data;
	}

	inline const std::vector<std::string_view>& largeStrings()
	{
		static const std::vector<std::string_view> data = {
			"This is a much longer string that contains significantly more characters and is designed to test the performance characteristics of string building operations when dealing with larger amounts of text content that might be more representative of real-world applications",
			"Large strings like this one are important for benchmarking because they help us understand how the string builder performs when concatenating substantial amounts of text data, which is common in applications that generate reports, logs, or formatted output",
			"Performance analysis of string concatenation with longer strings reveals different behavior patterns compared to small string operations, particularly in terms of memory allocation patterns and buffer management strategies" };
		return data;
	}

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

	inline const std::vector<std::string_view>& hugeStrings()
	{
		static const std::vector<std::string> storage = {
			generateBookPage( 1, 3000 ),
			generateBookPage( 2, 4000 ),
			generateBookPage( 3, 5000 ) };

		static const std::vector<std::string_view> data = {
			storage[0],
			storage[1],
			storage[2] };

		return data;
	}

	inline std::size_t calculateTotalSize( const std::vector<std::string_view>& strings )
	{
		std::size_t total = 0;
		for ( const auto& str : strings )
		{
			total += str.size() + 1; // +1 for separator
		}
		return total;
	}

	inline std::size_t smallStringsLen()
	{
		static const std::size_t value = calculateTotalSize( smallStrings() );
		return value;
	}

	inline std::size_t mediumStringsLen()
	{
		static const std::size_t value = calculateTotalSize( mediumStrings() );
		return value;
	}

	inline std::size_t largeStringsLen()
	{
		static const std::size_t value = calculateTotalSize( largeStrings() );
		return value;
	}

	inline std::size_t hugeStringsLen()
	{
		static const std::size_t value = calculateTotalSize( hugeStrings() );
		return value;
	}
} // namespace nfx::string::benchmark::testdata
