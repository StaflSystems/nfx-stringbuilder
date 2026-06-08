#==============================================================================
# nfx-stringbuilder - CMake targets
#==============================================================================

#----------------------------------------------
# Check if we have sources to build
#----------------------------------------------

if(NOT private_sources)
    return()
endif()

#----------------------------------------------
# Targets definition
#----------------------------------------------

# --- Create shared library if requested ---
if(NFX_STRINGBUILDER_BUILD_SHARED)
    add_library(${PROJECT_NAME} SHARED)
    target_sources(${PROJECT_NAME}
        PRIVATE
            ${private_sources}
    )

    set_target_properties(${PROJECT_NAME}
        PROPERTIES
            LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )

    add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
endif()

# --- Create static library if requested ---
if(NFX_STRINGBUILDER_BUILD_STATIC)
    add_library(${PROJECT_NAME}-static STATIC)
    target_sources(${PROJECT_NAME}-static
        PRIVATE
            ${private_sources}
    )

    set_target_properties(${PROJECT_NAME}-static
        PROPERTIES
            OUTPUT_NAME ${PROJECT_NAME}-static
            ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib
    )

    add_library(${PROJECT_NAME}::static ALIAS ${PROJECT_NAME}-static)
endif()

#----------------------------------------------
# Targets properties
#----------------------------------------------

function(configure_target target_name)
    # --- Include directories ---
    target_include_directories(${target_name}
        PUBLIC
            $<BUILD_INTERFACE:${NFX_STRINGBUILDER_INCLUDE_DIR}>
            $<INSTALL_INTERFACE:include>
        PRIVATE
            ${NFX_STRINGBUILDER_SOURCE_DIR}
    )

    # --- Properties ---
    set_target_properties(${target_name}
        PROPERTIES
            CXX_STANDARD ${NFX_STRINGBUILDER_CXX_STANDARD}
            CXX_STANDARD_REQUIRED ON
            CXX_EXTENSIONS OFF
            DEBUG_POSTFIX "-d"
            POSITION_INDEPENDENT_CODE ON
            VERSION ${PROJECT_VERSION}
            SOVERSION ${PROJECT_VERSION_MAJOR}
    )

    # --- Public language requirement ---
    # Propagate a minimum of C++17 to consumers of the static/shared targets.
    # The headers compile under both C++17 and C++20; consumers may build at a
    # higher standard than the library was compiled with.
    target_compile_features(${target_name} PUBLIC cxx_std_17)

    # --- Enable specific CPU features ---
    if(NFX_STRINGBUILDER_ENABLE_SIMD)
        include(CheckCXXSourceCompiles)

        # GCC/Clang: -march=native or fallback to -mavx2
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            # Check if compiler supports -march=native
            set(CMAKE_REQUIRED_FLAGS "-march=native")
            check_cxx_source_compiles("int main() { return 0; }" COMPILER_SUPPORTS_MARCH_NATIVE)
            unset(CMAKE_REQUIRED_FLAGS)

            if(COMPILER_SUPPORTS_MARCH_NATIVE)
                target_compile_options(${target_name}
                    PRIVATE
                        $<$<CONFIG:Release>:-march=native>
                        $<$<CONFIG:RelWithDebInfo>:-march=native>
                )
                message(STATUS "nfx-stringbuilder: Enabling native CPU optimizations (GCC/Clang -march=native)")
            else()
                # Fallback to explicit AVX2
                target_compile_options(${target_name}
                    PRIVATE
                        $<$<CONFIG:Release>:-mavx2>
                        $<$<CONFIG:RelWithDebInfo>:-mavx2>
                )
                message(STATUS "nfx-stringbuilder: Enabling AVX2 support (GCC/Clang -mavx2)")
            endif()
        elseif(MSVC)
            # MSVC: /arch:AVX2
            target_compile_options(${target_name}
                PRIVATE
                    $<$<CONFIG:Release>:/arch:AVX2>
                    $<$<CONFIG:RelWithDebInfo>:/arch:AVX2>
            )
            message(STATUS "nfx-stringbuilder: Enabling AVX2 support (MSVC /arch:AVX2)")
        endif()

        # Note: Debug builds intentionally do NOT enable SIMD for easier debugging
    else()
        message(STATUS "nfx-stringbuilder: SIMD optimizations disabled (NFX_STRINGBUILDER_ENABLE_SIMD=OFF)")
    endif()

    # --- Compiler warnings ---
    target_compile_options(${target_name}
        PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
            $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Werror>
    )
endfunction()

# --- Apply configuration to both targets ---
if(NFX_STRINGBUILDER_BUILD_SHARED)
    configure_target(${PROJECT_NAME})
    if(WIN32)
        set_target_properties(${PROJECT_NAME}
            PROPERTIES
                WINDOWS_EXPORT_ALL_SYMBOLS TRUE
        )
        configure_file(
            ${CMAKE_CURRENT_SOURCE_DIR}/cmake/nfxStringBuilderVersion.rc.in
            ${CMAKE_BINARY_DIR}/nfxStringBuilder.rc
            @ONLY
        )
        target_sources(${PROJECT_NAME} PRIVATE ${CMAKE_BINARY_DIR}/nfxStringBuilder.rc)
    endif()
endif()

if(NFX_STRINGBUILDER_BUILD_STATIC)
    configure_target(${PROJECT_NAME}-static)
endif()
