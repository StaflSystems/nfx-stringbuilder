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
| **BM_StdString_SmallStrings**     | **28.0 ns** |     33.9 ns |           53.5 ns |               57.6 ns |                50.6 ns |      63.8 ns |
| **BM_StringStream_SmallStrings**  |  **147 ns** |      167 ns |            219 ns |                429 ns |                 381 ns |       473 ns |
| **BM_StringBuilder_SmallStrings** |     41.4 ns | **40.9 ns** |           93.0 ns |               71.4 ns |                62.8 ns |      70.4 ns |

### Medium Strings

| Operation                          |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_MediumStrings**     | **62.9 ns** |     72.7 ns |           1191 ns |                180 ns |                 166 ns |      1164 ns |
| **BM_StringStream_MediumStrings**  |  **172 ns** |      197 ns |            228 ns |                704 ns |                 658 ns |      1842 ns |
| **BM_StringBuilder_MediumStrings** |     67.7 ns | **57.7 ns** |            116 ns |                105 ns |                98.0 ns |       113 ns |

### Large Strings

| Operation                         |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| --------------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_LargeStrings**     | **68.5 ns** |     75.6 ns |            127 ns |                143 ns |                 138 ns |       140 ns |
| **BM_StringStream_LargeStrings**  |  **197 ns** |      217 ns |            315 ns |                814 ns |                 729 ns |      2018 ns |
| **BM_StringBuilder_LargeStrings** |     68.8 ns | **58.9 ns** |            111 ns |               92.5 ns |                91.2 ns |       113 ns |

## Pool Efficiency

| Operation                           |  Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------------- | ---------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_RapidCycles**        | **147 ns** |      165 ns |            352 ns |                516 ns |                 500 ns |       436 ns |
| **BM_StringBuilder_PoolEfficiency** |     321 ns |  **280 ns** |            614 ns |                374 ns |                 373 ns |       380 ns |

## Mixed Operations

| Operation                            |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------ | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_MixedOperations**     | **77.5 ns** |     87.2 ns |            147 ns |                246 ns |                 246 ns |       250 ns |
| **BM_StringStream_MixedOperations**  |  **415 ns** |      439 ns |            415 ns |               1609 ns |                1580 ns |      3119 ns |
| **BM_StringBuilder_MixedOperations** |      206 ns |  **186 ns** |            304 ns |                296 ns |                 302 ns |       324 ns |

## Capacity Hints

| Operation                     | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StringBuilder_NoHint**   |   1828 ns | **1542 ns** |           2306 ns |               2612 ns |                2774 ns |      3094 ns |
| **BM_StringBuilder_WithHint** |   1645 ns | **1380 ns** |           2087 ns |               2349 ns |                2461 ns |      2723 ns |

## format() Method

| Operation                             |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdFormat_ComplexFormatting**    | **1870 ns** |     1896 ns |          11893 ns |               3075 ns |                3298 ns |      3468 ns |
| **BM_StringBuilder_FormatMethod**     | **2060 ns** |     2148 ns |           2738 ns |               2950 ns |                2868 ns |      3293 ns |
| **BM_StringBuilder_ManualFormatting** |      644 ns |  **597 ns** |           1428 ns |                839 ns |                 892 ns |      1025 ns |

## Variadic append()

| Operation                                        |  Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------------------ | ---------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StringBuilder_MultipleAppends**             |    72.5 ns | **60.2 ns** |            110 ns |               83.4 ns |                96.7 ns |       112 ns |
| **BM_StringBuilder_VariadicAppend**              |    71.7 ns | **62.9 ns** |            110 ns |               84.8 ns |                97.6 ns |       108 ns |
| **BM_StringBuilder_MultipleAppends_WithNumeric** |     111 ns |  **100 ns** |            230 ns |                121 ns |                 129 ns |       154 ns |
| **BM_StringBuilder_VariadicAppend_WithNumeric**  | **112 ns** |      111 ns |            225 ns |                139 ns |                 151 ns |       160 ns |

## Advanced

| Operation                           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StringBuilder_BufferReuse**    |    692 ns |  **586 ns** |            867 ns |                866 ns |                 981 ns |      1121 ns |
| **BM_StringBuilder_ZeroAlloc**      |   60.1 ns | **53.6 ns** |           86.0 ns |               63.9 ns |                70.0 ns |      92.4 ns |
| **BM_StringBuilder_MemoryPressure** |   1378 ns | **1238 ns** |           2074 ns |               2220 ns |                2517 ns |      2855 ns |

---

_Updated on December 07, 2025_
