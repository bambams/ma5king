# CMake file for MA5kinG
#
# by Milan Mimica
#
# (what a shame)
#

cmake_minimum_required(VERSION 2.6)
if(COMMAND cmake_policy)
  cmake_policy(SET CMP0003 NEW)
endif(COMMAND cmake_policy)

if(DEBUG)
    set(CMAKE_CONFIGURATION_TYPES "Debug" CACHE INTERNAL "internal")
else(DEBUG)
    set(CMAKE_CONFIGURATION_TYPES "Release" CACHE INTERNAL "internal")
endif(DEBUG)

project(MA5kinG C CXX)


# Search for C header files in these directories.
include_directories(
    include
)

include(FileList.txt)

option(DEBUG "Build the debug library" off)
option(STATIC_ALLEGRO "Link to static Allegro lib" off)

set(INSTALL_PREFIX "")

if(MSVC_IDE)
    set(COMPILER_MSVC 1)

    # Guess VCINSTALLDIR from the value of CMAKE_C_COMPILER if it's not set.
    if("$ENV{VCINSTALLDIR}" STREQUAL "")
        string(REGEX REPLACE "/bin/[^/]*$" "" VCINSTALLDIR "${CMAKE_C_COMPILER}")
        message(STATUS "Guessed MSVC directory: ${VCINSTALLDIR}")
    else("$ENV{VCINSTALLDIR}" STREQUAL "")
        file(TO_CMAKE_PATH "$ENV{VCINSTALLDIR}" VCINSTALLDIR)
        message(STATUS "Using VCINSTALLDIR: ${VCINSTALLDIR}")
    endif("$ENV{VCINSTALLDIR}" STREQUAL "")

    # Install in VCINSTALLDIR by default
    if(INSTALL_PREFIX STREQUAL "")
        set(CMAKE_INSTALL_PREFIX ${VCINSTALLDIR})
    else(INSTALL_PREFIX STREQUAL "")
        set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})
    endif(INSTALL_PREFIX STREQUAL "")
endif(MSVC_IDE)

if(MINGW)
    # For alplatf.h
    set(ALLEGRO_MINGW32 1)

    # Guess MINGDIR from the value of CMAKE_C_COMPILER if it's not set.
    if("$ENV{MINGDIR}" STREQUAL "")
        string(REGEX REPLACE "/bin/[^/]*$" "" MINGDIR "${CMAKE_C_COMPILER}")
        message(STATUS "Guessed MinGW directory: ${MINGDIR}")
    else("$ENV{MINGDIR}" STREQUAL "")
        file(TO_CMAKE_PATH "$ENV{MINGDIR}" MINGDIR)
        message(STATUS "Using MINGDIR: ${MINGDIR}")
    endif("$ENV{MINGDIR}" STREQUAL "")

    # Search in MINGDIR for headers and libraries.
    set(CMAKE_PREFIX_PATH "${MINGDIR}")

    # Install to MINGDIR
    if(INSTALL_PREFIX STREQUAL "")
        set(CMAKE_INSTALL_PREFIX ${MINGDIR})
    else(INSTALL_PREFIX STREQUAL "")
        set(CMAKE_INSTALL_PREFIX ${INSTALL_PREFIX})
    endif(INSTALL_PREFIX STREQUAL "")
endif(MINGW)

# List of source files
set(LIBRARY_SOURCES ${MASKING_SRC_FILES})

if(COMPILER_MSVC)
    list(APPEND LIBRARY_SOURCES ${MASKING_INCLUDE_FILES})
    set(MSVC_WARN_FLAGS "-D_CRT_SECURE_NO_WARNINGS -D_CRT_SECURE_NO_DEPRECATE")
endif(COMPILER_MSVC)

if(STATIC_ALLEGRO)
    set(STATIC_FLAG "-DALLEGRO_STATICLINK")
    if(DEBUG)
        set(ALLEGRO_SUFFIX "-debug-static")
        set(DEBUGMODE_FLAG "-DDEBUGMODE")
    else(DEBUG)
        set(ALLEGRO_SUFFIX "-static")
    endif(DEBUG)
else(STATIC_ALLEGRO)
    if(DEBUG)
        set(DEBUGMODE_FLAG "-DDEBUGMODE")
        set(ALLEGRO_SUFFIX "-debug")
    else(DEBUG)
        set(ALLEGRO_SUFFIX "")
    endif(DEBUG)
