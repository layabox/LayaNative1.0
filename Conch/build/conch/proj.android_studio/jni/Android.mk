
LOCAL_PATH:= $(call my-dir)

#liblayaair.a

include $(CLEAR_VARS)
LOCAL_MODULE := liblayaair
LOCAL_CFLAGS := \
  -fexceptions \
  -Wno-multichar \
  -DANDROID \
  -DJS_V8 \
  -DIN_LIBRARY \
  -D_GLIBCXX_PERMIT_BACKWARD_HASH \
  -DANDROID_ARM_NEON=TRUE \
  -DANDROID_STL=c++_shared \
  -DANDROID_TOOLCHAIN=clang \
  -fno-rtti \
  -DAL_ALEXT_PROTOTYPES \
  #todo -DJS_V8_DEBUGGER \

LOCAL_CPPFLAGS += -std=c++11
ifeq ($(APP_PERFTEST),1)
LOCAL_CFLAGS += -DPERFTEST 
endif

LOCAL_CXXFLAGS := -O3

ifeq ($(TARGET_ARCH),arm64)
   LOCAL_CFLAGS += -DV8_COMPRESS_POINTERS
   LOCAL_CPPFLAGS += -DV8_COMPRESS_POINTERS
endif

LOCAL_SRC_FILES := \
        ../../../../source/conch/JSWrapper/v8debug/debug-agent.cpp \
        ../../../../source/conch/JSWrapper/v8debug/V8WSSv.cpp \
        ../../../../source/conch/JSWrapper/JSInterface/JSInterface.cpp \
        ../../../../source/conch/JSWrapper/JSInterface/V8/JSEnv.cpp \
        ../../../../source/conch/JSWrapper/JSInterface/V8/JsBinder.cpp \
        ../../../../source/conch/JSWrapper/JSInterface/V8/JSArrayBuffer.cpp \
		../../../../source/conch/JSWrapper/JSInterface/V8/IsolateData.cpp \
		../../../../source/conch/JSWrapper/JSInterface/JSObjBase.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSAndroidEditBox.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSAppCache.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSAudio.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSConchConfig.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSConsole.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSDOMParser.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSFile.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSFileReader.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSFileSystem.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSGlobalExportCFun.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSGraphics.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSHistory.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSImage.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSInput.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSLayaContext.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSMarket.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSMeasureTextInfo.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSNode2D.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSNotify.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSParticleTemplate2D.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSRuntime.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSTextMemoryCanvas.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSWebGLContext.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSWebSocket.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSXmlAttr.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSXmlNode.cpp \
        ../../../../source/conch/JSWrapper/LayaWrap/JSZip.cpp\
        ../../../../source/conch/JSWrapper/LayaWrap/XMLHttpRequest.cpp \
		../../../../source/conch/JSWrapper/LayaWrap/JSShaderActiveInfo.cpp \
		../../../../source/conch/JSWrapper/LayaWrap/JSArrayBufferRef.cpp \
        ../../../../source/conch/Audio/android/JCAudioMp3Media.cpp \
        ../../../../source/conch/Audio/JCAudioManager.cpp \
        ../../../../source/conch/WebSocket/WebSocket.cpp \
        ../../../../source/conch/JCCmdDispatchManager.cpp \
        ../../../../source/conch/JCConch.cpp \
        ../../../../source/conch/JCConchRender.cpp \
        ../../../../source/conch/JCPublicCmdDispath.cpp \
        ../../../../source/conch/JCThreadCmdMgr.cpp \
        ../../../../source/conch/JCScriptRuntime.cpp \
        ../../../../source/conch/JCSystemConfig.cpp \
        ../../../../source/conch/CToJavaBridge.cpp \
        ../../../../source/conch/JNIFun.cpp \
		../../../../source/conch/Bridge/JCConchBridge.cpp \
        #../../../../source/conch/JSWrapper/v8debug/debug-agent.cpp \


LOCAL_C_INCLUDES := ../../../../../ThirdParty/curl/include/android \
        ../../../../../ThirdParty/png/include/android \
        ../../../../../ThirdParty/jpeg/include/android \
        ../../../../../ThirdParty/freetype/include/android \
        ../../../../../ThirdParty/zip/include/android \
        ../../../../../ThirdParty/v8_8.8/include \
        ../../../../../ThirdParty/ogg/include/android \
        ../../../../../ThirdParty/websockets/include/android \
		../../../../../ThirdParty/openssl/include/android \
        ../../../../../ThirdParty/openal/include/android \
        ../../../../include/common \
        ../../../../include/render \
        ../../../../include/msgpack/include \
        ../../../../include/render/MonkVG/include \
        ../../../../include/render/MonkVG/src/opengl \
        ../../../../include/render/MonkVG/src \
        ../../../../include/render/MonkVG/glu/include \
        ../../../../include/render/MonkVG/gles2-bc \
        

LOCAL_IS64:=armv7
ifeq ($(TARGET_ARCH),arm64)
LOCAL_IS64:=arm64
else ifeq ($(TARGET_ARCH),x86)
LOCAL_IS64:=x86
endif
LOCAL_DISABLE_FATAL_LINKER_WARNINGS := true
LOCAL_LDLIBS    := -lOpenSLES -llog -lGLESv3 -landroid -ljnigraphics -lm -lz -lc++ \
        ../../../../libs/android-$(LOCAL_IS64)/librender.a \
        ../../../../libs/android-$(LOCAL_IS64)/libcommon.a \
        ../../../../../ThirdParty/curl/lib/android-$(LOCAL_IS64)/libcurl.a \
        ../../../../../ThirdParty/v8_8.8/lib/android-$(LOCAL_IS64)/libv8_monolith.a \
        ../../../../../ThirdParty/v8_8.8/lib/android-$(LOCAL_IS64)/libinspector.a \
        ../../../../../ThirdParty/websockets/lib/android-$(LOCAL_IS64)/libwebsockets.a \
        ../../../../../ThirdParty/jpeg/lib/android-$(LOCAL_IS64)/libjpeg.a \
        ../../../../../ThirdParty/png/lib/android-$(LOCAL_IS64)/libpng.a \
        ../../../../../ThirdParty/freetype/lib/android-$(LOCAL_IS64)/libfreetype.a \
        ../../../../../ThirdParty/zip/lib/android-$(LOCAL_IS64)/libzip.a \
		../../../../../ThirdParty/ogg/lib/android-$(LOCAL_IS64)/libvorbisfile.a \
		../../../../../ThirdParty/ogg/lib/android-$(LOCAL_IS64)/libvorbis.a \
		../../../../../ThirdParty/ogg/lib/android-$(LOCAL_IS64)/libogg.a \
        ../../../../../ThirdParty/openssl/lib/android-$(LOCAL_IS64)/libssl.a \
        ../../../../../ThirdParty/openssl/lib/android-$(LOCAL_IS64)/libcrypto.a \
		../../../../../ThirdParty/zlib/lib/android-$(LOCAL_IS64)/libz.a \
        ../../../../../ThirdParty/openal/lib/android-$(LOCAL_IS64)/libopenal.a \
			


include $(BUILD_SHARED_LIBRARY)

