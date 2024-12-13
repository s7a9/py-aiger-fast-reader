cmake_minimum_required(VERSION 3.10)
project(aiger_fast_reader)

# Project-local pybind11 directory
set(LOCAL_PYBIND11_DIR "${CMAKE_CURRENT_SOURCE_DIR}/pybind11")

# Check if local pybind11 exists
if(EXISTS "${LOCAL_PYBIND11_DIR}/CMakeLists.txt")
    # Use local pybind11
    add_subdirectory(${LOCAL_PYBIND11_DIR})
    message(STATUS "Using local pybind11 from ${LOCAL_PYBIND11_DIR}")
else()
    # Fallback to system-wide pybind11
    find_package(pybind11 REQUIRED)
    message(STATUS "Using system-wide pybind11")
endif()

# Create your module
pybind11_add_module(aiger_fast_reader 
    src/aiger_fast_reader.cpp
)

# Link against pybind11
target_link_libraries(aiger_fast_reader PRIVATE pybind11::module)