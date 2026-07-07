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
 * @file StringBuilder.h
 * @brief High-performance string building with Small Buffer Optimization (SBO)
 * @details Growable character buffer optimized for efficient string construction
 *
 * ## StringBuilder Memory Architecture:
 *
 * ```
 * Memory Layout:
 * ┌──────────────────────────────────────────────────────┐
 * │              StringBuilder Buffer                    │
 * ├──────────────────────────────────────────────────────┤
 * │  Size ≤ 256 bytes:                                   │
 * │  ┌─────────────────────────────────────┐             │
 * │  │   Stack Buffer (SBO)                │             │
 * │  │   - Zero heap allocations           │             │
 * │  │   - Cache-friendly (256 bytes)      │             │
 * │  │   - Automatic cleanup               │             │
 * │  └─────────────────────────────────────┘             │
 * │                                                      │
 * │  Size > 256 bytes:                                   │
 * │  ┌─────────────────────────────────────┐             │
 * │  │   Heap Buffer (Dynamic)             │             │
 * │  │   - Exponential growth (< 8KB)      │             │
 * │  │   - Conservative growth (≥ 8KB)     │             │
 * │  │   - Managed by unique_ptr           │             │
 * │  └─────────────────────────────────────┘             │
 * └──────────────────────────────────────────────────────┘
 * ```
 *
 * ## Growth Strategy:
 *
 * ```
 * Buffer Expansion:
 * ┌──────────────────────────────────────────────────────┐
 * │  Small buffers (< 8KB):                              │
 * │    capacity_new = capacity_old × 2.0                 │
 * │    → Aggressive growth for frequent appends          │
 * │                                                      │
 * │  Large buffers (≥ 8KB):                              │
 * │    capacity_new = capacity_old + (capacity_old / 2)  │
 * │    → Conservative growth to limit memory overhead    │
 * └──────────────────────────────────────────────────────┘
 * ```
 */

#pragma once

#include <cstdint>
#include <memory>
#include <string>
#include <string_view>

#if defined( __has_include )
#    if __has_include( <version> )
#        include <version>
#    endif
#endif

/**
 * @def NFX_STRINGBUILDER_HAS_STD_FORMAT
 * @brief Defined to 1 when C++20 std::format support is available
 * @details std::format requires C++20. When building under C++17 (or with a
 *          standard library that lacks <format>), the format() method and the
 *          std::formatter specialization are disabled, while the rest of the
 *          StringBuilder API remains fully available.
 */
#if defined( __cpp_lib_format ) && __cpp_lib_format >= 201907L
#    include <format>
#    define NFX_STRINGBUILDER_HAS_STD_FORMAT 1
#endif

/**
 * @def NFX_STRINGBUILDER_FORCE_INLINE
 * @brief Cross-platform forced inline macro for critical hot-path methods
 */
#if defined( _MSC_VER )
#    define NFX_STRINGBUILDER_FORCE_INLINE __forceinline
#elif defined( __GNUC__ ) || defined( __clang__ )
#    define NFX_STRINGBUILDER_FORCE_INLINE __attribute__( ( always_inline ) ) inline
#else
#    define NFX_STRINGBUILDER_FORCE_INLINE inline
#endif

/**
 * @def NFX_STRINGBUILDER_LIKELY
 * @def NFX_STRINGBUILDER_UNLIKELY
 * @brief Branch-prediction hint attributes (C++20 [[likely]]/[[unlikely]])
 * @details Expand to the C++20 attributes when supported and to nothing under
 *          C++17, keeping hot-path branch hints warning-free across standards.
 *          The language-version check is required because some compilers report
 *          __has_cpp_attribute(likely) as available in C++17 as an extension.
 */
#if defined( _MSVC_LANG )
#    define NFX_STRINGBUILDER_CPLUSPLUS _MSVC_LANG
#else
#    define NFX_STRINGBUILDER_CPLUSPLUS __cplusplus
#endif

