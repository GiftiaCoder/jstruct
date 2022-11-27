cmake_minimum_required(VERSION 3.22)

project(Anya CXX)

include_directories(${PROJECT_SOURCE_DIR}/parallel-hashmap)
include_directories(${PROJECT_SOURCE_DIR}/jsoncpp/include)
include_directories(${PROJECT_SOURCE_DIR}/include)

link_directories(${PROJECT_SOURCE_DIR}/jsoncpp/build/lib)
set(JSONCPPLIB jsoncpp.a)

set(OPTI_FLAGS "-O2 -ggdb3 -g")
set(CXX_FLAGS "-Werror -Wall -Wextra -Wno-long-long -fexceptions -std=c++17 -pipe -fPIC")
set(CMAKE_CXX_FLAGS "${OPTI_FLAGS} ${CXX_FLAGS}")

enable_testing()
add_subdirectory(test)
