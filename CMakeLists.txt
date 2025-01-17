# Set the minimum required CMake version
cmake_minimum_required(VERSION 3.5)

# Define the project name and version
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

# Define PREEMPTIVE as true or false
option(PREEMPTIVE "Enable preemptive behavior" ON)

# Pass the PREEMPTIVE definition to the compiler
if(PREEMPTIVE)
    add_compile_definitions(PREEMPTIVE=1)
else()
    add_compile_definitions(PREEMPTIVE=0)
endif()

# Define the scheduler type
set(SCHEDULER "FIFO" CACHE STRING "Choose scheduler type (FIFO or PRIORITY_QUEUE)")

# Validate the scheduler type
if(NOT SCHEDULER MATCHES "^(FIFO|PRIORITY_QUEUE)$")
    message(FATAL_ERROR "Invalid SCHEDULER type. Choose FIFO or PRIORITY_QUEUE.")
endif()

# Pass the scheduler type definition to the compiler
add_compile_definitions(${SCHEDULER})

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
    src/OS/Scheduler/FIFOScheduler.cpp
    src/OS/Scheduler/PriorityQueue.cpp
    src/OS/Minix.cpp
    src/OS/PCB.cpp
)

# Create the executable target
add_executable(${PROJECT_NAME} ${SOURCES})

target_link_libraries(${PROJECT_NAME} pthread)

# Add include directories
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Print configuration information
message(STATUS "Building project: ${PROJECT_NAME}")
message(STATUS "Scheduler type: ${SCHEDULER}")
message(STATUS "Preemptive scheduling: ${PREEMPTIVE}")
