# Comparative Benchmarks

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

| Operation                                       | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_SmallStrings_ColdStart**         |   41.5 ns |     43.4 ns |           82.3 ns |                102 ns |                81.8 ns |       255 ns |
| **BM_StdString_SmallStrings_WarmStart**         |   16.3 ns |     17.0 ns |           18.3 ns |               20.0 ns |                16.3 ns |      44.5 ns |
| **BM_StringStream_SmallStrings_ColdStart**      |    195 ns |      185 ns |            255 ns |                661 ns |                 500 ns |      1939 ns |
| **BM_StringStream_SmallStrings_WarmStart**      |    118 ns |      113 ns |            133 ns |                568 ns |                 407 ns |      1484 ns |
| **BM_FmtMemoryBuffer_SmallStrings_ColdStart**   |   75.4 ns |     80.7 ns |            105 ns |                160 ns |                 113 ns |       306 ns |
| **BM_FmtMemoryBuffer_SmallStrings_WarmStart**   |   75.3 ns |     80.8 ns |            103 ns |                159 ns |                 115 ns |       294 ns |
| **BM_Abseil_SmallStrings_ColdStart**            |   83.2 ns |     79.3 ns |            123 ns |                146 ns |                 106 ns |       268 ns |
| **BM_Abseil_SmallStrings_WarmStart**            |   50.1 ns |     46.8 ns |           55.9 ns |               61.6 ns |                38.0 ns |       128 ns |
| **BM_nfx_StringBuilder_SmallStrings_ColdStart** |   76.1 ns |     59.0 ns |            120 ns |                130 ns |                95.4 ns |       274 ns |
| **BM_nfx_StringBuilder_SmallStrings_WarmStart** |   64.1 ns |     52.9 ns |           85.6 ns |                115 ns |                76.7 ns |       193 ns |

### Small strings with reserve

| Operation                                               | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_SmallStrings_Reserve_ColdStart**         |   30.8 ns |     29.8 ns |           49.2 ns |               69.2 ns |                47.7 ns |       103 ns |
| **BM_StdString_SmallStrings_Reserve_WarmStart**         |   15.5 ns |     17.7 ns |           18.5 ns |               20.9 ns |                19.1 ns |      35.0 ns |
| **BM_FmtMemoryBuffer_SmallStrings_Reserve_ColdStart**   |   77.2 ns |     82.2 ns |            105 ns |                177 ns |                 119 ns |       266 ns |
| **BM_FmtMemoryBuffer_SmallStrings_Reserve_WarmStart**   |   75.8 ns |     80.2 ns |            104 ns |                181 ns |                 114 ns |       261 ns |
| **BM_Abseil_SmallStrings_Reserve_ColdStart**            |   60.9 ns |     57.6 ns |           87.5 ns |                111 ns |                71.9 ns |       164 ns |
| **BM_Abseil_SmallStrings_Reserve_WarmStart**            |   47.7 ns |     48.5 ns |           58.3 ns |               67.5 ns |                45.4 ns |       114 ns |
| **BM_nfx_StringBuilder_SmallStrings_Reserve_ColdStart** |   77.2 ns |     65.7 ns |            123 ns |                148 ns |                93.3 ns |       234 ns |
| **BM_nfx_StringBuilder_SmallStrings_Reserve_WarmStart** |   63.3 ns |     53.6 ns |           86.0 ns |                118 ns |                77.7 ns |       258 ns |

### Medium strings

| Operation                                        | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------------------ | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_MediumStrings_ColdStart**         |   62.7 ns |     65.4 ns |            149 ns |                230 ns |                 149 ns |      1561 ns |
| **BM_StdString_MediumStrings_WarmStart**         |   11.3 ns |     12.4 ns |           17.0 ns |               34.7 ns |                20.8 ns |      36.8 ns |
| **BM_StringStream_MediumStrings_ColdStart**      |    176 ns |      178 ns |            239 ns |                980 ns |                 647 ns |      2666 ns |
| **BM_StringStream_MediumStrings_WarmStart**      |    101 ns |      100 ns |            113 ns |                823 ns |                 521 ns |      2383 ns |
| **BM_FmtMemoryBuffer_MediumStrings_ColdStart**   |   63.0 ns |     81.2 ns |           99.1 ns |                170 ns |                 109 ns |       184 ns |
| **BM_FmtMemoryBuffer_MediumStrings_WarmStart**   |   62.1 ns |     81.7 ns |           95.5 ns |                174 ns |                 115 ns |       183 ns |
| **BM_Abseil_MediumStrings_ColdStart**            |   98.5 ns |      102 ns |            188 ns |                287 ns |                 207 ns |      1441 ns |
| **BM_Abseil_MediumStrings_WarmStart**            |   38.7 ns |     37.8 ns |           46.1 ns |               74.7 ns |                49.1 ns |      81.9 ns |
| **BM_nfx_StringBuilder_MediumStrings_ColdStart** |   70.2 ns |     51.0 ns |            114 ns |                138 ns |                88.1 ns |       144 ns |
| **BM_nfx_StringBuilder_MediumStrings_WarmStart** |   55.7 ns |     44.1 ns |           80.6 ns |                126 ns |                91.2 ns |       125 ns |

