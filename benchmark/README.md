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

| Platform    | OS         | Benchmark Framework     | C++ Compiler           | nfx-stringbuilder version |
| ----------- | ---------- | ----------------------- | ---------------------- | :-----------------------: |
| **Linux**   | LMDE 7     | Google Benchmark v1.9.5 | GCC 14.2.0-x64         |          v0.5.0           |
| **Linux**   | LMDE 7     | Google Benchmark v1.9.5 | Clang 19.1.7-x64       |          v0.5.0           |
| **Windows** | Windows 10 | Google Benchmark v1.9.5 | MinGW GCC 14.2.0-x64   |          v0.5.0           |
| **Windows** | Windows 10 | Google Benchmark v1.9.5 | MSVC 19.44.35217.0-x64 |          v0.5.0           |


---

# Performance Results

## BM_Comparative - Small Strings

| Operation                           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ----------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StdString_SmallStrings**       |   51.9 ns |     54.0 ns |       _TBA_       |    _TBA_     |
| **BM_StringStream_SmallStrings**    |    197 ns |      212 ns |       _TBA_       |    _TBA_     |
| **BM_FmtMemoryBuffer_SmallStrings** |   80.8 ns |     89.0 ns |       _TBA_       |    _TBA_     |
| **BM_Abseil_SmallStrings**          |   92.1 ns |     99.0 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_SmallStrings**   |   28.5 ns |     29.2 ns |       _TBA_       |    _TBA_     |

## BM_Comparative - Medium Strings

| Operation                            | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------------------ | --------: | ----------: | :---------------: | :----------: |
| **BM_StdString_MediumStrings**       |   68.1 ns |     78.8 ns |       _TBA_       |    _TBA_     |
| **BM_StringStream_MediumStrings**    |    177 ns |      199 ns |       _TBA_       |    _TBA_     |
| **BM_FmtMemoryBuffer_MediumStrings** |   66.9 ns |     86.6 ns |       _TBA_       |    _TBA_     |
| **BM_Abseil_MediumStrings**          |    106 ns |      121 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_MediumStrings**   |   23.9 ns |     25.9 ns |       _TBA_       |    _TBA_     |

## BM_Comparative - Large Strings

| Operation                           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ----------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StdString_LargeStrings**       |   81.2 ns |     94.9 ns |       _TBA_       |    _TBA_     |
| **BM_StringStream_LargeStrings**    |    197 ns |      227 ns |       _TBA_       |    _TBA_     |
| **BM_FmtMemoryBuffer_LargeStrings** |   78.7 ns |     88.6 ns |       _TBA_       |    _TBA_     |
| **BM_Abseil_LargeStrings**          |    123 ns |      117 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_LargeStrings**   |   78.6 ns |     84.2 ns |       _TBA_       |    _TBA_     |

## BM_Comparative - Huge Strings

| Operation                          | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ---------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StdString_HugeStrings**       |    324 ns |      331 ns |       _TBA_       |    _TBA_     |
| **BM_StringStream_HugeStrings**    |    831 ns |      850 ns |       _TBA_       |    _TBA_     |
| **BM_FmtMemoryBuffer_HugeStrings** |    673 ns |      674 ns |       _TBA_       |    _TBA_     |
| **BM_Abseil_HugeStrings**          |    355 ns |      385 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_HugeStrings**   |    665 ns |      717 ns |       _TBA_       |    _TBA_     |

## BM_Comparative - Rapid Allocation Cycles

| Operation                          | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ---------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StdString_RapidCycles**       |   9.18 ns |     7.18 ns |       _TBA_       |    _TBA_     |
| **BM_FmtMemoryBuffer_RapidCycles** |    185 ns |      204 ns |       _TBA_       |    _TBA_     |
| **BM_Abseil_RapidCycles**          |    156 ns |      146 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_RapidCycles**   |   63.1 ns |     67.8 ns |       _TBA_       |    _TBA_     |

## BM_Comparative - Mixed Operations

| Operation                              | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| -------------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StdString_MixedOperations**       |    209 ns |      213 ns |       _TBA_       |    _TBA_     |
| **BM_StringStream_MixedOperations**    |    386 ns |      401 ns |       _TBA_       |    _TBA_     |
| **BM_FmtMemoryBuffer_MixedOperations** |   91.6 ns |      103 ns |       _TBA_       |    _TBA_     |
| **BM_Abseil_MixedOperations**          |   65.6 ns |     65.4 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_MixedOperations**   |   74.9 ns |     63.8 ns |       _TBA_       |    _TBA_     |

## BM_Comparative - Formatting

