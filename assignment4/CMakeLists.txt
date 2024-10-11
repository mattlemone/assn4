cmake_minimum_required(VERSION 3.12)
project(ArrayPerformance VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler options based on the compiler being used
if(MSVC)
    add_compile_options(/W4 /WX)
    set(STACK_FLAG "/STACK:10000000")
elseif(APPLE)
    add_compile_options(-Wall -Wextra -pedantic -Werror -O3)
    # On macOS, stack size is set differently
    set(STACK_FLAG "-Wl,-stack_size,0x960000")
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror -O3)
    set(STACK_FLAG "-Wl,--stack,10000000")
endif()

# Enable testing
enable_testing()

# Main executable
add_executable(ArrayPerformance
        main.cpp
        sortutils.cpp
)

# Set stack size for main executable
if(MSVC)
    set_target_properties(ArrayPerformance PROPERTIES LINK_FLAGS ${STACK_FLAG})
else()
    target_link_options(ArrayPerformance PRIVATE ${STACK_FLAG})
endif()

# Unit test executable
add_executable(UnitTestRunner
        TestPerformance.cpp
        sortutils.cpp
)

# Find GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
target_link_libraries(UnitTestRunner ${GTEST_LIBRARIES} pthread)

# Set stack size for unit test executable
if(MSVC)
    set_target_properties(UnitTestRunner PROPERTIES LINK_FLAGS ${STACK_FLAG})
else()
    target_link_options(UnitTestRunner PRIVATE ${STACK_FLAG})
endif()