### Medium strings with reserve

| Operation                                                | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| -------------------------------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_MediumStrings_Reserve_ColdStart**         |   24.0 ns |     22.5 ns |           49.3 ns |               81.3 ns |                58.6 ns |      68.1 ns |
| **BM_StdString_MediumStrings_Reserve_WarmStart**         |   11.8 ns |     12.9 ns |           17.7 ns |               33.9 ns |                22.8 ns |      28.9 ns |
| **BM_FmtMemoryBuffer_MediumStrings_Reserve_ColdStart**   |   65.3 ns |     80.9 ns |            102 ns |                170 ns |                 111 ns |       146 ns |
| **BM_FmtMemoryBuffer_MediumStrings_Reserve_WarmStart**   |   64.8 ns |     78.8 ns |           95.3 ns |                170 ns |                 113 ns |       142 ns |
| **BM_Abseil_MediumStrings_Reserve_ColdStart**            |   51.2 ns |     46.8 ns |           78.8 ns |                134 ns |                91.3 ns |       115 ns |
| **BM_Abseil_MediumStrings_Reserve_WarmStart**            |   38.6 ns |     35.7 ns |           46.1 ns |               75.8 ns |                48.3 ns |      65.4 ns |
| **BM_nfx_StringBuilder_MediumStrings_Reserve_ColdStart** |   69.6 ns |     52.2 ns |            117 ns |                136 ns |                90.2 ns |       114 ns |
| **BM_nfx_StringBuilder_MediumStrings_Reserve_WarmStart** |   55.5 ns |     43.7 ns |           80.5 ns |                124 ns |                84.2 ns |       100 ns |

### Large strings

| Operation                                       | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ----------------------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_LargeStrings_ColdStart**         |   75.3 ns |     75.1 ns |            126 ns |                175 ns |                 123 ns |       136 ns |
| **BM_StdString_LargeStrings_WarmStart**         |   12.9 ns |     13.5 ns |           17.3 ns |               26.1 ns |                18.6 ns |      17.2 ns |
| **BM_StringStream_LargeStrings_ColdStart**      |    203 ns |      200 ns |            316 ns |               1003 ns |                 694 ns |      1928 ns |
| **BM_StringStream_LargeStrings_WarmStart**      |   86.9 ns |     86.7 ns |            129 ns |                823 ns |                 577 ns |      1777 ns |
| **BM_FmtMemoryBuffer_LargeStrings_ColdStart**   |   71.3 ns |     83.9 ns |            138 ns |                200 ns |                 139 ns |       140 ns |
| **BM_FmtMemoryBuffer_LargeStrings_WarmStart**   |   59.5 ns |     72.5 ns |           94.7 ns |                139 ns |                97.3 ns |      96.1 ns |
| **BM_Abseil_LargeStrings_ColdStart**            |    105 ns |      104 ns |            175 ns |                210 ns |                 152 ns |       142 ns |
| **BM_Abseil_LargeStrings_WarmStart**            |   34.3 ns |     35.0 ns |           55.2 ns |               55.0 ns |                39.8 ns |      40.7 ns |
| **BM_nfx_StringBuilder_LargeStrings_ColdStart** |   63.3 ns |     53.2 ns |            112 ns |                116 ns |                84.2 ns |      99.1 ns |
| **BM_nfx_StringBuilder_LargeStrings_WarmStart** |   56.1 ns |     46.8 ns |           77.9 ns |                101 ns |                74.0 ns |      94.6 ns |

### Large strings with reserve

