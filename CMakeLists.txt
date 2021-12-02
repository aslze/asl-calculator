cmake_minimum_required(VERSION 2.8.12)

project(asl-calculator)

if(NOT TARGET asls)
	find_package(ASL REQUIRED)
endif()

set(TARGET asl-calculator)

set(SRC
	Calculator.cpp
	Calculator.h
)

add_library(${TARGET} STATIC ${SRC})
target_link_libraries(${TARGET} asls)
target_include_directories(${TARGET} PUBLIC .)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)

	set(TARGET calcdemo)

	set(SRC
		demo.cpp
	)

	add_executable(${TARGET} ${SRC}) 
	target_link_libraries(${TARGET} asl-calculator asls)

endif()