#if NFX_STRINGBUILDER_CPLUSPLUS >= 202002L && defined( __has_cpp_attribute )
#    if __has_cpp_attribute( likely ) >= 201803L
#        define NFX_STRINGBUILDER_LIKELY [[likely]]
#        define NFX_STRINGBUILDER_UNLIKELY [[unlikely]]
#    endif
#endif
#if !defined( NFX_STRINGBUILDER_LIKELY )
#    define NFX_STRINGBUILDER_LIKELY
#    define NFX_STRINGBUILDER_UNLIKELY
#endif

/**
 * @def NFX_STRINGBUILDER_HAS_FLOAT_TO_CHARS
 * @brief Whether std::to_chars(float/double) is usable on this toolchain.
 * @details Integer std::to_chars is universally available, but the
 *          floating-point overloads are not: IAR's libc++ ships them as deleted
 *          functions. When unavailable the float/double formatters fall back to
 *          std::snprintf. Define this to 0/1 before including to override.
 */
#if !defined( NFX_STRINGBUILDER_HAS_FLOAT_TO_CHARS )
#    if defined( __IAR_SYSTEMS_ICC__ )
#        define NFX_STRINGBUILDER_HAS_FLOAT_TO_CHARS 0
#    else
#        define NFX_STRINGBUILDER_HAS_FLOAT_TO_CHARS 1
#    endif
#endif

namespace nfx::string
{
    //=====================================================================
    // StringBuilder class
    //=====================================================================

    /**
     * @brief High-performance string builder with efficient memory management
     * @details Provides a growable character buffer optimized for string building operations.
     *          Features automatic capacity management, Small Buffer Optimization (SBO),
     *          iterator support, and zero-copy string_view access.
     *
     * @warning Not thread-safe - external synchronization required for concurrent access.
     *          Each StringBuilder instance should be used by a single thread or protected
     *          by external synchronization mechanisms.
     */
    class StringBuilder final
    {
    public:
        //----------------------------------------------
        // Construction
        //----------------------------------------------

        /** @brief Default constructor */
        StringBuilder();

        /**
         * @brief Constructor with specified initial capacity
         * @param initialCapacity Initial buffer capacity in bytes
         * @details Pre-allocates buffer to avoid reallocations during initial growth
         */
        explicit StringBuilder( size_t initialCapacity );

        /**
         * @brief Copy constructor
         * @param other The StringBuilder to copy from
         */
        StringBuilder( const StringBuilder& other );

        /**
         * @brief Move constructor
         * @param other The StringBuilder to move from
         */
        StringBuilder( StringBuilder&& other ) noexcept;

        //----------------------------------------------
        // Destruction
        //----------------------------------------------

        /** @brief Destructor */
        ~StringBuilder() = default;

        //----------------------------------------------
        // Assignment
        //----------------------------------------------

        /**
         * @brief Copy assignment operator
         * @param other The StringBuilder to copy from
         * @return Reference to this StringBuilder after assignment
         */
        StringBuilder& operator=( const StringBuilder& other );

        /**
         * @brief Move assignment operator
         * @param other The StringBuilder to move from
         * @return Reference to this StringBuilder after assignment
         */
        StringBuilder& operator=( StringBuilder&& other ) noexcept;

        //----------------------------------------------
        // Capacity and size information
        //----------------------------------------------

        /**
         * @brief Get current buffer size in bytes
         * @return Number of bytes currently stored in buffer
         * @details Returns actual content size, not allocated capacity
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline size_t size() const noexcept;

        /**
         * @brief Get current buffer capacity in bytes
         * @return Number of bytes allocated for buffer storage
         * @details Capacity may be larger than size to avoid frequent reallocations
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline size_t capacity() const noexcept;

        /**
         * @brief Check if buffer is empty
         * @return true if buffer contains no data, false otherwise
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline bool isEmpty() const noexcept;

        //----------------------------------------------
        // Data access
        //----------------------------------------------

        /**
         * @brief Get mutable pointer to buffer data
         * @return Pointer to first byte of buffer data
         * @details Provides direct memory access for high-performance operations
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline char* data() noexcept;

        /**
         * @brief Get immutable pointer to buffer data
         * @return Const pointer to first byte of buffer data
         * @details Safe read-only access to buffer contents
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline const char* data() const noexcept;

        /**
         * @brief Access buffer element by index (mutable)
         * @param index Zero-based index of element to access
         * @return Reference to element at specified index
         * @details No bounds checking - undefined behavior if index >= size()
         */
        inline char& operator[]( size_t index );

