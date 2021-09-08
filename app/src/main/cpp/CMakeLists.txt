# For more information about using CMake with Android Studio, read the
# documentation: https://d.android.com/studio/projects/add-native-code.html

# Sets the minimum version of CMake required to build the native library.

cmake_minimum_required(VERSION 3.10.2)

# Declares and names the project.

project("illegal_racer")

# Creates and names a library, sets it as either STATIC
# or SHARED, and provides the relative paths to its source code.
# You can define multiple libraries, and CMake builds them for you.
# Gradle automatically packages shared libraries with your APK.

add_library( # Sets the name of the library.
        native-lib

        # Sets the library as a shared library.
        SHARED

        # Provides a relative path to your source file(s).
        graphics/render/Renderer.cpp
        graphics/Shader.cpp
        graphics/render/MyRenderer.cpp
        graphics/drawable/Triangle.cpp
        graphics/drawable/Rectangle.cpp
        graphics/Texture.cpp
        graphics/loaders/Loaders.cpp
        controls/Camera.cpp
        controls/Touches.cpp
        native-lib.cpp)

find_library(log-lib log)

add_subdirectory(modules/libpng-android)
find_library(png libpng)

# Specifies libraries CMake should link to your target library. You
# can link multiple libraries, such as libraries you define in this
# build script, prebuilt third-party libraries, or system libraries.

target_link_libraries(
        # Specifies the target library.
        native-lib

        ${log-lib}
        android
        EGL
        GLESv3
        png #  These two guys are for manage png files. Png module requires zlib, so
        z   # it is linked below
)