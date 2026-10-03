cmake_minimum_required(VERSION 3.10)
project(AAF_Practicum)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(db_engine 
    src/main.cpp 
    src/cli.cpp 
    src/parser.cpp
)