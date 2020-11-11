/**
@file			JCIGLRender.h
@brief			
@author			James
@version		1.0
@date			2016_5_12
*/

#ifndef __JCIGLRender_H__
#define __JCIGLRender_H__

namespace laya
{
    class JCIGLRender
    {
    public:
        virtual void onGLReady() = 0;	                                        //环境准备好了,可以做一些准备工作
        virtual void renderFrame( long nCurrentFrame, bool bStopRender ) = 0;   //渲染
        virtual void invalidGLRes() = 0;                                        //把GLRes失效,包括texture shader vertexBuffer  一般用于设备切换到后台，需要再次恢复
    };
}
//------------------------------------------------------------------------------


#endif //__JCIGLRender_H__

//-----------------------------END FILE--------------------------------