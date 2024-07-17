#!/bin/sh

rm -rf LayaRuntime-iOS
mkdir LayaRuntime-iOS
mkdir LayaRuntime-iOS/libs

#—————————————————————merge static lib————————————————————————

rm -f conch/libconch.a
rm -rf armv7a
rm -rf armv64
mkdir armv7a
mkdir armv64

lipo ../ThirdParty/ios/lib/libboost_date_time.a -thin armv7 -output armv7a/libboost_date_time.a
lipo ../ThirdParty/ios/lib/libboost_date_time.a -thin arm64 -output armv64/libboost_date_time.a

lipo ../ThirdParty/ios/lib/libboost_filesystem.a -thin armv7 -output armv7a/libboost_filesystem.a
lipo ../ThirdParty/ios/lib/libboost_filesystem.a -thin arm64 -output armv64/libboost_filesystem.a

lipo ../ThirdParty/ios/lib/libboost_system.a -thin armv7 -output armv7a/libboost_system.a
lipo ../ThirdParty/ios/lib/libboost_system.a -thin arm64 -output armv64/libboost_system.a

lipo ../ThirdParty/ios/lib/libboost_thread.a -thin armv7 -output armv7a/libboost_thread.a
lipo ../ThirdParty/ios/lib/libboost_thread.a -thin arm64 -output armv64/libboost_thread.a

lipo ../ThirdParty/ios/lib/libfreetype.a -thin armv7 -output armv7a/libfreetype.a
lipo ../ThirdParty/ios/lib/libfreetype.a -thin arm64 -output armv64/libfreetype.a

#lipo ../ThirdParty/ios/lib/libjpeg.a -thin armv7 -output armv7a/libjpeg.a
#lipo ../ThirdParty/ios/lib/libjpeg.a -thin arm64 -output armv64/libjpeg.a

lipo ../ThirdParty/ios/lib/libpng.a -thin armv7 -output armv7a/libpng.a
lipo ../ThirdParty/ios/lib/libpng.a -thin arm64 -output armv64/libpng.a

lipo ../ThirdParty/ios/lib/libwebsockets.a -thin armv7 -output armv7a/libwebsockets.a
lipo ../ThirdParty/ios/lib/libwebsockets.a -thin arm64 -output armv64/libwebsockets.a

lipo ../ThirdParty/ios/lib/libzip.a -thin armv7 -output armv7a/libzip.a
lipo ../ThirdParty/ios/lib/libzip.a -thin arm64 -output armv64/libzip.a

lipo ../ThirdParty/ios/lib/libogg.a -thin armv7 -output armv7a/libogg.a
lipo ../ThirdParty/ios/lib/libogg.a -thin arm64 -output armv64/libogg.a

lipo ../ThirdParty/ios/lib/libvorbis.a -thin armv7 -output armv7a/libvorbis.a
lipo ../ThirdParty/ios/lib/libvorbis.a -thin arm64 -output armv64/libvorbis.a

lipo ../ThirdParty/ios/lib/libvorbisfile.a -thin armv7 -output armv7a/libvorbisfile.a
lipo ../ThirdParty/ios/lib/libvorbisfile.a -thin arm64 -output armv64/libvorbisfile.a

lipo ../Conch/libs/ios/libcommon.a -thin armv7 -output armv7a/libcommon.a
lipo ../Conch/libs/ios/libcommon.a -thin arm64 -output armv64/libcommon.a

lipo ../Conch/libs/ios/librender.a -thin armv7 -output armv7a/librender.a
lipo ../Conch/libs/ios/librender.a -thin arm64 -output armv64/librender.a

lipo ../Conch/libs/ios/libconchRuntime.a -thin armv7 -output armv7a/libconchRuntime.a
lipo ../Conch/libs/ios/libconchRuntime.a -thin arm64 -output armv64/libconchRuntime.a


cd armv7a

ar x libboost_date_time.a
ar x libboost_filesystem.a
ar x libboost_system.a
ar x libboost_thread.a
ar x libfreetype.a
#ar x libjpeg.a
ar x libpng.a
ar x libcommon.a
ar x librender.a
ar x libconchRuntime.a
ar x libwebsockets.a
ar x libzip.a
ar x libogg.a
ar x libvorbis.a
ar x libvorbisfile.a

ar cru libconch.a *.o
ranlib libconch.a

cd ..
cd armv64

ar x libboost_date_time.a
ar x libboost_filesystem.a
ar x libboost_system.a
ar x libboost_thread.a
ar x libfreetype.a
#ar x libjpeg.a
ar x libpng.a
ar x libcommon.a
ar x librender.a
ar x libconchRuntime.a
ar x libwebsockets.a
ar x libzip.a
ar x libogg.a
ar x libvorbis.a
ar x libvorbisfile.a

ar cru libconch.a *.o
ranlib libconch.a

cd ..

lipo -create armv7a/libconch.a armv64/libconch.a -output LayaRuntime-iOS/libs/libconch.a

rm -rf armv7a
rm -rf armv64

#—————————————————————copy .JSC.so—————————-

cp -f ../ThirdParty/JSC/libs/libJavaScriptCore.a LayaRuntime-iOS/libs/libJavaScriptCore.a
cp -f ../ThirdParty/JSC/libs/libbmalloc.a LayaRuntime-iOS/libs/libbmalloc.a
cp -f ../ThirdParty/ios/lib/libcurl.a LayaRuntime-iOS/libs/libcurl.a
cp -f ../ThirdParty/ios/lib/libcurls.a LayaRuntime-iOS/libs/libcurls.a
cp -f ../ThirdParty/ios/lib/libjpeg.a LayaRuntime-iOS/libs/libjpeg.a

#—————————————————————copy .h————————————————————————

rm -rf LayaRuntime-iOS/include
mkdir LayaRuntime-iOS/include
cp -rf ../Conch/include/conchRuntime/ LayaRuntime-iOS/include


#—————————————————————resource————————————————————————
rm -rf LayaRuntime-iOS/resource
mkdir LayaRuntime-iOS/resource
cp -rf ../Conch/build/conch/proj.ios/resource LayaRuntime-iOS/
rm -rf LayaRuntime-iOS/resource/market



