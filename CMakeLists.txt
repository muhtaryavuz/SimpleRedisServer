cmake_minimum_required(VERSION 3.5.0)
project(PubSubServer)

set(CMAKE_CXX_STANDARD 20)

FILE(GLOB_RECURSE HEADER_FILES include/*.hpp)
FILE(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

set(LINK_LIBRARIES
    pthread
)

target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})