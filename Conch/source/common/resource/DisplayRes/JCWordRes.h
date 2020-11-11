/**
@file			JCWordRes.h
@brief			
@author			James
@version		1.0
@date			2016_6_2
*/

#ifndef __JCWordRes_H__
#define __JCWordRes_H__

#include "JCInAtlasRes.h"
#include "../text/JCFontInfo.h"
#include <string.h>

namespace laya
{
    /** 
     * @brief 
    */
    class JCFreeTypeFontRender;
    class JCAtlas;
    class JCAtlasManager;
    class JCWordRes : public JCInAtlasRes
    {
    public:

        /** @brief构造函数
        */
        JCWordRes(JCAtlasManager* pAtlasManager,JCFreeTypeFontRender* pFreeTypeRender,JCFontInfo* pFont,int nColor,const char* sWord, float scaleX, float scaleY);

        /** @brief析构函数
        */
        virtual ~JCWordRes();

        /** @brief restoreRes恢复用的
         *  @return 成功或者失败
        */
        bool gpuRestoreRes(JCDisplayRes* pDisplayRes);

        Rectangle* getRectNoScaled()
        {
            return &m_kRectNoScaled;
        }
    public:

        JCAtlasManager*         m_pAtlasManager;
        JCFreeTypeFontRender*   m_pFreeTypeRender;
        JCFontInfo*             m_pFont;
        int                     m_nColor;
        std::string             m_sWord;
        Rectangle               m_kRectNoScaled;
        float                   m_fScaleX;
        float                   m_fScaleY;
    };
}

#endif //__JCWordRes_H__

//-----------------------------END FILE--------------------------------