        /**
         * @brief Access buffer element by index (immutable)
         * @param index Zero-based index of element to access
         * @return Const reference to element at specified index
         * @details No bounds checking - undefined behavior if index >= size()
         */
        inline const char& operator[]( size_t index ) const;

        //----------------------------------------------
        // Buffer management
        //----------------------------------------------

        /**
         * @brief Clear buffer content without deallocating memory
         * @details Sets size to 0 but preserves allocated capacity for reuse
         */
        inline void clear() noexcept;

        /**
         * @brief Reserve minimum capacity for buffer
         * @param newCapacity Minimum desired capacity in bytes
         * @details May allocate more than requested for efficiency
         * @throws std::bad_alloc if memory allocation fails
         */

        inline void reserve( size_t newCapacity );

        /**
         * @brief Resize buffer to specified size
         * @param newSize New buffer size in bytes
         * @details May truncate content or extend with undefined bytes
         * @throws std::bad_alloc if memory allocation fails
         */
        inline void resize( size_t newSize );

        //----------------------------------------------
        // Append operations
        //----------------------------------------------

        /**
         * @brief Appends string_view contents to the buffer efficiently
         * @param str String view to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( std::string_view str );

        /**
         * @brief Appends std::string contents to the buffer
         * @param str String to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( const std::string& str );

        /**
         * @brief Appends null-terminated C-string to the buffer
         * @param str Null-terminated C-string to append (null pointer handled gracefully)
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( const char* str );

        /**
         * @brief Appends string literal with compile-time length (zero strlen overhead)
         * @tparam N String literal length (deduced by compiler)
         * @param str String literal
         * @return Reference to this StringBuilder for chaining
         */
        template <size_t N>
        NFX_STRINGBUILDER_FORCE_INLINE StringBuilder& append( const char ( &str )[N] );

        /**
         * @brief Appends single character to the buffer
         * @param c Character to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( char c );

        /**
         * @brief Appends 8-bit signed integer to the buffer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( std::int8_t value );

        /**
         * @brief Appends 8-bit unsigned integer to the buffer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( std::uint8_t value );

        /**
         * @brief Appends 16-bit signed integer to the buffer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( std::int16_t value );

        /**
         * @brief Appends 16-bit unsigned integer to the buffer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( std::uint16_t value );

        /**
         * @brief Appends 32-bit signed integer to the buffer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( std::int32_t value );

        /**
         * @brief Appends 32-bit unsigned integer to the buffer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( std::uint32_t value );

        /**
         * @brief Appends 64-bit signed integer to the buffer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( std::int64_t value );

        /**
         * @brief Appends 64-bit unsigned integer to the buffer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( std::uint64_t value );

        /**
         * @brief Appends single-precision floating-point to the buffer
         * @param value Floating-point value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( float value );

        /**
         * @brief Appends double-precision floating-point to the buffer
         * @param value Floating-point value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& append( double value );

        //----------------------------------------------
        // Line operations
        //----------------------------------------------

        /**
         * @brief Appends string_view contents followed by a newline character
         * @param str String view to append (default: empty)
         * @return Reference to this StringBuilder for chaining
         * @details Efficiently appends the string and newline in one operation.
         *          Useful for building line-based formats like logs, CSV, or ISO 19848 messages.
         *
         * Example:
         * @code
         * builder.appendLine("Header")
         *        .appendLine("Data1")
         *        .appendLine("Data2");
         * // Result: "Header\nData1\nData2\n"
         * @endcode
         */
        inline StringBuilder& appendLine( std::string_view str = "" );

