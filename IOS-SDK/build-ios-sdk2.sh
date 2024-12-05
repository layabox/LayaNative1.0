#!/bin/sh

rm -rf LayaRuntime-iOS
mkdir LayaRuntime-iOS
mkdir LayaRuntime-iOS/libs

#—————————————————————merge static lib————————————————————————

rm -f conch/libconch.a
rm -rf armv7a
rm -rf armv64
rm -rf i386
rm -rf x86_64
mkdir armv7a
mkdir armv64
mkdir i386
mkdir x86_64

lipo -extract armv7 ../ThirdParty/freetype/lib/ios/libfreetype.a -o armv7a/libfreetype.a
lipo -extract arm64 ../ThirdParty/freetype/lib/ios/libfreetype.a -o armv64/libfreetype.a
lipo -extract i386 ../ThirdParty/freetype/lib/ios/libfreetype.a -o i386/libfreetype.a
lipo -extract x86_64 ../ThirdParty/freetype/lib/ios/libfreetype.a -o x86_64/libfreetype.a

lipo -extract armv7 ../ThirdParty/jpeg/lib/ios/libjpeg.a -o armv7a/libjpeg.a
lipo -extract arm64 ../ThirdParty/jpeg/lib/ios/libjpeg.a -o armv64/libjpeg.a
lipo -extract i386 ../ThirdParty/jpeg/lib/ios/libjpeg.a -o i386/libjpeg.a
lipo -extract x86_64 ../ThirdParty/jpeg/lib/ios/libjpeg.a -o x86_64/libjpeg.a

lipo -extract armv7 ../ThirdParty/png/lib/ios/libpng.a -o armv7a/libpng.a
lipo -extract arm64 ../ThirdParty/png/lib/ios/libpng.a -o armv64/libpng.a
lipo -extract i386 ../ThirdParty/png/lib/ios/libpng.a -o i386/libpng.a
lipo -extract x86_64 ../ThirdParty/png/lib/ios/libpng.a -o x86_64/libpng.a

lipo -extract armv7 ../ThirdParty/websockets/lib/ios/libwebsockets.a -o armv7a/libwebsockets.a
lipo -extract arm64 ../ThirdParty/websockets/lib/ios/libwebsockets.a -o armv64/libwebsockets.a
lipo -extract i386 ../ThirdParty/websockets/lib/ios/libwebsockets.a -o i386/libwebsockets.a
lipo -extract x86_64 ../ThirdParty/websockets/lib/ios/libwebsockets.a -o x86_64/libwebsockets.a

lipo -extract armv7 ../ThirdParty/zip/lib/ios/libzip.a -o armv7a/libzip.a
lipo -extract arm64 ../ThirdParty/zip/lib/ios/libzip.a -o armv64/libzip.a
lipo -extract i386 ../ThirdParty/zip/lib/ios/libzip.a -o i386/libzip.a
lipo -extract x86_64 ../ThirdParty/zip/lib/ios/libzip.a -o x86_64/libzip.a

lipo -extract armv7 ../ThirdParty/ogg/lib/ios/libogg.a -o armv7a/libogg.a
lipo -extract arm64 ../ThirdParty/ogg/lib/ios/libogg.a -o armv64/libogg.a
lipo -extract i386 ../ThirdParty/ogg/lib/ios/libogg.a -o i386/libogg.a
lipo -extract x86_64 ../ThirdParty/ogg/lib/ios/libogg.a -o x86_64/libogg.a

lipo -extract armv7 ../ThirdParty/ogg/lib/ios/libvorbis.a -o armv7a/libvorbis.a
lipo -extract arm64 ../ThirdParty/ogg/lib/ios/libvorbis.a -o armv64/libvorbis.a
lipo -extract i386 ../ThirdParty/ogg/lib/ios/libvorbis.a -o i386/libvorbis.a
lipo -extract x86_64 ../ThirdParty/ogg/lib/ios/libvorbis.a -o x86_64/libvorbis.a

lipo -extract armv7 ../ThirdParty/ogg/lib/ios/libvorbisfile.a -o armv7a/libvorbisfile.a
lipo -extract arm64 ../ThirdParty/ogg/lib/ios/libvorbisfile.a -o armv64/libvorbisfile.a
lipo -extract i386 ../ThirdParty/ogg/lib/ios/libvorbisfile.a -o i386/libvorbisfile.a
lipo -extract x86_64 ../ThirdParty/ogg/lib/ios/libvorbisfile.a -o x86_64/libvorbisfile.a

lipo -extract armv7 ../ThirdParty/zlib/lib/ios/libz.a -o armv7a/libz.a
lipo -extract arm64 ../ThirdParty/zlib/lib/ios/libz.a -o armv64/libz.a
lipo -extract i386 ../ThirdParty/zlib/lib/ios/libz.a -o i386/libz.a
lipo -extract x86_64 ../ThirdParty/zlib/lib/ios/libz.a -o x86_64/libz.a

