# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#

cmake_minimum_required (VERSION 3.15)

project (ProjectAlice LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20 CACHE STRING "The C++ standard to use")
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

# Copy everything from the assets directory into the CMAKE_CURRENT_BINARY_DIR/assets directory
file(GLOB_RECURSE TESTING ${PROJECT_SOURCE_DIR}/assets/*)
foreach(file ${TESTING})
	string(LENGTH "${PROJECT_SOURCE_DIR}/" SOURCE_DIR_LENGHT)
	string(SUBSTRING ${file} ${SOURCE_DIR_LENGHT} -1 file)
	list(APPEND ASSET_FILES "${CMAKE_CURRENT_BINARY_DIR}/${file}")

	add_custom_command(
			OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${file}
			DEPENDS ${PROJECT_SOURCE_DIR}/${file}
			COMMAND ${CMAKE_COMMAND} -E copy_if_different
					${PROJECT_SOURCE_DIR}/${file}
					${CMAKE_CURRENT_BINARY_DIR}/${file})
endforeach()

add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_executable(Alice WIN32 "src/main.cpp" ${ASSET_FILES})

# Used for storing the local path to the Victoria 2 directory
if(EXISTS ${PROJECT_SOURCE_DIR}/src/local_user_settings.hpp)
	target_compile_definitions(Alice PRIVATE LOCAL_USER_SETTINGS)
else()
	target_compile_definitions(Alice PRIVATE "GAME_DIR=\"NONE\"")
	target_compile_definitions(Alice PRIVATE "IGNORE_REAL_FILES_TESTS=1")
endif()

target_compile_definitions(Alice PRIVATE "PROJECT_ROOT=\"${PROJECT_SOURCE_DIR}\"")

if(NOT WIN32)
	add_compile_definitions(PREFER_ONE_TBB)
endif()

if(WIN32)
	# string(REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	# string(REPLACE "/W3" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	# string(REPLACE "/MDd" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})
	# string(REPLACE "/MD" "" CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE})
	# string(REPLACE "/EHsc" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	# string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
	# string(REPLACE "/RTC1" "" CMAKE_CXX_FLAGS_DEBUG ${CMAKE_CXX_FLAGS_DEBUG})

	set(CMAKE_CXX_FLAGS "")
	set(CMAKE_CXX_FLAGS_DEBUG "")
	set(CMAKE_CXX_FLAGS_RELEASE "")
	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		target_compile_options(Alice PRIVATE
										/bigobj /wd4100 /wd4189 /wd4065 /wd4201 /wd4324 /GR- /W4 /permissive- /WX /arch:AVX2 /GF /w34388 /w34389 -Wno-unused-parameter -Wno-unused-variable -Wno-unused-private-field /Z7 -Wno-invalid-offsetof -Wno-deprecated-volatile
			$<$<CONFIG:Debug>:			/RTC1 /EHsc /MTd /Od>
			$<$<NOT:$<CONFIG:Debug>>: 	/DNDEBUG /wd4530 /MT /O2 /Oi /sdl- /GS- /Gy /Gw /Zc:inline>)
		target_link_options(Alice PRIVATE
			$<$<CONFIG:Debug>: 			/DEBUG:FULL >
			$<$<NOT:$<CONFIG:Debug>>: 	/OPT:REF /OPT:ICF /LTCG>)
	else()
	set_target_properties(Alice PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_CURRENT_BINARY_DIR})
		target_compile_options(Alice PRIVATE
										/bigobj /wd4100 /wd4189 /wd4065 /wd4201 /wd4324 /GR- /W4 /permissive- /Zc:preprocessor /WX /arch:AVX2 /GF /w34388 /w34389 /Z7
			$<$<CONFIG:Debug>:			/RTC1 /EHsc /MTd /Od>
			$<$<NOT:$<CONFIG:Debug>>: 	/DNDEBUG /wd4530 /MT /O2 /Oi /GL /sdl- /GS- /Gy /Gw /Zc:preprocessor /Zc:inline>)
		target_link_options(Alice PRIVATE
			$<$<CONFIG:Debug>: 			/DEBUG:FULL >
			$<$<NOT:$<CONFIG:Debug>>: 	/DEBUG:FULL /OPT:REF /OPT:ICF /LTCG>)
	endif()
else() # GCC or CLANG
	if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
		target_compile_options(Alice PRIVATE
										-Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-switch -Wno-unused-private-field
			$<$<CONFIG:Debug>:			-msse4.1 -g -pipe>
			$<$<NOT:$<CONFIG:Debug>>: 	-msse4.1 -O3 -pipe -fno-rtti -DNDEBUG>)
	else()
		target_compile_options(Alice PRIVATE
										# -Wall -Wextra -Wpedantic -Werror -Wno-unused-parameter -Wno-unused-variable -Wno-switch -Wno-unused-private-field -Wno-unused-but-set-variable -Wno-parentheses
			$<$<CONFIG:Debug>:			-msse4.1 -g -pipe>
			$<$<NOT:$<CONFIG:Debug>>: 	-msse4.1 -O3 -pipe -fno-rtti -DNDEBUG>)
	endif()
endif()

# Include sub-projects.
add_subdirectory (dependencies)
add_subdirectory (ParserGenerator)

message("leftover CMAKE_CXX_FLAGS: \"${CMAKE_CXX_FLAGS}\"")
message("leftover CMAKE_CXX_FLAGS_DEBUG: \"${CMAKE_CXX_FLAGS_DEBUG}\"")
message("leftover CMAKE_CXX_FLAGS_RELEASE: \"${CMAKE_CXX_FLAGS_RELEASE}\"")

target_precompile_headers(Alice
	PRIVATE <stdint.h>
	PRIVATE <vector>
	PRIVATE <array>
	PRIVATE <optional>
	PRIVATE <memory>
	PRIVATE <variant>
	PRIVATE <string>
	PRIVATE <string_view>
	PRIVATE <charconv>
	PRIVATE <algorithm>
	PRIVATE <iterator>
	PRIVATE <stdlib.h>
	PRIVATE <ctype.h>
	PRIVATE <cmath>
	PRIVATE <functional>
	PRIVATE <cstdlib>
	PRIVATE <atomic>
	PRIVATE <chrono>
	PRIVATE [["simple_fs.hpp"]]
	PRIVATE [["constants.hpp"]]
	PRIVATE [["unordered_dense.h"]]
	PRIVATE [["dcon_generated.hpp"]]
	PRIVATE [["container_types.hpp"]]
	PRIVATE [["parsers_declarations.hpp"]]
	PRIVATE [["cyto_any.hpp"]]
	PRIVATE [["date_interface.hpp"]]
)


target_link_libraries(Alice PRIVATE dependency_DataContainer)
target_link_libraries(Alice PRIVATE libglew_static)
target_link_libraries(Alice PRIVATE stb_image)
target_link_libraries(Alice PRIVATE freetype)
target_link_libraries(Alice PRIVATE glm)
if (NOT WIN32)
	target_link_libraries(Alice PRIVATE dependency_tbb)
	target_link_libraries(Alice PRIVATE glfw)
	target_link_libraries(Alice PRIVATE miniaudio)
endif()

target_include_directories(Alice PUBLIC
	${PROJECT_SOURCE_DIR}/src
	${PROJECT_SOURCE_DIR}/src/common_types
	${PROJECT_SOURCE_DIR}/src/filesystem
	${PROJECT_SOURCE_DIR}/src/gamestate
	${PROJECT_SOURCE_DIR}/src/gui
	${PROJECT_SOURCE_DIR}/src/gui/topbar_subwindows
	${PROJECT_SOURCE_DIR}/src/gui/topbar_subwindows/diplomacy_subwindows
	${PROJECT_SOURCE_DIR}/src/gui/topbar_subwindows/production_subwindows
	${PROJECT_SOURCE_DIR}/src/gui/topbar_subwindows/politics_subwindows
	${PROJECT_SOURCE_DIR}/src/gui/topbar_subwindows/military_subwindows
	${PROJECT_SOURCE_DIR}/src/ogl
	${PROJECT_SOURCE_DIR}/src/parsing
	${PROJECT_SOURCE_DIR}/src/window
	${PROJECT_SOURCE_DIR}/src/text
	${PROJECT_SOURCE_DIR}/src/text/bmfontgl
	${PROJECT_SOURCE_DIR}/src/sound
	${PROJECT_SOURCE_DIR}/src/map
	${PROJECT_SOURCE_DIR}/src/nations
	${PROJECT_SOURCE_DIR}/src/provinces
	${PROJECT_SOURCE_DIR}/src/economy
	${PROJECT_SOURCE_DIR}/src/culture
	${PROJECT_SOURCE_DIR}/src/military
	${PROJECT_SOURCE_DIR}/src/scripting
	${PROJECT_SOURCE_DIR}/src/zstd
	"${glew_SOURCE_DIR}/include/GL"
	"ankerl")

# GENERATE CONTAINER
set(CONTAINER_PATH ${PROJECT_SOURCE_DIR}/src/gamestate/dcon_generated)

# The command to build the generated file
add_custom_command(
  OUTPUT ${CONTAINER_PATH}.hpp
  COMMAND DCONGENERATOR ${CONTAINER_PATH}.txt
  DEPENDS ${CONTAINER_PATH}.txt
  VERBATIM)

# Sets a dependency on the generated file
add_custom_target(GENERATE_CONTAINER DEPENDS ${CONTAINER_PATH}.hpp)
add_dependencies(Alice GENERATE_CONTAINER ParserGenerator)

# The command to build the generated parsers file
add_custom_command(
  OUTPUT ${PROJECT_SOURCE_DIR}/src/parsing/parser_defs_generated.hpp
  COMMAND ParserGenerator ${PROJECT_SOURCE_DIR}/src/parsing/parser_defs.txt
  DEPENDS ${PROJECT_SOURCE_DIR}/src/parsing/parser_defs.txt
  VERBATIM)

# Sets a dependency on the generated file
add_custom_target(GENERATE_PARSERS DEPENDS ${PROJECT_SOURCE_DIR}/src/parsing/parser_defs_generated.hpp)
add_dependencies(Alice GENERATE_PARSERS)


if (BUILD_TESTING)
    enable_testing()
	add_subdirectory(tests)
endif()

