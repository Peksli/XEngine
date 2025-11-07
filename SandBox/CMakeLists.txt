cmake_minimum_required(VERSION 3.27)

set(CLIENT_NAME SandBox)

project(${CLIENT_NAME})

file(GLOB_RECURSE sources  RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.cpp)
file(GLOB_RECURSE includes RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} *.h  )
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${sources} ${includes})

# Configure SandBox.exe 
add_executable(${CLIENT_NAME} 
	${sources} 
	${includes}
)
target_include_directories(${CLIENT_NAME} PRIVATE 
	${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(${CLIENT_NAME} PRIVATE 
	XEngine
)