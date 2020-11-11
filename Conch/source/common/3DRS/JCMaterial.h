/**
@file			JCMaterial.h
@brief			
@author			James
@version		1.0
@date			2016_5_30
*/

#ifndef __JCMaterial_H__
#define __JCMaterial_H__

#include <stdint.h>
#include <vector>
#include "JCNamedData.h"
#include "../resource/DisplayRes/JCDisplayRes.h"
#include "../resource/DisplayRes/JCTexture.h"

namespace laya
{
    class JCGpuProgram;
    class JCMaterialBase {
    public:
        virtual unsigned int getKey() {
            return 0;
        }
    };
    enum BLEND_TYPE
    {
        BLEND_TYPE_NONE = -1,
        BLEND_TYPE_SOURCE_OVER = 0,
        BLEND_TYPE_SOURCE_ATOP = 1,
        BLEND_TYPE_SOURCE_IN = 2,
        BLEND_TYPE_SOURCE_OUT = 3,
        BLEND_TYPE_DESTINATION_OVER = 4,
        BLEND_TYPE_DESTINATION_ATOP = 5,
        BLEND_TYPE_DESTINATION_IN = 6,
        BLEND_TYPE_DESTINATION_OUT = 7,
        BLEND_TYPE_LIGHTER = 8,
        BLEND_TYPE_COPY = 9,
        BLEND_TYPE_XOR = 10,
        BLEND_TYPE_NORMAL = 11,
        BLEND_TYPE_ADD = 12,
        BLEND_TYPE_MULTIPLY = 13,
        BLEND_TYPE_SCREEN = 14,
        BLEND_TYPE_OVERLAY = 15,
        BLEND_TYPE_LIGHT = 16,
        BLEND_TYPE_MASK = 17
    };
    class JCMaterial : public JCMaterialBase
    {
    public:

        JCMaterial();

        ~JCMaterial();
        
        void setAlpha(float nAlpha);

        float getAlpha();

        inline JCGpuProgram* getGpuProgram() 
        {
            return m_pGPUProgram; 
        }

        inline bool getEnableZ()
        { 
            return m_bEnableZ>0; 
        }
        int getBlendType();

        void setBlendType(int blend);

		void setTargetBlendType(int blend);

        float* getFilterData();

        //后面的参数是因为考虑以后多层纹理的事情，现在先没用
        inline JCTexture* getTexture(int i=0) 
        {
            if (i < 0 || i>1)
                return nullptr;
            return m_pTexture[i];
        }
        virtual unsigned int getKey();

        void setShaderProgram(JCGpuProgram* pProg);

        inline JCGpuProgram* getShaderProgram()
        {
            return m_pGPUProgram; 
        }

        void setTexture(int id, JCTexture* pTex);

        void setFillColorShaderParam( float p_nAlpha, int p_nBlendType);

        void setDrawImageShaderParam(JCTexture* pTexture, float p_nAlpha, int p_nBlendType);

        void setFillImageShaderParam(JCTexture* pTexture, float p_nAlpha, int p_nBlendType,float nParam1,float nParam2,float nParam3,float nParam4 );

        void setDrawFilterImageShaderParam(JCTexture* pTexture, float p_nAlpha, int p_nBlendType, float p_nFilterR, float p_nFilterG, float p_nFilterB, float p_nFilterGray);

        virtual JCNamedData* getNamedData();

    public:

        unsigned int    m_nBlendSrcFactor;
        unsigned int    m_nBlendDstFacort;
		unsigned int    m_nAlphaSrcFactor;
        unsigned int    m_nAlphaDstFacort;
        
    public:

        //在这中间插入变量请跟 James说一下，很重要，因为是要hash值的
        //------------------------------------------------------------------------------
        JCTexture*      m_pTexture[2];                     //Texture
        JCGpuProgram*   m_pGPUProgram;                  //shader
        float           m_fAlpha;                       //alpha                                
        int             m_nBlendType;                   //混合方式                        
        float           m_pFilterData[4];               //变颜色                                        
        int             m_bEnableZ;                     //enableZ
        unsigned int    m_nKey;                         //其实是个 long long 64位 
        //------------------------------------------------------------------------------

    protected:

        int             m_nGpuProgramType;              //shader的类型决定了这个是材质的类型。       
        float           m_fColor[4];                    //for test                            
        float           m_pShaderParam1[4];             //保存shader参数用的    
        float           m_pShaderParam2[4];             //保存shader参数用的    
        bool            m_bChanged;
    };
}
//------------------------------------------------------------------------------


#endif //__JCMaterial_H__

//-----------------------------END FILE--------------------------------