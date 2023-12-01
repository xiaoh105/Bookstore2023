cmake_minimum_required(VERSION 3.22)
project(Homework5_Bookstore)

set(CMAKE_CXX_STANDARD 17)

include_directories(Include)

add_executable(Homework5_Bookstore
        src/main.cpp
        Include/FileReader.h
        src/DataStructure/FileMap.cpp
        Include/FileMap.h
        Include/Utils.h
        src/Utils.cpp)