        /**
         * @brief Appends std::string contents followed by a newline character
         * @param str String to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& appendLine( const std::string& str );

        /**
         * @brief Appends null-terminated C-string followed by a newline character
         * @param str Null-terminated C-string to append (null pointer handled gracefully)
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& appendLine( const char* str );

        //----------------------------------------------
        // Prepend operations
        //----------------------------------------------

        /**
         * @brief Prepends string_view contents to the buffer
         * @param str String view to prepend
         * @return Reference to this StringBuilder for chaining
         * @details Shifts existing content and inserts at the beginning.
         *          Performance: O(n) due to memory shift operation.
         */
        StringBuilder& prepend( std::string_view str );

        /**
         * @brief Prepends std::string contents to the buffer
         * @param str String to prepend
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& prepend( const std::string& str );

        /**
         * @brief Prepends null-terminated C-string to the buffer
         * @param str Null-terminated C-string to prepend (null pointer handled gracefully)
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& prepend( const char* str );

        /**
         * @brief Prepends single character to the buffer
         * @param c Character to prepend
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& prepend( char c );

        /**
         * @brief Prepends 8-bit signed integer to the buffer
         * @param value Integer value to prepend
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& prepend( std::int8_t value );

        /**
         * @brief Prepends 8-bit unsigned integer to the buffer
         * @param value Integer value to prepend
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& prepend( std::uint8_t value );

        /**
         * @brief Prepends 16-bit signed integer to the buffer
         * @param value Integer value to prepend
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& prepend( std::int16_t value );

        /**
         * @brief Prepends 16-bit unsigned integer to the buffer
         * @param value Integer value to prepend
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& prepend( std::uint16_t value );

        /**
         * @brief Prepends 32-bit signed integer to the buffer
         * @param value Integer value to prepend
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& prepend( std::int32_t value );

        /**
         * @brief Prepends 32-bit unsigned integer to the buffer
         * @param value Integer value to prepend
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& prepend( std::uint32_t value );

        /**
         * @brief Prepends 64-bit signed integer to the buffer
         * @param value Integer value to prepend
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& prepend( std::int64_t value );

        /**
         * @brief Prepends 64-bit unsigned integer to the buffer
         * @param value Integer value to prepend
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& prepend( std::uint64_t value );

        /**
         * @brief Prepends single-precision floating-point to the buffer
         * @param value Floating-point value to prepend
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& prepend( float value );

        /**
         * @brief Prepends double-precision floating-point to the buffer
         * @param value Floating-point value to prepend
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& prepend( double value );

        //----------------------------------------------
        // Batch operations
        //----------------------------------------------

        /**
         * @brief Append multiple arguments in one call (strings, characters, numeric types)
         * @tparam T Type of the first argument (any type supported by append())
         * @tparam Args Types of the remaining arguments (any type supported by append())
         * @param first First argument to append
         * @param args Remaining arguments to append
         * @return Reference to this StringBuilder for chaining
         *
         * Example:
         * @code
         * builder.append("User ", userId, " (", userName, ") logged in at ", timestamp);
         * @endcode
         */
        template <typename T, typename... Args, typename = std::enable_if_t<( sizeof...( Args ) > 0 )>>
        inline StringBuilder& append( T&& first, Args&&... args );

        //----------------------------------------------
        // Join operations
        //----------------------------------------------

        /**
         * @brief Join container elements with delimiter
         * @tparam Container Container type (vector, array, list, etc.) with begin()/end()
         * @param items Container of items to join
         * @param delimiter Delimiter to insert between elements
         * @return Reference to this StringBuilder for chaining
         * @details Appends all container elements separated by delimiter.
         *          Works with any container providing iterators and elements convertible to string.
         *          Uses append() internally, so supports strings, string_view, and types with operator<<.
         * 
         * Example:
         * @code
         * std::vector<std::string> fields = {"GPGGA", "123519", "4807.038", "N"};
         * builder.join(fields, ",");  // "GPGGA,123519,4807.038,N"
         * 
         * std::vector<int> numbers = {1, 2, 3, 4, 5};
         * builder.join(numbers, ", ");  // "1, 2, 3, 4, 5"
         * @endcode
         */
        template <typename Container>
        inline StringBuilder& join( const Container& items, std::string_view delimiter );

