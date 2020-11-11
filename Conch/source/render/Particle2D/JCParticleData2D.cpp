/**
@file			JCParticleData2D.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_10_31
*/

//包含头文件
#include "JCParticleData2D.h"
#include <stdlib.h>
#include <math.h>


//------------------------------------------------------------------------------
namespace laya
{
    #define PI          3.1415926535897932f
    JCParticleData2D::JCParticleData2D( )
    {
        m_nDurationAddScale=0;
        m_nTime = 0;
    }
    JCParticleData2D::~JCParticleData2D()
    {

    }

    void JCParticleData2D::initData(JCParticleSetting2D* p_pParticleSetting, float* p_vPosition, float* p_vVelocity, float p_nTime)
    {
        m_vPosition[0] = p_vPosition[0];
        m_vPosition[1] = p_vPosition[1];
        m_vPosition[2] = p_vPosition[2];
        JCParticleData2D::scaleVector3(p_vVelocity, p_pParticleSetting->m_nEmitterVelocitySensitivity, m_vVelocity);
        float horizontalVelocity = JCParticleData2D::lerp(p_pParticleSetting->m_nMinHorizontalVelocity, p_pParticleSetting->m_nMaxHorizontalVelocity, (float)rand() / RAND_MAX);
        float horizontalAngle = ((float)rand() / RAND_MAX)*PI * 2;
        m_vVelocity[0] += horizontalVelocity*cos(horizontalAngle);
        m_vVelocity[2] += horizontalVelocity*sin(horizontalAngle);
        m_vVelocity[1] += JCParticleData2D::lerp(p_pParticleSetting->m_nMinVerticalVelocity, p_pParticleSetting->m_nMaxVerticalVelocity, (float)rand() / RAND_MAX);

        int i;
		if (p_pParticleSetting->m_bDisableColor)
		{
			for (i = 0; i < 4; i++)
			{
				m_vStartColor[i] = 1;
				m_vEndColor[i] = 1;
			}
			//m_vStartColor[i] = JCParticleData2D::lerp(p_pParticleSetting->m_vMinStartColor[i], p_pParticleSetting->m_vMaxStartColor[i], (float)rand() / RAND_MAX);
			//m_vEndColor[i] = JCParticleData2D::lerp(p_pParticleSetting->m_vMinEndColor[i], p_pParticleSetting->m_vMaxEndColor[i], (float)rand() / RAND_MAX);
		}
		else
		{
			if (p_pParticleSetting->m_bColorComponentInter)
			{
				for (i = 0; i < 4; i++)
				{
					m_vStartColor[i] = JCParticleData2D::lerp(p_pParticleSetting->m_vMinStartColor[i], p_pParticleSetting->m_vMaxStartColor[i], (float)rand() / RAND_MAX);
					m_vEndColor[i] = JCParticleData2D::lerp(p_pParticleSetting->m_vMinEndColor[i], p_pParticleSetting->m_vMaxEndColor[i], (float)rand() / RAND_MAX);
				}
			}
			else
			{
				JCParticleData2D::lerpVector4(p_pParticleSetting->m_vMinStartColor, p_pParticleSetting->m_vMaxStartColor, (float)rand() / RAND_MAX, m_vStartColor);
				JCParticleData2D::lerpVector4(p_pParticleSetting->m_vMinEndColor, p_pParticleSetting->m_vMaxEndColor, (float)rand() / RAND_MAX, m_vEndColor);
			}
		}

        float sizeRandom = (float)rand() / RAND_MAX;
        m_vSizeRotation[0] = JCParticleData2D::lerp(p_pParticleSetting->m_nMinStartSize, p_pParticleSetting->m_nMaxStartSize, sizeRandom);//StartSize
        m_vSizeRotation[1] = JCParticleData2D::lerp(p_pParticleSetting->m_nMinEndSize, p_pParticleSetting->m_nMaxEndSize, sizeRandom);//EndSize
        m_vSizeRotation[2] = JCParticleData2D::lerp(p_pParticleSetting->m_nMinRotateSpeed, p_pParticleSetting->m_nMaxRotateSpeed, (float)rand() / RAND_MAX);//Rotation

        float radiusRandom = (float)rand() / RAND_MAX;
        m_vRadius[0] = JCParticleData2D::lerp(p_pParticleSetting->m_nMinStartRadius, p_pParticleSetting->m_nMaxStartRadius, radiusRandom);//StartRadius
        m_vRadius[1] = JCParticleData2D::lerp(p_pParticleSetting->m_nMinEndRadius, p_pParticleSetting->m_nMaxEndRadius, radiusRandom);//EndRadius

        m_vRadian[0] = JCParticleData2D::lerp(p_pParticleSetting->m_nMinHorizontalStartRadian, p_pParticleSetting->m_nMaxHorizontalStartRadian, (float)rand() / RAND_MAX);//StartHorizontalRadian
        m_vRadian[1] = JCParticleData2D::lerp(p_pParticleSetting->m_nMinVerticalStartRadian, p_pParticleSetting->m_nMaxVerticalStartRadian, (float)rand() / RAND_MAX);//StartVerticleRadian
        bool  useEndRadian = p_pParticleSetting->m_bUseEndRadian;
        m_vRadian[2] = useEndRadian ? JCParticleData2D::lerp(p_pParticleSetting->m_nMinHorizontalEndRadian, p_pParticleSetting->m_nMaxHorizontalEndRadian, (float)rand() / RAND_MAX) : m_vRadian[0];//EndHorizontalRadian
        m_vRadian[3] = useEndRadian ? JCParticleData2D::lerp(p_pParticleSetting->m_nMinVerticalEndRadian, p_pParticleSetting->m_nMaxVerticalEndRadian, (float)rand() / RAND_MAX) : m_vRadian[1];//EndVerticleRadian

        m_nDurationAddScale = p_pParticleSetting->m_nAgeAddScale *((float)rand() / RAND_MAX);
        m_nTime = p_nTime;
    }

    float JCParticleData2D::lerp(float p_nLeft, float p_nRight, float p_nAmout)
    {
        return p_nLeft*(1 - p_nAmout) + p_nRight*p_nAmout;
    }

    void JCParticleData2D::subtractVector3(float* p_vL, float *p_vR, float *p_vO)
    {
        p_vO[0] = p_vL[0] - p_vR[0];
        p_vO[1] = p_vL[1] - p_vR[1];
        p_vO[2] = p_vL[2] - p_vR[2];
    }

    void JCParticleData2D::scaleVector3(float* p_vF, float p_nB, float*p_vE)
    {
        p_vE[0] = p_vF[0] * p_nB;
        p_vE[1] = p_vF[1] * p_nB;
        p_vE[2] = p_vF[2] * p_nB;
    }

    void JCParticleData2D::lerpVector4(float* p_vL, float* p_vR, float p_nT, float* p_vO)
    {
        float ax = p_vL[0], ay = p_vL[1], az = p_vL[2], aw = p_vL[3];
        p_vO[0] = ax + p_nT * (p_vR[0] - ax);
        p_vO[1] = ay + p_nT * (p_vR[1] - ay);
        p_vO[2] = az + p_nT * (p_vR[2] - az);
        p_vO[3] = aw + p_nT * (p_vR[3] - aw);
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------