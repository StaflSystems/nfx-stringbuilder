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
 * @file Sample_StringBuilder.cpp
 * @brief Demonstrates high-performance string building with nfx-stringbuilder
 * @details This sample shows how to use nfx-stringbuilder for pooled string operations,
 *          including basic building, stream operators, iterator usage, and performance monitoring
 */

#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <thread>

#include <nfx/string/StringBuilder.h>

int main()
{
	using namespace nfx::string;

	std::cout << "=== nfx-stringbuilder String Building ===\n\n";

	//=====================================================================
	// 1. Basic string building operations
	//=====================================================================
	{
		std::cout << "1. Basic string building operations\n";
		std::cout << "------------------------------------\n";

		auto lease = StringBuilderPool::lease();
		auto builder = lease.create();

		builder.append( "Hello" );
		builder.append( ", " );
		builder.append( "World" );
		builder.append( '!' );

		std::cout << "Result: " << lease.toString() << "\n";
		std::cout << "Buffer size: " << lease.buffer().size() << " bytes\n";
		std::cout << "Buffer capacity: " << lease.buffer().capacity() << " bytes\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 2. Stream operators for fluent building
	//=====================================================================
	{
		std::cout << "2. Stream operators for fluent building\n";
		std::cout << "----------------------------------------\n";

		auto lease = StringBuilderPool::lease();
		auto builder = lease.create();

		builder << "Stream " << "operators " << "are " << "convenient!";

		std::cout << "Result: " << lease.toString() << "\n";
		std::cout << "Note: << operator enables fluent chaining\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 3. Buffer reuse and pooling
	//=====================================================================
	{
		std::cout << "3. Buffer reuse and pooling\n";
		std::cout << "---------------------------\n";

		StringBuilderPool::resetStats();

		// First lease
		{
			auto lease = StringBuilderPool::lease();
			lease.create().append( "First lease" );
			std::cout << "Lease 1: " << lease.toString() << "\n";
		} // Returns to pool

		// Second lease (reuses buffer)
		{
			auto lease = StringBuilderPool::lease();
			lease.create().append( "Second lease" );
			std::cout << "Lease 2: " << lease.toString() << "\n";
		}

		auto stats = StringBuilderPool::stats();
		std::cout << "Pool hit rate: " << std::fixed << std::setprecision( 1 )
				  << ( stats.hitRate * 100.0 ) << "%\n";
		std::cout << "Note: Second lease reused buffer from pool\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 4. Iterator support
	//=====================================================================
	{
		std::cout << "4. Iterator support\n";
		std::cout << "-------------------\n";

		auto lease = StringBuilderPool::lease();
		auto builder = lease.create();
		builder << "Iterator";

		std::cout << "Characters: ";
		for ( char c : builder )
		{
			std::cout << c << " ";
		}
		std::cout << "\n";
		std::cout << "Note: Range-based for loop support\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 5. Array access and manipulation
	//=====================================================================
	{
		std::cout << "5. Array access and manipulation\n";
		std::cout << "--------------------------------\n";

		auto lease = StringBuilderPool::lease();
		auto builder = lease.create();
		builder << "Hello World";

		std::cout << "Original: " << lease.toString() << "\n";
		std::cout << "Character at [6]: '" << builder[6] << "'\n";

		builder[6] = 'w';
		std::cout << "After [6] = 'w': " << lease.toString() << "\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 6. Building complex strings (SQL query example)
	//=====================================================================
	{
		std::cout << "6. Building complex strings (SQL query example)\n";
		std::cout << "-----------------------------------------------\n";

		auto lease = StringBuilderPool::lease();
		auto builder = lease.create();

		std::vector<std::string> columns{ "id", "name", "email" };
		std::vector<std::string> conditions{ "active = 1", "age > 18" };

		builder << "SELECT ";
		for ( size_t i = 0; i < columns.size(); ++i )
		{
			if ( i > 0 )
				builder << ", ";
			builder << columns[i];
		}

		builder << " FROM users WHERE ";
		for ( size_t i = 0; i < conditions.size(); ++i )
		{
			if ( i > 0 )
				builder << " AND ";
			builder << conditions[i];
		}

		std::cout << "Query: " << lease.toString() << "\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 7. Thread safety demonstration
	//=====================================================================
	{
		std::cout << "7. Thread safety demonstration\n";
		std::cout << "------------------------------\n";

		StringBuilderPool::resetStats();

		std::vector<std::thread> threads;
		std::vector<std::string> results( 4 );

		for ( size_t t = 0; t < 4; ++t )
		{
			threads.emplace_back( [t, &results]() {
				auto lease = StringBuilderPool::lease();
				auto builder = lease.create();
				builder << "Thread " << std::to_string( t ) << " data";
				results[t] = lease.toString();
			} );
		}

		for ( auto& thread : threads )
		{
			thread.join();
		}

		for ( size_t i = 0; i < results.size(); ++i )
		{
			std::cout << "  " << results[i] << "\n";
		}

		auto stats = StringBuilderPool::stats();
		std::cout << "Multi-threaded hit rate: " << std::fixed << std::setprecision( 1 )
				  << ( stats.hitRate * 100.0 ) << "%\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 8. Pool statistics and monitoring
	//=====================================================================
	{
		std::cout << "8. Pool statistics and monitoring\n";
		std::cout << "---------------------------------\n";

		auto stats = StringBuilderPool::stats();

		std::cout << "Total requests: " << stats.totalRequests << "\n";
		std::cout << "Thread-local hits: " << stats.threadLocalHits << "\n";
		std::cout << "Shared pool hits: " << stats.dynamicStringBufferPoolHits << "\n";
		std::cout << "New allocations: " << stats.newAllocations << "\n";
		std::cout << "Overall hit rate: " << std::fixed << std::setprecision( 1 )
				  << ( stats.hitRate * 100.0 ) << "%\n";
		std::cout << "Current pool size: " << StringBuilderPool::size() << "\n";

		size_t cleared = StringBuilderPool::clear();
		std::cout << "Cleared " << cleared << " buffers from pool\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 9. std::format integration with DynamicStringBuffer
	//=====================================================================
	{
		std::cout << "9. std::format integration with DynamicStringBuffer\n";
		std::cout << "---------------------------------------------------\n";

		auto lease = StringBuilderPool::lease();
		auto builder = lease.create();
		builder << "Formatted content";

		// Direct formatting of DynamicStringBuffer
		std::string formatted = std::format(
			"Buffer contains: {}",
			lease.buffer() );

		std::cout << formatted << "\n";

		// Formatting in logging scenarios
		std::cout << std::format(
			"Debug: buffer size={}, content={}\n",
			lease.buffer().size(),
			lease.buffer() );

		std::cout << "Note: Zero-copy formatting via std::formatter specialization\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 10. std::format integration with StringBuilder
	//=====================================================================
	{
		std::cout << "10. std::format integration with StringBuilder\n";
		std::cout << "----------------------------------------------\n";

		auto lease = StringBuilderPool::lease();
		auto builder = lease.create();
		builder << "Building...";

		// Format during construction (useful for debugging)
		std::cout << std::format(
			"Current state: {}\n",
			builder );

		builder << " more text";
		std::cout << std::format(
			"After append: {}\n",
			builder );

		std::cout << "Note: Useful for debugging string construction process\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 11. Numeric type support (integers, floats, booleans)
	//=====================================================================
	{
		std::cout << "11. Numeric type support (integers, floats, booleans)\n";
		std::cout << "-----------------------------------------------------\n";

		auto lease = StringBuilderPool::lease();
		auto builder = lease.create();

		// Integers
		std::int32_t count = 42;
		std::uint64_t bigNumber = 18446744073709551615ULL;
		builder << "Count: " << count << ", Big: " << bigNumber;
		std::cout << "Integers: " << lease.toString() << "\n";

		// Floating-point
		lease.buffer().clear();
		float pi = 3.14159f;
		double e = 2.718281828;
		builder << "Pi: " << pi << ", E: " << e;
		std::cout << "Floats: " << lease.toString() << "\n";

		// Boolean
		lease.buffer().clear();
		bool enabled = true;
		bool disabled = false;
		builder << "Enabled: " << enabled << ", Disabled: " << disabled;
		std::cout << "Booleans: " << lease.toString() << "\n";

		// Mixed expression
		lease.buffer().clear();
		builder << "Result: " << 10 << " + " << 20 << " = " << 30;
		std::cout << "Mixed: " << lease.toString() << "\n";

		std::cout << "Note: All numeric types seamlessly integrate with << operator\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 12. Type-safe formatting with format() method
	//=====================================================================
	{
		std::cout << "12. Type-safe formatting with format() method\n";
		std::cout << "----------------------------------------------\n";

		auto lease = StringBuilderPool::lease();
		auto builder = lease.create();

		// Basic formatting
		std::string username = "Alice";
		int userId = 12345;
		builder.format( "User {} (ID: {})", username, userId );
		std::cout << "Basic: " << lease.toString() << "\n";

		// Format specifiers
		lease.buffer().clear();
		builder.format( "Hex: 0x{:X}, Binary: 0b{:b}, Padded: {:08}", 255, 42, 7 );
		std::cout << "Specifiers: " << lease.toString() << "\n";

		// Floating-point precision
		lease.buffer().clear();
		double pi = 3.141592653589793;
		builder.format( "Pi with 2 decimals: {:.2f}, Scientific: {:.2e}", pi, pi );
		std::cout << "Precision: " << lease.toString() << "\n";

		// Mixed types with alignment
		lease.buffer().clear();
		builder.format( "Left: {:<10} | Center: {:^10} | Right: {:>10}", "abc", "def", "ghi" );
		std::cout << "Alignment: " << lease.toString() << "\n";

		// Chaining format() with operator<<
		lease.buffer().clear();
		builder << "Prefix: "
				<< 100
				<< " | ";
		builder.format( "Formatted: {} items @ ${:.2f} each", 5, 19.99 );
		std::cout << "Chaining: " << lease.toString() << "\n";

		std::cout << "Note: format() uses std::format internally for zero-overhead type-safe formatting\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 13. Capacity hints for pre-sized buffers
	//=====================================================================
	{
		std::cout << "13. Capacity hints for pre-sized buffers\n";
		std::cout << "-----------------------------------------\n";

		// Without capacity hint (default behavior)
		auto lease1 = StringBuilderPool::lease();
		std::cout << "Default capacity: " << lease1.buffer().capacity() << " bytes\n";

		// With capacity hint (1KB pre-allocated)
		auto lease2 = StringBuilderPool::lease( 1024 );
		std::cout << "With hint (1024): " << lease2.buffer().capacity() << " bytes\n";

		// Building large string with hint
		auto lease = StringBuilderPool::lease( 2048 ); // Pre-allocate 2KB
		auto builder = lease.create();

		// Simulate building a large SQL query
		builder << "SELECT id, name, email, created_at, updated_at, status, role, department "
				<< "FROM users "
				<< "WHERE status = 'active' AND role IN ('admin', 'moderator', 'user') "
				<< "AND created_at >= '2024-01-01' "
				<< "ORDER BY created_at DESC "
				<< "LIMIT 100;";

		std::cout << "Built query: " << lease.toString().length() << " bytes\n";
		std::cout << "Buffer capacity: " << lease.buffer().capacity() << " bytes\n";
		std::cout << "Note: Pre-allocation avoids multiple reallocations during string construction\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 14. Understanding reserve() vs resize() - Common itfall
	//=====================================================================
	{
		std::cout << "14. Understanding reserve() vs resize() - Common Pitfall\n";
		std::cout << "--------------------------------------------------------\n";

		// WRONG: Using resize() for pre-allocation
		std::cout << "Wrong approach - using resize() for pre-allocation:\n";
		{
			auto lease = StringBuilderPool::lease();
			auto& buffer = lease.buffer();

			buffer.resize( 100 ); // Creates 100 bytes of uninitialized data
			std::cout << "  After resize(100): size=" << buffer.size() << " bytes\n";
			std::cout << "  Content contains garbage (uninitialized memory)\n";

			auto builder = lease.create();
			builder.append( "Hello" ); // Appends AFTER the 100 garbage bytes
			std::cout << "  After append('Hello'): size=" << buffer.size() << " bytes\n";
			std::cout << "  Problem: 'Hello' is at position 100, preceded by garbage\n";
			std::cout << "\n";
		}

		// CORRECT: Using reserve() for pre-allocation
		std::cout << "Correct approach - using reserve() for pre-allocation:\n";
		{
			auto lease = StringBuilderPool::lease();
			auto& buffer = lease.buffer();

			buffer.reserve( 100 ); // Pre-allocates capacity, size remains 0
			std::cout << "  After reserve(100): size=" << buffer.size()
					  << ", capacity=" << buffer.capacity() << " bytes\n";

			auto builder = lease.create();
			builder.append( "Hello" ); // Appends from position 0
			std::cout << "  After append('Hello'): size=" << buffer.size() << " bytes\n";
			std::cout << "  Content: '" << lease.toString() << "'\n";
			std::cout << "  Success: 'Hello' starts at position 0, no garbage\n";
			std::cout << "\n";
		}

		// VALID: Using resize() for raw buffer manipulation
		std::cout << "Valid use of resize() - raw buffer manipulation:\n";
		{
			auto lease = StringBuilderPool::lease();
			auto& buffer = lease.buffer();

			buffer.resize( 32 );		// Allocate fixed-size buffer
			char* data = buffer.data(); // Get raw pointer

			// Write directly to buffer (e.g., from C API, fread, etc.)
			std::memcpy( data, "Direct write to buffer memory!", 31 );
			data[31] = '\0';

			std::cout << "  After resize(32) and direct write: '" << lease.toString() << "'\n";
			std::cout << "  Use case: Direct memory operations, C API integration\n";
			std::cout << "\n";
		}

		// Best practice: Capacity hints
		std::cout << "Best practice - capacity hints:\n";
		{
			auto lease = StringBuilderPool::lease( 100 ); // Hint at creation time
			auto& buffer = lease.buffer();
			std::cout << "  After lease(100): size=" << buffer.size()
					  << ", capacity=" << buffer.capacity() << " bytes\n";

			auto builder = lease.create();
			builder.append( "Hello" );
			std::cout << "  After append('Hello'): '" << lease.toString() << "'\n";
			std::cout << "\n";
		}

		// Alternative: Using StringBuilder.reserve() directly
		std::cout << "Alternative - StringBuilder.reserve() method:\n";
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			builder.reserve( 100 ); // Reserve capacity through StringBuilder
			std::cout << "  After builder.reserve(100): size=" << builder.length()
					  << ", capacity=" << lease.buffer().capacity() << " bytes\n";

			builder.append( "Hello" );
			std::cout << "  After append('Hello'): '" << lease.toString() << "'\n";
			std::cout << "\n";
		}

		std::cout << "Summary:\n";
		std::cout << "  reserve(n): Allocates capacity, keeps size=0 (for pre-allocation)\n";
		std::cout << "  resize(n):  Sets size=n immediately (for raw buffer operations)\n";
		std::cout << "  lease(n):   Pre-allocates with capacity hint (best practice)\n";
		std::cout << "\n";
	}

	//=====================================================================
	// 15. Variadic append() - Batch operations
	//=====================================================================
	{
		std::cout << "15. Variadic append() - Batch operations\n";
		std::cout << "----------------------------------------\n";

		// Traditional approach: Multiple append() calls
		std::cout << "Traditional approach - chained append():\n";
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

			std::cout << "  Result: " << lease.toString() << "\n";
			std::cout << "  Note: 9 separate append() calls\n";
			std::cout << "\n";
		}

		// Modern approach: Variadic append() - single call
		std::cout << "Modern approach - variadic append():\n";
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			builder.append( "User ", 12345, " (", "Alice", ") logged in at ",
				1735000000, " with score ", 99.5, "%" );

			std::cout << "  Result: " << lease.toString() << "\n";
			std::cout << "  Note: Single append() call with multiple arguments\n";
			std::cout << "\n";
		}

		// SQL query construction with variadic append()
		std::cout << "SQL query construction:\n";
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			std::string table = "users";
			int minAge = 18;
			std::string status = "active";

			builder.append( "SELECT id, name, email FROM ", table,
				" WHERE age >= ", minAge, " AND status = '", status, "'" );

			std::cout << "  Query: " << lease.toString() << "\n";
			std::cout << "  Note: Mixes string literals, variables, and numeric types\n";
			std::cout << "\n";
		}

		// Comparison with stream operators
		std::cout << "Comparison with stream operators:\n";
		{
			auto lease = StringBuilderPool::lease();
			auto builder = lease.create();

			// Using << operator
			builder << "User " << 42 << ": " << "Bob" << " (age: " << 25 << ")";
			std::string result1 = lease.toString();

			// Using variadic append()
			lease.buffer().clear();
			builder.append( "User ", 42, ": ", "Bob", " (age: ", 25, ")" );
			std::string result2 = lease.toString();

			std::cout << "  Stream operator:   " << result1 << "\n";
			std::cout << "  Variadic append(): " << result2 << "\n";
		}

		std::cout << "\n";
	}

	return 0;
}
