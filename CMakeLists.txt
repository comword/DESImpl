cmake_minimum_required(VERSION 3.5)

project(DESProj)
#set(CMAKE_BUILD_TYPE "Release")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

SET(CMAKE_MODULE_PATH
        ${CMAKE_MODULE_PATH}
        ${CMAKE_SOURCE_DIR}/CMakeModules
        )

set(CMAKE_CXX_STANDARD 14)
add_subdirectory(core)
add_subdirectory(jni)
#add_subdirectory(network)

set(OPENSSL_ROOT_DIR "/usr/local/opt/openssl/")
find_package(OpenSSL REQUIRED)
add_executable(main main.cpp)
target_include_directories(main PUBLIC ${descore_SOURCE_DIR}/include)
target_link_libraries(main descore)
target_link_libraries(main OpenSSL::Crypto)