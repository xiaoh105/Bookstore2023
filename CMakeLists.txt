cmake_minimum_required(VERSION 3.22)
project(Homework5_Bookstore)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Ofast")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -funroll-loops")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-use")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")

include_directories(include)

add_executable(code
        src/backend/main.cpp
        src/backend/Utils.cpp
        src/backend/InstructionValidator.cpp
        src/backend/BookSystem.cpp
        src/backend/UserSystem.cpp
        src/backend/Log.cpp
        src/backend/Executor.cpp
        src/backend/FileMap.cpp)