endif(STATIC_ALLEGRO)

# XXX should use pkg-config
set(LIBS allegro${ALLEGRO_SUFFIX}
    allegro_audio${ALLEGRO_SUFFIX}
    allegro_color${ALLEGRO_SUFFIX}
    allegro_dialog${ALLEGRO_SUFFIX}
    allegro_font${ALLEGRO_SUFFIX}
    allegro_image${ALLEGRO_SUFFIX}
    allegro_main${ALLEGRO_SUFFIX}
    allegro_primitives${ALLEGRO_SUFFIX}
    allegro_ttf${ALLEGRO_SUFFIX}
    allegro_acodec${ALLEGRO_SUFFIX})

# required A5 addons' deps, named differently on different platforms
if(WIN32)
    list(APPEND LIBS png z vorbisfile vorbis ogg freetype jpeg)
else(WIN32)
    list(APPEND LIBS png z vorbisfile vorbis ogg freetype jpeg)
endif(WIN32)

find_package(OpenGL)

if(STATIC_ALLEGRO)
    if(WIN32)
        list(APPEND LIBS dsound dinput8 dxguid winmm d3d9 opengl32 glu32 psapi)
	else(WIN32)
		list(APPEND LIBS "-lopenal -lasound -lm -lpthread -lSM -lICE -lX11 -lXext -lXcursor -lXxf86vm -lXinerama -lXpm")
    endif(WIN32)
    list(APPEND LIBS ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
else(STATIC_ALLEGRO)
	if(UNIX)
		list(APPEND LIBS "-lopenal -lasound -lm -lpthread -lSM -lICE -lX11 -lXext -lXcursor -lXxf86vm -lXinerama -lXpm")
        list(APPEND LIBS ${OPENGL_gl_LIBRARY} ${OPENGL_glu_LIBRARY})
	endif(UNIX)
endif(STATIC_ALLEGRO)

if(UNIX)
    if(DEBUG)
        set(CMAKE_C_FLAGS "-O0 -g2 -Wall -ansi")
    else(DEBUG)
        set(CMAKE_C_FLAGS "-O2 -g0 -Wall -ansi")
    endif(DEBUG)
endif(UNIX)

set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)

add_library(MA5kinG STATIC ${LIBRARY_SOURCES})
set_target_properties(MA5kinG
                      PROPERTIES
                      COMPILE_FLAGS "${CMAKE_C_FLAGS} ${DEBUGMODE_FLAG} ${STATIC_FLAG} ${MSVC_WARN_FLAGS}"
                      OUTPUT_NAME masking
)

add_executable(ex09
               WIN32
               examples/ex09.cpp)
set_target_properties(ex09
                      PROPERTIES
                      COMPILE_FLAGS "-I../include/MASkinG/ ${CMAKE_C_FLAGS} ${DEBUGMODE_FLAG} ${STATIC_FLAG} ${MSVC_WARN_FLAGS}"
                      OUTPUT_NAME ex09)
target_link_libraries(ex09 MA5kinG ${LIBS})

add_executable(ex05
               WIN32
               examples/ex05.cpp)
set_target_properties(ex05
                      PROPERTIES
                      COMPILE_FLAGS "-I../include/MASkinG/ ${CMAKE_C_FLAGS} ${DEBUGMODE_FLAG} ${STATIC_FLAG} ${MSVC_WARN_FLAGS}"
                      OUTPUT_NAME ex05)
target_link_libraries(ex05 MA5kinG ${LIBS})

add_executable(ex02
               WIN32
               examples/ex02.cpp)
set_target_properties(ex02
                      PROPERTIES
                      COMPILE_FLAGS "-I../include/MASkinG/ ${CMAKE_C_FLAGS} ${DEBUGMODE_FLAG} ${STATIC_FLAG} ${MSVC_WARN_FLAGS}"
                      OUTPUT_NAME ex02)
target_link_libraries(ex02 MA5kinG ${LIBS})

install(TARGETS MA5kinG
        DESTINATION lib
        LIBRARY PERMISSIONS
            OWNER_READ OWNER_WRITE OWNER_EXECUTE
            GROUP_READ             GROUP_EXECUTE
            WORLD_READ             WORLD_EXECUTE
)

install(FILES ${MASKING_INCLUDE_FILES}
        PUBLIC_HEADER DESTINATION include/MASkinG
)

install(FILES include/MASkinG.h
        PUBLIC_HEADER DESTINATION include
)
