/**
@file			JCICmdDispatch.h
@brief			
@author			James
@version		1.0
@date			2016_5_27
*/

#ifndef __JCICmdDispatch_H__
#define __JCICmdDispatch_H__

#include "buffer/JCBuffer.h"

//#define DISPATH_DEBUG

namespace laya
{
    class JCICmdDispatch
    {
    public:

        virtual ~JCICmdDispatch() {};

        /** @brief 开始JS线程，这个时候可以做一些初始化的工作
         *  @return 是否成功
        */
        virtual bool restartJSThread() = 0;

        /** @brief 解析渲染指令流
         *  @param[in] 指令流 
         *  @param[in] 当前FrameCount
         *  @return 解析是否成功
        */
        virtual bool dispatchRenderCmd( JCMemClass& pRenderCmd,int nFrameCount ) = 0;

        /** @brief 当设备丢失的时候,解析渲染指令流
        *  @param[in] 指令流
        *  @param[in] 当前FrameCount
        *  @return 解析是否成功
        */
        virtual bool dispatchRenderCmdWhenLostedDevice(JCMemClass& pRenderCmd, int nFrameCount) = 0;
    };
}

#endif //__JCICmdDispatch_H__

//-----------------------------END FILE--------------------------------
