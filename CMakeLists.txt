cmake_minimum_required(VERSION 3.19)
project(BigInt)

set(CMAKE_CXX_STANDARD 14)

add_executable(BigInt src/BigInt.cpp test/main.cpp)

include_directories(src lib)

target_link_libraries(BigInt gmp gmpxx)