#==============================================================================
# nfx-stringbuilder - CMake Sources
#==============================================================================

#----------------------------------------------
# Source files
#----------------------------------------------

set(PRIVATE_SOURCES)

list(APPEND PRIVATE_SOURCES
	${NFX_STRINGBUILDER_SOURCE_DIR}/DynamicStringBufferPool.cpp
	${NFX_STRINGBUILDER_SOURCE_DIR}/StringBuilder.cpp
)
