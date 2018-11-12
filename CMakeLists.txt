cmake_minimum_required (VERSION 3.5)
project (utf_utils)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_program(CLANG_TIDY NAMES clang-tidy)
function(XLANG_TARGET_GUIDELINE_CHECKS target)
    if(CLANG_TIDY)
        set_property(
            TARGET ${target}
            PROPERTY CXX_CLANG_TIDY ${CLANG_TIDY} "-checks=cppcoreguidelines-*")
    endif()
endfunction(XLANG_TARGET_GUIDELINE_CHECKS)

include_directories(src)

set(Sources
    src/utf_utils.cpp
    src/utf_utils.h

    test/av_utf8.c
    test/av_utf8.h
    test/boost_utf8.hpp
    test/boost_utf8_config.hpp
    test/hoehrmann.cpp
    test/hoehrmann.h
    test/llvm_convert_utf.c
    test/llvm_convert_utf.h
    test/test_basics.cpp
    test/test_conversions_16.cpp
    test/test_conversions_32.cpp
    test/test_main.cpp
    test/test_main.h
    test/phoyd.h
)

include_directories(test)
add_executable(utf_utils_test ${Sources})
xlang_target_guideline_checks(utf_utils_test)

set(CMAKE_VERBOSE_MAKEFILE 1)
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")

    set(CMAKE_C_FLAGS_DEBUG   "-std=gnu++17 -g -Wall -pedantic -Wextra")
    set(CMAKE_CXX_FLAGS_DEBUG "-std=gnu++17 -g -Wall -pedantic -Wextra -Wno-unused-parameter -Wno-c++17-extensions")

    set(CMAKE_C_FLAGS_RELEASE   "-std=gnu++17 -O3 -march=native -Wall -pedantic -Wextra")
    set(CMAKE_CXX_FLAGS_RELEASE "-std=gnu++17 -O3 -march=native -stdlib=libc++ -Wall -pedantic -Wextra -Wno-unused-parameter  -Wno-c++17-extensions")

elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")

    set(CMAKE_C_FLAGS_DEBUG   "-std=gnu++17 -g -Wall -pedantic")
    set(CMAKE_CXX_FLAGS_DEBUG "-std=gnu++17 -g  -Wall -pedantic -Wextra")

    set(CMAKE_C_FLAGS_RELEASE   "-std=gnu++17 -O3 -march=native -Wall -pedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "-std=gnu++17 -O3 -march=native -Wall -pedantic -Wextra")

endif()


