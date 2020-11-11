/**
@file			JCParticleData2D.h
@brief			
@author			hugao
@version		1.0
@date			2016_11_1
*/

#ifndef __JCParticleData2D_H__
#define __JCParticleData2D_H__


//包含头文件
//------------------------------------------------------------------------------
#include "JCParticleSetting2D.h"
//包含头文件
//------------------------------------------------------------------------------
namespace laya
{
    class JCParticleData2D
    {
   public:
        /** @brief构造函数
        */
        JCParticleData2D();

        /** @brief析构函数
        */
        ~JCParticleData2D();
    public:
        void initData(JCParticleSetting2D* p_pParticleSetting,float* p_vPosition,float* p_vVelocity,float p_nTime);
        static float lerp(float p_nLeft, float p_nRight, float p_nAmout);
        static void subtractVector3(float* p_vL, float *p_vR, float *p_vO);
        static void scaleVector3(float* p_vF, float p_nB, float*p_vE);
        static void lerpVector4(float* p_vL,float* p_vR,float p_nT,float* p_vO);
    public:
        float m_vPosition[3];//float3
        float m_vVelocity[3];//float3
        float m_vStartColor[4];//float4
        float m_vEndColor[4];//float4
        float m_vSizeRotation[3];//float3
        float m_vRadius[2];//float2
        float m_vRadian[4];//float4
        float m_nDurationAddScale;
        float m_nTime;
    };
}
//------------------------------------------------------------------------------


#endif //__JCParticleData2D_H__

//-----------------------------END FILE--------------------------------