cmake_minimum_required(VERSION 4.0)
project(secondlab)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(googletest)

add_executable(secondlab main.cpp
        classes/secondlab/headers/Sequence.h
        classes/secondlab/headers/ArraySequence.h
        classes/secondlab/headers/ListSequence.h
        classes/secondlab/headers/DynamicArray.h
        classes/secondlab/headers/LinkedList.h
        classes/secondlab/headers/MutableArraySequence.h
        classes/secondlab/headers/ImmutableArraySequence.h
        classes/secondlab/headers/MutableListSequence.h
        classes/secondlab/headers/MutableListSequence.h
        classes/secondlab/headers/Option.h
        classes/secondlab/headers/IEnumerator.h
        classes/secondlab/headers/MutableArraySequence.h
        classes/secondlab/headers/ImmutableListSequence.h
        classes/secondlab/headers/UI.h
        classes/secondlab/cpp/UI.cpp
        classes/secondlab/cpp/BItSequence.cpp
        classes/secondlab/headers/BItSequence.h

)
add_executable(tests_second_lab
        "tests/tests secondlab/testsDynamicArray.cpp"
        "tests/tests secondlab/testsLinkedList.cpp"
        "tests/tests secondlab/testsSequence.cpp"
)


target_link_libraries(tests_second_lab PRIVATE GTest::gtest_main)