lipo -extract armv7 ../ThirdParty/curl/lib/ios/libcurl.a -o armv7a/libcurl.a
lipo -extract arm64 ../ThirdParty/curl/lib/ios/libcurl.a -o armv64/libcurl.a
lipo -extract i386 ../ThirdParty/curl/lib/ios/libcurl.a -o i386/libcurl.a
lipo -extract x86_64 ../ThirdParty/curl/lib/ios/libcurl.a -o x86_64/libcurl.a

lipo -extract armv7 ../ThirdParty/openssl/lib/ios/libssl.a -o armv7a/libssl.a
lipo -extract arm64 ../ThirdParty/openssl/lib/ios/libssl.a -o armv64/libssl.a
lipo -extract i386 ../ThirdParty/openssl/lib/ios/libssl.a -o i386/libssl.a
lipo -extract x86_64 ../ThirdParty/openssl/lib/ios/libssl.a -o x86_64/libssl.a


lipo -extract armv7 ../ThirdParty/openssl/lib/ios/libcrypto.a -o armv7a/libcrypto.a
lipo -extract arm64 ../ThirdParty/openssl/lib/ios/libcrypto.a -o armv64/libcrypto.a
lipo -extract i386 ../ThirdParty/openssl/lib/ios/libcrypto.a -o i386/libcrypto.a
lipo -extract x86_64 ../ThirdParty/openssl/lib/ios/libcrypto.a -o x86_64/libcrypto.a

lipo -extract armv7 ../ThirdParty/tpg/lib/ios/libTPGDec.a -o armv7a/libTPGDec.a
lipo -extract arm64 ../ThirdParty/tpg/lib/ios/libTPGDec.a -o armv64/libTPGDec.a
lipo -extract i386 ../ThirdParty/tpg/lib/ios/libTPGDec.a -o i386/libTPGDec.a
lipo -extract x86_64 ../ThirdParty/tpg/lib/ios/libTPGDec.a -o x86_64/libTPGDec.a

lipo -extract armv7 ../ThirdParty/tpg/lib/ios/liblibYUV.a -o armv7a/liblibYUV.a
lipo -extract arm64 ../ThirdParty/tpg/lib/ios/liblibYUV.a -o armv64/liblibYUV.a
lipo -extract i386 ../ThirdParty/tpg/lib/ios/liblibYUV.a -o i386/liblibYUV.a
lipo -extract x86_64 ../ThirdParty/tpg/lib/ios/liblibYUV.a -o x86_64/liblibYUV.a

cp ../Conch/libs/ios/libcommon.a armv64/libcommon.a
#lipo -extract armv7 ../Conch/libs/ios/libcommon.a -o armv7a/libcommon.a
#lipo -extract arm64 ../Conch/libs/ios/libcommon.a -o armv64/libcommon.a
#lipo -extract i386 ../Conch/libs/ios-sim/libcommon.a -o i386/libcommon.a
lipo -extract x86_64 ../Conch/libs/ios-sim/libcommon.a -o x86_64/libcommon.a

cp ../Conch/libs/ios/librender.a armv64/librender.a
#lipo -extract armv7 ../Conch/libs/ios/librender.a -o armv7a/librender.a
#lipo -extract arm64 ../Conch/libs/ios/librender.a -o armv64/librender.a
#lipo -extract i386 ../Conch/libs/ios-sim/librender.a -o i386/librender.a
lipo -extract x86_64 ../Conch/libs/ios-sim/librender.a -o x86_64/librender.a

cp ../Conch/libs/ios/libconchRuntime.a armv64/libconchRuntime.a
#lipo -extract armv7 ../Conch/libs/ios/libconchRuntime.a -o armv7a/libconchRuntime.a
#lipo -extract arm64 ../Conch/libs/ios/libconchRuntime.a -o armv64/libconchRuntime.a
#lipo -extract i386 ../Conch/libs/ios-sim/libconchRuntime.a -o i386/libconchRuntime.a
lipo -extract x86_64 ../Conch/libs/ios-sim/libconchRuntime.a -o x86_64/libconchRuntime.a


#cd armv7a

#libtool -static *.a -o libconch.a

#cd ..
cd armv64

libtool -static *.a -o libconch.a

cd ..
#cd i386

#libtool -static *.a -o libconch.a

#cd ..
cd x86_64

libtool -static *.a -o libconch.a

cd ..

lipo -create x86_64/libconch.a  armv64/libconch.a -output LayaRuntime-iOS/libs/libconch.a

rm -rf armv7a
rm -rf armv64
rm -rf i386
rm -rf x86_64

#—————————————————————copy .h————————————————————————

rm -rf LayaRuntime-iOS/include
mkdir LayaRuntime-iOS/include
cp -rf ../Conch/include/conchRuntime/ LayaRuntime-iOS/include


#—————————————————————resource————————————————————————
rm -rf LayaBox/resource
mkdir LayaBox/resource
cp -rf ../Conch/build/conch/proj.ios/resource LayaBox/
rm -rf LayaRuntime-iOS/resource/market



