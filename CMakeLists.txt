cmake_minimum_required(VERSION 2.8.11)

set(TARGET calculator)

project(${TARGET})

set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin/ )

if(NOT TARGET asls)
	find_package(ASL REQUIRED)
endif()

set(SRC
	Calculator.cpp
	Calculator.h
	demo.cpp
)

add_executable(${TARGET} ${SRC}) 

target_link_libraries(${TARGET} asls)
