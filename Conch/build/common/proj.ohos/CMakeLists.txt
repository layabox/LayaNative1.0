cmake_minimum_required(VERSION 3.6)
project(common)
set(common_src
    ../../../source/common/3DRS/JC2DShaderManager.cpp
    ../../../source/common/3DRS/JCConchMesh.cpp
    ../../../source/common/3DRS/JCConchMesh2D.cpp
    ../../../source/common/3DRS/JCGL.cpp
    ../../../source/common/3DRS/JCGpuProgram.cpp
    ../../../source/common/3DRS/JCMaterial.cpp
    ../../../source/common/3DRS/JCMaterialManager.cpp
    ../../../source/common/3DRS/JCMesh.cpp
    ../../../source/common/3DRS/JCMeshBuilder.cpp
    ../../../source/common/3DRS/JCNamedData.cpp
    ../../../source/common/3DRS/JCRenderer.cpp
    ../../../source/common/3DRS/JCShaderDefine.cpp
    ../../../source/common/3DRS/JCVertexBuffer.cpp
    ../../../source/common/buffer/JCBuffer.cpp
    ../../../source/common/downloadCache/JCFileTable.cpp
    ../../../source/common/downloadCache/JCServerFileCache.cpp
    ../../../source/common/downloadMgr/JCCurlWrap.cpp
    ../../../source/common/downloadMgr/JCHttpHeader.cpp
    ../../../source/common/downloadMgr/JCDownloadMgr.cpp
    ../../../source/common/event/JCEmitter.cpp
    ../../../source/common/event/JCEventBase.cpp
    ../../../source/common/fileSystem/JCFileSystem.cpp
    ../../../source/common/FontRender/JCFreeTypeRender.cpp
    ../../../source/common/FontRender/JCMeasureTextManager.cpp
	../../../source/common/geom/JCMatrix44.cpp
    ../../../source/common/geom/Matrix32.cpp
    ../../../source/common/geom/Point.cpp
    ../../../source/common/geom/Rectangle.cpp
    ../../../source/common/imageLib/JCImageRW.cpp
    ../../../source/common/imageLib/JCJpegImg.cpp
    ../../../source/common/imageLib/JCPngImg.cpp
	../../../source/common/imageLib/JCTPGImg.cpp
    ../../../source/common/imageLib/JCGifImg.cpp
	../../../source/common/imageLib/JCETCImg.cpp
	../../../source/common/imageLib/JCPVRImg.cpp
    ../../../source/common/misc/JCGetClockExact.cpp
    ../../../source/common/misc/JCLayaThreadPool.cpp
    ../../../source/common/misc/JCWorkerThread.cpp
    ../../../source/common/misc/JCWorkSemaphore.cpp
    ../../../source/common/util/JCColor.cpp
    ../../../source/common/util/JCCommonMethed.cpp
    ../../../source/common/util/JCCrypto.cpp
    ../../../source/common/util/JCJson.cpp
    ../../../source/common/util/JCIniFile.cpp
    ../../../source/common/util/JCLayaUrl.cpp
    ../../../source/common/util/JCMemorySurvey.cpp
    ../../../source/common/util/JCXml.cpp
    ../../../source/common/util/JCZipFile.cpp
    ../../../source/common/util/JCZlib.cpp
    ../../../source/common/util/Log.cpp
    ../../../source/common/util/JCFlog.cpp
    ../../../source/common/resource/Atlas/JCAtlas.cpp
    ../../../source/common/resource/Atlas/JCAtlasManager.cpp
    ../../../source/common/resource/Audio/JCAudioWavplayer-openharmony.cpp
    ../../../source/common/resource/Audio/JCOggParser.cpp
    ../../../source/common/resource/Audio/JCWaveInfo.cpp
    ../../../source/common/resource/Audio/JCWaveParser.cpp
    ../../../source/common/resource/DisplayRes/JCWordRes.cpp
    ../../../source/common/resource/DisplayRes/JCInAtlasRes.cpp
    ../../../source/common/resource/DisplayRes/JCTexture.cpp
	../../../source/common/resource/DisplayRes/JCRenderTarget.cpp
    ../../../source/common/resource/text/JCTextManager.cpp
    ../../../source/common/resource/text/JCFontInfo.cpp
    ../../../source/common/resource/text/JCFontManager.cpp
    ../../../source/common/resource/JCFileResManager.cpp
    ../../../source/common/resource/JCResManager.cpp
    ../../../source/common/resource/JCResource.cpp
)
add_library(common STATIC ${common_src})

target_include_directories(common PUBLIC
    ../../../../ThirdParty/curl/include/ohos
    ../../../../ThirdParty/png/include/ohos
    ../../../../ThirdParty/jpeg/include/ohos
    ../../../../ThirdParty/freetype/include/ohos
    ../../../../ThirdParty/zip/include/ohos
    ../../../../ThirdParty/ogg/include/ohos
    ../../../../ThirdParty/zlib/include/ohos
    ../../../../source/common/glm
    ../../../../Conch/source
)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")
target_compile_options(common PRIVATE -DV8_COMPRESS_POINTERS)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -DV8_COMPRESS_POINTERS")
target_compile_options(common PRIVATE -O3)
add_definitions(-frtti)