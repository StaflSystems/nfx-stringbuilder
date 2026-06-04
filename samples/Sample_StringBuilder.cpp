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
 * @details This sample shows how to use nfx-stringbuilder for efficient string operations,
 *          including basic building, stream operators, iterator usage, and formatting
 */

#include <nfx/string/StringBuilder.h>

#include <array>
#include <chrono>
#include <cstring>
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <thread>

#if defined( NFX_STRINGBUILDER_HAS_STD_FORMAT )
#    include <format>
#endif

//=====================================================================
// Helper function: Calculate NMEA-0183 checksum
//=====================================================================
/**
 * @brief Calculate NMEA-0183 checksum (XOR of all characters between $ and *)
 * @param sentence NMEA sentence without $ prefix and * suffix
 * @return Checksum as 2-digit uppercase hex string
 */
std::string calculateNmeaChecksum( std::string_view sentence )
{
    uint8_t checksum = 0;
    for( char c : sentence )
    {
        checksum ^= static_cast<uint8_t>( c );
    }

    char result[3];
    std::snprintf( result, sizeof( result ), "%02X", checksum );
    return std::string{ result };
}

int main()
{
    using namespace nfx::string;

    std::cout << "=== nfx-stringbuilder String Building ===\n\n";

    //=====================================================================
    // 1. Basic string building operations (method chaining)
    //=====================================================================
    {
        std::cout << "1. Basic string building operations (method chaining)\n";
        std::cout << "-----------------------------------------------------\n";

        StringBuilder builder;
        builder.append( "Hello" ).append( ", " ).append( "World" ).append( '!' );

        std::cout << "Result: " << builder.toString() << "\n";
        std::cout << "Buffer size: " << builder.size() << " bytes\n";
        std::cout << "Buffer capacity: " << builder.capacity() << " bytes\n";
        std::cout << "Note: append() returns reference for fluent chaining\n";
        std::cout << "\n";
    }

    //=====================================================================
    // 2. Stream operators for fluent building
    //=====================================================================
    {
        std::cout << "2. Stream operators for fluent building\n";
        std::cout << "----------------------------------------\n";

        StringBuilder builder;
        builder << "Stream " << "operators " << "are " << "convenient!";

        std::cout << "Result: " << builder.toString() << "\n";
        std::cout << "Note: << operator also enables fluent chaining\n";
        std::cout << "\n";
    }

    //=====================================================================
    // 3. Buffer reuse with clear()
    //=====================================================================
    {
        std::cout << "3. Buffer reuse with clear()\n";
        std::cout << "----------------------------\n";

        StringBuilder builder;

        // First use
        builder.append( "First use" );
        std::cout << "First: " << builder.toString() << "\n";

        // Reuse same buffer
        builder.clear();
        builder.append( "Second use" );
        std::cout << "Second: " << builder.toString() << "\n";

        std::cout << "Note: clear() resets size but retains capacity for reuse\n";
        std::cout << "Capacity after clear: " << builder.capacity() << " bytes\n";
        std::cout << "\n";
    }

    //=====================================================================
    // 4. Iterator support
    //=====================================================================
    {
        std::cout << "4. Iterator support\n";
        std::cout << "-------------------\n";

        StringBuilder builder;
        builder.append( "Iterator" );

        std::cout << "Characters: ";
        for( char c : builder )
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

        StringBuilder builder;
        builder.append( "Hello World" );

        std::cout << "Original: " << builder.toString() << "\n";
        std::cout << "Character at [6]: '" << builder[6] << "'\n";

        builder[6] = 'w';
        std::cout << "After [6] = 'w': " << builder.toString() << "\n";
        std::cout << "\n";
    }

    //=====================================================================
    // 6. Building complex strings (SQL query example)
    //=====================================================================
    {
        std::cout << "6. Building complex strings (SQL query example)\n";
        std::cout << "-----------------------------------------------\n";

        StringBuilder builder;

        std::vector<std::string> columns{ "id", "name", "email" };
        std::vector<std::string> conditions{ "active = 1", "age > 18" };

        // Start with SELECT clause
        builder.append( "SELECT " );
        for( size_t i = 0; i < columns.size(); ++i )
        {
            if( i > 0 )
                builder.append( ", " );
            builder.append( columns[i] );
        }

        // Add WHERE clause using stream operators
        builder << " FROM users WHERE ";
        for( size_t i = 0; i < conditions.size(); ++i )
        {
            if( i > 0 )
                builder << " AND ";
            builder << conditions[i];
        }

        std::cout << "Query: " << builder.toString() << "\n";
        std::cout << "Note: Mix append() for iteration and << for simple concatenation\n";
        std::cout << "\n";
    }

    //=====================================================================
    // 7. Thread safety demonstration
    //=====================================================================
    {
        std::cout << "7. Thread safety demonstration\n";
        std::cout << "------------------------------\n";

        std::vector<std::thread> threads;
        std::vector<std::string> results( 4 );

        for( size_t t = 0; t < 4; ++t )
        {
            threads.emplace_back( [t, &results]() {
                StringBuilder builder;
                builder.append( "Thread " );
                builder.append( std::to_string( t ) );
                builder.append( " data" );
                results[t] = builder.toString();
            } );
        }

        for( auto& thread : threads )
        {
            thread.join();
        }

        for( size_t i = 0; i < results.size(); ++i )
        {
            std::cout << "  " << results[i] << "\n";
        }

        std::cout << "Note: StringBuilder instances are thread-safe when not shared\n";
        std::cout << "\n";
    }

    //=====================================================================
    // 8. std::format integration with StringBuilder
    //=====================================================================
    {
        std::cout << "8. std::format integration with StringBuilder\n";
        std::cout << "-----------------------------------------------\n";

#if defined( NFX_STRINGBUILDER_HAS_STD_FORMAT )
        StringBuilder builder;
        builder.append( "Formatted content" );

        // Direct formatting of StringBuilder
        std::string formatted = std::format( "Buffer contains: {}", builder );

        std::cout << formatted << "\n";

        // Formatting in logging scenarios
        std::cout << std::format( "Debug: buffer size={}, content={}\n", builder.size(), builder );

        std::cout << "Note: Zero-copy formatting via std::formatter specialization\n";
#else
        std::cout << "Note: std::format integration requires C++20 (disabled in this build)\n";
#endif
        std::cout << "\n";
    }

    //=====================================================================
    // 9. std::format integration with StringBuilder
    //=====================================================================
    {
        std::cout << "9. std::format integration with StringBuilder\n";
        std::cout << "----------------------------------------------\n";

#if defined( NFX_STRINGBUILDER_HAS_STD_FORMAT )
        StringBuilder builder;
        builder.append( "Building..." );

        std::cout << std::format( "Current state: {}\n", builder );

        builder.append( " more text" );
        std::cout << std::format( "After append: {}\n", builder );

        std::cout << "Note: Useful for debugging string construction process\n";
#else
        std::cout << "Note: std::format integration requires C++20 (disabled in this build)\n";
#endif
        std::cout << "\n";
    }

    //=====================================================================
    // 10. Numeric type support (integers, floats, booleans)
    //=====================================================================
    {
        std::cout << "10. Numeric type support (integers, floats, booleans)\n";
        std::cout << "-----------------------------------------------------\n";

        StringBuilder builder;

        // Stream operators with integers
        std::int32_t count = 42;
        std::uint64_t bigNumber = 18446744073709551615ULL;
        builder << "Count: " << count << ", Big: " << bigNumber;
        std::cout << "Integers: " << builder.toString() << "\n";

        // Stream operators with floats
        builder.clear();
        float pi = 3.14159f;
        double e = 2.718281828;
        builder << "Pi: " << pi << ", E: " << e;
        std::cout << "Floats: " << builder.toString() << "\n";

        // Stream operators with booleans
        builder.clear();
        bool enabled = true;
        bool disabled = false;
        builder << "Enabled: " << enabled << ", Disabled: " << disabled;
        std::cout << "Booleans: " << builder.toString() << "\n";

        // Mixed types with stream operators
        builder.clear();
        builder << "Result: " << 10 << " + " << 20 << " = " << 30;
        std::cout << "Mixed: " << builder.toString() << "\n";

        std::cout << "Note: All numeric types seamlessly integrate with << operator\n";
        std::cout << "\n";
    }

    //=====================================================================
    // 11. Type-safe formatting with format() method
    //=====================================================================
    {
        std::cout << "11. Type-safe formatting with format() method\n";
        std::cout << "----------------------------------------------\n";

#if defined( NFX_STRINGBUILDER_HAS_STD_FORMAT )
        StringBuilder builder;

        std::string username = "Alice";
        int userId = 12345;
        builder.append( std::format( "User {} (ID: {})", username, userId ) );
        std::cout << "Basic: " << builder.toString() << "\n";

        builder.clear();
        builder.append( std::format( "Hex: 0x{:X}, Binary: 0b{:b}, Padded: {:08}", 255, 42, 7 ) );
        std::cout << "Specifiers: " << builder.toString() << "\n";

        builder.clear();
        double pi = 3.141592653589793;
        builder.append( std::format( "Pi with 2 decimals: {:.2f}, Scientific: {:.2e}", pi, pi ) );
        std::cout << "Precision: " << builder.toString() << "\n";

        builder.clear();
        builder.append( std::format( "Left: {:<10} | Center: {:^10} | Right: {:>10}", "abc", "def", "ghi" ) );
        std::cout << "Alignment: " << builder.toString() << "\n";

        builder.clear();
        builder.append( "Prefix: " );
        builder.append( std::to_string( 100 ) );
        builder.append( " | " );
        builder.append( std::format( "Formatted: {} items @ ${:.2f} each", 5, 19.99 ) );
        std::cout << "Chaining: " << builder.toString() << "\n";

        std::cout << "Note: format() uses std::format internally for zero-overhead type-safe formatting\n";
#else
        std::cout << "Note: std::format-based formatting requires C++20 (disabled in this build)\n";
#endif
        std::cout << "\n";
    }

    //=====================================================================
    // 12. Capacity hints for pre-sized buffers
    //=====================================================================
    {
        std::cout << "12. Capacity hints for pre-sized buffers\n";
        std::cout << "-----------------------------------------\n";

        // Without capacity hint (default behavior)
        StringBuilder builder1;
        std::cout << "Default capacity: " << builder1.capacity() << " bytes\n";

        // With capacity hint (1KB pre-allocated)
        StringBuilder builder2( 1024 );
        std::cout << "With hint (1024): " << builder2.capacity() << " bytes\n";

        // Building large string with hint
        StringBuilder builder( 2048 );

        builder.append( "SELECT id, name, email, created_at, updated_at, status, role, department " );
        builder.append( "FROM users " );
        builder.append( "WHERE status = 'active' AND role IN ('admin', 'moderator', 'user') " );
        builder.append( "AND created_at >= '2024-01-01' " );
        builder.append( "ORDER BY created_at DESC " );
        builder.append( "LIMIT 100;" );

        std::cout << "Built query: " << builder.toString().size() << " bytes\n";
        std::cout << "Buffer capacity: " << builder.capacity() << " bytes\n";
        std::cout << "Note: Pre-allocation avoids multiple reallocations during string construction\n";
        std::cout << "\n";
    }

    //=====================================================================
    // 13. Understanding reserve() vs resize() - Common Pitfall
    //=====================================================================
    {
        std::cout << "13. Understanding reserve() vs resize() - Common Pitfall\n";
        std::cout << "--------------------------------------------------------\n";

        // WRONG: Using resize() for pre-allocation
        std::cout << "Wrong approach - using resize() for pre-allocation:\n";
        {
            StringBuilder builder;

            builder.resize( 100 );
            std::cout << "  After resize(100): size=" << builder.size() << " bytes\n";
            std::cout << "  Content contains garbage (uninitialized memory)\n";

            builder.append( "Hello" );
            std::cout << "  After append('Hello'): size=" << builder.size() << " bytes\n";
            std::cout << "  Problem: 'Hello' is at position 100, preceded by garbage\n";
            std::cout << "\n";
        }

        // CORRECT: Using reserve() for pre-allocation
        std::cout << "Correct approach - using reserve() for pre-allocation:\n";
        {
            StringBuilder builder;

            builder.reserve( 100 );
            std::cout << "  After reserve(100): size=" << builder.size() << ", capacity=" << builder.capacity()
                      << " bytes\n";

            builder.append( "Hello" );
            std::cout << "  After append('Hello'): size=" << builder.size() << " bytes\n";
            std::cout << "  Content: '" << builder.toString() << "'\n";
            std::cout << "  Success: 'Hello' starts at position 0, no garbage\n";
            std::cout << "\n";
        }

        // VALID: Using resize() for raw buffer manipulation
        std::cout << "Valid use of resize() - raw buffer manipulation:\n";
        {
            StringBuilder builder;

            builder.resize( 32 );        // Allocate fixed-size buffer
            char* data = builder.data(); // Get raw pointer

            // Write directly to buffer (e.g., from C API, fread, etc.)
            std::memcpy( data, "Direct write to buffer memory!", 31 );
            data[31] = '\0';

            std::cout << "  After resize(32) and direct write: '" << builder.toString() << "'\n";
            std::cout << "  Use case: Direct memory operations, C API integration\n";
            std::cout << "\n";
        }

        // Best practice: Capacity hints
        std::cout << "Best practice - capacity hints:\n";
        {
            StringBuilder builder( 100 );
            std::cout << "  After StringBuilder(100): size=" << builder.size() << ", capacity=" << builder.capacity()
                      << " bytes\n";

            builder.append( "Hello" );
            std::cout << "  After append('Hello'): '" << builder.toString() << "'\n";
            std::cout << "\n";
        }

        std::cout << "Alternative - buffer.reserve() method:\n";
        {
            StringBuilder builder;

            builder.reserve( 100 );
            std::cout << "  After buffer.reserve(100): size=" << builder.size() << ", capacity=" << builder.capacity()
                      << " bytes\n";

            builder.append( "Hello" );
            std::cout << "  After append('Hello'): '" << builder.toString() << "'\n";
            std::cout << "\n";
        }

        std::cout << "Summary:\n";
        std::cout << "  reserve(n):       Allocates capacity, keeps size=0 (for pre-allocation)\n";
        std::cout << "  resize(n):        Sets size=n immediately (for raw buffer operations)\n";
        std::cout << "  StringBuilder(n): Pre-allocates with capacity hint (best practice)\n";
        std::cout << "\n";
    }

    //=====================================================================
    // 14. Variadic append() - Batch operations
    //=====================================================================
    {
        std::cout << "14. Variadic append() - Batch operations\n";
        std::cout << "----------------------------------------\n";

        // Traditional approach: Chained append() calls
        std::cout << "Traditional approach - chained append():\n";
        {
            StringBuilder builder;
            builder.append( "User " ).append( "12345" ).append( " (" ).append( "Alice" ).append( ") logged in" );

            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: Chained append() calls return reference\n";
            std::cout << "\n";
        }

        // Modern approach: Stream operators
        std::cout << "Modern approach - stream operators:\n";
        {
            StringBuilder builder;
            builder << "User " << 12345 << " (" << "Alice" << ") logged in";

            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: Stream operators handle any type automatically\n";
            std::cout << "\n";
        }

        // Batching approach: Variadic append()
        std::cout << "Batching approach - variadic append():\n";
        {
            StringBuilder builder;
            builder.append( "User ", 12345, " (", "Alice", ") logged in" );

            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: Single append() call with multiple arguments\n";
            std::cout << "\n";
        }

        // Batching with mixed types
        std::cout << "Batching with mixed types:\n";
        {
            StringBuilder builder;
            int userId = 42;
            std::string userName = "Bob";
            double loginTime = 123.456;
            bool success = true;

            builder.append( "User ", userId, " (", userName, ") logged in at ", loginTime, " ms - Success: ", success );

            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: Handles int, string, double, bool in one call\n";
            std::cout << "\n";
        }

        // Performance: Single batch vs multiple appends
        std::cout << "Performance comparison:\n";
        {
            // Multiple calls
            StringBuilder builder1;
            auto start1 = std::chrono::high_resolution_clock::now();
            for( int i = 0; i < 1000; ++i )
            {
                builder1.clear();
                builder1.append( "Item " );
                builder1.append( i );
                builder1.append( ": " );
                builder1.append( "Value" );
            }
            auto end1 = std::chrono::high_resolution_clock::now();
            auto duration1 = std::chrono::duration_cast<std::chrono::microseconds>( end1 - start1 ).count();

            // Single batch call
            StringBuilder builder2;
            auto start2 = std::chrono::high_resolution_clock::now();
            for( int i = 0; i < 1000; ++i )
            {
                builder2.clear();
                builder2.append( "Item ", i, ": ", "Value" );
            }
            auto end2 = std::chrono::high_resolution_clock::now();
            auto duration2 = std::chrono::duration_cast<std::chrono::microseconds>( end2 - start2 ).count();

            std::cout << "  Multiple append() calls: " << duration1 << " µs\n";
            std::cout << "  Single batch append():   " << duration2 << " µs\n";
            std::cout << "  Speedup: " << std::fixed << std::setprecision( 2 )
                      << ( static_cast<double>( duration1 ) / duration2 ) << "x\n";
            std::cout << "  Note: Batching reduces function call overhead\n";
            std::cout << "\n";
        }

        // SQL query construction with chaining
        std::cout << "SQL query construction (method chaining):\n";
        {
            StringBuilder builder;
            std::string table = "users";
            int minAge = 18;
            std::string status = "active";

            builder.append( "SELECT id, name, email FROM " )
                .append( table )
                .append( " WHERE age >= " )
                .append( minAge )
                .append( " AND status = '" )
                .append( status )
                .append( "'" );

            std::cout << "  Query: " << builder.toString() << "\n";
            std::cout << "\n";
        }

        // SQL query construction with stream operators
        std::cout << "SQL query construction (stream operators):\n";
        {
            StringBuilder builder;
            std::string table = "users";
            int minAge = 18;
            std::string status = "active";

            builder << "SELECT id, name, email FROM " << table << " WHERE age >= " << minAge << " AND status = '"
                    << status << "'";

            std::cout << "  Query: " << builder.toString() << "\n";
            std::cout << "  Note: Stream operators mix string literals, variables, and numeric types\n";
            std::cout << "\n";
        }

        // SQL query construction with batching
        std::cout << "SQL query construction (batch append):\n";
        {
            StringBuilder builder;
            std::string table = "users";
            int minAge = 18;
            std::string status = "active";

            builder.append(
                "SELECT id, name, email FROM ", table, " WHERE age >= ", minAge, " AND status = '", status, "'" );

            std::cout << "  Query: " << builder.toString() << "\n";
            std::cout << "  Note: Single append() with all arguments - most efficient\n";
            std::cout << "\n";
        }

        // Comparison of both styles
        std::cout << "Comparison of all three styles:\n";
        {
            StringBuilder builder1;
            builder1.append( "User " )
                .append( 42 )
                .append( ": " )
                .append( "Bob" )
                .append( " (age: " )
                .append( 25 )
                .append( ")" );

            StringBuilder builder2;
            builder2 << "User " << 42 << ": " << "Bob" << " (age: " << 25 << ")";

            StringBuilder builder3;
            builder3.append( "User ", 42, ": ", "Bob", " (age: ", 25, ")" );

            std::cout << "  Method chaining:   " << builder1.toString() << "\n";
            std::cout << "  Stream operators:  " << builder2.toString() << "\n";
            std::cout << "  Batch append():    " << builder3.toString() << "\n";
            std::cout << "  Note: All produce identical results\n";
            std::cout << "        - Chaining: Most explicit, good for complex logic\n";
            std::cout << "        - Streams:  Most readable, familiar C++ idiom\n";
            std::cout << "        - Batching: Most efficient, fewest function calls\n";
        }

        std::cout << "\n";
    }

    //=====================================================================
    // 15. Line-based operations - appendLine() / appendLn()
    //=====================================================================
    {
        std::cout << "15. Line-based operations - appendLine() / appendLn()\n";
        std::cout << "------------------------------------------------------\n";

        // Basic line appending
        std::cout << "Basic line appending:\n";
        {
            StringBuilder builder;
            builder.appendLine( "First line" ).appendLine( "Second line" ).appendLine( "Third line" );

            std::cout << "  Result:\n" << builder.toString();
            std::cout << "  Note: Each appendLine() adds '\\n' automatically\n";
            std::cout << "\n";
        }

        // Building CSV-like data
        std::cout << "Building CSV-like data:\n";
        {
            StringBuilder builder;
            builder.appendLine( "Name,Age,City" )
                .appendLine( "Alice,30,Paris" )
                .appendLine( "Bob,25,London" )
                .appendLine( "Charlie,35,Berlin" );

            std::cout << "  Result:\n" << builder.toString();
            std::cout << "\n";
        }

        std::cout << "Building NMEA-0183 messages with checksums:\n";
        {
            StringBuilder builder;

            // Helper lambda to build NMEA sentence with checksum
            auto buildNmeaSentence = [&builder]( std::string_view sentence ) {
                builder.append( "$" ).append( sentence );
                std::string checksum = calculateNmeaChecksum( sentence );
                builder.append( "*" ).append( checksum );
                builder.appendLine();
            };

            // GPS Fix Data (GPGGA)
            buildNmeaSentence( "GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,," );

            // GPS DOP and active satellites (GPGSA)
            buildNmeaSentence( "GPGSA,A,3,04,05,,09,12,,,24,,,,,2.5,1.3,2.1" );

            // Satellites in view (GPGSV)
            buildNmeaSentence( "GPGSV,2,1,08,01,40,083,46,02,17,308,41,12,07,344,39,14,22,228,45" );

            // Meteorological Composite (WIMDA) - Full message
            buildNmeaSentence( "WIMDA,29.92,I,1.013,B,15.5,C,10.2,C,45.0,,12.5,C,270.0,T,270.0,M,5.2,N,2.7,M" );

            // Wind Speed and Angle (WIMWV)
            buildNmeaSentence( "WIMWV,270.0,T,5.2,N,A" );

            // Water Temperature (WIMTW)
            buildNmeaSentence( "WIMTW,10.2,C" );

            std::cout << "  Result:\n" << builder.toString();
            std::cout << "  Note: All messages include valid NMEA-0183 checksums\n";
            std::cout << "        Format: $SENTENCE*XX where XX is XOR checksum\n";
            std::cout << "\n";
        }

        // Building a realistic NMEA-0183 data stream
        std::cout << "Building realistic NMEA-0183 data stream:\n";
        {
            StringBuilder builder;

            // Simulate a GPS/Weather station output
            auto addNmea = [&builder]( std::string_view sentence ) {
                builder.append( "$" ).append( sentence );
                builder.append( "*" ).append( calculateNmeaChecksum( sentence ) );
                builder.appendLine();
            };

            // Position update
            addNmea( "GPRMC,123519,A,4807.038,N,01131.000,E,022.4,084.4,230394,003.1,W" );
            addNmea( "GPGGA,123519,4807.038,N,01131.000,E,1,08,0.9,545.4,M,46.9,M,," );

            // Weather data
            addNmea( "WIMDA,30.12,I,1.020,B,18.3,C,12.5,C,52.0,,10.8,C,285.0,T,285.0,M,12.5,N,6.4,M" );
            addNmea( "WIMWV,285.0,T,12.5,N,A" );

            // Depth sounder
            addNmea( "SDDBT,45.2,f,13.8,M,7.5,F" );

            std::cout << "  Result:\n" << builder.toString();
            std::cout << "\n";
        }

        // Building log entries
        std::cout << "Building multi-line log entries:\n";
        {
            StringBuilder builder;

            auto now = std::chrono::system_clock::now();
            auto timestamp = std::chrono::duration_cast<std::chrono::milliseconds>( now.time_since_epoch() ).count();

            builder.appendLine( "[INFO] Application started" )
                .append( "[DEBUG] Timestamp: " )
                .append( timestamp )
                .appendLine()
                .appendLine( "[INFO] Configuration loaded" )
                .appendLine( "[WARN] Cache not found, creating new" );

            std::cout << "  Result:\n" << builder.toString();
            std::cout << "\n";
        }

        // Building JSON-like structure (simplified)
        std::cout << "Building structured text:\n";
        {
            StringBuilder builder;
            builder.appendLine( "{" )
                .appendLine( "  \"name\": \"StringBuilder\"," )
                .appendLine( "  \"version\": \"0.5.0\"," )
                .appendLine( "  \"features\": [" )
                .appendLine( "    \"SBO\"," )
                .appendLine( "    \"Zero-copy\"," )
                .appendLine( "    \"High-performance\"" )
                .appendLine( "  ]" )
                .appendLine( "}" );

            std::cout << "  Result:\n" << builder.toString();
            std::cout << "  Note: Indentation handled manually\n";
            std::cout << "\n";
        }

        std::cout << "\n";
    }

    //=====================================================================
    // 16. Join operations - Combining container elements
    //=====================================================================
    {
        std::cout << "16. Join operations - Combining container elements\n";
        std::cout << "---------------------------------------------------\n";

        // Basic join with string vector
        std::cout << "Basic join with string delimiter:\n";
        {
            StringBuilder builder;
            std::vector<std::string> words = { "Hello", "World", "from", "StringBuilder" };
            builder.join( words, " " );

            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: Joins vector elements with delimiter\n";
            std::cout << "\n";
        }

        // Join with character delimiter
        std::cout << "Join with character delimiter:\n";
        {
            StringBuilder builder;
            std::vector<std::string> fields = { "Alice", "30", "Paris" };
            builder.join( fields, ',' );

            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: Single char delimiter (optimized)\n";
            std::cout << "\n";
        }

        // Join numeric values
        std::cout << "Join numeric values:\n";
        {
            StringBuilder builder;
            std::vector<int> numbers = { 1, 2, 3, 4, 5 };
            builder.join( numbers, ", " );

            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: Works with any type supporting append()\n";
            std::cout << "\n";
        }

        // Building CSV line
        std::cout << "Building CSV line:\n";
        {
            StringBuilder builder;
            std::vector<std::string> csvFields = { "Name", "Age", "City", "Country" };

            // Header
            builder.join( csvFields, "," ).appendLine();

            // Data rows
            std::vector<std::string> row1 = { "Alice", "30", "Paris", "France" };
            builder.join( row1, "," ).appendLine();

            std::vector<std::string> row2 = { "Bob", "25", "London", "UK" };
            builder.join( row2, "," ).appendLine();

            std::cout << "  Result:\n" << builder.toString();
            std::cout << "\n";
        }

        // Building NMEA sentence fields
        std::cout << "Building NMEA sentence with join():\n";
        {
            StringBuilder builder;
            std::vector<std::string> gpsFields = { "GPGGA", "123519", "4807.038", "N",    "01131.000", "E", "1", "08",
                                                   "0.9",   "545.4",  "M",        "46.9", "M",         "",  "" };

            builder.append( "$" );
            builder.join( gpsFields, "," );

            // Calculate checksum on the sentence (without $)
            std::string sentence = builder.toString().substr( 1 ); // Skip $
            std::string checksum = calculateNmeaChecksum( sentence );

            builder.append( "*" ).append( checksum );
            builder.appendLine();

            std::cout << "  Result: " << builder.toString();
            std::cout << "  Note: join() simplifies NMEA field assembly\n";
            std::cout << "\n";
        }

        // Join with prefix/suffix
        std::cout << "Join with wrapping:\n";
        {
            StringBuilder builder;
            std::vector<std::string> items = { "apple", "banana", "cherry" };

            builder.append( "Items: [" );
            builder.join( items, ", " );
            builder.append( "]" );

            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "\n";
        }

        // Join with array
        std::cout << "Join with std::array:\n";
        {
            StringBuilder builder;
            std::array<int, 4> coordinates = { 10, 20, 30, 40 };
            builder.append( "Coordinates: " );
            builder.join( coordinates, ", " );

            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "\n";
        }

        // Empty container
        std::cout << "Join empty container:\n";
        {
            StringBuilder builder;
            std::vector<std::string> empty;
            builder.append( "Before" );
            builder.join( empty, "," );
            builder.append( "After" );

            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: Empty join doesn't add anything\n";
            std::cout << "\n";
        }

        // Building SQL IN clause
        std::cout << "Building SQL IN clause:\n";
        {
            StringBuilder builder;
            std::vector<int> userIds = { 101, 102, 103, 104 };

            builder.append( "SELECT * FROM users WHERE id IN (" );
            builder.join( userIds, ", " );
            builder.append( ")" );

            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "\n";
        }

        std::cout << "\n";
    }

    //=====================================================================
    // 17. Prepend operations - Building strings in reverse
    //=====================================================================
    {
        std::cout << "17. Prepend operations - Building strings in reverse\n";
        std::cout << "----------------------------------------------------\n";

        // Basic prepend
        std::cout << "Basic prepend:\n";
        {
            StringBuilder builder;
            builder.append( "World" );
            builder.prepend( "Hello " );
            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: prepend() shifts existing content and inserts at beginning\n";
            std::cout << "\n";
        }

        // Prepend chaining
        std::cout << "Prepend chaining:\n";
        {
            StringBuilder builder;
            builder.append( "!" );
            builder.prepend( "World" ).prepend( " " ).prepend( "Hello" );
            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: Chained prepends execute left-to-right\n";
            std::cout << "\n";
        }

        // Mixing append and prepend
        std::cout << "Mixing append and prepend:\n";
        {
            StringBuilder builder;
            builder.append( "Middle" );
            builder.prepend( "Start " );
            builder.append( " End" );
            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: Useful for wrapping existing content\n";
            std::cout << "\n";
        }

        // Prepend with numeric types
        std::cout << "Prepend with numeric types:\n";
        {
            StringBuilder builder;
            builder.append( " items" );
            builder.prepend( 42 );
            std::cout << "  Result: " << builder.toString() << "\n";

            builder.clear();
            builder.append( " seconds" );
            builder.prepend( 3.14 );
            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "\n";
        }

        // Building stack-like structures
        std::cout << "Building stack-like structures (LIFO):\n";
        {
            StringBuilder builder;
            for( int i = 1; i <= 5; ++i )
            {
                builder.prepend( std::to_string( i ) ).prepend( " " );
            }
            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: Each prepend pushes previous content right\n";
            std::cout << "\n";
        }

        // Log prefix injection
        std::cout << "Log prefix injection:\n";
        {
            StringBuilder builder;
            builder.append( "User logged in successfully" );

            // Add timestamp prefix
            builder.prepend( " " );
            builder.prepend( "10:30:45" );

            // Add log level
            builder.prepend( " " );
            builder.prepend( "[INFO]" );

            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: Build message first, then add metadata\n";
            std::cout << "\n";
        }

        // Performance consideration
        std::cout << "Performance consideration:\n";
        {
            std::cout << "  prepend() uses memmove() to shift existing content\n";
            std::cout << "  Time complexity: O(n) where n is current buffer size\n";
            std::cout << "  Best practice: Build in append order when possible\n";
            std::cout << "  Use prepend() when logic requires reverse construction\n";
            std::cout << "\n";
        }

        // Wrapping content
        std::cout << "Wrapping content with quotes:\n";
        {
            StringBuilder builder;
            std::string username = "Alice";
            builder.append( username );
            builder.prepend( "\"" );
            builder.append( "\"" );
            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "\n";
        }

        // Building paths in reverse
        std::cout << "Building paths in reverse:\n";
        {
            StringBuilder builder;
            builder.append( "file.txt" );
            builder.prepend( "/" );
            builder.prepend( "documents" );
            builder.prepend( "/" );
            builder.prepend( "home" );
            builder.prepend( "/" );
            std::cout << "  Result: " << builder.toString() << "\n";
            std::cout << "  Note: Useful when traversing directory tree upward\n";
            std::cout << "\n";
        }
    }

    return 0;
}
