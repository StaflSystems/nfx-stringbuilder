# Changelog

## [Unreleased]

### Added

- NIL

### Changed

- NIL

### Deprecated

- NIL

### Removed

- NIL

### Fixed

- NIL

### Security

- NIL

## [0.4.0] - 2025-12-18

### Changed

- **BREAKING**: Simplified architecture - removed pooling system in favor of direct `StringBuilder` instantiation

### Removed

- **BREAKING**: `StringBuilderPool` static factory class and three-tier pooling system (thread-local, shared, dynamic)
- **BREAKING**: `StringBuilderLease` RAII wrapper for pooled buffer management
- `DynamicStringBufferPool` internal class

## [0.3.0] - 2025-12-07

### Added

- Method chaining support for all `StringBuilder::append()` methods (now return `StringBuilder&`)
- Numeric `append()` methods for `int8_t`, `uint8_t`, `int16_t`, `uint16_t`, `int32_t`, `uint32_t`, `int64_t`, `uint64_t`, `float`, and `double`
- Variadic `append()` template overload for batch operations - append multiple arguments in a single call (supports strings, chars, and numeric types)

### Changed

- Stream operators (`operator<<`) for all numeric types optimized with `std::to_chars` implementation
- Optimized capacity checks in `DynamicStringBuffer` append methods
- SIMD optimizations (AVX2/SSE2) for string copy operations in `DynamicStringBuffer::append()`
- Growth factor increased from 1.5 to 2.0 for better reallocation performance

## [0.2.0] - 2025-12-03

### Added

- `StringBuilder::reserve()` method for consistent API with `DynamicStringBuffer`

### Changed

- Reorganized CMake build configuration for better modularity
- Decentralized dependency management to test/benchmark modules
- Improved package portability with PATH_VARS configuration

## [0.1.1] - 2025-11-28

### Changed

- Consolidated packaging tool detection in CMake configuration

### Fixed

- Removed incorrect runtime dependencies from DEB/RPM packages

## [0.1.0] - 2025-11-23 - Initial Release

### Added

- **Core String Building**

  - `DynamicStringBuffer` with Small Buffer Optimization (256-byte stack buffer)
  - `StringBuilder` with fluent interface and stream operators (`<<`)
  - `StringBuilderLease` RAII wrapper for automatic pool cleanup
  - `StringBuilderPool` static factory with three-tier pooling strategy
  - `std::formatter` specializations for `DynamicStringBuffer` and `StringBuilder` enabling direct usage in `std::format()`
  - Numeric type support via `operator<<` overloads for `int32_t`, `uint32_t`, `int64_t`, `uint64_t`, `float`, `double`
  - Type-safe formatting via `format(fmt, args...)` method using `std::format_to` with `std::back_inserter`
  - Private `push_back(char)` method as inline alias to `append(char)` for `std::back_inserter` compatibility
  - Capacity hints via `StringBuilderPool::lease(size_t capacityHint)` for pre-sized buffer allocation

- **Performance Features**

  - Thread-local caching for zero-lock hotpath performance
  - Shared cross-thread buffer pool with mutex protection
  - Automatic capacity management with 1.5x growth factor
  - Zero-copy operations using `string_view`
  - High cache hit rates (90%+ in typical workloads)

- **Developer Experience**

  - Iterator support for STL compatibility and range-based for loops
  - Pool statistics tracking (hit rates, allocations, requests)
  - Direct buffer access for low-level operations
  - Comprehensive error handling with move-only semantics

- **Documentation**

  - README with feature overview
  - Detailed API documentation with Doxygen comments
  - Sample application demonstrating library usage
  - Build and installation instructions

- **Testing & Benchmarking**
  - Unit test suite
  - Performance benchmarks for all operations
  - Cross-compiler performance validation
