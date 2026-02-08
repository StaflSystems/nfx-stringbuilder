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
| ----------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StdString_SmallStrings**       |           |             |                   |              |
| **BM_StringStream_SmallStrings**    |           |             |                   |              |
| **BM_FmtMemoryBuffer_SmallStrings** |           |             |                   |              |
| **BM_Abseil_SmallStrings**          |           |             |                   |              |
| **BM_StringBuilder_SmallStrings**   |           |             |                   |              |

## BM_Comparative - Medium Strings

| Operation                            | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------------------ | --------: | ----------: | ----------------: | -----------: |
| **BM_StdString_MediumStrings**       |           |             |                   |              |
| **BM_StringStream_MediumStrings**    |           |             |                   |              |
| **BM_FmtMemoryBuffer_MediumStrings** |           |             |                   |              |
| **BM_Abseil_MediumStrings**          |           |             |                   |              |
| **BM_StringBuilder_MediumStrings**   |           |             |                   |              |

## BM_Comparative - Large Strings

| Operation                           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ----------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StdString_LargeStrings**       |           |             |                   |              |
| **BM_StringStream_LargeStrings**    |           |             |                   |              |
| **BM_FmtMemoryBuffer_LargeStrings** |           |             |                   |              |
| **BM_Abseil_LargeStrings**          |           |             |                   |              |
| **BM_StringBuilder_LargeStrings**   |           |             |                   |              |

## BM_Comparative - Huge Strings

| Operation                          | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ---------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StdString_HugeStrings**       |           |             |                   |              |
| **BM_StringStream_HugeStrings**    |           |             |                   |              |
| **BM_FmtMemoryBuffer_HugeStrings** |           |             |                   |              |
| **BM_Abseil_HugeStrings**          |           |             |                   |              |
| **BM_StringBuilder_HugeStrings**   |           |             |                   |              |

## BM_Comparative - Rapid Allocation Cycles

| Operation                          | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ---------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StdString_RapidCycles**       |           |             |                   |              |
| **BM_FmtMemoryBuffer_RapidCycles** |           |             |                   |              |
| **BM_Abseil_RapidCycles**          |           |             |                   |              |
| **BM_StringBuilder_RapidCycles**   |           |             |                   |              |

## BM_Comparative - Mixed Operations

| Operation                              | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| -------------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StdString_MixedOperations**       |           |             |                   |              |
| **BM_StringStream_MixedOperations**    |           |             |                   |              |
| **BM_FmtMemoryBuffer_MixedOperations** |           |             |                   |              |
| **BM_Abseil_MixedOperations**          |           |             |                   |              |
| **BM_StringBuilder_MixedOperations**   |           |             |                   |              |

## BM_Comparative - Formatting

| Operation                             | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StdFormat_ComplexFormatting**    |           |             |                   |              |
| **BM_FmtFormat_ComplexFormatting**    |           |             |                   |              |
| **BM_Abseil_ComplexFormatting**       |           |             |                   |              |
| **BM_StringBuilder_WithFormat**       |           |             |                   |              |
| **BM_StringBuilder_ManualFormatting** |           |             |                   |              |

## BM_StringBuilder_Core - Append Operations

| Operation                       | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StringBuilder_AppendChar** |           |             |                   |              |
| **BM_StringBuilder_AppendCStr** |           |             |                   |              |
| **BM_StringBuilder_AppendInt**  |           |             |                   |              |
| **BM_StringBuilder_AppendBool** |           |             |                   |              |

## BM_StringBuilder_Core - Stream Operators

| Operation                               | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| --------------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StringBuilder_StreamOperator**     |           |             |                   |              |
| **BM_StringBuilder_ChainedAppends**     |           |             |                   |              |
| **BM_StringBuilder_StreamOperatorInt**  |           |             |                   |              |
| **BM_StringBuilder_StreamOperatorBool** |           |             |                   |              |

## BM_StringBuilder_Core - Other Operations

