cmake_minimum_required(VERSION 4.0)
project(secondlab)

set(CMAKE_CXX_STANDARD 20)

add_executable(secondlab main.cpp
        classes/cpp/Sequence.cpp
        classes/headers/Sequence.h
        classes/cpp/ArraySequence.cpp
        classes/headers/ArraySequence.h
        classes/cpp/ListSequence.cpp
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
        classes/headers/LinkedListEnumerator.h
        classes/headers/UI.h
        classes/headers/UI.cpp
        classes/headers/BItSequence.cpp
        classes/headers/BItSequence.h
)
