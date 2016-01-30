##########################################################################
## CMakeLists.txt
## CMake is an open-source, cross-platform tool designed to build,
## test and package software. CMake works with a native build system
## to generate compilation files and executables independently of the compiler.
##  - https://cmake.org/
##  - https://cmake.org/cmake/help/latest/guide/tutorial/index.html
##
#
# STEP 1: Configuration with CMake
# Linux/MacOS: cmake -S . -B build -G Ninja
# Windows: cmake -S . -B build -G Ninja -DCMAKE_C_COMPILER="C:/Program Files/LLVM/bin/clang.exe" -DCMAKE_CXX_COMPILER="C:/Program Files/LLVM/bin/clang++.exe"
#
# STEP 2: Compilation
# Compilation with CMake (Command Line). Once the project has been configured correctly,
# Linux/MacOS/Windows: cmake --build build
#
# Step 3: Execution
# Windows: .\build\tldcheck.exe
# MacOS or Linux: ./build/tldcheck
#
# Steps 1,2,3: Alternative with VSCode
# Here we do everything from VSCode, configure CMake and Compile
# Open VSCode > "CMake: Configure" (within VSCode command (Ctrl/Cmd + Shift + P)).
# Select Clang as compiler and make sure Ninja is selected as preferred Generator
# It will configure or generate automatically.
# Alternatively you can enter command > Tasks: Run Tasks and select the one you're interested in.
# You can also press Ctrl|Cmd + Shift + B to build.
#
# RUN:
# ./build/tldcheck.exe
#

##########################################################################

cmake_minimum_required(VERSION 3.10)

# Project name
project(TLDCheck LANGUAGES CXX)

# Set the C++ version to use
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS OFF)  # Ensure it doesn't use non-standard extensions
# Only enable modules on non-macOS systems to avoid conflicts
if (NOT CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fmodules -fcxx-modules")
endif()
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Detect the operating system and configure the compiler to use
# the specific version of Clang 17 that should be installed on the system
#
# VERY IMPORTANT:
# 1) Make sure that the Clang 17 compiler is installed on the system.
# Otherwise, the configuration process will fail.
#
# 2) Make sure that the Clang 17 compiler is in the PATH
#
# 3) Make sure that Clang 17 is selected in the VSCode CMake extension,
# this is done from the CMake icon in the left sidebar, selecting the
# "Select a Kit" option and choosing "Clang 17 ..."
#
if (CMAKE_SYSTEM_NAME STREQUAL "Darwin")  # macOS
    message(STATUS "Configuring for MacOS")
    set(CMAKE_C_COMPILER "/opt/homebrew/opt/llvm@17/bin/clang")
    set(CMAKE_CXX_COMPILER "/opt/homebrew/opt/llvm@17/bin/clang++")
    set(LLVM_PATH "/opt/homebrew/opt/llvm@17")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I${LLVM_PATH}/include")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I${LLVM_PATH}/include")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${LLVM_PATH}/lib")
    # Avoid Warning: reexported library with install name
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-reexport_library,/opt/homebrew/opt/llvm@17/lib/libunwind.1.dylib")

elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")  # Linux
    message(STATUS "Configuring for Linux")
    set(CMAKE_C_COMPILER "/usr/bin/clang-17")
    set(CMAKE_CXX_COMPILER "/usr/bin/clang++-17")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -I/usr/include/llvm-17")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/usr/include/llvm-17")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L/usr/lib/llvm-17/lib")

    # Force the compiler on Linux
    if (NOT DEFINED CMAKE_CXX_COMPILER_INITIALIZED_TO_DEFAULT)
        set(CMAKE_CXX_COMPILER "/usr/bin/clang++-17" CACHE FILEPATH "C++ compiler" FORCE)
    endif()
    if (NOT DEFINED CMAKE_C_COMPILER_INITIALIZED_TO_DEFAULT)
        set(CMAKE_C_COMPILER "/usr/bin/clang-17" CACHE FILEPATH "C compiler" FORCE)
    endif()

elseif(CMAKE_SYSTEM_NAME STREQUAL "Windows")  # Windows
    message(STATUS "Configuring for Windows")
    set(CMAKE_C_COMPILER "C:\\Program Files\\LLVM\\bin\\clang.exe")
    set(CMAKE_CXX_COMPILER "C:\\Program Files\\LLVM\\bin\\clang++.exe")
    set(LLVM_PATH "C:\\Program Files\\LLVM")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} \"-I${LLVM_PATH}\\include\"")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} \"-I${LLVM_PATH}\\include\"")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} \"-L${LLVM_PATH}\\lib\"")
else()
    message(FATAL_ERROR "Unsupported operating system: ${CMAKE_SYSTEM_NAME}")
endif()

# Add the main executable
add_executable(tldcheck
    src/main.cpp
    src/uri.cpp
)

# Install the executable according to the platform
#if (CMAKE_SYSTEM_NAME STREQUAL "Windows")
#    install(TARGETS tldcheck DESTINATION "C:/usr/local/bin")
#elseif(CMAKE_SYSTEM_NAME STREQUAL "Linux" OR CMAKE_SYSTEM_NAME STREQUAL "Darwin")
#    install(TARGETS tldcheck DESTINATION "/usr/local/bin")
#endif()
