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

| Platform    | OS         | Benchmark Framework     | C++ Compiler              | nfx-stringbuilder version |
| ----------- | ---------- | ----------------------- | ------------------------- | :-----------------------: |
| **Linux**   | LMDE 7     | Google Benchmark v1.9.4 | GCC 14.2.0-x64            |          v0.4.0           |
| **Linux**   | LMDE 7     | Google Benchmark v1.9.4 | Clang 19.1.7-x64          |          v0.4.0           |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | MinGW GCC 14.2.0-x64      |          v0.4.0           |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | Clang-GNU-CLI 19.1.5-x64  |          v0.4.0           |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | Clang-MSVC-CLI 19.1.5-x64 |          v0.4.0           |
| **Windows** | Windows 10 | Google Benchmark v1.9.4 | MSVC 19.44.35217.0-x64    |          v0.4.0           |

---

# Performance Results

## Small strings

| Operation                           |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_SmallStrings**       | **48.6 ns** |     55.0 ns |            134 ns |                128 ns |                 111 ns |      99.3 ns |
| **BM_StringStream_SmallStrings**    |  **214 ns** |      240 ns |            374 ns |                745 ns |                 711 ns |       738 ns |
| **BM_FmtMemoryBuffer_SmallStrings** | **83.4 ns** |     91.0 ns |            163 ns |                177 ns |                 160 ns |       120 ns |
| **BM_Abseil_SmallStrings**          | **88.8 ns** |      103 ns |            190 ns |                158 ns |                 143 ns |       112 ns |
| **BM_StringBuilder_SmallStrings**   | **54.4 ns** |     60.6 ns |            107 ns |                105 ns |                  86 ns |      76.3 ns |

## Medium Strings

| Operation                            |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------ | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_MediumStrings**       | **70.8 ns** |     80.9 ns |            217 ns |                211 ns |                 197 ns |      1054 ns |
| **BM_StringStream_MediumStrings**    |  **194 ns** |      208 ns |            329 ns |                838 ns |                 837 ns |      1733 ns |
| **BM_FmtMemoryBuffer_MediumStrings** | **71.2 ns** |     82.6 ns |            139 ns |                157 ns |                 141 ns |       109 ns |
| **BM_Abseil_MediumStrings**          |  **109 ns** |      116 ns |            263 ns |                262 ns |                 268 ns |      1082 ns |
| **BM_StringBuilder_MediumStrings**   | **54.6 ns** |     57.0 ns |            141 ns |                108 ns |                 100 ns |      81.1 ns |

## Large Strings

| Operation                           |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_LargeStrings**       | **80.5 ns** |     88.8 ns |            175 ns |                165 ns |                 154 ns |       122 ns |
| **BM_StringStream_LargeStrings**    |  **220 ns** |      236 ns |            422 ns |                893 ns |                 854 ns |      1804 ns |
| **BM_FmtMemoryBuffer_LargeStrings** | **77.5 ns** |     89.7 ns |            190 ns |                188 ns |                 184 ns |       134 ns |
| **BM_Abseil_LargeStrings**          |  **113 ns** |      118 ns |            243 ns |                182 ns |                 188 ns |       143 ns |
| **BM_StringBuilder_LargeStrings**   | **88.5 ns** |     93.4 ns |            220 ns |                230 ns |                 215 ns |       165 ns |

## Huge Strings

| Operation                          |  Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------------------------- | ---------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_HugeStrings**       | **299 ns** |      305 ns |            800 ns |                584 ns |                 558 ns |       422 ns |
| **BM_StringStream_HugeStrings**    | **669 ns** |      701 ns |           1735 ns |               1974 ns |                1904 ns |      2791 ns |
| **BM_FmtMemoryBuffer_HugeStrings** |     621 ns |  **601 ns** |           1466 ns |               1435 ns |                1381 ns |       967 ns |
| **BM_Abseil_HugeStrings**          | **342 ns** |      355 ns |            772 ns |                692 ns |                 698 ns |       499 ns |
| **BM_StringBuilder_HugeStrings**   | **619 ns** |      627 ns |           1804 ns |               1699 ns |                1573 ns |      1184 ns |

## Rapid Allocation Cycles

| Operation                          |  Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------------------------- | ---------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_RapidCycles**       | **199 ns** |      213 ns |            540 ns |                549 ns |                 544 ns |       469 ns |
| **BM_FmtMemoryBuffer_RapidCycles** | **406 ns** |      443 ns |            812 ns |                850 ns |                 837 ns |       755 ns |
| **BM_Abseil_RapidCycles**          |     343 ns |  **340 ns** |            716 ns |                608 ns |                 614 ns |       509 ns |
| **BM_StringBuilder_RapidCycles**   |     335 ns |  **310 ns** |            670 ns |                661 ns |                 656 ns |       571 ns |

## Mixed Operations

| Operation                              |  Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| -------------------------------------- | ---------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_MixedOperations**       |    90.2 ns | **88.0 ns** |            211 ns |                214 ns |                 215 ns |       187 ns |
| **BM_StringStream_MixedOperations**    | **496 ns** |      496 ns |            642 ns |               2360 ns |                2616 ns |      3567 ns |
| **BM_FmtMemoryBuffer_MixedOperations** |     210 ns |  **204 ns** |            290 ns |                280 ns |                 328 ns |       274 ns |
| **BM_Abseil_MixedOperations**          |     309 ns |  **298 ns** |            518 ns |                419 ns |                 439 ns |       379 ns |
| **BM_StringBuilder_MixedOperations**   |     165 ns |  **158 ns** |            240 ns |                225 ns |                 218 ns |       191 ns |

## Capacity Hints

| Operation                     | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StringBuilder_NoHint**   |   1533 ns | **1519 ns** |           2118 ns |               2177 ns |                1950 ns |      1685 ns |
| **BM_StringBuilder_WithHint** |   1539 ns | **1336 ns** |           1965 ns |               1784 ns |                1765 ns |      1476 ns |

## Formatting

| Operation                             |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdFormat_ComplexFormatting**    |     2129 ns | **1981 ns** |           3761 ns |               2995 ns |                3369 ns |      3035 ns |
| **BM_FmtFormat_ComplexFormatting**    | **1310 ns** |     1455 ns |           2292 ns |               2554 ns |                2727 ns |      2380 ns |
| **BM_Abseil_ComplexFormatting**       |     2256 ns | **2008 ns** |           3941 ns |               2983 ns |                3449 ns |      3081 ns |
| **BM_StringBuilder_WithFormat**       |     2221 ns | **1952 ns** |           3471 ns |               1688 ns |                3209 ns |      2886 ns |
| **BM_StringBuilder_ManualFormatting** |      599 ns |  **579 ns** |           1619 ns |                698 ns |                 732 ns |       698 ns |

## Advanced

| Operation                           |   Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------------- | ----------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StringBuilder_BufferReuse**    |      525 ns |      516 ns |        **497 ns** |                607 ns |                 628 ns |       537 ns |
| **BM_StringBuilder_ZeroAlloc**      |     44.8 ns | **44.2 ns** |           49.6 ns |               45.0 ns |                  47 ns |      59.8 ns |
| **BM_StringBuilder_MemoryPressure** | **1304 ns** |     1335 ns |           2845 ns |               2561 ns |                2727 ns |      2113 ns |

---

_Updated on December 18, 2025_
