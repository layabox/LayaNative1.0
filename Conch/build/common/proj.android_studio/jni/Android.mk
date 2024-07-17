
LOCAL_PATH:= $(call my-dir)

#libcommon.a
include $(CLEAR_VARS)
LOCAL_MODULE := libcommon
LOCAL_CFLAGS := \
  -fexceptions \
  -Wno-multichar \
  -DANDROID \
  -DIN_LIBRARY \
  -D_GLIBCXX_PERMIT_BACKWARD_HASH \
  -DANDROID_ARM_NEON=TRUE \
  -DANDROID_STL=c++_shared \
  -DANDROID_TOOLCHAIN=clang \
  -frtti

LOCAL_CPPFLAGS += -std=c++11

ifeq ($(APP_PERFTEST),1)
LOCAL_CFLAGS += -DPERFTEST 
endif

ifeq ($(TARGET_ARCH),arm64)
   LOCAL_CFLAGS += -DV8_COMPRESS_POINTERS
   LOCAL_CPPFLAGS+= -DV8_COMPRESS_POINTERS
endif

LOCAL_CXXFLAGS := -O3

LOCAL_SRC_FILES := \
        ../../../../source/common/3DRS/JC2DShaderManager.cpp \
        ../../../../source/common/3DRS/JCConchMesh.cpp \
        ../../../../source/common/3DRS/JCConchMesh2D.cpp \
        ../../../../source/common/3DRS/JCGL.cpp \
        ../../../../source/common/3DRS/JCGpuProgram.cpp \
        ../../../../source/common/3DRS/JCMaterial.cpp \
        ../../../../source/common/3DRS/JCMaterialManager.cpp \
        ../../../../source/common/3DRS/JCMesh.cpp \
        ../../../../source/common/3DRS/JCMeshBuilder.cpp \
        ../../../../source/common/3DRS/JCNamedData.cpp \
        ../../../../source/common/3DRS/JCRenderer.cpp \
        ../../../../source/common/3DRS/JCShaderDefine.cpp \
        ../../../../source/common/3DRS/JCVertexBuffer.cpp \
        ../../../../source/common/buffer/JCBuffer.cpp \
        ../../../../source/common/downloadCache/JCFileTable.cpp \
        ../../../../source/common/downloadCache/JCServerFileCache.cpp \
        ../../../../source/common/downloadMgr/JCCurlWrap.cpp \
        ../../../../source/common/downloadMgr/JCHttpHeader.cpp \
        ../../../../source/common/downloadMgr/JCDownloadMgr.cpp \
        ../../../../source/common/event/JCEmitter.cpp \
        ../../../../source/common/event/JCEventBase.cpp \
        ../../../../source/common/fileSystem/JCFileSystem.cpp \
        ../../../../source/common/FontRender/JCFreeTypeRender.cpp \
        ../../../../source/common/FontRender/JCMeasureTextManager.cpp \
		../../../../source/common/geom/JCMatrix44.cpp \
        ../../../../source/common/geom/Matrix32.cpp \
        ../../../../source/common/geom/Point.cpp \
        ../../../../source/common/geom/Rectangle.cpp \
        ../../../../source/common/imageLib/JCImageRW.cpp \
        ../../../../source/common/imageLib/JCJpegImg.cpp \
        ../../../../source/common/imageLib/JCPngImg.cpp \
		     ../../../../source/common/imageLib/JCTPGImg.cpp \
        ../../../../source/common/imageLib/JCGifImg.cpp \
		../../../../source/common/imageLib/JCETCImg.cpp \
		../../../../source/common/imageLib/JCPVRImg.cpp \
        ../../../../source/common/misc/JCGetClockExact.cpp \
        ../../../../source/common/misc/JCLayaThreadPool.cpp \
        ../../../../source/common/misc/JCWorkerThread.cpp \
        ../../../../source/common/misc/JCWorkSemaphore.cpp \
        ../../../../source/common/util/JCColor.cpp \
        ../../../../source/common/util/JCCommonMethed.cpp \
        ../../../../source/common/util/JCCrypto.cpp \
        ../../../../source/common/util/JCJson.cpp \
        ../../../../source/common/util/JCIniFile.cpp \
        ../../../../source/common/util/JCLayaUrl.cpp \
        ../../../../source/common/util/JCMemorySurvey.cpp \
        ../../../../source/common/util/JCXml.cpp \
        ../../../../source/common/util/JCZipFile.cpp \
        ../../../../source/common/util/JCZlib.cpp \
        ../../../../source/common/util/Log.cpp \
        ../../../../source/common/util/JCFlog.cpp \
        ../../../../source/common/resource/Atlas/JCAtlas.cpp \
        ../../../../source/common/resource/Atlas/JCAtlasManager.cpp \
        ../../../../source/common/resource/Audio/JCAudioWavplayer.cpp \
        ../../../../source/common/resource/Audio/JCOggParser.cpp \
        ../../../../source/common/resource/Audio/JCWaveInfo.cpp \
        ../../../../source/common/resource/Audio/JCWaveParser.cpp \
        ../../../../source/common/resource/DisplayRes/JCWordRes.cpp \
        ../../../../source/common/resource/DisplayRes/JCInAtlasRes.cpp \
        ../../../../source/common/resource/DisplayRes/JCTexture.cpp \
		../../../../source/common/resource/DisplayRes/JCRenderTarget.cpp \
        ../../../../source/common/resource/text/JCTextManager.cpp \
        ../../../../source/common/resource/text/JCFontInfo.cpp \
        ../../../../source/common/resource/text/JCFontManager.cpp \
        ../../../../source/common/resource/JCFileResManager.cpp \
        ../../../../source/common/resource/JCResManager.cpp \
        ../../../../source/common/resource/JCResource.cpp \


LOCAL_C_INCLUDES := ../../../../../ThirdParty/curl/include/android \
        ../../../../../ThirdParty/png/include/android \
        ../../../../../ThirdParty/jpeg/include/android \
        ../../../../../ThirdParty/freetype/include/android \
        ../../../../../ThirdParty/zip/include/android \
        ../../../../../ThirdParty/ogg/include/android \
		../../../../../ThirdParty/zlib/include/android \
        ../../../../../ThirdParty/openal/include/android \
        ../../../../source/common/glm \

#LOCAL_IS64:=32
#LOCAL_LDLIBS    := -llog -lz -landroid			\
#		../../../../libs/android$(LOCAL_IS64)/libcurl.a \
#		../../../../libs/android$(LOCAL_IS64)/libgnustl_static.a \
#		../../../../libs/android$(LOCAL_IS64)/libjpeg.a \
#		../../../../libs/android$(LOCAL_IS64)/libpng.a \
#		../../../../libs/android$(LOCAL_IS64)/libfreetype.a \
#		../../../../libs/android$(LOCAL_IS64)/libzip.a \
#		../../../../libs/android$(LOCAL_IS64)/libogg.a \
#		../../../../libs/android$(LOCAL_IS64)/libvorbis.a \
#		../../../../libs/android$(LOCAL_IS64)/libvorbis-jni.a \


include $(BUILD_STATIC_LIBRARY)

