# nfx-stringbuilder

<!-- Project Info -->

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg?style=flat-square)](https://github.com/nfx-libs/nfx-stringbuilder/blob/main/LICENSE.txt) [![GitHub release (latest by date)](https://img.shields.io/github/v/release/nfx-libs/nfx-stringbuilder?style=flat-square)](https://github.com/nfx-libs/nfx-stringbuilder/releases) [![GitHub tag (latest by date)](https://img.shields.io/github/tag/nfx-libs/nfx-stringbuilder?style=flat-square)](https://github.com/nfx-libs/nfx-stringbuilder/tags)<br/>

![C++20](https://img.shields.io/badge/C%2B%2B-20-blue?style=flat-square) ![CMake](https://img.shields.io/badge/CMake-3.20%2B-green.svg?style=flat-square) ![Cross Platform](https://img.shields.io/badge/Platform-Linux_Windows-lightgrey?style=flat-square)

<!-- CI/CD Status -->

[![Linux GCC](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-stringbuilder/build-linux-gcc.yml?branch=main&label=Linux%20GCC&style=flat-square)](https://github.com/nfx-libs/nfx-stringbuilder/actions/workflows/build-linux-gcc.yml) [![Linux Clang](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-stringbuilder/build-linux-clang.yml?branch=main&label=Linux%20Clang&style=flat-square)](https://github.com/nfx-libs/nfx-stringbuilder/actions/workflows/build-linux-clang.yml) [![Windows MinGW](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-stringbuilder/build-windows-mingw.yml?branch=main&label=Windows%20MinGW&style=flat-square)](https://github.com/nfx-libs/nfx-stringbuilder/actions/workflows/build-windows-mingw.yml) [![Windows MSVC](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-stringbuilder/build-windows-msvc.yml?branch=main&label=Windows%20MSVC&style=flat-square)](https://github.com/nfx-libs/nfx-stringbuilder/actions/workflows/build-windows-msvc.yml) [![CodeQL](https://img.shields.io/github/actions/workflow/status/nfx-libs/nfx-stringbuilder/codeql.yml?branch=main&label=CodeQL&style=flat-square)](https://github.com/nfx-libs/nfx-stringbuilder/actions/workflows/codeql.yml)

> A cross-platform C++20 high-performance string builder with Small Buffer Optimization and efficient memory management

## Overview

**nfx-stringbuilder** is a modern C++20 library providing efficient string building capabilities with zero heap allocations for small strings (≤256 bytes). Designed for applications requiring high-performance string concatenation with minimal allocations, it features Small Buffer Optimization (SBO), comprehensive type support, and C++20 std::format integration.

## Key Features

### 🛠️ Rich String Building Interface

- **Fluent API**: Method chaining and stream operators (`<<`) for natural concatenation
- **Variadic append()**: Batch multiple arguments in a single call for optimal performance
- **Type Support**: Strings, string_view, C-strings, characters, and numeric types (int8/16/32/64, uint8/16/32/64, float, double)
- **C++20 std::format Integration**: Template `format()` method for modern formatting
- **std::formatter Specializations**: Zero-copy integration with `std::format` for StringBuilder
- **Capacity Hints**: Pre-allocate buffers with constructor parameter for optimal performance
- **Direct Buffer Access**: High-performance operations without wrappers
- **Iterator Support**: Range-based for loops and STL algorithms

### 📊 Real-World Applications

- **Web Servers & HTTP**: Building request/response strings, headers, JSON payloads
- **Logging Systems**: High-frequency log message formatting with minimal allocations
- **SQL Query Generation**: Dynamic query construction from parameters and conditions
- **Template Rendering**: Efficient HTML/XML generation from templates
- **Data Serialization**: CSV, JSON, XML output generation with streaming performance
- **Command-Line Tools**: Building formatted output, tables, and progress indicators

### ⚡ Performance Optimized

- **Small Buffer Optimization (SBO)**: 256-byte stack buffer eliminates heap allocations for most strings
- **Zero-Copy Operations**: `string_view` access without allocation
- **Batch Operations**: Variadic append() reduces function call overhead
- **Sub-Microsecond Operations**: Competitive with {fmt} and Abseil
- **Memory Efficiency**: Smart capacity management and growth strategies

### 🌍 Cross-Platform Support

- **Operating Systems**: Linux, Windows
- **Compilers**: GCC 14+, Clang 18+, MSVC 2022+
- **Thread-Safe**: Each StringBuilder instance is thread-safe when not shared
- **Consistent Behavior**: Same performance characteristics across platforms

## Quick Start

### Requirements

- C++20 compatible compiler:
  - **GCC 14+** (14.2.0 tested)
  - **Clang 18+** (19.1.7 tested)
  - **MSVC 2022+** (19.44+ tested)
- CMake 3.20 or higher

### CMake Integration

```cmake
# --- Library build types ---
option(NFX_STRINGBUILDER_BUILD_STATIC                 "Build static library"               ON )
option(NFX_STRINGBUILDER_BUILD_SHARED                 "Build shared library"               OFF)

# --- Build components ---
option(NFX_STRINGBUILDER_BUILD_TESTS                  "Build tests"                        OFF)
option(NFX_STRINGBUILDER_BUILD_SAMPLES                "Build samples"                      OFF)
option(NFX_STRINGBUILDER_BUILD_BENCHMARKS             "Build benchmarks"                   OFF)
option(NFX_STRINGBUILDER_BUILD_COMPARATIVE_BENCHMARKS "Build comparative benchmarks"       OFF)
option(NFX_STRINGBUILDER_BUILD_DOCUMENTATION          "Build Doxygen documentation"        OFF)

# --- Installation ---
option(NFX_STRINGBUILDER_INSTALL_PROJECT              "Install project"                    OFF)

# --- Packaging ---
option(NFX_STRINGBUILDER_PACKAGE_SOURCE               "Enable source package generation"   OFF)
option(NFX_STRINGBUILDER_PACKAGE_ARCHIVE              "Enable TGZ/ZIP package generation"  OFF)
option(NFX_STRINGBUILDER_PACKAGE_DEB                  "Enable DEB package generation"      OFF)
option(NFX_STRINGBUILDER_PACKAGE_RPM                  "Enable RPM package generation"      OFF)
option(NFX_STRINGBUILDER_PACKAGE_WIX                  "Enable WiX Windows installer (MSI)" OFF)
```

### Using in Your Project

#### Option 1: Using FetchContent (Recommended)

```cmake
include(FetchContent)
FetchContent_Declare(
  nfx-stringbuilder
  GIT_REPOSITORY https://github.com/nfx-libs/nfx-stringbuilder.git
  GIT_TAG        main  # or use specific version tag like "0.1.0"
)
FetchContent_MakeAvailable(nfx-stringbuilder)

# Link with static library
target_link_libraries(your_target PRIVATE nfx-stringbuilder::static)
```

#### Option 2: As a Git Submodule

```bash
# Add as submodule
git submodule add https://github.com/nfx-libs/nfx-stringbuilder.git third-party/nfx-stringbuilder
```

```cmake
# In your CMakeLists.txt
add_subdirectory(third-party/nfx-stringbuilder)
target_link_libraries(your_target PRIVATE nfx-stringbuilder::static)
```

#### Option 3: Using find_package (After Installation)

```cmake
find_package(nfx-stringbuilder REQUIRED)
target_link_libraries(your_target PRIVATE nfx-stringbuilder::static)
```

### Building

> ⚠️ **Important**: The library can use SIMD instructions (AVX2/SSE2) for optimized string copy operations. **Compiler flags are required** to enable these optimizations. Without proper flags, the library uses standard `memcpy` implementations.

**Compiler Flags for SIMD:**

- **GCC/Clang**: `-march=native` (auto-detect) or specific flags like `-msse2`, `-mavx`, `-mavx2`
- **MSVC**: `/arch:AVX` or `/arch:AVX2`

**CMake Example:**

```cmake
target_compile_options(your_target PRIVATE
    $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2>
    $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-march=native>
)
```

**Build Commands:**

```bash
# Clone the repository
git clone https://github.com/nfx-libs/nfx-stringbuilder.git
cd nfx-stringbuilder

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build the library
cmake --build . --config Release --parallel

# Run tests (optional)
ctest -C Release --output-on-failure

# Run benchmarks (optional)
./bin/benchmarks/BM_StringBuilder
```

### Documentation

nfx-stringbuilder includes API documentation generated with Doxygen.

#### 📚 Online Documentation

The complete API documentation is available online at:
**https://nfx-libs.github.io/nfx-stringbuilder**

#### Building Documentation Locally

```bash
# Configure with documentation enabled
cmake .. -DCMAKE_BUILD_TYPE=Release -DNFX_STRINGBUILDER_BUILD_DOCUMENTATION=ON

# Build the documentation
cmake --build . --target nfx-stringbuilder-documentation
```

#### Requirements

- **Doxygen** - Documentation generation tool
- **Graphviz Dot** (optional) - For generating class diagrams

#### Accessing Local Documentation

After building, open `./build/doc/html/index.html` in your web browser.

## Usage Examples

### Basic String Building

```cpp
#include <nfx/string/StringBuilder.h>

using namespace nfx::string;

int main()
{
    // Create a StringBuilder instance
    StringBuilder builder;

    // Build strings with fluent interface
    builder.append("Hello")
           .append(", ")
           .append("World")
           .append("!");

    // Convert to std::string
    std::string result = builder.toString();
    // Output: "Hello, World!"

    return 0;
}
```

### Efficient String Concatenation

```cpp
#include <nfx/string/StringBuilder.h>
#include <vector>

using namespace nfx::string;

std::string buildReport(const std::vector<std::string>& items)
{
    StringBuilder builder;

    // Reserve capacity for better performance
    builder.reserve(1024);

    // Build header
    builder.append("=== Report ===\n");

    // Add items
    for (size_t i = 0; i < items.size(); ++i)
    {
        builder.append("Item ")
               .append(std::to_string(i + 1))
               .append(": ")
               .append(items[i])
               .append("\n");
    }

    // Add footer
    builder.append("Total items: ")
           .append(std::to_string(items.size()));

    return builder.toString();
}
```

### Working with Different String Types

```cpp
#include <nfx/string/StringBuilder.h>

using namespace nfx::string;

void demonstrateStringTypes()
{
    StringBuilder builder;

    // std::string
    std::string stdStr = "from std::string";
    builder.append(stdStr)
           .append(" | ");

    // string_view (zero-copy)
    std::string_view sv = "from string_view";
    builder.append(sv)
           .append(" | ");

    // C-string
    builder.append("from C-string")
           .append(" | ");

    // Single characters
    builder.append('A')
           .append('B')
           .append('C');

    std::string result = builder.toString();
    // Output: "from std::string | from string_view | from C-string | ABC"
}
```

### Numeric Type Support

```cpp
#include <nfx/string/StringBuilder.h>

using namespace nfx::string;

void demonstrateNumericTypes()
{
    StringBuilder builder;

    // Integer types
    int32_t i32 = -42;
    uint32_t u32 = 42;
    int64_t i64 = -9223372036854775807LL;
    uint64_t u64 = 18446744073709551615ULL;

    builder.append("int32_t: ")
           .append(i32)  // Direct numeric append
           .append(", uint32_t: ")
           .append(u32)
           .append(", int64_t: ")
           .append(i64)
           .append(", uint64_t: ")
           .append(u64);

    // Floating-point types
    float f = 3.14159f;
    double d = 2.718281828459045;

    builder.append("\nfloat: ")
           .append(f)  // Direct numeric append
           .append(", double: ")
           .append(d);

    std::string result = builder.toString();
    // Output: "int32_t: -42, uint32_t: 42, int64_t: -9223372036854775807, uint64_t: 18446744073709551615
    //          float: 3.14159, double: 2.71828"
}

void demonstrateStreamOperators()
{
    StringBuilder builder;

    // Stream operators work with all numeric types
    int value = 42;
    double price = 19.99;

    builder << "Product #" << value << " costs $" << price;

    std::string result = builder.toString();
    // Output: "Product #42 costs $19.99"
}

void demonstrateVariadicAppend()
{
    StringBuilder builder;

    // Append multiple arguments at once
    int userId = 12345;
    const char* userName = "alice";
    int64_t timestamp = 1672531200;

    builder.append("User ", userId, " (", userName, ") logged in at ", timestamp);

    std::string result = builder.toString();
    // Output: "User 12345 (alice) logged in at 1672531200"
}
```

### format() Method Integration

```cpp
#include <nfx/string/StringBuilder.h>

using namespace nfx::string;

void demonstrateFormatMethod()
{
    StringBuilder builder;

    // Using StringBuilder's format() method directly
    builder.append("User: ");
    builder.format("{} (ID: {:08})", "Alice", 123);
    builder.append("\n");

    // Format with floating-point precision
    builder.append("Price: ");
    builder.format("${:.2f}", 19.99);
    builder.append("\n");

    // Format with hex and binary
    builder.append("Value: ");
    builder.format("hex=0x{:X}, bin=0b{:b}", 255, 15);

    std::string result = builder.toString();
    // Output: "User: Alice (ID: 00000123)
    //          Price: $19.99
    //          Value: hex=0xFF, bin=0b1111"
}

void demonstrateFormatChaining()
{
    StringBuilder builder;

    // format() returns reference for chaining
    builder.format("Name: {}", "Bob")
           .append(" | ")
           .format("Age: {}", 30)
           .append(" | ")
           .format("Score: {:.1f}", 95.7);

    std::string result = builder.toString();
    // Output: "Name: Bob | Age: 30 | Score: 95.7"
}
```

### C++20 std::formatter Integration

```cpp
#include <nfx/string/StringBuilder.h>
#include <format>

using namespace nfx::string;

void demonstrateStdFormatter()
{
    StringBuilder builder;

    builder.append("Hello, ")
           .append("World!");

    std::string formatted = std::format("Result: {}", builder);

    std::string bufferFormatted = std::format("Buffer content: {}", builder);
}
```

### Capacity Hints for Pre-allocation

```cpp
#include <nfx/string/StringBuilder.h>

using namespace nfx::string;

void demonstrateCapacityHints()
{
    // Pre-allocate 2048 bytes to avoid reallocations
    StringBuilder builder(2048);

    // Build large strings without reallocation
    for (int i = 0; i < 100; ++i)
    {
        builder.append("Item ")
               .append(std::to_string(i))
               .append(": Some long description text here\n");
    }

    std::string result = builder.toString();
    // No heap reallocations occurred during building
}
```

### Direct Buffer Manipulation

```cpp
#include <nfx/string/StringBuilder.h>

using namespace nfx::string;

void directBufferAccess()
{
    StringBuilder builder;

    // Append methods with chaining
    builder.append("Direct ")
           .append("buffer ")
           .append("access")
           .append('!');

    // Get size and capacity
    size_t size = builder.size();         // Current content size
    size_t capacity = builder.capacity(); // Allocated capacity

    // Zero-copy string_view access
    std::string_view view = builder.toStringView();

    // Iterator support
    for (char c : builder)
    {
        // Process each character
    }

    // Clear for reuse
    builder.clear();
}
```

### Multi-threaded Usage

```cpp
#include <nfx/string/StringBuilder.h>
#include <thread>
#include <vector>

using namespace nfx::string;

void threadWorker(int threadId)
{
    // Each thread creates its own StringBuilder instance
    for (int i = 0; i < 100; ++i)
    {
        StringBuilder builder;

        builder.append("Thread ")
               .append(std::to_string(threadId))
               .append(" - Iteration ")
               .append(std::to_string(i));

        std::string result = builder.toString();
        // Process result...
    }
}

void demonstrateMultithreading()
{
    std::vector<std::thread> threads;

    // Spawn multiple threads
    for (int i = 0; i < 4; ++i)
    {
        threads.emplace_back(threadWorker, i);
    }

    // Wait for completion
    for (auto& t : threads)
    {
        t.join();
    }

    std::cout << "All threads completed successfully\n";
}
```

**Sample Output:**

```
[INFO] User logged in: username=john.doe, ip=192.168.1.100, timestamp=2025-10-31
[ERROR] Connection failed: timeout after 30s
```

## Installation & Packaging

nfx-stringbuilder provides comprehensive packaging options for distribution.

### Package Generation

```bash
# Configure with packaging options
cmake .. -DCMAKE_BUILD_TYPE=Release \
         -DNFX_STRINGBUILDER_BUILD_STATIC=ON \
         -DNFX_STRINGBUILDER_BUILD_SHARED=ON \
         -DNFX_STRINGBUILDER_PACKAGE_ARCHIVE=ON \
         -DNFX_STRINGBUILDER_PACKAGE_DEB=ON \
         -DNFX_STRINGBUILDER_PACKAGE_RPM=ON

# Generate binary packages
cmake --build . --target package
# or
cd build && cpack

# Generate source packages
cd build && cpack --config CPackSourceConfig.cmake
```

### Supported Package Formats

| Format      | Platform       | Description                        | Requirements |
| ----------- | -------------- | ---------------------------------- | ------------ |
| **TGZ/ZIP** | Cross-platform | Compressed archive packages        | None         |
| **DEB**     | Debian/Ubuntu  | Native Debian packages             | `dpkg-dev`   |
| **RPM**     | RedHat/SUSE    | Native RPM packages                | `rpm-build`  |
| **WiX**     | Windows        | Professional MSI installer         | `WiX 3.11+`  |
| **Source**  | Cross-platform | Source code distribution (TGZ+ZIP) | None         |

### Installation

```bash
# Linux (DEB-based systems)
sudo dpkg -i nfx-stringbuilder_*_amd64.deb

# Linux (RPM-based systems)
sudo rpm -ivh nfx-stringbuilder-*-Linux.rpm

# Windows (MSI installer)
nfx-stringbuilder-0.1.0-MSVC.msi

# Manual installation (extract archive)
tar -xzf nfx-stringbuilder-*-Linux.tar.gz -C /usr/local/
```

## Project Structure

```
nfx-stringbuilder/
├── benchmark/     # Performance benchmarks with Google Benchmark
├── cmake/         # CMake modules and configuration
├── include/nfx/   # Public headers
├── samples/       # Example usage and demonstrations
├── src/           # Implementation files
└── test/          # Comprehensive unit tests with GoogleTest
```

## Performance

For detailed performance metrics and benchmarks, see:

- [Standard benchmarks](benchmark/README.md)

## Roadmap

See [TODO.md](TODO.md) for upcoming features and project roadmap.

## Changelog

See the [CHANGELOG.md](CHANGELOG.md) for a detailed history of changes, new features, and bug fixes.

## License

This project is licensed under the MIT License.

## Dependencies

### Development Dependencies

- **[GoogleTest](https://github.com/google/googletest)**: Testing framework (BSD 3-Clause License) - Development only
- **[Google Benchmark](https://github.com/google/benchmark)**: Performance benchmarking framework (Apache 2.0 License) - Development only
- **[{fmt}](https://github.com/fmtlib/fmt)**: Fast formatting library (MIT License) - Benchmarking only
- **[Abseil](https://github.com/abseil/abseil-cpp)**: C++ common libraries (Apache 2.0 License) - Benchmarking only

All dependencies are automatically fetched via CMake FetchContent when building the library, tests, or benchmarks.

---

_Updated on December 18, 2025_