        /**
         * @brief Join container elements with character delimiter
         * @tparam Container Container type (vector, array, list, etc.) with begin()/end()
         * @param items Container of items to join
         * @param delimiter Character delimiter to insert between elements
         * @return Reference to this StringBuilder for chaining
         * @details Optimized variant for single-character delimiters (e.g., ',', ';', '|').
         * 
         * Example:
         * @code
         * std::vector<std::string> fields = {"A", "B", "C"};
         * builder.join(fields, ',');  // "A,B,C"
         * @endcode
         */
        template <typename Container>
        inline StringBuilder& join( const Container& items, char delimiter );

        //----------------------------------------------
        // Stream operators
        //----------------------------------------------

        /**
         * @brief Stream operator for string_view
         * @param str String view to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( std::string_view str );

        /**
         * @brief Stream operator for std::string
         * @param str String to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( const std::string& str );

        /**
         * @brief Stream operator for C-string
         * @param str Null-terminated C-string to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( const char* str );

        /**
         * @brief Stream operator for string literal (zero strlen overhead)
         * @tparam N String literal length (deduced by compiler)
         * @param str String literal
         * @return Reference to this StringBuilder for chaining
         */
        template <size_t N>
        NFX_STRINGBUILDER_FORCE_INLINE StringBuilder& operator<<( const char ( &str )[N] );

        /**
         * @brief Stream operator for single character
         * @param c Character to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( char c );

        /**
         * @brief Stream operator for 8-bit signed integer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( std::int8_t value );

        /**
         * @brief Stream operator for 8-bit unsigned integer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( std::uint8_t value );

        /**
         * @brief Stream operator for 16-bit signed integer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( std::int16_t value );

        /**
         * @brief Stream operator for 16-bit unsigned integer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( std::uint16_t value );

        /**
         * @brief Stream operator for 32-bit signed integer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( std::int32_t value );

        /**
         * @brief Stream operator for 32-bit unsigned integer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( std::uint32_t value );

        /**
         * @brief Stream operator for 64-bit signed integer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( std::int64_t value );

        /**
         * @brief Stream operator for 64-bit unsigned integer
         * @param value Integer value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( std::uint64_t value );

        /**
         * @brief Stream operator for single-precision floating-point
         * @param value Floating-point value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( float value );

        /**
         * @brief Stream operator for double-precision floating-point
         * @param value Floating-point value to append
         * @return Reference to this StringBuilder for chaining
         */
        inline StringBuilder& operator<<( double value );

        //----------------------------------------------
        // Formatting operations
        //----------------------------------------------

#if defined( NFX_STRINGBUILDER_HAS_STD_FORMAT )
        /**
         * @brief Format and append text using std::format
         * @tparam Args Types of the formatting arguments
         * @param fmt Format string with format specifiers
         * @param args Arguments to be formatted
         * @return Reference to this StringBuilder for chaining
         * @details Provides type-safe formatting directly into the builder without intermediate allocations.
         *          Uses std::format_to with back_inserter for optimal performance.
         *          Example: builder.format("User {} (ID: {}) logged in at {}", name, userId, timestamp);
         * @note Requires C++20 std::format support (NFX_STRINGBUILDER_HAS_STD_FORMAT).
         */
        template <typename... Args>
        inline StringBuilder& format( std::format_string<Args...> fmt, Args&&... args );
#endif // NFX_STRINGBUILDER_HAS_STD_FORMAT

        //----------------------------------------------
        // String conversion
        //----------------------------------------------

        /**
         * @brief Convert buffer content to std::string
         * @return String copy of buffer content
         * @details Creates new string object - consider toStringView() for read-only access
         */
        [[nodiscard]] inline std::string toString() const&;

