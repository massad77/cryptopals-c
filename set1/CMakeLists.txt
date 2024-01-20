cmake_minimum_required(VERSION 3.14)
project (cryptopals)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif()

# GoogleTest requires at least C++14
set(CMAKE_CXX_STANDARD 14)
add_compile_options(-Wall -Wextra -Wpedantic)

# Add googletest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/3d787f5a0d58cfc37a0563bb15647a0d8aa2c1bf.zip
  DOWNLOAD_EXTRACT_TIMESTAMP true
)

# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(base64_test base64_conversion_test.cpp base64_conversion.c)
target_link_libraries(base64_test GTest::gtest_main)

add_executable(fixed_xor_test base64_conversion.c fixed_xor_test.cpp fixed_xor.c)
target_link_libraries(fixed_xor_test GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(base64_test)
gtest_discover_tests(fixed_xor_test)

add_executable (set1 main.c base64_conversion.c fixed_xor.c)
