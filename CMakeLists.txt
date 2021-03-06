# It must be set before project
set(CMAKE_BUILD_TYPE "Debug" CACHE STRING "Choose the type of build (by default Debug)")

cmake_minimum_required(VERSION 3.8)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules)

project(falltergeist)
set(falltergeist_VERSION  0.3.1)

include_directories(src)

if(EXISTS ${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	set(USE_CONAN TRUE)
endif()

if (USE_CONAN)
	include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
	conan_basic_setup()
endif()

find_package(ZLIB REQUIRED)
if(NOT ZLIB_FOUND)
	message(FATAL_ERROR "zlib library not found")
endif(NOT ZLIB_FOUND)
include_directories(${ZLIB_INCLUDE_DIRS})

find_package(SDL2 REQUIRED)
if(NOT SDL2_FOUND)
	message(FATAL_ERROR "SDL2 library not found")
endif(NOT SDL2_FOUND)
include_directories(${SDL2_INCLUDE_DIR})

find_package(SDL2_mixer REQUIRED)
if(NOT SDLMIXER_FOUND)
	message(FATAL_ERROR "SDL2_mixer library not found")
endif(NOT SDLMIXER_FOUND)
include_directories(${SDL_MIXER_INCLUDE_DIR})

find_package(SDL2_image REQUIRED)
if(NOT SDLIMAGE_FOUND)
	message(FATAL_ERROR "SDL2_image library not found")
endif(NOT SDLIMAGE_FOUND)
include_directories(${SDL_IMAGE_INCLUDE_DIR})

find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
	message(FATAL_ERROR "GL library not found")
endif(NOT OPENGL_FOUND)
include_directories(${OPENGL_INCLUDE_DIR})

if (NOT USE_CONAN)
	find_package(GLEW REQUIRED)
	if(NOT GLEW_FOUND)
		message(FATAL_ERROR "GLEW library not found")
	endif(NOT GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIR})
endif()

find_package(GLM REQUIRED)
if(NOT GLM_FOUND)
	message(FATAL_ERROR "GLM library not found")
endif(NOT GLM_FOUND)
include_directories(${GLM_INCLUDE_DIR})

file(GLOB_RECURSE SOURCES  src/*.cpp)

if(MSVC)
        file(GLOB_RECURSE HEADERS  src/*.h)
        set(_additional_includes "package/windows.rc" pchheader.cpp ${HEADERS})
endif()

add_executable(falltergeist main.cpp ${SOURCES} ${_additional_includes})

set_target_properties(falltergeist PROPERTIES
	CXX_STANDARD 14
	CXX_STANDARD_REQUIRED YES
	CXX_EXTENSIONS NO
)

if(MSVC)
	set_property(TARGET falltergeist APPEND PROPERTY LINK_FLAGS_DEBUG /NODEFAULTLIB:MSVCRT)

	set_property(TARGET falltergeist APPEND PROPERTY LINK_FLAGS_RELEASE /NODEFAULTLIB:MSVCRTD)
	set_property(TARGET falltergeist APPEND PROPERTY LINK_FLAGS_MINSIZEREL /NODEFAULTLIB:MSVCRTD)
	set_property(TARGET falltergeist APPEND PROPERTY LINK_FLAGS_RELWITHDEBINFO /NODEFAULTLIB:MSVCRTD)

	include(cmake/modules/PrecompiledHeader.cmake)
	add_precompiled_header(falltergeist pchheader.h FORCEINCLUDE SOURCE_CXX pchheader.cpp)

	set_property(GLOBAL PROPERTY USE_FOLDERS ON)
	source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Source Files" FILES ${SOURCES})
	source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" PREFIX "Header Files" FILES ${HEADERS})

	add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
	add_definitions(-Wall)
endif()

if (CONAN_LIBS)
	target_link_libraries(falltergeist ${CONAN_LIBS})
else()
	target_link_libraries(falltergeist ${ZLIB_LIBRARIES} ${SDL2_LIBRARY} ${SDL_MIXER_LIBRARY} ${SDL_IMAGE_LIBRARY} ${OPENGL_gl_LIBRARY} ${GLEW_LIBRARY})
endif()

include(cmake/install/windows.cmake)
include(cmake/install/linux.cmake)
include(cmake/install/apple.cmake)

# Copy required data files to the build directory
add_custom_command(TARGET falltergeist POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                       ${CMAKE_SOURCE_DIR}/data $<TARGET_FILE_DIR:falltergeist>/data)

# Appveyor
set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_FILE_NAME falltergeist-${falltergeist_VERSION}-${CMAKE_SYSTEM_NAME})
include(CPack)
