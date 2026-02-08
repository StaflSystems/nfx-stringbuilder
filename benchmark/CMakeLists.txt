#==============================================================================
# nfx-stringbuilder - Benchmark suite CMake configuration
#==============================================================================

#----------------------------------------------
# Benchmark condition check
#----------------------------------------------

if(NOT NFX_STRINGBUILDER_BUILD_BENCHMARKS)
    return()
endif()

#----------------------------------------------
# Google Benchmark dependency
#----------------------------------------------

include(FetchContent)

set(NFX_STRINGBUILDER_DEPS_BENCHMARK_VERSION "1.9.5")

find_package(benchmark ${NFX_STRINGBUILDER_DEPS_BENCHMARK_VERSION} QUIET)
if(NOT benchmark_FOUND)
    message(STATUS "Google Benchmark not found on system, using FetchContent")

    set(BENCHMARK_ENABLE_TESTING        OFF CACHE BOOL "Build benchmark tests"                         FORCE)
    set(BENCHMARK_ENABLE_EXCEPTIONS     ON  CACHE BOOL "Enable exceptions in benchmark library"        FORCE)
    set(BENCHMARK_ENABLE_LTO            OFF CACHE BOOL "Enable link time optimization"                 FORCE)
    set(BENCHMARK_USE_LIBCXX            OFF CACHE BOOL "Use libc++ as the standard library"            FORCE)
    set(BENCHMARK_ENABLE_WERROR         OFF CACHE BOOL "Treat warnings as errors"                      FORCE)
    set(BENCHMARK_FORCE_WERROR          OFF CACHE BOOL "Force warnings as errors regardless of issues" FORCE)
    set(BENCHMARK_BUILD_32_BITS         OFF CACHE BOOL "Build a 32-bit version of the library"         FORCE)
    set(BENCHMARK_ENABLE_INSTALL        OFF CACHE BOOL "Install benchmark targets"                     FORCE)
    set(BENCHMARK_ENABLE_DOXYGEN        OFF CACHE BOOL "Build documentation with Doxygen"              FORCE)
    set(BENCHMARK_INSTALL_DOCS          OFF CACHE BOOL "Install documentation"                         FORCE)
    set(BENCHMARK_DOWNLOAD_DEPENDENCIES OFF CACHE BOOL "Download and build unmet dependencies"         FORCE)
    set(BENCHMARK_ENABLE_GTEST_TESTS    OFF CACHE BOOL "Build benchmark GTest-based tests"             FORCE)
    set(BENCHMARK_USE_BUNDLED_GTEST     OFF CACHE BOOL "Use bundled GoogleTest for benchmark"          FORCE)
    set(BENCHMARK_ENABLE_LIBPFM         OFF CACHE BOOL "Enable performance counters via libpfm"        FORCE)
    set(ENABLE_ASSEMBLY_TESTS_DEFAULT   OFF CACHE BOOL "Enable assembly tests by default"              FORCE)

    FetchContent_Declare(
        googleBenchmark
            GIT_REPOSITORY https://github.com/google/benchmark.git
            GIT_TAG        v${NFX_STRINGBUILDER_DEPS_BENCHMARK_VERSION}
            GIT_SHALLOW    TRUE
    )

    FetchContent_MakeAvailable(googleBenchmark)
else()
    message(STATUS "Using system-installed Google Benchmark version ${benchmark_VERSION} at ${benchmark_DIR}")
endif()

