project (cryptopals)

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING
      "Choose the type of build, options are: None Debug Release RelWithDebInfo MinSizeRel."
      FORCE)
endif()

add_executable (set1 main.c base64_conversion.c)
add_executable (test_base64_conversion test_base64_conversion.c base64_conversion.c)
