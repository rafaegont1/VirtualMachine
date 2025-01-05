# Set the cmake minimum version
cmake_minimum_required(VERSION 3.5)

# Define the project name
project(minix)

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
    src/HW/ISA/Code.cpp
    src/HW/ISA/Encoding.cpp
    src/HW/RAM/Allocator.cpp
    src/HW/RAM/DataSpace.cpp
    src/HW/CPU/RegFile.cpp
    src/HW/CPU/ALU.cpp
    src/HW/CPU/Pipeline.cpp
    src/HW/CPU/Core.cpp
    src/main.cpp
    src/OS/FCFS.cpp
    src/OS/Minix.cpp
    src/OS/PCB.cpp
)

# Create the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

# Add all header files under the include directory to target
target_include_directories(${PROJECT_NAME} PRIVATE include)
