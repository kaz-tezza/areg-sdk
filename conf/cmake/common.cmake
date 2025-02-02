# ###########################################################################
# Common settings for all projects
# ###########################################################################

# Identify the OS
set(AREG_OS ${CMAKE_SYSTEM_NAME})
set(AREG_PROCESSOR ${CMAKE_SYSTEM_PROCESSOR})

# Determining bitness by size of void pointer
# 8 bytes ==> 64-bits (x64) and 4 bytes ==> 32-nit (x86)
if(NOT ${CMAKE_SIZEOF_VOID_P} MATCHES "8")
    set(AREG_BITNESS "32")
endif()

# -----------------------------------------------------
# areg specific internal variable settings
# -----------------------------------------------------
# The toolchain
if (CYGWIN)
    message(STATUS ">>> The Cmake Compiler ID is ${CMAKE_CXX_COMPILER_ID}, set CYGWIN")
    set(AREG_TOOLCHAIN "CYGWIN")
else()
    set(AREG_TOOLCHAIN "${CMAKE_CXX_COMPILER_ID}")
endif()

# The development environment -- POSIX or Win32 API
set(AREG_DEVELOP_ENV)
# The linker flags
set(AREG_LDFLAGS)
# The compiler options
set(AREG_COMPILER_OPTIONS)

# Adding common definition
add_definitions(-DUNICODE -D_UNICODE)
remove_definitions(-D_MBCS -DMBCS)

if(CMAKE_BUILD_TYPE MATCHES Release)
    add_definitions(-DNDEBUG)
    remove_definitions(-DDEBUG -D_DEBUG)
else()
    add_definitions(-DDEBUG)
    remove_definitions(-DNDEBUG -D_NDEBUG)
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

    if(AREG_BITNESS MATCHES "32")
        list(APPEND AREG_COMPILER_OPTIONS -m32)
    else()
        list(APPEND AREG_COMPILER_OPTIONS -m64)
    endif()

    # Clang compile options
    list(APPEND AREG_COMPILER_OPTIONS -pthread -Wall -c -fmessage-length=0 -stdlib=libstdc++ ${AREG_USER_DEFINES})
    # Linker flags (-l is not necessary)
    list(APPEND AREG_LDFLAGS c++ m ncurses pthread rt ${AREG_USER_DEF_LIBS})

elseif (CMAKE_COMPILER_IS_GNUCXX )

    # POSIX API
    add_definitions(-DPOSIX)
    set(AREG_DEVELOP_ENV "Posix")

    if (CMAKE_BUILD_TYPE MATCHES "Release")
        list(APPEND AREG_COMPILER_OPTIONS -O2)
    else()
        list(APPEND AREG_COMPILER_OPTIONS -O0 -g3)
    endif()

    if(AREG_BITNESS MATCHES "32")
        list(APPEND AREG_COMPILER_OPTIONS -m32)
    else()
        list(APPEND AREG_COMPILER_OPTIONS -m64)
    endif()

    # GNU compile options
    if (CYGWIN)
        list(APPEND AREG_COMPILER_OPTIONS -pthread -Wall -c -fmessage-length=0 -MMD -std=gnu++17 ${AREG_USER_DEFINES})
    else()
        list(APPEND AREG_COMPILER_OPTIONS -pthread -Wall -c -fmessage-length=0 -MMD -std=c++17 ${AREG_USER_DEFINES})
    endif()
    # Linker flags (-l is not necessary)
    list(APPEND AREG_LDFLAGS stdc++ m ncurses pthread rt ${AREG_USER_DEF_LIBS})

elseif (MSVC)

    # Visual Studio C++
    # Windows / Win32 API
    set(AREG_DEVELOP_ENV "Win32")
    add_definitions(-DWINDOWS -D_WINDOWS -DWIN32 -D_WIN32)

    if(NOT CMAKE_BUILD_TYPE MATCHES "Release")
        list(APPEND AREG_COMPILER_OPTIONS -Od -RTC1 -c)
    endif()
        
    if (${AREG_BITNESS} MATCHES "64")
        add_definitions(-DWIN64 -D_WIN64)
    endif()

    # MS Visual C++ compile options
    list(APPEND AREG_COMPILER_OPTIONS)
    # Linker flags (-l is not necessary)
    list(APPEND AREG_LDFLAGS advapi32 psapi shell32 ws2_32 ${AREG_USER_DEF_LIBS})

else()

    message(WARNING ">>> Unsupported compiler type. The result is unpredictable, by default use GNU compiler settings and POSIX API")
    add_definitions(-DPOSIX)
    # POSIX API
    add_definitions(-DPOSIX)
    set(AREG_DEVELOP_ENV "Posix")

    if (CMAKE_BUILD_TYPE MATCHES "Release")
        list(APPEND AREG_COMPILER_OPTIONS -O2)
    else()
        list(APPEND AREG_COMPILER_OPTIONS -O0 -g3)
    endif()

    if(AREG_BITNESS MATCHES "32")
        list(APPEND AREG_COMPILER_OPTIONS -m32)
    else()
        list(APPEND AREG_COMPILER_OPTIONS -m64)
    endif()

    # Compile options
    list(APPEND AREG_COMPILER_OPTIONS -pthread -Wall -c -fmessage-length=0 -MMD -std=c++17 ${AREG_USER_DEFINES})
    # Linker flags (-l is not necessary)
    list(APPEND AREG_LDFLAGS stdc++ m ncurses pthread rt "${AREG_USER_DEF_LIBS}")

