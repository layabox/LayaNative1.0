#!/bin/sh

function buildStaticLib()
{
	PROJECT_PATH=$1
	SCHEME=$2
	CONFIGURATION=$3
	TARGET=$4
	EXPORT_PATH=$5

	echo “********************${TARGET}****************************”
	echo “********************clean****************************”
	xcodebuild clean  -quiet -project ${PROJECT_PATH}  -configuration ${CONFIGURATION} -alltargets #-target ${TARGET} -scheme ${SCHEME}
	echo “********************build****************************”
	rm -rf ./build
	mkdir ./build
	xcodebuild ONLY_ACTIVE_ARCH=NO -quiet -project ${PROJECT_PATH}  -configuration ${CONFIGURATION}   -target ${TARGET} -sdk iphoneos  BUILD_DIR="${EXPORT_PATH=$(pwd)/build}"
	xcodebuild  -quiet -project ${PROJECT_PATH}  -configuration ${CONFIGURATION}   -target ${TARGET} -sdk iphonesimulator  BUILD_DIR="${EXPORT_PATH=$(pwd)/build}"
	#xcodebuild  -quiet -project ${PROJECT_PATH}  -configuration ${CONFIGURATION}   -target ${TARGET} -sdk iphonesimulator -arch i386 x86_64 BUILD_DIR="${EXPORT_PATH=$(pwd)/build}"
}

buildStaticLib ../Conch/build/conchRuntime/proj.ios/conchRuntime.xcodeproj conchRuntime Release conchRuntime $(pwd)/conchRuntimeBuild
cp -rf $(pwd)/conchRuntimeBuild/Release-iphoneos/libconchRuntime.a ../Conch/libs/ios/libconchRuntime.a
cp -rf $(pwd)/conchRuntimeBuild/Release-iphonesimulator/libconchRuntime.a ../Conch/libs/ios-sim/libconchRuntime.a
rm -rf $(pwd)/conchRuntimeBuild

buildStaticLib ../Conch/build/common/proj.ios/common.xcodeproj common Release common $(pwd)/commonBuild
cp -rf $(pwd)/commonBuild/Release-iphoneos/libcommon.a ../Conch/libs/ios/libcommon.a
cp -rf $(pwd)/commonBuild/Release-iphonesimulator/libcommon.a ../Conch/libs/ios-sim/libcommon.a
rm -rf $(pwd)/commonBuild

buildStaticLib ../Conch/build/render/proj.ios/render.xcodeproj render Release render $(pwd)/renderBuild
cp -rf $(pwd)/renderBuild/Release-iphoneos/librender.a ../Conch/libs/ios/librender.a
cp -rf $(pwd)/renderBuild/Release-iphonesimulator/librender.a ../Conch/libs/ios-sim/librender.a
rm -rf $(pwd)/renderBuild
