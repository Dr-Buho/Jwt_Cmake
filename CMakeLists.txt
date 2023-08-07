# CMakeList.txt: jwt_cmake_cpp 的 CMake 项目，在此处包括源代码并定义
# 项目特定的逻辑。
#
cmake_minimum_required (VERSION 3.8)

project (jwt)

# in windows platform
# just to see the vcpkg install with https://vcpkg.io/en/getting-started.html
# step1 need to install the vcpkg and set the path to system library.
# step2 need to set default DCOMPILE CMAKE to system path.

if(CMAKE_CL_64)
    message(STATUS "x86x64 set build x64")
    set(ADDRESS_MODEL 64)
    set(NODE_TARGET x64)
endif()

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    # set mac gcc standard
    set(CMAKE_CXX_STANDARD 11)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)
    set(CMAKE_CXX_EXTENSIONS OFF)

    # set target os version
    message(STATUS "set CMAKE_OSX_DEPLOYMENT_TARGET 10.11")
    set(CMAKE_OSX_DEPLOYMENT_TARGET "10.11" CACHE STRING "Minimum OS X deployment version" FORCE)

    # set target arch type
    message(STATUS "MACOS arm64 set build x86_64")
    set(CMAKE_OSX_ARCHITECTURES "x86_64" CACHE STRING "Build architectures for Mac OS X" FORCE)

    #set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -march=x86-64")
    #set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=x86-64")

    #format dylib name
    set(CMAKE_SHARED_LIBRARY_PREFIX "")

    set(INC_DIR ${CMAKE_CURRENT_LIST_DIR}/openssl-macos/include)
    set(LINK_DIR ${CMAKE_CURRENT_LIST_DIR}/openssl-macos/lib)

    add_library(crypto_lib STATIC IMPORTED)
    set_target_properties(crypto_lib PROPERTIES IMPORTED_LOCATION "${LINK_DIR}/libcrypto.a")

    add_library(ssl_lib STATIC IMPORTED)
    set_target_properties(ssl_lib PROPERTIES IMPORTED_LOCATION "${LINK_DIR}/libssl.a")

    include_directories(${INC_DIR})
    link_directories(${LINK_DIR})
ELSE()
    # note that the winos static library must use perl Configure VC-Win64A no-share to build it.
    # otherwise the static lib linking nothing.
    message("START SET WINOS INIT LIBRARY")
    set(INC_DIR ${CMAKE_CURRENT_LIST_DIR}/openssl-winos/include)
    set(LINK_DIR ${CMAKE_CURRENT_LIST_DIR}/openssl-winos/lib)

    add_library(crypto_lib STATIC IMPORTED)
    set_target_properties(crypto_lib PROPERTIES IMPORTED_LOCATION "${LINK_DIR}/libcrypto.lib")

    add_library(ssl_lib STATIC IMPORTED)
    set_target_properties(ssl_lib PROPERTIES IMPORTED_LOCATION "${LINK_DIR}/libssl.lib")

    include_directories(${INC_DIR})
    link_directories(${LINK_DIR})
ENDIF()

set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, options are: Debug Release RelWithDebInfo MinSizeRel." FORCE)

set(HEADER jwt_verify.h)
set(CPP jwt_verify.cpp)

add_library(jwt_verify SHARED ${HEADER} ${CPP})

IF(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    message("MACOS LINK LIB")
    target_link_libraries(jwt_verify PRIVATE crypto_lib ssl_lib)
    target_link_directories(jwt_verify PRIVATE "${CMAKE_SOURCE_DIR}/openssl-macos/include")
ELSE()
    message("WINOS LINK LIB")
    #winos need to linking other two system library ws2_32 Crypt32.
    target_link_libraries(jwt_verify PRIVATE crypto_lib ssl_lib ws2_32 Crypt32)
    target_link_directories(jwt_verify PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/openssl-winos/include")
ENDIF()

set_target_properties(jwt_verify PROPERTIES OUTPUT_NAME jwt_verify)