if(NFX_STRINGBUILDER_BUILD_COMPARATIVE_BENCHMARKS)
    set(NFX_STRINGBUILDER_DEPS_FMT_VERSION    "12.1.0")
    set(NFX_STRINGBUILDER_DEPS_ABSEIL_VERSION "20240722.0")

    # --- {fmt} ---
    find_package(fmt ${NFX_STRINGBUILDER_DEPS_FMT_VERSION} EXACT QUIET)
    if(NOT fmt_FOUND)
        message(STATUS "{fmt} not found on system, using FetchContent")

        set(FMT_PEDANTIC       OFF CACHE BOOL "" FORCE)
        set(FMT_WERROR         OFF CACHE BOOL "" FORCE)
        set(FMT_DOC            OFF CACHE BOOL "" FORCE)
        set(FMT_INSTALL        OFF CACHE BOOL "" FORCE)
        set(FMT_TEST           OFF CACHE BOOL "" FORCE)
        set(FMT_FUZZ           OFF CACHE BOOL "" FORCE)
        set(FMT_CUDA_TEST      OFF CACHE BOOL "" FORCE)
        set(FMT_OS             ON  CACHE BOOL "" FORCE)
        set(FMT_MODULE         OFF CACHE BOOL "" FORCE)
        set(FMT_SYSTEM_HEADERS ON  CACHE BOOL "" FORCE)
        set(FMT_UNICODE        ON  CACHE BOOL "" FORCE)

        FetchContent_Declare(
            fmt
                GIT_REPOSITORY https://github.com/fmtlib/fmt.git
                GIT_TAG        ${NFX_STRINGBUILDER_DEPS_FMT_VERSION}
                GIT_SHALLOW    TRUE
        )

        FetchContent_MakeAvailable(fmt)
    else()
        message(STATUS "Using system-installed {fmt} version ${fmt_VERSION} at ${fmt_DIR}")
    endif()

    # --- Abseil ---
    find_package(absl ${NFX_STRINGBUILDER_DEPS_ABSEIL_VERSION} EXACT QUIET)
    if(NOT absl_FOUND)
        message(STATUS "Abseil not found on system, using FetchContent")

        set(ABSL_ENABLE_INSTALL               OFF CACHE BOOL "" FORCE)
        set(ABSL_PROPAGATE_CXX_STD            ON  CACHE BOOL "" FORCE)
        set(ABSL_USE_SYSTEM_INCLUDES          OFF CACHE BOOL "" FORCE)
        set(ABSL_MSVC_STATIC_RUNTIME          OFF CACHE BOOL "" FORCE)
        set(ABSL_BUILD_TESTING                OFF CACHE BOOL "" FORCE)
        set(ABSL_BUILD_TEST_HELPERS           OFF CACHE BOOL "" FORCE)
        set(ABSL_USE_EXTERNAL_GOOGLETEST      OFF CACHE BOOL "" FORCE)
        set(ABSL_FIND_GOOGLETEST              OFF CACHE BOOL "" FORCE)
        set(ABSL_USE_GOOGLETEST_HEAD          OFF CACHE BOOL "" FORCE)
        set(ABSL_BUILD_MONOLITHIC_SHARED_LIBS OFF CACHE BOOL "" FORCE)

        FetchContent_Declare(
            abseil
                GIT_REPOSITORY https://github.com/abseil/abseil-cpp.git
                GIT_TAG        ${NFX_STRINGBUILDER_DEPS_ABSEIL_VERSION}
                GIT_SHALLOW    TRUE
        )

        FetchContent_MakeAvailable(abseil)
    else()
        message(STATUS "Using system-installed Abseil version ${absl_VERSION} at ${absl_DIR}")
    endif()
endif()

#----------------------------------------------
# Benchmarks source files
#----------------------------------------------

set(benchmark_sources "")

list(APPEND benchmark_sources
    BM_StringBuilder.cpp
)

#----------------------------------------------
# Configure benchmark executables
#----------------------------------------------

foreach(benchmark_source ${benchmark_sources})
    get_filename_component(benchmark_target_name ${benchmark_source} NAME_WE)
    if(NOT TARGET ${benchmark_target_name})
        add_executable(${benchmark_target_name} ${benchmark_source})

        #----------------------------------------------
        # Target linking
        #----------------------------------------------

        target_link_libraries(${benchmark_target_name}
            PRIVATE
                nfx-stringbuilder::static
                benchmark::benchmark
        )

        #----------------------------------------------
        # Optional library support
        #----------------------------------------------

        if(NFX_STRINGBUILDER_BUILD_COMPARATIVE_BENCHMARKS)
            if ( TARGET fmt::fmt )
                target_link_libraries( ${benchmark_target_name} PRIVATE fmt::fmt )
                target_compile_definitions( ${benchmark_target_name} PRIVATE HAS_FMT )
            endif()

            if ( TARGET absl::strings )
                target_link_libraries( ${benchmark_target_name} PRIVATE absl::strings )
                target_compile_definitions( ${benchmark_target_name} PRIVATE HAS_ABSEIL )
            endif()
        endif()

        #----------------------------------------------
        # Enable specific CPU features
        #----------------------------------------------

        target_compile_options(${benchmark_target_name}
            PRIVATE
                $<$<CXX_COMPILER_ID:MSVC>:/arch:AVX2>
                $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-march=native>
        )

        #----------------------------------------------
        # Properties
        #----------------------------------------------

        set_target_properties(${benchmark_target_name}
            PROPERTIES
                CXX_STANDARD 20
                CXX_STANDARD_REQUIRED ON
                CXX_EXTENSIONS OFF
                POSITION_INDEPENDENT_CODE ON
                DEBUG_POSTFIX "-d"
                RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
                RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
                RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
        )
    endif()
endforeach()
