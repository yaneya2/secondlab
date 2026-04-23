cmake_minimum_required(VERSION 4.0)
project(secondlab)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(googletest)

add_executable(secondlab main.cpp
        classes/headers/Sequence.h
        classes/headers/ArraySequence.h
        classes/headers/ListSequence.h
        classes/headers/DynamicArray.h
        classes/headers/LinkedList.h
        classes/headers/MutableArraySequence.h
        classes/headers/ImmutableArraySequence.h
        classes/headers/MutableListSequence.h
        classes/headers/MutableListSequence.h
        classes/headers/Option.h
        classes/headers/IEnumerator.h
        classes/headers/MutableArraySequence.h
        classes/headers/ImmutableListSequence.h
        classes/headers/UI.h
        classes/cpp/UI.cpp
        classes/cpp/BItSequence.cpp
        classes/headers/BItSequence.h
        classes/headers/Segment.h
        classes/headers/DoublyLinkedList.h
        classes/headers/SegmentedDeque.h
        classes/headers/Student.h
        classes/headers/Teacher.h

)
add_executable(tests
        tests/testsDynamicArray.cpp
        tests/testsLinkedList.cpp
        tests/testsSequence.cpp
)

target_link_libraries(tests PRIVATE GTest::gtest_main)
