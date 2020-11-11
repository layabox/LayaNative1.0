
/**
@file			JCDisplayRes.h
@brief			
@author			James
@version		1.0
@date			2016_5_26
*/

#ifndef __JCDisplayRes_H__
#define __JCDisplayRes_H__

#include <stdio.h>
#include <functional>
#include "../../imageLib/JCImageRW.h"
#include "../../geom/Rectangle.h"

namespace laya
{
    class JCTexture;
    
    enum BOARDER_TYPE
    {
        BOARDER_TYPE_NO = 0,
        BOARDER_TYPE_RIGHT = 1,
        BOARDER_TYPE_LEFT = 2,
        BOARDER_TYPE_BOTTOM = 4,
        BOARDER_TYPE_TOP = 8,
        BOARDER_TYPE_ALL = 15,
    };

    enum ENABLE_TYPE
    {
        ENALBE_TYPE_FAIL = 0,			    //失败
        ENABLE_TYPE_SUCCESS = 1,			//成功
        ENALBE_TYPE_RESOTRE_SUCCESS = 2,	//曾经失效过，又恢复成功了
    };

    /** 
     * @brief 
    */
    class JCDisplayRes
    {
    public:

        /** @brief构造函数
        */
        JCDisplayRes()
        {
            m_bNeedRefresh = true;
        }

        /** @brief析构函数
        */
        virtual ~JCDisplayRes()
        {

        }

        /** @brief enable
         *  @return 返回失败、成功、刚刚恢复回来
        */
        virtual ENABLE_TYPE enable()
        {
            return ENALBE_TYPE_FAIL;
        }
        virtual JCTexture* getTexture()
        {
            return NULL;
        }
        virtual bool mergeBitmapDataToTexture(BitmapData* pBitmapData, BOARDER_TYPE nBoarderFlag, int x, int y)
        {
            return false;
        }
        virtual bool isInAtlas()
        {
            return false;
        }
        void setRestoreFunction(std::function<bool(JCDisplayRes*)> pRestoreFunction)
        {
            m_pRestoreFunction = pRestoreFunction;
        }
        Rectangle* getRect()
        {
            return &m_kRect;
        }
        void setRect( float x,float y,float w,float h )
        {
            m_kRect.x = x;
            m_kRect.y = y;
            m_kRect.width = w;
            m_kRect.height = h;
        }

    public:
        bool                                m_bNeedRefresh;         ///<是否需要刷新
        Rectangle                           m_kRect;
        std::function<bool(JCDisplayRes*)>  m_pRestoreFunction;     ///<resotre函数
    };
}

#endif //__JCDisplayRes_H__

//-----------------------------END FILE--------------------------------