cmake_minimum_required(VERSION 3.12.0)
project(EDAProjectPartTwo VERSION 0.1.0)

include(CTest)

message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
if (NOT MSVC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wno-unused-function -Wno-unused-parameter")
endif()

file(GLOB_RECURSE SOURCES RELATIVE ${CMAKE_SOURCE_DIR} "src/*.c")
add_executable(EDAProjectPartTwo ${SOURCES})

enable_testing()

add_executable(avl_test "test/avl_test.c" "src/avl_node.c")
add_test(NAME avl_test COMMAND avl_test)

set_target_properties(EDAProjectPartTwo avl_test PROPERTIES
	C_STANDARD 11
	C_STANDARD_REQUIRED ON
	C_EXTENSIONS OFF
)