| Operation                             | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StdFormat_ComplexFormatting**    |    179 ns |      154 ns |       _TBA_       |    _TBA_     |
| **BM_FmtFormat_ComplexFormatting**    |    113 ns |      117 ns |       _TBA_       |    _TBA_     |
| **BM_Abseil_ComplexFormatting**       |   59.1 ns |     65.6 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_WithFormat**       |    192 ns |      190 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_ManualFormatting** |   66.0 ns |     67.7 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Core - Append Operations

| Operation                             | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_AppendChar**       |   63.1 ns |     65.9 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_AppendCString**    |   63.5 ns |     83.9 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_AppendStringView** |    105 ns |      114 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_AppendString**     |    161 ns |      189 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_AppendInt**        |    289 ns |      131 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_AppendDouble**     |   3653 ns |     3608 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_AppendBool**       |    107 ns |     96.3 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Core - Stream Operators

| Operation                           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ----------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_StreamOperator** |   62.2 ns |     62.4 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_ChainedAppends** |   62.1 ns |     69.6 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Core - Other Operations

| Operation                               | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| --------------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_Prepend**            |    135 ns |      143 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_MixedAppendPrepend** |   85.7 ns |     90.5 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_ClearAndReuse**      |   20.2 ns |     21.2 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_ToStringView**       |   1.73 ns |     1.83 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_ToString**           |   14.7 ns |     14.9 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Join - Small Collections

| Operation                            | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------------------ | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_JoinSmall_Comma** |   29.0 ns |     35.2 ns |       _TBA_       |    _TBA_     |
| **BM_ManualLoop_JoinSmall_Comma**    |   51.3 ns |     50.1 ns |       _TBA_       |    _TBA_     |
| **BM_Abseil_JoinSmall_Comma**        |   38.8 ns |     40.7 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Join - Medium Collections

| Operation                            | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------------------ | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_JoinMedium_Pipe** |   24.1 ns |     29.2 ns |       _TBA_       |    _TBA_     |
| **BM_ManualLoop_JoinMedium_Pipe**    |   71.5 ns |     70.8 ns |       _TBA_       |    _TBA_     |
| **BM_Abseil_JoinMedium_Pipe**        |   30.4 ns |     33.2 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Join - Large Collections

| Operation                              | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| -------------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_JoinLarge_Newline** |   66.2 ns |     71.4 ns |       _TBA_       |    _TBA_     |
| **BM_ManualLoop_JoinLarge_Newline**    |   73.4 ns |     69.4 ns |       _TBA_       |    _TBA_     |
| **BM_Abseil_JoinLarge_Newline**        |   32.4 ns |     34.3 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Join - Numeric Arrays

| Operation                        | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| -------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_JoinNumbers** |    353 ns |      382 ns |       _TBA_       |    _TBA_     |
| **BM_ManualLoop_JoinNumbers**    |    767 ns |      719 ns |       _TBA_       |    _TBA_     |
| **BM_Abseil_JoinNumbers**        |   1115 ns |     1327 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Join - Edge Cases

| Operation                       | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_JoinEmpty**  |   1.25 ns |     2.45 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_JoinSingle** |   5.72 ns |     6.78 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Lines - Small Strings

| Operation                               | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| --------------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_AppendLine_Small**   |   29.5 ns |     26.1 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_AppendManual_Small** |   27.6 ns |     27.4 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Lines - Medium Strings

| Operation                                | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ---------------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_AppendLine_Medium**   |   23.6 ns |     24.3 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_AppendManual_Medium** |   24.0 ns |     23.8 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Lines - Large Strings

| Operation                               | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| --------------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_AppendLine_Large**   |   72.1 ns |     70.7 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_AppendManual_Large** |   67.4 ns |     68.2 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Lines - Workloads

| Operation                         | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| --------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_LogWorkload**  |  0.817 us |    0.566 us |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_CsvWorkload**  |   2327 ns |     2031 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_NmeaWorkload** |    273 ns |      224 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Lines - Edge Cases

| Operation                             | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_AppendLine_Empty** |   75.4 ns |     58.9 ns |       _TBA_       |    _TBA_     |

## BM_StringBuilder_Advanced

| Operation                           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ----------------------------------- | --------: | ----------: | :---------------: | :----------: |
| **BM_StringBuilder_NoHint**         |  0.520 us |    0.446 us |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_WithHint**       |  0.387 us |    0.299 us |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_BufferReuse**    |   82.6 ns |     84.0 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_ZeroAlloc**      |   17.3 ns |     15.8 ns |       _TBA_       |    _TBA_     |
| **BM_StringBuilder_MemoryPressure** |   1.02 us |     1.15 us |       _TBA_       |    _TBA_     |

---

_Updated on February 11, 2026_
