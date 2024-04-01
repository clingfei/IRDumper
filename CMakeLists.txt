cmake_minimum_required(VERSION 3.4.3)
project(DIPass)
if (DEFINED LLVM_DIR)
    set(ENV{LLVM_DIR} "${LLVM_DIR}")
endif()
if (DEFINED ENV{LLVM_DIR})
    find_package(LLVM "13.0.0" REQUIRED CONFIG)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConcif.cmake in: ${LT_LLVM_INSTALL_DIR}")

    message("LLVM STATUS:
        Definitions ${LLVM_DEFINITIONS}
        Includes    ${LLMV_INCLUDE_DIRS}
        Libraries   ${LLVM_LIBRARY_DIRS}
        Targets     ${LLVM_TARGETS_TO_BUILD}"
    )

    include_directories(${LLVM_INCLUDE_DIRS})
    link_directories(${LLVM_LIBRARY_DIRS})
    add_definitions(${LLVM_DEFINITIONS})

    # Use the same C++ standard as LLVM does
    set(CMAKE_CXX_STANDARD 14 CACHE STRING "")

    # Build type
    if (NOT CMAKE_BUILD_TYPE)
        set(CMAKE_BUILD_TYPE Debug CACHE
            STRING "Build type (default Debug):" FORCE)
    endif()

    # LLVM is normally built without RTTI. Be consistent with that.
    if(NOT LLVM_ENABLE_RTTI)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
    endif()

    # Compiler flags
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -fdiagnostics-color=always -Wno-everything")

    include_directories("include")
    add_subdirectory("src")
else()
    message(FATAL_ERROR "\
        WARNING: The LLVM_DIR var was not set!\n\
        Please set this to environment variable to point to the LLVM build directory\
        (e.g. on linux: export LLVM_DIR=/path/to/llvm/build/dir)")
endif()
    