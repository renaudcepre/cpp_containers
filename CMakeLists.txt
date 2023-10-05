cmake_minimum_required(VERSION 3.17)
project(cpp_containers_tests)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
cmake_policy(SET CMP0135 NEW)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
        main
        tests/test_vector_func.cpp
        tests/test_vector_cop.cpp
        includes/Array.hpp
        includes/List.h
        includes/ListIterator.h
        includes/ReverseIterator.h
        includes/Utility.h
        includes/Vector.h
        includes/VectorIterator.h
        includes/Allocator.h
)
target_link_libraries(
        main
        gtest_main
)

include(GoogleTest)
gtest_discover_tests(main)

