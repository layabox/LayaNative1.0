/**
@file			JCParticleTemplate2D.h
@brief			
@author			hugao
@version		1.0
@date			2016_10_31
*/

#ifndef __JCParticleTemplate2D_H__
#define __JCParticleTemplate2D_H__


//包含头文件
#include "JCParticleData2D.h"
#include "JCParticleSetting2D.h"
//------------------------------------------------------------------------------
namespace laya
{
    /**
        * @brief
    */
    class JCParticleTemplate2D
    {
    public:

        /** @brief构造函数
        */
        JCParticleTemplate2D(int p_nId);

        /** @brief析构函数
        */
        ~JCParticleTemplate2D();
    public:
        void initialize();

        void loadContent();

        void update(int p_nElapsedTime);
        
        void retireActiveParticles();

        void freeRetiredParticles();

        void addNewParticlesToVertexBuffer();

        void addParticleArray(float* p_pPosition, float* p_pVelocity);

        int  render();

        void loop(float p_nX,float p_nY,float* pUV);

        void emit(float p_nX, float p_nY);

        void initSetting(char * p_pSetting);

        float getRandom(float p_nValue);

        void advanceTime(float p_fPassedTime,float p_nX,float p_nY);

        void rebuildUV(float *pUv);

        void play();

        void stop();

    public:
        char* m_pVetexBuffer2D;
        char* m_pIndexBuffer2D;
        float m_nX;
        float m_nY;
        int   m_nFloatCountPerVertex;
        int   m_nShaderID;
        int   m_nID;
        int   m_nImageID;
        //param
        int   m_nFirstActiveElement;
        int   m_nFirstNewElement;
        int   m_nFirstFreeElement;
        int   m_nFirstRetiredElement;
        int   m_nDrawCounter;
        //uniform
        float m_nCurrentTime;
        float m_vUV[4] = {0,0,1,1};

        JCParticleData2D *    m_pParticleData;
        JCParticleSetting2D * m_pParticleSetting;
    private:
        float m_fEmissionTime;
        float m_fMinEmissionTime;
        float m_fEmissionRate;
        float m_frameTime;
        bool  m_bStart;

    };
}
//------------------------------------------------------------------------------


#endif //__JCParticleTemplate2D_H__

//-----------------------------END FILE--------------------------------