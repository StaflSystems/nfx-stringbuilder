#==============================================================================
# nfx-stringbuilder - Top-level CMake configuration
#==============================================================================

#----------------------------------------------
# Project definition
#----------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(nfx-stringbuilder
    VERSION 0.7.0
    DESCRIPTION "Cross-platform C++20 library for zero-allocation string building with Small Buffer Optimization"
    HOMEPAGE_URL "https://github.com/nfx-libs/nfx-stringbuilder"
    LANGUAGES CXX
)

#----------------------------------------------
# Version configuration
#----------------------------------------------

set(NFX_STRINGBUILDER_VERSION_MAJOR ${PROJECT_VERSION_MAJOR} CACHE INTERNAL "nfx-stringbuilder major version")
set(NFX_STRINGBUILDER_VERSION_MINOR ${PROJECT_VERSION_MINOR} CACHE INTERNAL "nfx-stringbuilder minor version")
set(NFX_STRINGBUILDER_VERSION_PATCH ${PROJECT_VERSION_PATCH} CACHE INTERNAL "nfx-stringbuilder patch version")
set(NFX_STRINGBUILDER_VERSION       ${PROJECT_VERSION}       CACHE INTERNAL "nfx-stringbuilder version")

#----------------------------------------------
# Testing setup
#----------------------------------------------

enable_testing()

#----------------------------------------------
# Build options
#----------------------------------------------

# --- Library build types ---
option(NFX_STRINGBUILDER_BUILD_STATIC                 "Build static library"               ON )
option(NFX_STRINGBUILDER_BUILD_SHARED                 "Build shared library"               OFF)

# --- Language standard ---
# Minimum required standard is C++17. C++20 additionally enables the
# std::format()-based formatting API and the std::formatter specialization.
set(NFX_STRINGBUILDER_CXX_STANDARD "20" CACHE STRING "C++ standard used to build nfx-stringbuilder (17 or 20)")
set_property(CACHE NFX_STRINGBUILDER_CXX_STANDARD PROPERTY STRINGS "17" "20")

# --- Build components ---
option(NFX_STRINGBUILDER_BUILD_TESTS                  "Build tests"                        OFF)
option(NFX_STRINGBUILDER_BUILD_SAMPLES                "Build samples"                      OFF)
option(NFX_STRINGBUILDER_BUILD_BENCHMARKS             "Build benchmarks"                   OFF)
option(NFX_STRINGBUILDER_BUILD_COMPARATIVE_BENCHMARKS "Build comparative benchmarks"       OFF)
option(NFX_STRINGBUILDER_BUILD_DOCUMENTATION          "Build Doxygen documentation"        OFF)

# --- Performance optimizations ---
option(NFX_STRINGBUILDER_ENABLE_SIMD                  "Enable SIMD optimizations"          ON )

# --- Installation ---
option(NFX_STRINGBUILDER_INSTALL_PROJECT              "Install project"                    OFF)

# --- Packaging ---
option(NFX_STRINGBUILDER_PACKAGE_SOURCE               "Enable source package generation"   OFF)

#----------------------------------------------
# CMake modules configuration
#----------------------------------------------

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

#----------------------------------------------
# Main configuration
#----------------------------------------------

include(nfxStringBuilderBuildConfig)
include(nfxStringBuilderSources)
include(nfxStringBuilderTargets)

#----------------------------------------------
# Installation and packaging
#----------------------------------------------

include(nfxStringBuilderInstall)
include(nfxStringBuilderPackaging)

#----------------------------------------------
# Targets
#----------------------------------------------

add_subdirectory(test)
add_subdirectory(samples)
add_subdirectory(benchmark)
add_subdirectory(doc)