endif()

# -------------------------------------------------------
# Setup product paths
# -------------------------------------------------------

# Relative path of the output folder for the builds
set(AREG_PRODUCT_PATH)
if (NOT "${AREG_TOOLCHAIN}" STREQUAL "${CMAKE_CXX_COMPILER_ID}")
    set(AREG_PRODUCT_PATH "${AREG_USER_DEF_OUTPUT_DIR}/build/${AREG_TOOLCHAIN}-${CMAKE_CXX_COMPILER_ID}/${AREG_OS}-${AREG_BITNESS}-${AREG_PROCESSOR}-${CMAKE_BUILD_TYPE}")
else()
    set(AREG_PRODUCT_PATH "${AREG_USER_DEF_OUTPUT_DIR}/build/${AREG_TOOLCHAIN}/${AREG_OS}-${AREG_BITNESS}-${AREG_PROCESSOR}-${CMAKE_BUILD_TYPE}")
endif()
string(TOLOWER "${AREG_PRODUCT_PATH}" AREG_PRODUCT_PATH)
# The absolute path for builds
if (NOT DEFINED AREG_OUTPUT_DIR OR AREG_OUTPUT_DIR STREQUAL "")
    set(AREG_OUTPUT_DIR "${AREG_BUILD_ROOT}/${AREG_PRODUCT_PATH}")
endif()

# The absolute path for generated files
set(AREG_GENERATE_DIR "${AREG_BUILD_ROOT}/${AREG_USER_DEF_OUTPUT_DIR}/generate")
# The absolute path for obj files.
set(AREG_OUTPUT_OBJ "${AREG_OUTPUT_DIR}/obj")
# The absolute path for static libraries
set(AREG_OUTPUT_LIB "${AREG_OUTPUT_DIR}/lib")
# The absolute path for all executables and shared libraries
set(AREG_OUTPUT_BIN "${AREG_OUTPUT_DIR}/bin")

# Project inclues
set(AREG_INCLUDES "-I${AREG_BASE} -I${AREG_GENERATE_DIR} -I${AREG_USER_DEF_INCLUDES}")

# Setting output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${AREG_OUTPUT_BIN})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${AREG_OUTPUT_BIN})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${AREG_OUTPUT_LIB})

# Make sure that the output directories are set for each configuration.
foreach( OUTPUTCONFIG ${CMAKE_CONFIGURATION_TYPES} )
    string( TOUPPER ${OUTPUTCONFIG} AREG_OUTPUTCONFIG )
    set( CMAKE_RUNTIME_OUTPUT_DIRECTORY_${AREG_OUTPUTCONFIG} ${AREG_OUTPUT_BIN} )
    set( CMAKE_LIBRARY_OUTPUT_DIRECTORY_${AREG_OUTPUTCONFIG} ${AREG_OUTPUT_BIN} )
    set( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_${AREG_OUTPUTCONFIG} ${AREG_OUTPUT_LIB} )
endforeach( OUTPUTCONFIG CMAKE_CONFIGURATION_TYPES )

# Adding compile options
add_compile_options(${AREG_COMPILER_OPTIONS})

# Adding areg/product directory for clean-up
set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_CLEAN_FILES ${AREG_OUTPUT_DIR})

# Adding library search paths
link_directories(BEFORE "${AREG_OUTPUT_BIN} ${AREG_OUTPUT_LIB} ${AREG_USER_DEF_LIB_PATHS}")

# Only for Linux
if(UNIX AND NOT CYGWIN)
    set(CMAKE_EXECUTABLE_SUFFIX ".out")
endif()

message(STATUS "-------------------- Status Report Begin --------------------")
message(STATUS ">>> Build executables are with extension \'${CMAKE_EXECUTABLE_SUFFIX}\'")
message(STATUS ">>> Build for \'${CMAKE_SYSTEM_NAME}\' ${AREG_BITNESS}-bit platform ${AREG_PROCESSOR} with compiler \'${CMAKE_CXX_COMPILER}\', ID \'${AREG_TOOLCHAIN}\', and build type \'${CMAKE_BUILD_TYPE}\'")
message(STATUS ">>> Binary output folder \'${AREG_OUTPUT_BIN}\'")
message(STATUS ">>> Library output folder \'${AREG_OUTPUT_LIB}\'")
message(STATUS ">>> Build examples is '${AREG_BUILD_EXAMPLES}\', build tests is \'${AREG_BUILD_TESTS}\'")
message(STATUS "-------------------- Status Report End ----------------------")
