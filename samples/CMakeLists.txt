#==============================================================================
# nfx-stringbuilder - Samples
#==============================================================================

#----------------------------------------------
# Sample condition check
#----------------------------------------------

if(NOT NFX_STRINGBUILDER_BUILD_SAMPLES)
    return()
endif()

#----------------------------------------------
# Samples source files
#----------------------------------------------

set(sample_sources "")

list(APPEND sample_sources
    Sample_StringBuilder.cpp
)

#----------------------------------------------
# Configure samples executables
#----------------------------------------------

foreach(sample_source ${sample_sources})
    get_filename_component(sample_target_name ${sample_source} NAME_WE)

    if(NOT TARGET ${sample_target_name})
        add_executable(${sample_target_name} ${sample_source})

        #----------------------------------------------
        # Target linking
        #----------------------------------------------

        target_link_libraries(${sample_target_name}
            PRIVATE
                nfx-stringbuilder::static
        )

        #----------------------------------------------
        # Enable specific CPU features
        #----------------------------------------------

        if(NFX_STRINGBUILDER_ENABLE_SIMD)
            include(CheckCXXSourceCompiles)

            if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
                set(CMAKE_REQUIRED_FLAGS "-march=native")
                check_cxx_source_compiles("int main() { return 0; }" COMPILER_SUPPORTS_MARCH_NATIVE)
                unset(CMAKE_REQUIRED_FLAGS)

                if(COMPILER_SUPPORTS_MARCH_NATIVE)
                    target_compile_options(${sample_target_name}
                        PRIVATE
                            $<$<CONFIG:Release>:-march=native>
                            $<$<CONFIG:RelWithDebInfo>:-march=native>
                    )
                else()
                    target_compile_options(${sample_target_name}
                        PRIVATE
                            $<$<CONFIG:Release>:-mavx2>
                            $<$<CONFIG:RelWithDebInfo>:-mavx2>
                    )
                endif()
            elseif(MSVC)
                target_compile_options(${sample_target_name}
                    PRIVATE
                        $<$<CONFIG:Release>:/arch:AVX2>
                        $<$<CONFIG:RelWithDebInfo>:/arch:AVX2>
                )
            endif()
        endif()

        #----------------------------------------------
        # Compiler warnings
        #----------------------------------------------

        target_compile_options(${sample_target_name}
            PRIVATE
                $<$<CXX_COMPILER_ID:MSVC>:/W4 /WX>
                $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-Wall -Wextra -Werror>
        )

        #----------------------------------------------
        # Properties
        #----------------------------------------------

        set_target_properties(${sample_target_name}
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
