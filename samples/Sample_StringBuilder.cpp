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

	return 0;
}
