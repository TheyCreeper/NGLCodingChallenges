cmake_minimum_required(VERSION 3.14)
project(BestSineMethodEver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Python
find_package(Python3 COMPONENTS Interpreter Development REQUIRED)

# Add pybind11 from source (recommended for CLion)
include(FetchContent)
FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11.git
        GIT_TAG v2.11.1
)
FetchContent_MakeAvailable(pybind11)

# Create executable
add_executable(BestSineMethodEver main.cpp)

# Link pybind11
target_link_libraries(BestSineMethodEver PRIVATE pybind11::embed)

# Set Python path so executable can find math_utils.py
target_compile_definitions(BestSineMethodEver PRIVATE
        PYTHONPATH="$<TARGET_FILE_DIR:BestSineMethodEver>"
)