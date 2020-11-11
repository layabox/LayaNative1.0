/**
@file			JCMeasureTextManager.h
@brief			为了提高效率，把Measure完的文字宽高都保存下来，只能在JS线程使用
@author			James
@version		1.0
@date			2016_6_7
*/

#ifndef __JCMeasureTextManager_H__
#define __JCMeasureTextManager_H__

#include <unordered_map>
#include "JCFreeTypeRender.h"
#include "../resource/text/JCFontInfo.h"
#include <vector>

namespace laya
{
    /** 
     * @brief 
    */
    class JCFreeTypeFontRender;
    class JCMeasureTextManager
    {
    public:

        struct WordSize
        {
            WordSize()
            {
                width = 0;
                height = 0;
            }
            int width;
            int height;
        };

    public:

        typedef std::unordered_map<std::string, WordSize*>   MapWord;
        typedef std::vector<MapWord*>                        VectorMapWord;

    public:

        /** @brief构造函数
        */
        JCMeasureTextManager();

        /** @brief析构函数
        */
        ~JCMeasureTextManager();

        /** @brief 获得textSize
         *  @param[in] fontInfo
         *  @param[in] 字符串
         *  @return 返回宽度和高度
        */
        bool measureText(JCFontInfo* pFontInfo, const char* sText,int& nResultWidth,int& nResultHeight );

        /** @brief 获得wodRes，先去map中查找，如果有直接返回，如果没有再创建新的
         *  @param[in] FontInfo
         *  @param[in] 单个文字
         *  @return wordRes指针
        */
        JCMeasureTextManager::WordSize* getWordSize(JCFontInfo* pFontInfo, const char* sWord);

        void setFreeTypeFontRender(JCFreeTypeFontRender* pFreeTypeFontRender);
        
        void clearAll();

    public:

        JCFreeTypeFontRender*   m_pFreeTypeRender;
        VectorMapWord           m_vWordInfo;

    };
}
//------------------------------------------------------------------------------


#endif //__JCMeasureTextManager_H__

//-----------------------------END FILE--------------------------------