| Operation                           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ----------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StringBuilder_Prepend**        |           |             |                   |              |
| **BM_StringBuilder_ClearAndReuse**  |           |             |                   |              |
| **BM_StringBuilder_ToString**       |           |             |                   |              |
| **BM_StringBuilder_ToStringView**   |           |             |                   |              |
| **BM_StringBuilder_CopyConstruct**  |           |             |                   |              |
| **BM_StringBuilder_CopyAssignment** |           |             |                   |              |
| **BM_StringBuilder_MoveConstruct**  |           |             |                   |              |
| **BM_StringBuilder_MoveAssignment** |           |             |                   |              |

## BM_StringBuilder_Join - Small Collections

| Operation                             | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StringBuilder_JoinSmall**        |           |             |                   |              |
| **BM_StdString_JoinSmall**            |           |             |                   |              |
| **BM_Abseil_JoinSmall**               |           |             |                   |              |
| **BM_StringBuilder_JoinNumericSmall** |           |             |                   |              |

## BM_StringBuilder_Join - Medium Collections

| Operation                              | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| -------------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StringBuilder_JoinMedium**        |           |             |                   |              |
| **BM_StdString_JoinMedium**            |           |             |                   |              |
| **BM_Abseil_JoinMedium**               |           |             |                   |              |
| **BM_StringBuilder_JoinNumericMedium** |           |             |                   |              |

## BM_StringBuilder_Join - Large Collections

| Operation                             | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StringBuilder_JoinLarge**        |           |             |                   |              |
| **BM_StdString_JoinLarge**            |           |             |                   |              |
| **BM_Abseil_JoinLarge**               |           |             |                   |              |
| **BM_StringBuilder_JoinNumericLarge** |           |             |                   |              |

## BM_StringBuilder_Join - Edge Cases

| Operation                            | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ------------------------------------ | --------: | ----------: | ----------------: | -----------: |
| **BM_StringBuilder_JoinEmpty**       |           |             |                   |              |
| **BM_StringBuilder_JoinSingleElem**  |           |             |                   |              |
| **BM_StringBuilder_JoinNoDelimiter** |           |             |                   |              |

## BM_StringBuilder_Lines - Small Strings

| Operation                               | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| --------------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StringBuilder_AppendLineSmall**    |           |             |                   |              |
| **BM_StringBuilder_AppendLnSmall**      |           |             |                   |              |
| **BM_StringBuilder_ManualNewlineSmall** |           |             |                   |              |

## BM_StringBuilder_Lines - Medium Strings

| Operation                                | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ---------------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StringBuilder_AppendLineMedium**    |           |             |                   |              |
| **BM_StringBuilder_AppendLnMedium**      |           |             |                   |              |
| **BM_StringBuilder_ManualNewlineMedium** |           |             |                   |              |

## BM_StringBuilder_Lines - Large Strings

| Operation                               | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| --------------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StringBuilder_AppendLineLarge**    |           |             |                   |              |
| **BM_StringBuilder_AppendLnLarge**      |           |             |                   |              |
| **BM_StringBuilder_ManualNewlineLarge** |           |             |                   |              |

## BM_StringBuilder_Lines - Workloads

| Operation                         | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| --------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StringBuilder_LogWorkload**  |           |             |                   |              |
| **BM_StringBuilder_CSVWorkload**  |           |             |                   |              |
| **BM_StringBuilder_NMEAWorkload** |           |             |                   |              |

## BM_StringBuilder_Advanced

| Operation                           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows MSVC |
| ----------------------------------- | --------: | ----------: | ----------------: | -----------: |
| **BM_StringBuilder_NoHint**         |           |             |                   |              |
| **BM_StringBuilder_WithHint**       |           |             |                   |              |
| **BM_StringBuilder_BufferReuse**    |           |             |                   |              |
| **BM_StringBuilder_ZeroAlloc**      |           |             |                   |              |
| **BM_StringBuilder_MemoryPressure** |           |             |                   |              |

---

_Updated on February 8, 2026_
