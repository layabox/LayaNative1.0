/**
@file			JCTextManager.h
@brief			
@author			James
@version		1.0
@date			2016_6_2
*/

#ifndef __JCTextManager_H__
#define __JCTextManager_H__

#include <map>
#include <unordered_map>
#include "../DisplayRes/JCWordRes.h"
#include <string>  
#include <iostream>  
#include <unordered_map>  
#include <functional>
#include "../../util/JCCommonMethod.h"
namespace laya
{
    /** 
     * @brief 
    */
    class JCAtlasManager;
    class JCFreeTypeFontRender;




    struct WordResKey
    {
        int m_nFontId;
        int m_nColor;
        std::string  m_sWord;
        short m_nScaledFontSizeX;
        short m_nScaledFontSizeY;
        WordResKey(int nFontId, int nColor, const std::string& sWord, short nScaledFontSizeX, short nScaledFontSizeY)
            : m_nFontId(nFontId), m_nColor(nColor), m_sWord(sWord), m_nScaledFontSizeX(nScaledFontSizeX), m_nScaledFontSizeY(nScaledFontSizeY)
        {}
        bool operator==(const WordResKey& ins) const
        {
            return m_nFontId == ins.m_nFontId && 
                m_nColor == ins.m_nColor && 
                m_sWord == ins.m_sWord && 
                m_nScaledFontSizeX == ins.m_nScaledFontSizeX &&
                m_nScaledFontSizeY == ins.m_nScaledFontSizeY;
        }
    };

    inline bool operator< (const WordResKey& left, const WordResKey& right)
    {
        if (left.m_nFontId != right.m_nFontId)
        {
            return left.m_nFontId < right.m_nFontId;
        }

        if (left.m_nColor != right.m_nColor)
        {
            return left.m_nColor < right.m_nColor;
        }

        if (left.m_sWord != right.m_sWord)
        {
            return left.m_sWord < right.m_sWord;
        }

        if (left.m_nScaledFontSizeX != right.m_nScaledFontSizeX)
        {
            return left.m_nScaledFontSizeX < right.m_nScaledFontSizeX;
        }

        if (left.m_nScaledFontSizeY != right.m_nScaledFontSizeY)
        {
            return left.m_nScaledFontSizeY < right.m_nScaledFontSizeY;
        }

        return false;
    }


    
    struct WordResKey_Hash
    {
        size_t operator()(const WordResKey& ins) const
        {
            size_t seed = 0;
            hash_combine(seed, std::hash<int>{}(ins.m_nFontId));
            hash_combine(seed, std::hash<int>{}(ins.m_nColor));
            hash_combine(seed, std::hash<std::string>{}(ins.m_sWord));
            hash_combine(seed, std::hash<short>{}(ins.m_nScaledFontSizeX));
            hash_combine(seed, std::hash<short>{}(ins.m_nScaledFontSizeY));
            return seed;
        }
    };



    class JCTextManager
    {
    public:

        typedef std::unordered_map<WordResKey, JCWordRes*, WordResKey_Hash> MapWordRes;

    public:

        /** @brief构造函数
        */
        JCTextManager(JCAtlasManager* pAtlasManager);

        /** @brief析构函数
        */
        ~JCTextManager();

        /** @brief 获得wodRes，先去map中查找，如果有直接返回，如果没有再创建新的
         *  @param[in] FonfInfo
         *  @param[in] 文字的颜色
         *  @param[in] 单个文字
         *  @return wordRes指针
        */
        JCWordRes* getWordRes(JCFontInfo* pFontInfo, int nColor, const char* pWord, float scaleX, float scaleY);

        void setFreeTypeRender(JCFreeTypeFontRender* pFreeTypeRender);

        void clearAll();

    public:

        JCAtlasManager*         m_pAtlasManager;
        JCFreeTypeFontRender*   m_pFreeTypeRender;
        MapWordRes              m_vWordRes;

    };
}

#endif //__JCTextManager_H__

//-----------------------------END FILE--------------------------------
