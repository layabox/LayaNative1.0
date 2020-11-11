/**
@file			JCParticleTemplate2D.cpp
@brief			
@author			hugao
@version		1.0
@date			2016_10_31
*/

//包含头文件
#include "JCParticleTemplate2D.h"
#include <stdlib.h>
#include <algorithm>


//------------------------------------------------------------------------------
namespace laya
{
    //------------------------------------------------------------------------------
    JCParticleTemplate2D::JCParticleTemplate2D(int p_nID)
    {
        m_nImageID = -1;
        m_nID = p_nID;
        m_nFloatCountPerVertex = 29;
        m_nCurrentTime = 0;
        m_nFirstActiveElement = 0;
        m_nFirstFreeElement = 0;
        m_nFirstRetiredElement = 0;
        m_nFirstNewElement = 0;
        m_nDrawCounter = 0;
        m_nShaderID = 1024;//shader2dID 为1024;
        m_pVetexBuffer2D = NULL;
        m_pIndexBuffer2D = NULL;
        m_pParticleSetting = new JCParticleSetting2D();
       // m_pParticleSetting->m_nMaxPartices = 120;
        //m_pParticleSetting->m_nTextureCount = 1;
        m_pParticleData = new JCParticleData2D();

        m_fEmissionTime = 1.7976931348623157e+308;
        m_frameTime = 0;
        m_fMinEmissionTime = 1.0 / 60.0;
        m_fEmissionRate = 60;
        m_bStart = false;
    }


    JCParticleTemplate2D::~JCParticleTemplate2D()
    {
		if (m_pParticleSetting != NULL)
		{
			delete m_pParticleSetting;
			m_pParticleSetting = NULL;
		}
		if (m_pParticleData != NULL)
		{
			delete m_pParticleData;
			m_pParticleData = NULL;
		}
        if (m_pVetexBuffer2D != NULL)
        {
            delete [] m_pVetexBuffer2D;
            m_pVetexBuffer2D = NULL;
        }
        if (m_pIndexBuffer2D != NULL)
        {
            delete[] m_pIndexBuffer2D;
            m_pIndexBuffer2D = NULL;
        }
    }

    void JCParticleTemplate2D::play()
    {
        m_bStart = true;
    }

    void JCParticleTemplate2D::stop()
    {
        m_bStart = false;
    }

    void JCParticleTemplate2D::initialize()
    {
        int size = m_pParticleSetting->m_nMaxPartices*m_nFloatCountPerVertex * 16;
        if (m_pVetexBuffer2D != NULL)
        {
            delete [] m_pVetexBuffer2D;
            m_pVetexBuffer2D = NULL;
        }
        m_pVetexBuffer2D = new char[size];
        memset(m_pVetexBuffer2D, 0, size);
        int particleOffset;
        float cornerY=0;
        float cornerYSegement = m_pParticleSetting->m_nTextureCount != 0 ? 1.0 / (float)m_pParticleSetting->m_nTextureCount : 1.0;
        float * tempBuffer = (float *)m_pVetexBuffer2D;
        for (int i=0; i < m_pParticleSetting->m_nMaxPartices; i++)
        {
            float r = ((float)rand()/RAND_MAX);//产生0到1之间的小数
            for (cornerY; cornerY < m_pParticleSetting->m_nTextureCount; cornerY += cornerYSegement)
            {
                if (r < cornerY + cornerYSegement)
                    break;
            }
            particleOffset = i*m_nFloatCountPerVertex * 4;
            tempBuffer[particleOffset] = -1;
            tempBuffer[particleOffset + 1] = -1;
            tempBuffer[particleOffset + 2] = 0;
            tempBuffer[particleOffset + 3] = cornerY;

            tempBuffer[particleOffset + m_nFloatCountPerVertex] = 1;
            tempBuffer[particleOffset + m_nFloatCountPerVertex + 1] = -1;
            tempBuffer[particleOffset + m_nFloatCountPerVertex + 2] = 1;
            tempBuffer[particleOffset + m_nFloatCountPerVertex + 3] = cornerY;

            tempBuffer[particleOffset + m_nFloatCountPerVertex * 2] = 1;
            tempBuffer[particleOffset + m_nFloatCountPerVertex * 2 + 1] = 1;
            tempBuffer[particleOffset + m_nFloatCountPerVertex * 2 + 2] = 1;
            tempBuffer[particleOffset + m_nFloatCountPerVertex * 2 + 3] = cornerY + cornerYSegement;

            tempBuffer[particleOffset + m_nFloatCountPerVertex * 3] = -1;
            tempBuffer[particleOffset + m_nFloatCountPerVertex * 3 + 1] = 1;
            tempBuffer[particleOffset + m_nFloatCountPerVertex * 3 + 2] = 0;
            tempBuffer[particleOffset + m_nFloatCountPerVertex * 3 + 3] = cornerY + cornerYSegement;
        }
    }

