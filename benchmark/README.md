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

## StringBuilder Benchmarks

### Small strings

| Operation                           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_SmallStrings**       |           |             |               144 |                       |                        |              |
| **BM_StringStream_SmallStrings**    |           |             |               406 |                       |                        |              |
| **BM_StringBuilder_SmallStrings**   |           |             |               177 |                       |                        |              |
| **BM_FmtMemoryBuffer_SmallStrings** |           |             |               236 |                       |                        |              |
| **BM_Abseil_SmallStrings**          |           |             |               128 |                       |                        |              |

### Medium Strings

| Operation                            | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------ | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_MediumStrings**       |           |             |                   |                       |                        |              |
| **BM_StringStream_MediumStrings**    |           |             |                   |                       |                        |              |
| **BM_StringBuilder_MediumStrings**   |           |             |                   |                       |                        |              |
| **BM_FmtMemoryBuffer_MediumStrings** |           |             |                   |                       |                        |              |
| **BM_Abseil_MediumStrings**          |           |             |                   |                       |                        |              |

### Large Strings

| Operation                           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_LargeStrings**       |           |             |                   |                       |                        |              |
| **BM_StringStream_LargeStrings**    |           |             |                   |                       |                        |              |
| **BM_StringBuilder_LargeStrings**   |           |             |                   |                       |                        |              |
| **BM_FmtMemoryBuffer_LargeStrings** |           |             |                   |                       |                        |              |
| **BM_Abseil_LargeStrings**          |           |             |                   |                       |                        |              |

### Huge Strings

| Operation                          | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_HugeStrings**       |           |             |                   |                       |                        |              |
| **BM_StringStream_HugeStrings**    |           |             |                   |                       |                        |              |
| **BM_StringBuilder_HugeStrings**   |           |             |                   |                       |                        |              |
| **BM_FmtMemoryBuffer_HugeStrings** |           |             |                   |                       |                        |              |
| **BM_Abseil_HugeStrings**          |           |             |                   |                       |                        |              |

## Rapid Allocation Cycles

| Operation                          | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_RapidCycles**       |           |             |                   |                       |                        |              |
| **BM_FmtMemoryBuffer_RapidCycles** |           |             |                   |                       |                        |              |
| **BM_Abseil_RapidCycles**          |           |             |                   |                       |                        |              |
| **BM_StringBuilder_RapidCycles**   |           |             |                   |                       |                        |              |

## Mixed Operations

| Operation                              | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| -------------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_MixedOperations**       |           |             |                   |                       |                        |              |
| **BM_StringStream_MixedOperations**    |           |             |                   |                       |                        |              |
| **BM_FmtMemoryBuffer_MixedOperations** |           |             |                   |                       |                        |              |
| **BM_Abseil_MixedOperations**          |           |             |                   |                       |                        |              |
| **BM_StringBuilder_MixedOperations**   |           |             |                   |                       |                        |              |

## Capacity Hints

| Operation                     | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StringBuilder_NoHint**   |           |             |                   |                       |                        |              |
| **BM_StringBuilder_WithHint** |           |             |                   |                       |                        |              |

## Formatting

| Operation                             | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdFormat_ComplexFormatting**    |           |             |                   |                       |                        |              |
| **BM_FmtFormat_ComplexFormatting**    |           |             |                   |                       |                        |              |
| **BM_Abseil_ComplexFormatting**       |           |             |                   |                       |                        |              |
| **BM_StringBuilder_WithFormat**       |           |             |                   |                       |                        |              |
| **BM_StringBuilder_ManualFormatting** |           |             |                   |                       |                        |              |

## Advanced

| Operation                           | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StringBuilder_BufferReuse**    |           |             |                   |                       |                        |              |
| **BM_StringBuilder_ZeroAlloc**      |           |             |                   |                       |                        |              |
| **BM_StringBuilder_MemoryPressure** |           |             |                   |                       |                        |              |

---

_Updated on December 17, 2025_
