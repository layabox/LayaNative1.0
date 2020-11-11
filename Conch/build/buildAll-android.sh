#!/bin/sh
cd common/proj.android/jni/
ndk-build
cd ../../../

cd render/proj.android/jni/
ndk-build
cd ../../../

cp common/proj.android/obj/local/armeabi-v7a/libcommon.a ../libs/android-armv7/
cp render/proj.android/obj/local/armeabi-v7a/librender.a ../libs/android-armv7/

cp common/proj.android/obj/local/arm64-v8a/libcommon.a ../libs/android-arm64/
cp render/proj.android/obj/local/arm64-v8a/librender.a ../libs/android-arm64/

cp common/proj.android/obj/local/x86/libcommon.a ../libs/android-x86/
cp render/proj.android/obj/local/x86/librender.a ../libs/android-x86/

cp common/proj.android/obj/local/armeabi/libcommon.a ../libs/android-arm/
cp render/proj.android/obj/local/armeabi/librender.a ../libs/android-arm/

touch ../source/conch/JCConch.cpp
cd conch/proj.android/jni/
ndk-build
cd ../../../
