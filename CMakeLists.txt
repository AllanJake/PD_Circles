cmake_minimum_required(VERSION 3.14)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 20)

if (NOT "$ENV{PLAYDATE_SDK_PATH}" STREQUAL "")
	# Convert path from Windows
	file(TO_CMAKE_PATH "$ENV{PLAYDATE_SDK_PATH}" SDK)
else()
	execute_process(
			COMMAND bash -c "grep '^SDKRoot' \"$HOME/.Playdate/config\" | head -n 1 | cut -c9-"
			COMMAND head -n 1
			COMMAND cut -c9-
			OUTPUT_VARIABLE SDK
			OUTPUT_STRIP_TRAILING_WHITESPACE
	)
endif()

if (NOT EXISTS ${SDK})
	message(FATAL_ERROR "SDK Path not found; set ENV value PLAYDATE_SDK_PATH")
	return()
endif()

set(CMAKE_CONFIGURATION_TYPES "Debug;Release")
set(CMAKE_XCODE_GENERATE_SCHEME TRUE)

# Game Name Customization
set(PLAYDATE_GAME_NAME C_Circles)
set(PLAYDATE_GAME_DEVICE ${PLAYDATE_GAME_NAME}_DEVICE)

project(${PLAYDATE_GAME_NAME} CXX ASM)

# Collect all source and header files
file(GLOB_RECURSE SOURCE_FILES
    src/Source/*.cpp
	src/Header/*.h
    src/Header/*.hpp
)

if (TOOLCHAIN STREQUAL "armgcc")
	add_executable(${PLAYDATE_GAME_DEVICE} ${SOURCE_FILES})
	target_include_directories(${PLAYDATE_GAME_DEVICE} PRIVATE 
		"${SDK}/C_API"
	)
else()
	add_library(${PLAYDATE_GAME_NAME} SHARED ${SOURCE_FILES})
	target_include_directories(${PLAYDATE_GAME_NAME} PRIVATE 
		"${SDK}/C_API"
	)
endif()


# Ensures default visibility (safe fallback)
add_compile_options(-fvisibility=hidden)

include(${SDK}/C_API/buildsupport/playdate_game.cmake)
