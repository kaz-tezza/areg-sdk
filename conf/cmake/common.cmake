# ###########################################################################
# Common settings for all projects
# ###########################################################################


# Identify the OS
if(APPLE)
    set(AREG_OS "macOS")
elseif(UNIX)
    set(AREG_OS "Linux")
else()
    set(AREG_OS "Windows")
endif()

# Determining bitness by size of void pointer
# 8 bytes ==> x64 and 4 bytes ==> x86
if(NOT ${CMAKE_SIZEOF_VOID_P} MATCHES "8")
    set(Platform "x86")
endif()

# -----------------------------------------------------
# areg specific internal variable settings
# -----------------------------------------------------
# The toolset
set(AREG_TOOLCHAIN "${CMAKE_CXX_COMPILER_ID}")
# Relative path of the output folder for the builds
string(TOLOWER "${UserDefOutput}/build/${AREG_TOOLCHAIN}/${AREG_OS}-${Platform}-${CMAKE_BUILD_TYPE}" AREG_PRODUCT_PATH)
# The absolute path for builds
set(AREG_OUTPUT_DIR "${AregBuildRoot}/${AREG_PRODUCT_PATH}")
# The absolute path for generated files
set(AREG_GENERATE_DIR "${AregBuildRoot}/${UserDefOutput}/generate")
# The absolute path for obj files.
set(AREG_OUTPUT_OBJ "${AREG_OUTPUT_DIR}/obj")
# The absolute path for static libraries
set(AREG_OUTPUT_LIB "${AREG_OUTPUT_DIR}/lib")
# The absolute path for all executables and shared libraries
set(AREG_OUTPUT_BIN "${AREG_OUTPUT_DIR}/bin")
# Project inclues
set(AREG_INCLUDES "-I${AREG_BASE} -I${AREG_GENERATE_DIR} -I${UserDefIncludes}")

# The development environment -- POSIX or Win32 API
set(AREG_DEVELOP_ENV)
# The linker flags
set(AREG_LDFLAGS)
# The compiler options
set(AREG_COMPILER_OPTIONS)

# Adding common definition
add_definitions(-DUNICODE)

if(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DNDEBUG)
else()
    add_definitions(-DDEBUG)
endif()

# Checking Compiler for adding corresponded tweaks and flags
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")

    # POSIX API
    add_definitions(-DPOSIX)
    set(AREG_DEVELOP_ENV "Posix")

    if (CMAKE_BUILD_TYPE MATCHES "Release")
        list(APPEND AREG_COMPILER_OPTIONS -O2)
    else()
        list(APPEND AREG_COMPILER_OPTIONS -O0 -g3)
    endif()

    # Clang compile options
    list(APPEND AREG_COMPILER_OPTIONS -pthread -Wall -c -fmessage-length=0 -stdlib=libc++ ${UserDefines})
    # Linker flags (-l is not necessary)
    list(APPEND AREG_LDFLAGS c++ m ncurses pthread rt)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "GNU")

    # POSIX API
    add_definitions(-DPOSIX)
    set(AREG_DEVELOP_ENV "Posix")

    if (CMAKE_BUILD_TYPE MATCHES "Release")
        list(APPEND AREG_COMPILER_OPTIONS -O2)
    else()
        list(APPEND AREG_COMPILER_OPTIONS -O0 -g3)
    endif()

    # GNU compile options
    list(APPEND AREG_COMPILER_OPTIONS -pthread -Wall -c -fmessage-length=0 -MMD -std=c++17 ${UserDefines})
    # Linker flags (-l is not necessary)
    list(APPEND AREG_LDFLAGS stdc++ m ncurses pthread rt)

elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")

    # Visual Studio C++
    # Windows / Win32 API
    add_definitions(-DWINDOWS)
    set(AREG_DEVELOP_ENV "Win32")
    if(NOT CMAKE_BUILD_TYPE MATCHES "Release")
        list(APPEND AREG_COMPILER_OPTIONS -Od -RTC1 -c)
    endif()

    # MS Visual C++ compile options
    list(APPEND AREG_COMPILER_OPTIONS -std=c++17)
    # Linker flags (-l is not necessary)
    list(APPEND AREG_LDFLAGS advapi32 psapi shell32 ws2_32)

else()

    message(WARNING "Unsupported compiler type. The result is unpredictable, by default use GNU compiler settings and POSIX API")
    add_definitions(-DPOSIX)
    # POSIX API
    add_definitions(-DPOSIX)
    set(AREG_DEVELOP_ENV "Posix")

    if (CMAKE_BUILD_TYPE MATCHES "Release")
        list(APPEND AREG_COMPILER_OPTIONS -O2)
    else()
        list(APPEND AREG_COMPILER_OPTIONS -O0 -g3)
    endif()

    # Compile options
    list(APPEND AREG_COMPILER_OPTIONS -pthread -Wall -c -fmessage-length=0 -MMD -std=c++17 ${UserDefines})
    # Linker flags (-l is not necessary)
    list(APPEND AREG_LDFLAGS stdc++ m ncurses pthread rt)

endif()

# flags for bitness
if(Platform MATCHES "x86_64" AND NOT AREG_DEVELOP_ENV MATCHES "Windows")
    if(Bitness MATCHES "32")
        list(APPEND AREG_COMPILER_OPTIONS -m32)
    else()
        list(APPEND AREG_COMPILER_OPTIONS -m64)
    endif()
endif()

# Adding compile options
add_compile_options(${AREG_COMPILER_OPTIONS})

# Adding areg/product directory for clean-up
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${AREG_OUTPUT_DIR})

# Setting output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${AREG_OUTPUT_LIB})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${AREG_OUTPUT_BIN})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${AREG_OUTPUT_BIN})

# Only for Linux
if(UNIX AND NOT APPLE)
    set(CMAKE_EXECUTABLE_SUFFIX ".out")
endif()

message(STATUS ">>> \'${CMAKE_BUILD_TYPE}\' build for \'${CMAKE_SYSTEM_NAME}\' platform with compiler \'${CMAKE_CXX_COMPILER}\', ID \'${CMAKE_CXX_COMPILER_ID}\'")
message(STATUS ">>> Build binary output folder \'${AREG_OUTPUT_BIN}\'")
message(STATUS ">>> Build library output folder \'${AREG_OUTPUT_LIB}\'")
