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
| **Linux**   | LMDE 7     | Google Benchmark v1.9.4 | GCC 14.2.0-x64            | v0.3.0                    |
| **Linux**   | LMDE 7     | Google Benchmark v1.9.4 | Clang 19.1.7-x64          | v0.3.0                    |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | MinGW GCC 14.2.0-x64      | v0.3.0                    |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | Clang-GNU-CLI 19.1.5-x64  | v0.3.0                    |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | Clang-MSVC-CLI 19.1.5-x64 | v0.3.0                    |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | MSVC 19.44.35217.0-x64    | v0.3.0                    |

---

# Performance Results

## String Concatenation Benchmarks

### Small strings

| Operation                         |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| --------------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_SmallStrings**     | **29.8 ns** |     35.6 ns |           69.8 ns |               77.9 ns |                53.3 ns |      56.0 ns |
| **BM_StringStream_SmallStrings**  |  **159 ns** |      178 ns |            260 ns |                620 ns |                 426 ns |       433 ns |
| **BM_StringBuilder_SmallStrings** |     45.3 ns | **41.0 ns** |            113 ns |               93.0 ns |                65.4 ns |      63.3 ns |

### Medium Strings

| Operation                          |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_MediumStrings**     | **68.8 ns** |     77.3 ns |            181 ns |                232 ns |                 169 ns |      1044 ns |
| **BM_StringStream_MediumStrings**  |  **190 ns** |      213 ns |            279 ns |                994 ns |                 717 ns |      1611 ns |
| **BM_StringBuilder_MediumStrings** |     76.3 ns | **61.3 ns** |            135 ns |                136 ns |                96.2 ns |       101 ns |

### Large Strings

| Operation                         |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| --------------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_LargeStrings**     | **81.5 ns** |     87.4 ns |            142 ns |                173 ns |                 137 ns |       124 ns |
| **BM_StringStream_LargeStrings**  |  **213 ns** |      235 ns |            360 ns |               1001 ns |                 777 ns |      1683 ns |
| **BM_StringBuilder_LargeStrings** |     67.4 ns | **62.8 ns** |            127 ns |                112 ns |                89.7 ns |      95.3 ns |

## Pool Efficiency

| Operation                              |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| -------------------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_RapidCycles_ColdStart** |  **156 ns** |      174 ns |            486 ns |                628 ns |                 486 ns |       469 ns |
| **BM_StdString_RapidCycles_WarmStart** | **75.8 ns** |     67.8 ns |            112 ns |                157 ns |                 130 ns |       131 ns |
| **BM_StringBuilder_PoolEfficiency**    |      335 ns |  **290 ns** |            687 ns |                439 ns |                 356 ns |       403 ns |

## Mixed Operations

| Operation                            |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------ | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_MixedOperations**     | **87.1 ns** |     95.4 ns |            182 ns |                282 ns |                 234 ns |       219 ns |
| **BM_StringStream_MixedOperations**  |  **461 ns** |      477 ns |            469 ns |               1890 ns |                1616 ns |      2562 ns |
| **BM_StringBuilder_MixedOperations** |      224 ns |  **186 ns** |            316 ns |                295 ns |                 270 ns |       255 ns |

## Capacity Hints

| Operation                     |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StringBuilder_NoHint**   | **1941 ns** |     1542 ns |           2456 ns |               2696 ns |                2469 ns |      2437 ns |
| **BM_StringBuilder_WithHint** | **1761 ns** |     1453 ns |           2164 ns |               2296 ns |                2194 ns |      2236 ns |

## format() Method

| Operation                             |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdFormat_ComplexFormatting**    | **2116 ns** |     2084 ns |           3073 ns |               3162 ns |                2859 ns |      3093 ns |
| **BM_StringBuilder_FormatMethod**     | **2325 ns** |     2396 ns |           2857 ns |               2943 ns |                2622 ns |      2876 ns |
| **BM_StringBuilder_ManualFormatting** |      699 ns |  **625 ns** |           1476 ns |                817 ns |                 810 ns |       813 ns |

## Variadic append()

| Operation                                        |  Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------------------ | ---------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StringBuilder_MultipleAppends**             |    69.4 ns | **63.9 ns** |            113 ns |               86.4 ns |                83.7 ns |      81.6 ns |
| **BM_StringBuilder_VariadicAppend**              |    71.0 ns | **63.7 ns** |            111 ns |               84.3 ns |                84.0 ns |      83.8 ns |
| **BM_StringBuilder_MultipleAppends_WithNumeric** |     112 ns | **96.2 ns** |            231 ns |                111 ns |                 117 ns |       120 ns |
| **BM_StringBuilder_VariadicAppend_WithNumeric**  | **111 ns** |      109 ns |            227 ns |                128 ns |                 137 ns |       132 ns |

## Advanced

| Operation                           |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StringBuilder_BufferReuse**    |      724 ns |  **544 ns** |            869 ns |                802 ns |                 888 ns |       907 ns |
| **BM_StringBuilder_ZeroAlloc**      |     63.6 ns | **51.7 ns** |           86.3 ns |               56.1 ns |                59.8 ns |      68.0 ns |
| **BM_StringBuilder_MemoryPressure** | **1457 ns** |     1433 ns |           2075 ns |               1916 ns |                2222 ns |      1981 ns |

---

_Updated on December 14, 2025_