    void JCParticleTemplate2D::rebuildUV(float *pUv)
    {
        if (pUv == NULL)return;
        if (m_vUV[0] != pUv[0] || m_vUV[1] != pUv[1] || m_vUV[2] != pUv[2] || m_vUV[3] != pUv[3])
        {
            float * tempBuffer = (float *)m_pVetexBuffer2D;
            float rectw = pUv[2] - pUv[0];
            float recth = pUv[3] - pUv[1];

            float beforeRectW = m_vUV[2] - m_vUV[0];
            float beforeRectH= m_vUV[3] - m_vUV[1];

            for (int i = 0,n= m_pParticleSetting->m_nMaxPartices * 4; i <n ; i++)
            {
                int j = i*m_nFloatCountPerVertex;
                tempBuffer[j + 2] = ((tempBuffer[j + 2] - m_vUV[0]) / beforeRectW)* rectw + pUv[0];
                tempBuffer[j + 3] = ((tempBuffer[j + 3] - m_vUV[1]) / beforeRectH)* recth + pUv[1];
            }
            memcpy(m_vUV, pUv, 16);
        }
    }

    void JCParticleTemplate2D::initSetting(char * p_pSetting)
    {
        int size = sizeof(JCParticleSetting2D);
        memcpy(m_pParticleSetting, p_pSetting, size);
        loadContent();
        initialize();
    }


    void JCParticleTemplate2D::loadContent()
    {
        if (m_pIndexBuffer2D != NULL)
        {
            delete[] m_pIndexBuffer2D;
            m_pIndexBuffer2D = NULL;
        }
        m_pIndexBuffer2D = new char[m_pParticleSetting->m_nMaxPartices * 12];
        unsigned short *temp = (unsigned short *)m_pIndexBuffer2D;
        for (int i = 0; i < m_pParticleSetting->m_nMaxPartices;i++)
        {
            temp[i * 6 + 0] = (i * 4);
            temp[i * 6 + 1] = (i * 4 + 1);
            temp[i * 6 + 2] = (i * 4 + 2);
            temp[i * 6 + 3] = (i * 4);
            temp[i * 6 + 4] = (i * 4 + 2);
            temp[i * 6 + 5] = (i * 4 + 3);
        }
    }

    void JCParticleTemplate2D::update(int p_nElapsedTime)
    {
        m_nCurrentTime += p_nElapsedTime / 1000.0;
        retireActiveParticles();
        freeRetiredParticles();
        if (m_nFirstActiveElement == m_nFirstFreeElement)
            m_nCurrentTime = 0;
        if (m_nFirstRetiredElement == m_nFirstActiveElement)
            m_nDrawCounter = 0;
    }

    void JCParticleTemplate2D::retireActiveParticles()
    {
        float *_vertices = (float *)m_pVetexBuffer2D;
        while (m_nFirstActiveElement != m_nFirstNewElement) {
            int index = m_nFirstActiveElement * m_nFloatCountPerVertex * 4 + 28;//28为Time
            float particleAge =m_nCurrentTime - _vertices[index];
            if (particleAge < m_pParticleSetting->m_nDuration)
                break;

            _vertices[index] = m_nDrawCounter;

            m_nFirstActiveElement++;

            if (m_nFirstActiveElement >= m_pParticleSetting->m_nMaxPartices)
                m_nFirstActiveElement = 0;
        }
    }

    void JCParticleTemplate2D::freeRetiredParticles()
    {
        float * _vertices = (float *)m_pVetexBuffer2D;
        while (m_nFirstRetiredElement != m_nFirstActiveElement) {

           float age = m_nDrawCounter - _vertices[m_nFirstRetiredElement * m_nFloatCountPerVertex * 4 + 28];//28为Time,注意Numver到Int类型转换,JS中可忽略
                                                                                                           //GPU从不滞后于CPU两帧，出于显卡驱动BUG等安全因素考虑滞后三帧
            if (age < 3.0)
                break;

            m_nFirstRetiredElement++;

            if (m_nFirstRetiredElement >= m_pParticleSetting->m_nMaxPartices)
                m_nFirstRetiredElement = 0;
        }
    }