        /**
         * @brief Convert buffer to string by moving (rvalue-qualified)
         * @return std::string with moved heap buffer or copied stack buffer
         * @details For heap buffers, transfers ownership without copying.
         *          For stack buffers, performs a copy.
         *          Resets builder to empty state after move.
         */
        [[nodiscard]] std::string toString() &&;

        /**
         * @brief Get string_view of buffer content
         * @return String view referencing buffer data
         * @details Zero-copy access - view becomes invalid if buffer is modified
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline std::string_view toStringView() const noexcept;

        //----------------------------------------------
        // STL support
        //----------------------------------------------

        /**
         * @brief Appends single character to the buffer (for STL compatibility)
         * @param c Character to append
         * @details Inline alias for append(char) to support std::back_inserter
         */
        inline void push_back( char c );

        //----------------------------------------------
        // Iterator interface
        //----------------------------------------------

        /** @brief Character type for iterator compatibility */
        using value_type = char;

        /** @brief Mutable iterator type for buffer traversal */
        using Iterator = char*;

        /** @brief Immutable iterator type for buffer traversal */
        using ConstIterator = const char*;

        /** @brief Type alias for iterator */
        using iterator = Iterator;

        /** @brief Type alias for const iterator */
        using const_iterator = ConstIterator;

        /**
         * @brief Get mutable iterator to beginning of buffer
         * @return Iterator pointing to first element
         * @details Enables range-based for loops and STL algorithms
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline Iterator begin() noexcept;

        /**
         * @brief Get immutable iterator to beginning of buffer
         * @return Const iterator pointing to first element
         * @details Safe read-only iteration over buffer contents
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline ConstIterator begin() const noexcept;

        /**
         * @brief Get mutable iterator to end of buffer
         * @return Iterator pointing one past last element
         * @details Standard STL end iterator semantics
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline Iterator end() noexcept;

        /**
         * @brief Get immutable iterator to end of buffer
         * @return Const iterator pointing one past last element
         * @details Standard STL end iterator semantics
         * @note This function is marked [[nodiscard]] - the return value should not be ignored
         */
        [[nodiscard]] inline ConstIterator end() const noexcept;

    private:
        //----------------------------------------------
        // Private members
        //----------------------------------------------

        /** @brief Stack buffer size optimized for typical string operations */
        static constexpr size_t STACK_BUFFER_SIZE = 256;

        /**
         * @brief Growth strategy constants
         * @details Growth strategy based on buffer size:
         *          - Small buffers (< GROWTH_THRESHOLD): 2.0x multiplicative growth (aggressive for frequent appends)
         *          - Large buffers (≥ GROWTH_THRESHOLD): 1.5x multiplicative growth (standard exponential growth)
         *
         * Rationale:
         * - 2.0x for small: Minimize reallocation count during initial growth
         * - 1.5x for large: Balance memory overhead vs reallocation frequency
         * - Growth threshold at 8KB provides optimal balance based on benchmarks
         */
        static constexpr size_t GROWTH_THRESHOLD = 8192;
        static constexpr double AGGRESSIVE_GROWTH_FACTOR = 2.0;
        static constexpr double STANDARD_GROWTH_FACTOR = 1.5;

        /** @brief Stack-allocated buffer for small strings (aligned for SIMD) */
        alignas( 32 ) char m_stackBuffer[STACK_BUFFER_SIZE];

        /** @brief Heap-allocated buffer for large strings */
        std::unique_ptr<char[]> m_heapBuffer;

        /** @brief Active buffer pointer (points to either m_stackBuffer or m_heapBuffer) */
        char* m_buffer;

        /** @brief Current size of data in buffer */
        size_t m_size;

        /** @brief Current capacity of buffer */
        size_t m_capacity;

        /** @brief True if using heap buffer, false if using stack buffer */
        bool m_onHeap;

        //----------------------------------------------
        // Private methods
        //----------------------------------------------

        /**
         * @brief Ensures buffer has at least the specified capacity
         * @param neededCapacity Minimum required capacity
         */
        void ensureCapacity( size_t neededCapacity );
    };
} // namespace nfx::string

#include "nfx/detail/string/StringBuilder.inl"
