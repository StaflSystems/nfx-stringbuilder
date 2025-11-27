#==============================================================================
# nfx-stringbuilder - CMake Sources
#==============================================================================

#----------------------------------------------
# Source files
#----------------------------------------------

set(private_sources)

list(APPEND private_sources
	${NFX_STRINGBUILDER_SOURCE_DIR}/DynamicStringBufferPool.cpp
	${NFX_STRINGBUILDER_SOURCE_DIR}/StringBuilder.cpp
)
