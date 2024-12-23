# Set the cmake minimum version
cmake_minimum_required(VERSION 3.5)

# Define the project name
project(vm)

# Generate symbols for IDE indexer
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Set default C++ standard to C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Add compiler options for GNU/Clang
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# Set source files
set(SOURCES
    src/main.cpp
    src/Process.cpp
    src/OS/BootLoader.cpp
    src/CPU/RegFile.cpp
    src/CPU/Instruction.cpp
    src/CPU/ALU.cpp
    src/CPU/Pipeline.cpp
    src/CPU/Core.cpp
    src/RAM.cpp
    src/VirtualMachine.cpp
)

# Create the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Add all header files under the include directory to target
target_include_directories(${PROJECT_NAME} PRIVATE include)
