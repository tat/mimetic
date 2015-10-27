#!/bin/bash

ANDROID_NDK_ROOT=$1
CMAKEPARAMS="-DCMAKE_BUILD_TYPE=Release -DANDROID_NDK=${ANDROID_NDK_ROOT} -DANDROID_NATIVE_API_LEVEL=android-15 -DANDROID_STL_FORCE_FEATURES=ON -DCMAKE_TOOLCHAIN_FILE=../../cmake/Toolchains/android.toolchain.cmake"

mkdir buildandroid
cd buildandroid

mkdir armeabi-v7a
cd armeabi-v7a
cmake ${CMAKEPARAMS} -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.9 -DANDROID_ABI=armeabi-v7a ../../
make
cd ..

mkdir arm64-v8a
cd arm64-v8a
cmake ${CMAKEPARAMS} -DANDROID_TOOLCHAIN_NAME=aarch64-linux-android-4.9 -DANDROID_ABI=arm64-v8a ../../
make
cd ..

mkdir mips
cd mips
cmake ${CMAKEPARAMS} -DANDROID_TOOLCHAIN_NAME=mipsel-linux-android-4.9 -DANDROID_ABI=mips ../../
make
cd ..

mkdir mips64
cd mips64
cmake ${CMAKEPARAMS} -DANDROID_TOOLCHAIN_NAME=mips64el-linux-android-4.9 -DANDROID_ABI=mips64 ../../
make
cd ..

mkdir x86
cd x86
cmake ${CMAKEPARAMS} -DANDROID_TOOLCHAIN_NAME=x86-4.9 -DANDROID_ABI=x86 ../../
make
cd ..

mkdir x86_64
cd x86_64
cmake ${CMAKEPARAMS} -DANDROID_TOOLCHAIN_NAME=x86_64-4.9 -DANDROID_ABI=x86_64 ../../
make
cd ..
