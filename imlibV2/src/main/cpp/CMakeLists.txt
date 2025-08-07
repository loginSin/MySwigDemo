# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html.
# For more examples on how to use CMake, see https://github.com/android/ndk-samples.

# Sets the minimum CMake version required for this project.
# 1. 设置 cmake 最低版本
cmake_minimum_required(VERSION 3.10.2)

# Declares the project name. The project name can be accessed via ${ PROJECT_NAME},
# Since this is the top level CMakeLists.txt, the project name is also accessible
# with ${CMAKE_PROJECT_NAME} (both CMake variables are in-sync within the top level
# build script scope).
project("imlibv2")

# 2. 设置 C++ 版本
set(CMAKE_CXX_STANDARD 17)

# 指定头文件
include_directories(
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/../
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/includes
)

# 指定 .a 库路径
link_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/libs/${ANDROID_ABI}
)

#if(DEFINED PACKAGE_FIND_FILE)
#    include(${PACKAGE_FIND_FILE})
#endif()

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.
#
# In this top level CMakeLists.txt, ${CMAKE_PROJECT_NAME} is used to define
# the target library name; in the sub-module's CMakeLists.txt, ${PROJECT_NAME}
# is preferred for the same purpose.
#
# In order to load a library into your app from Java/Kotlin, you must call
# System.loadLibrary() and pass the name of the library defined here;
# for GameActivity/NativeActivity derived applications, the same library name must be
# used in the AndroidManifest.xml file.
file(GLOB SOURCES "*.cpp" "*/*.cpp" "*/*/*.cpp")
add_library(${CMAKE_PROJECT_NAME} SHARED
        # List C/C++ source files with relative paths to this CMakeLists.txt.
        ${SOURCES}
)

# Specifies libraries CMake should link to your target library. You
# can link libraries from various origins, such as libraries defined in this
# build script, prebuilt third-party libraries, or Android system libraries.
target_link_libraries(${CMAKE_PROJECT_NAME}
        # List libraries link to the target library
        android
        log
        z
        crypto
        rust_universal_imsdk
)