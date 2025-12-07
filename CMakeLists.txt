#==============================================================================
# nfx-stringbuilder - Top-level CMake configuration
#==============================================================================

#----------------------------------------------
# Project definition
#----------------------------------------------

cmake_minimum_required(VERSION 3.20)

project(nfx-stringbuilder
	VERSION 0.2.0
	DESCRIPTION "High-performance C++20 library for zero-allocation string building with thread-safe pooling"
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
option(NFX_STRINGBUILDER_BUILD_STATIC        "Build static library"               OFF)
option(NFX_STRINGBUILDER_BUILD_SHARED        "Build shared library"               OFF)

# --- Build components ---
option(NFX_STRINGBUILDER_BUILD_TESTS         "Build tests"                        OFF)
option(NFX_STRINGBUILDER_BUILD_SAMPLES       "Build samples"                      OFF)
option(NFX_STRINGBUILDER_BUILD_BENCHMARKS    "Build benchmarks"                   OFF)
option(NFX_STRINGBUILDER_BUILD_DOCUMENTATION "Build Doxygen documentation"        OFF)

# --- Installation ---
option(NFX_STRINGBUILDER_INSTALL_PROJECT     "Install project"                    OFF)

# --- Packaging ---
option(NFX_STRINGBUILDER_PACKAGE_SOURCE      "Enable source package generation"   OFF)
option(NFX_STRINGBUILDER_PACKAGE_ARCHIVE     "Enable TGZ/ZIP package generation"  OFF)
option(NFX_STRINGBUILDER_PACKAGE_DEB         "Enable DEB package generation"      OFF)
option(NFX_STRINGBUILDER_PACKAGE_RPM         "Enable RPM package generation"      OFF)
option(NFX_STRINGBUILDER_PACKAGE_WIX         "Enable WiX Windows installer (MSI)" OFF)

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
