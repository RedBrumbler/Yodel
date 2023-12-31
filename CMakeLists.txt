cmake_minimum_required(VERSION 3.22)

option(QUEST "Build for quest" ON)
option(WINDOWS "Build for windows" OFF)

if (QUEST)
    include (${CMAKE_CURRENT_LIST_DIR}/cmake/quest.cmake)
endif()

if (WINDOWS)
    include (${CMAKE_CURRENT_LIST_DIR}/cmake/windows.cmake)
    set(PACKAGE_VERSION "0.1.0")
    set(SHARED_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shared)
    set(EXTERN_DIR ${CMAKE_CURRENT_SOURCE_DIR}/shared)
endif()

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

project(yodel VERSION ${PACKAGE_VERSION})

set(SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(
    ${CMAKE_PROJECT_NAME}
    SHARED
    ${SOURCE_DIR}/main.cpp
    ${SOURCE_DIR}/loader.cpp
    ${SOURCE_DIR}/modloader.cpp
)

target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${INCLUDE_DIR})
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${SHARED_DIR})

add_compile_definitions(MOD_ID="${CMAKE_PROJECT_NAME}")
add_compile_definitions(VERSION="${CMAKE_PROJECT_VERSION}")
add_compile_options(-fPIE -flto -fPIC -fno-rtti -fno-exceptions -O3 -Oz -fvisibility=hidden -Wall -Wextra -Wpedantic -Werror)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/git.cmake)

if (QUEST)
    add_compile_definitions(QUEST)
    add_compile_definitions(FLAMINGO_HEADER_ONLY)
    add_compile_definitions(FMT_HEADER_ONLY)
    target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE ${EXTERN_DIR}/includes)
    target_link_libraries(${CMAKE_PROJECT_NAME} log ${EXTERN_DIR}/libs/libcapstone.a)
    target_include_directories(${CMAKE_PROJECT_NAME} SYSTEM PRIVATE ${EXTERN_DIR}/includes/fmt/fmt/include)
endif()

if (WINDOWS)
    add_compile_definitions(WINDOWS)
endif()