| Operation                                               | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_LargeStrings_Reserve_ColdStart**         |   24.3 ns |     23.8 ns |           50.3 ns |               66.6 ns |                53.1 ns |      48.0 ns |
| **BM_StdString_LargeStrings_Reserve_WarmStart**         |   12.3 ns |     14.0 ns |           16.9 ns |               20.9 ns |                17.6 ns |      21.4 ns |
| **BM_FmtMemoryBuffer_LargeStrings_Reserve_ColdStart**   |   67.4 ns |     79.9 ns |            126 ns |                175 ns |                 133 ns |       132 ns |
| **BM_FmtMemoryBuffer_LargeStrings_Reserve_WarmStart**   |   59.4 ns |     75.3 ns |           93.8 ns |                135 ns |                 105 ns |       105 ns |
| **BM_Abseil_LargeStrings_Reserve_ColdStart**            |   46.7 ns |     45.8 ns |           87.1 ns |               98.2 ns |                79.9 ns |      73.8 ns |
| **BM_Abseil_LargeStrings_Reserve_WarmStart**            |   34.5 ns |     34.5 ns |           54.3 ns |               49.8 ns |                39.9 ns |      41.2 ns |
| **BM_nfx_StringBuilder_LargeStrings_Reserve_ColdStart** |   66.0 ns |     56.1 ns |            115 ns |                105 ns |                82.5 ns |       102 ns |
| **BM_nfx_StringBuilder_LargeStrings_Reserve_WarmStart** |   56.2 ns |     47.5 ns |           78.8 ns |               92.2 ns |                74.9 ns |      93.1 ns |

### Huge strings

| Operation                                      | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ---------------------------------------------- | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_HugeStrings_ColdStart**         |    279 ns |      280 ns |            532 ns |                575 ns |                 403 ns |       418 ns |
| **BM_StdString_HugeStrings_WarmStart**         |    101 ns |      102 ns |            117 ns |                126 ns |                 103 ns |      89.1 ns |
| **BM_StringStream_HugeStrings_ColdStart**      |    690 ns |      688 ns |           1128 ns |               1879 ns |                1475 ns |      2576 ns |
| **BM_StringStream_HugeStrings_WarmStart**      |    236 ns |      236 ns |            349 ns |               1747 ns |                1346 ns |      2475 ns |
| **BM_FmtMemoryBuffer_HugeStrings_ColdStart**   |    565 ns |      540 ns |           1025 ns |               1369 ns |                1057 ns |       973 ns |
| **BM_FmtMemoryBuffer_HugeStrings_WarmStart**   |    281 ns |      273 ns |            414 ns |                509 ns |                 410 ns |       343 ns |
| **BM_Abseil_HugeStrings_ColdStart**            |    314 ns |      312 ns |            539 ns |                639 ns |                 487 ns |       482 ns |
| **BM_Abseil_HugeStrings_WarmStart**            |    170 ns |      177 ns |            198 ns |                200 ns |                 149 ns |       148 ns |
| **BM_nfx_StringBuilder_HugeStrings_ColdStart** |    760 ns |      755 ns |           1543 ns |               1851 ns |                1430 ns |      1476 ns |
| **BM_nfx_StringBuilder_HugeStrings_WarmStart** |    297 ns |      280 ns |            397 ns |                473 ns |                 380 ns |       418 ns |

### Huge strings with reserve

| Operation                                              | Linux GCC | Linux Clang | Windows MinGW GCC | Windows Clang-GNU-CLI | Windows Clang-MSVC-CLI | Windows MSVC |
| ------------------------------------------------------ | --------: | ----------: | ----------------: | --------------------: | ---------------------: | -----------: |
| **BM_StdString_HugeStrings_Reserve_ColdStart**         |    112 ns |      112 ns |            217 ns |                246 ns |                 203 ns |       194 ns |
| **BM_StdString_HugeStrings_Reserve_WarmStart**         |   94.4 ns |     95.6 ns |            119 ns |                129 ns |                99.9 ns |      94.3 ns |
| **BM_FmtMemoryBuffer_HugeStrings_Reserve_ColdStart**   |    312 ns |      303 ns |            490 ns |                612 ns |                 504 ns |       426 ns |
| **BM_FmtMemoryBuffer_HugeStrings_Reserve_WarmStart**   |    298 ns |      290 ns |            412 ns |                494 ns |                 427 ns |       340 ns |
| **BM_Abseil_HugeStrings_Reserve_ColdStart**            |    177 ns |      176 ns |            306 ns |                321 ns |                 281 ns |       258 ns |
| **BM_Abseil_HugeStrings_Reserve_WarmStart**            |    161 ns |      161 ns |            198 ns |                183 ns |                 164 ns |       150 ns |
| **BM_nfx_StringBuilder_HugeStrings_Reserve_ColdStart** |    375 ns |      351 ns |            666 ns |                714 ns |                 638 ns |       631 ns |
| **BM_nfx_StringBuilder_HugeStrings_Reserve_WarmStart** |    287 ns |      270 ns |            392 ns |                443 ns |                 393 ns |       414 ns |

---

_Updated on December 14, 2025_
