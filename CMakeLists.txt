cmake_minimum_required(VERSION 3.16)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

##
# Function that generates source groups from directories
##
function(GenerateSourceGoups Sources)
	foreach(Source ${Sources})
		get_filename_component(SourcePath "${Source}" PATH)
		if (WIN32)
			string(REPLACE "/" "\\" SourcePath "${SourcePath}")
		endif()
		source_group("${SourcePath}" FILES "${Source}")
	endforeach()
endfunction()

##
# Start
##
project(s4_rtti VERSION "1.0.0")
enable_testing()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(S4_RTTI_PROJECT_ROOT_DIR ${CMAKE_SOURCE_DIR})
set(S4_RTTI_PROJECT_DIST_DIR "${S4_RTTI_PROJECT_ROOT_DIR}/dist")
set(S4_RTTI_PROJECT_COMMON_BUILD_DIR "${S4_RTTI_PROJECT_ROOT_DIR}/common_build")

if(MSVC)
	add_compile_options(/W4 /WX /wd4251 /wd5105 /wd4100 /wd5103 /Zc:preprocessor)
else()
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++2a")
	add_compile_options(-Wall -Wextra -Werror -Wno-unused-variable -Wno-unused-parameter)
endif()

# Output dirs
if (WIN32)
	set(S4_RTTI_PROJECT_FULL_DIST_DIR "${S4_RTTI_PROJECT_DIST_DIR}") # Config name is added autmatically on windows
else()
	set(S4_RTTI_PROJECT_FULL_DIST_DIR "${S4_RTTI_PROJECT_DIST_DIR}/${BUILD_CONFIG_NAME}")
endif()
	
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${S4_RTTI_PROJECT_FULL_DIST_DIR})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${S4_RTTI_PROJECT_FULL_DIST_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${S4_RTTI_PROJECT_FULL_DIST_DIR})

configure_file(
	"${PROJECT_SOURCE_DIR}/config/s4_rtti_cmake_config.hpp.in" 
	"${PROJECT_BINARY_DIR}/config/s4_rtti_cmake_config.hpp"
)
include_directories("${PROJECT_BINARY_DIR}/config")

# Fetch dependencies
include(FetchContent)
FetchContent_Declare(
  Catch2
  GIT_REPOSITORY "https://github.com/catchorg/Catch2.git"
  GIT_TAG "v3.0.0-preview3"
)
FetchContent_Declare(
  s4_core
  GIT_REPOSITORY "https://github.com/PiotrMoscicki/s4_core.git"
  GIT_TAG "v0.0.2"
)

FetchContent_MakeAvailable(Catch2 s4_core)
list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)

# Target names
set(S4_RTTI_TARGET s4_rtti)
set(S4_RTTI_EXAMPLE_TARGET s4_rtti_example)
set(S4_RTTI_TEST_TARGET s4_rtti_test)
set(S4_RTTI_TEST_LIB_TARGET s4_rtti_test_lib)
set(S4_RTTI_TEST_INTERFACE_TARGET s4_rtti_test_interface)

# Subdirectories
add_subdirectory(src)
add_subdirectory(example)
add_subdirectory(test)

#install(FILES src/s4_rtti-config.cmake DESTINATION lib)
#install(EXPORT ${S4_RTTI_TARGET} DESTINATION lib)