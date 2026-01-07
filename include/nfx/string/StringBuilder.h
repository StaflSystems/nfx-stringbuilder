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
#include <format>
#include <memory>
#include <string>
#include <string_view>

/**
 * @def NFX_STRINGBUILDER_FORCE_INLINE
 * @brief Cross-platform forced inline macro for critical hot-path methods
 */
#if defined( _MSC_VER )
#	define NFX_STRINGBUILDER_FORCE_INLINE __forceinline
#elif defined( __GNUC__ ) || defined( __clang__ )
#	define NFX_STRINGBUILDER_FORCE_INLINE __attribute__( ( always_inline ) ) inline
#else
#	define NFX_STRINGBUILDER_FORCE_INLINE inline
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

		/**
		 * @brief Format and append text using std::format
		 * @tparam Args Types of the formatting arguments
		 * @param fmt Format string with format specifiers
		 * @param args Arguments to be formatted
		 * @return Reference to this StringBuilder for chaining
		 * @details Provides type-safe formatting directly into the builder without intermediate allocations.
		 *          Uses std::format_to with back_inserter for optimal performance.
		 *          Example: builder.format("User {} (ID: {}) logged in at {}", name, userId, timestamp);
		 */
		template <typename... Args>
		inline StringBuilder& format( std::format_string<Args...> fmt, Args&&... args );

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
