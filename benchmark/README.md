# Benchmarks

---

## Test Environment

### Hardware Configuration

| Component                | Specification                                                 |
| ------------------------ | ------------------------------------------------------------- |
| **Computer Model**       | Lenovo ThinkPad P15v Gen 3                                    |
| **CPU**                  | 12th Gen Intel Core i7-12800H (20 logical, 14 physical cores) |
| **Base Clock**           | 2.80 GHz                                                      |
| **Turbo Clock**          | 4.80 GHz                                                      |
| **L1 Data Cache**        | 48 KiB (×6 P-cores) + 32 KiB (×8 E-cores)                     |
| **L1 Instruction Cache** | 32 KiB (×6 P-cores) + 64 KiB (×2 E-core clusters)             |
| **L2 Unified Cache**     | 1.25 MiB (×6 P-cores) + 2 MiB (×2 E-core clusters)            |
| **L3 Unified Cache**     | 24 MiB (×1 shared)                                            |
| **RAM**                  | DDR4-3200 (32GB)                                              |
| **GPU**                  | NVIDIA RTX A2000 4GB GDDR6                                    |

### Software Configuration

| Platform    | OS         | Benchmark Framework     | C++ Compiler              | nfx-stringbuilder Version |
| ----------- | ---------- | ----------------------- | ------------------------- | ------------------------- |
| **Linux**   | LMDE 7     | Google Benchmark v1.9.4 | GCC 14.2.0-x64            | v0.1.0                    |
| **Linux**   | LMDE 7     | Google Benchmark v1.9.4 | Clang 19.1.7-x64          | v0.1.0                    |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | MinGW GCC 14.2.0-x64      | v0.1.0                    |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | Clang-GNU-CLI 19.1.5-x64  | v0.1.0                    |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | Clang-MSVC-CLI 19.1.5-x64 | v0.1.0                    |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | MSVC 19.44.35217.0-x64    | v0.1.0                    |

---

# Performance Results

## String Concatenation Benchmarks

### Small Strings

| Operation                         | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| --------------------------------- | --------- | ----------- | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_SmallStrings**     | 27.6 ns   | **26.6 ns** |           49.2 ns |               47.5 ns |                45.3 ns |      54.8 ns |
| **BM_StringStream_SmallStrings**  | 143 ns    | **136 ns**  |            198 ns |                346 ns |                 335 ns |       412 ns |
| **BM_StringBuilder_SmallStrings** | 42.1 ns   | **36.7 ns** |           85.4 ns |               56.1 ns |                56.3 ns |      69.3 ns |

### Medium Strings

| Operation                          | Linux GCC   | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------------------------- | ----------- | ----------- | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_MediumStrings**     | **55.3 ns** | 55.9 ns     |            139 ns |                143 ns |                 145 ns |      1019 ns |
| **BM_StringStream_MediumStrings**  | 161 ns      | **159 ns**  |            221 ns |                540 ns |                 554 ns |      1568 ns |
| **BM_StringBuilder_MediumStrings** | 64.1 ns     | **55.6 ns** |            110 ns |               86.6 ns |                90.8 ns |      99.9 ns |

### Large Strings

| Operation                         | Linux GCC   | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| --------------------------------- | ----------- | ----------- | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_LargeStrings**     | **68.1 ns** | 68.4 ns     |            118 ns |                113 ns |                 119 ns |       122 ns |
| **BM_StringStream_LargeStrings**  | 186 ns      | **176 ns**  |            301 ns |                594 ns |                 606 ns |      1657 ns |
| **BM_StringBuilder_LargeStrings** | 57.1 ns     | **48.1 ns** |            108 ns |               77.2 ns |                79.9 ns |      87.9 ns |

## Pool Efficiency Benchmarks

| Operation                           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------------- | --------- | ----------- | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_RapidCycles**        | 121 ns    | **112 ns**  |            425 ns |                410 ns |                 406 ns |       453 ns |
| **BM_StringBuilder_PoolEfficiency** | 427 ns    | **359 ns**  |            967 ns |                650 ns |                 672 ns |       758 ns |
| **BM_StringBuilder_BufferReuse**    | 731 ns    | **541 ns**  |           1004 ns |                939 ns |                 943 ns |      5213 ns |
| **BM_StringBuilder_ZeroAlloc**      | 59.8 ns   | **50.3 ns** |           90.5 ns |               54.6 ns |                60.0 ns |      68.4 ns |

## Mixed Operations Benchmarks

| Operation                            | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------ | --------- | ----------- | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_MixedOperations**     | 73.9 ns   | **72.4 ns** |            161 ns |                203 ns |                 201 ns |       222 ns |
| **BM_StringStream_MixedOperations**  | 361 ns    | **343 ns**  |            425 ns |               1233 ns |                1250 ns |      2459 ns |
| **BM_StringBuilder_MixedOperations** | 215 ns    | **173 ns**  |            276 ns |                253 ns |                 256 ns |       272 ns |
| **BM_StringBuilder_MemoryPressure**  | 1771 ns   | **1605 ns** |           2881 ns |               2698 ns |                2751 ns |      2844 ns |

## Capacity Hints Benchmarks

| Operation                     | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------- | --------- | ----------- | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StringBuilder_NoHint**   | 1999 ns   | **1693 ns** |           2599 ns |               2499 ns |                2568 ns |      2803 ns |
| **BM_StringBuilder_WithHint** | 1679 ns   | **1410 ns** |           2088 ns |               2063 ns |                2076 ns |      2311 ns |

## format() Method Benchmarks

| Operation                             | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------- | --------- | ----------- | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdFormat_ComplexFormatting**    | 1879 ns   | **1733 ns** |           2886 ns |               2397 ns |                2483 ns |      2936 ns |
| **BM_StringBuilder_FormatMethod**     | 2381 ns   | **2311 ns** |           2858 ns |               2551 ns |                2505 ns |      2874 ns |
| **BM_StringBuilder_ManualFormatting** | 1974 ns   | **1829 ns** |           3401 ns |               3430 ns |                3434 ns |      3619 ns |

## Variadic append() Benchmarks

| Operation                                        | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------------------ | --------- | ----------- | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StringBuilder_MultipleAppends**             | TBD       | TBD         |               TBD |                   TBD |                    TBD |          TBD |
| **BM_StringBuilder_VariadicAppend**              | TBD       | TBD         |               TBD |                   TBD |                    TBD |          TBD |
| **BM_StringBuilder_MultipleAppends_WithNumeric** | TBD       | TBD         |               TBD |                   TBD |                    TBD |          TBD |
| **BM_StringBuilder_VariadicAppend_WithNumeric**  | TBD       | TBD         |               TBD |                   TBD |                    TBD |          TBD |

---

_Updated on December 07, 2025_
