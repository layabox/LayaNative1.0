cmake_minimum_required(VERSION 3.6)
project(render)
set(render_src
    ../../../source/render/Html5Render/JCHtml5Context.cpp
    ../../../source/render/Html5Render/JCHtml5Render.cpp
    ../../../source/render/Html5Render/JCHtml5RenderManager.cpp
    ../../../source/render/Image/JCImage.cpp
    ../../../source/render/Image/JCImageManager.cpp
    ../../../source/render/Manager/JCBufferManager.cpp
    ../../../source/render/Manager/JCShaderManager.cpp
    ../../../source/render/Node/JCGraphics.cpp
    ../../../source/render/Node/JCGraphicsWordInfo.cpp
    ../../../source/render/Node/JCGraphicsCmdDispath.cpp
    ../../../source/render/Node/JCNode.cpp
    ../../../source/render/Node/JCNode2D.cpp
    ../../../source/render/Node/JCNode2DCmdDispath.cpp
    ../../../source/render/Node/JCNode2DRenderer.cpp
    ../../../source/render/Node/JCNodeStyle.cpp
    ../../../source/render/Performance/JCPerfHUD.cpp
    ../../../source/render/RenderUtil/JCVBOManager.cpp
    ../../../source/render/SaveData/JCContextSaveData.cpp
    ../../../source/render/Text/JCTextMemoryCanvas.cpp
    ../../../source/render/WebGLRender/JCIDGenerator.cpp
    ../../../source/render/WebGLRender/JCProgramLocationTable.cpp
    ../../../source/render/WebGLRender/JCWebGLRender.cpp
    ../../../source/render/WebGLRender/WebGL.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLES20/Attribute.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLES20/MatrixStack.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLES20/OpenGLES20Context.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLES20/OpenGLES20Implementation.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLES20/OpenGLESState.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLES20/Shader.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLES20/ShaderFile.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLES20/ShaderProgram.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLES20/ShaderSource.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLES20/Uniform.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLESConfig.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLESContext.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLESFile.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLESImplementation.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLESString.cpp
    ../../../source/render/MonkVG/gles2-bc/OpenGLESUtil.cpp
    ../../../source/render/MonkVG/glu/libtess/dict.c
    ../../../source/render/MonkVG/glu/libtess/geom.c
    ../../../source/render/MonkVG/glu/libtess/memalloc.c
    ../../../source/render/MonkVG/glu/libtess/mesh.c
    ../../../source/render/MonkVG/glu/libtess/normal.c
    ../../../source/render/MonkVG/glu/libtess/priorityq-heap.c
    ../../../source/render/MonkVG/glu/libtess/priorityq.c
    ../../../source/render/MonkVG/glu/libtess/render.c
    ../../../source/render/MonkVG/glu/libtess/sweep.c
    ../../../source/render/MonkVG/glu/libtess/tess.c
    ../../../source/render/MonkVG/glu/libtess/tessmono.c
    ../../../source/render/MonkVG/glu/libutil/error.c
    ../../../source/render/MonkVG/glu/libutil/glue.c
    ../../../source/render/MonkVG/glu/libutil/project.c
    ../../../source/render/MonkVG/glu/libutil/registry.c
    ../../../source/render/MonkVG/src/mkBaseObject.cpp
    ../../../source/render/MonkVG/src/mkBatch.cpp
    ../../../source/render/MonkVG/src/mkContext.cpp
    ../../../source/render/MonkVG/src/mkFont.cpp
    ../../../source/render/MonkVG/src/mkImage.cpp
    ../../../source/render/MonkVG/src/mkMath.cpp
    ../../../source/render/MonkVG/src/mkPaint.cpp
    ../../../source/render/MonkVG/src/mkParameter.cpp
    ../../../source/render/MonkVG/src/mkPath.cpp
    ../../../source/render/MonkVG/src/mkVGU.cpp
    ../../../source/render/MonkVG/src/laya/layaContext.cpp
    ../../../source/render/MonkVG/src/laya/layaPaint.cpp
    ../../../source/render/MonkVG/src/laya/layaPath.cpp
    ../../../source/render/MonkVG/src/opengl/glBatch.cpp
    ../../../source/render/MonkVG/src/opengl/glContext.cpp
    ../../../source/render/MonkVG/src/opengl/glFont.cpp
    ../../../source/render/MonkVG/src/opengl/glImage.cpp
    ../../../source/render/MonkVG/src/opengl/glPaint.cpp
    ../../../source/render/MonkVG/src/opengl/glPath.cpp
    ../../../source/render/Particle2D/JCParticleData2D.cpp
    ../../../source/render/Particle2D/JCParticleSetting2D.cpp
    ../../../source/render/Particle2D/JCParticleTemplate2D.cpp
    ../../../source/render/Particle2D/JCParticleTemplate2DCmdDispath.cpp
)
add_library(render STATIC ${render_src})

target_include_directories(render PUBLIC
    ../../../../ThirdParty/curl/include/ohos
    ../../../../ThirdParty/png/include/ohos
    ../../../../ThirdParty/jpeg/include/ohos
    ../../../../ThirdParty/freetype/include/ohos
    ../../../../ThirdParty/zip/include/ohos
    ../../../include/common
    ../../../include/webglPlus
    ../../../include/conch
    ../../../source/render/MonkVG/include
    ../../../source/render/MonkVG/src/opengl
    ../../../source/render/MonkVG/src
    ../../../source/render/MonkVG/glu/include
    ../../../source/render/MonkVG/gles2-bc   
)

set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -std=c++11")

target_compile_options(${PROJECT_NAME} PRIVATE -DV8_COMPRESS_POINTERS)
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -DV8_COMPRESS_POINTERS")
add_definitions(-frtti)