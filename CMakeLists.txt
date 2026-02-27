cmake_minimum_required(VERSION 3.10)
project(HighPerformanceOrderCache VERSION 1.0 LANGUAGES CXX)

# Use C++17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add compiler optimizations for Release builds
if(CMAKE_BUILD_TYPE STREQUAL "Release")
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -march=native")
  elseif(MSVC)
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2")
  endif()
endif()

# Find Google Test
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})
find_package(Threads REQUIRED)

# Include project headers
include_directories(include)

# Source files
set(SOURCES
    src/OrderCache.cpp
    src/MatchingEngine.cpp
    tests/OrderCacheTests.cpp
)

# Build test executable
add_executable(HighPerformanceOrderCache ${SOURCES})

# Link against Google Test and pthread
target_link_libraries(HighPerformanceOrderCache
    ${GTEST_BOTH_LIBRARIES}
    Threads::Threads
)

# Enable testing
enable_testing()
add_test(NAME HighPerformanceOrderCache COMMAND HighPerformanceOrderCacheTests)