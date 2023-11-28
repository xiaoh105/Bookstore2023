cmake_minimum_required(VERSION 3.22)
project(Homework5_Bookstore)

set(CMAKE_CXX_STANDARD 17)

add_executable(Homework5_Bookstore src/main.cpp
        src/DataStructure/FileReader.cpp
        src/DataStructure/FileReader.h)