    void JCParticleTemplate2D::addNewParticlesToVertexBuffer()
    {
        m_nFirstNewElement = m_nFirstFreeElement;
    }

    void JCParticleTemplate2D::addParticleArray(float* p_pPosition, float* p_pVelocity)
    {
        float* _vertices = (float *)m_pVetexBuffer2D;
        int nextFreeParticle = m_nFirstFreeElement + 1;
        if (nextFreeParticle >= m_pParticleSetting->m_nMaxPartices)
            nextFreeParticle = 0;
        if (nextFreeParticle == m_nFirstRetiredElement)
            return;

        //var particleData:ParticleData = ParticleData.Create(settings, position, velocity, _currentTime);
        m_pParticleData->initData(m_pParticleSetting, p_pPosition, p_pVelocity,m_nCurrentTime);
        
        int startIndex = m_nFirstFreeElement*m_nFloatCountPerVertex * 4;
        for (int i = 0; i < 4; i++)
        {
            int j, offset;
            for (j = 0, offset = 4; j < 3; j++)
                _vertices[startIndex + i * m_nFloatCountPerVertex + offset + j] = m_pParticleData->m_vPosition[j];

            for (j = 0, offset = 7; j < 3; j++)
                _vertices[startIndex + i * m_nFloatCountPerVertex + offset + j] = m_pParticleData->m_vVelocity[j];

            for (j = 0, offset = 10; j < 4; j++)
				_vertices[startIndex + i * m_nFloatCountPerVertex + offset + j] = m_pParticleData->m_vStartColor[j];

            for (j = 0, offset = 14; j < 4; j++)
				_vertices[startIndex + i * m_nFloatCountPerVertex + offset + j] = m_pParticleData->m_vEndColor[j];


            for (j = 0, offset = 18; j < 3; j++)//StartSize,EndSize,Rotation
                _vertices[startIndex + i * m_nFloatCountPerVertex + offset + j] = m_pParticleData->m_vSizeRotation[j];

            for (j = 0, offset = 21; j < 2; j++)//StartRadius,EndRadius
                _vertices[startIndex + i * m_nFloatCountPerVertex + offset + j] = m_pParticleData->m_vRadius[j];

            for (j = 0, offset = 23; j < 4; j++)//StartHorizontalRadian,StartVerticleRadian,EndHorizontalRadian,EndVerticleRadian
                _vertices[startIndex + i * m_nFloatCountPerVertex + offset + j] = m_pParticleData->m_vRadian[j];

            _vertices[startIndex + i * m_nFloatCountPerVertex + 27] = m_pParticleData->m_nDurationAddScale;

            _vertices[startIndex + i * m_nFloatCountPerVertex + 28] = m_pParticleData->m_nTime;

        }
        m_nFirstFreeElement = nextFreeParticle;

    }

    void JCParticleTemplate2D::loop(float p_nX,float p_nY,float*pUv)
    {
        rebuildUV(pUv);
        if (m_bStart)
        {
           advanceTime(1.0 / 60.0, p_nX, p_nY);
        }
        update(1000/60.0);
        if (m_nFirstNewElement != m_nFirstFreeElement)
        {
            addNewParticlesToVertexBuffer();
        }
        m_nDrawCounter++;
    }

    float JCParticleTemplate2D::getRandom(float p_nValue)
    {
        return (((float)rand() / RAND_MAX) * 2 - 1)*p_nValue;
    }

    void JCParticleTemplate2D::emit(float p_nX, float p_nY)
    {
        float position[3] = { 0,0,0 };
        position[0] = getRandom(m_pParticleSetting->m_vPositionVariance[0]);
        position[1] = getRandom(m_pParticleSetting->m_vPositionVariance[1]);
        position[2] = getRandom(m_pParticleSetting->m_vPositionVariance[2]);
        float temp[3] = { 0,0,0 };
        position[0] += p_nX;
        position[1] += p_nY;
        addParticleArray(position,temp);
    }

    void JCParticleTemplate2D::advanceTime(float p_fPassedTime,float p_nX,float p_nY)
    {
        m_fEmissionTime -= p_fPassedTime;
        if (m_fEmissionTime < 0) return;
        m_frameTime += p_fPassedTime;
        if (m_frameTime < m_fMinEmissionTime) return;
        while (m_frameTime > m_fMinEmissionTime) {
            m_frameTime -= m_fMinEmissionTime;
            emit(p_nX,p_nY);
        }
    }
}
//------------------------------------------------------------------------------


//-----------------------------END FILE--------------------------------