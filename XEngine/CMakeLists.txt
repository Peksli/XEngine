cmake_minimum_required(VERSION 3.27)

set(ENGINE_NAME XEngine)
project(${ENGINE_NAME})

file(GLOB_RECURSE sources  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE includes RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h  )
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources} ${includes})

# Configure XEngine lib
add_library(${ENGINE_NAME} STATIC 
	${sources} 
	${includes}
)
target_precompile_headers(${ENGINE_NAME} PRIVATE 
	XEnginePch.h
) 
target_compile_definitions(${ENGINE_NAME} PUBLIC
	$<$<CONFIG:Debug>:XEngine_DEBUG_BUILD>
)
target_include_directories(${ENGINE_NAME} PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# Configure deps
include(XEngineDeps.cmake)