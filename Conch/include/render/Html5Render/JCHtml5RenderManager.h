/**
@file			JCHtml5RenderManager.h
@brief			
@author			James
@version		1.0
@date			2016_6_1
*/

#ifndef __JCHtml5RenderManager_H__
#define __JCHtml5RenderManager_H__

#include <unordered_map>

namespace laya
{
    /** 
     * @brief 
    */
    class JCHtml5Render;
    class JCHtml5RenderManager
    {
    public:

        typedef std::unordered_map<int,JCHtml5Render*>     MapHtml5Render;

    public:

        /** @brief构造函数
        */
        JCHtml5RenderManager();

        /** @brief析构函数
        */
        ~JCHtml5RenderManager();

        void pushHtml5Render(JCHtml5Render* pHtml5Render);

        bool removeHtml5Render( int nID );

        JCHtml5Render* getHtml5Render( int nID );

        bool invalidGLRes();

        void clearAll();

    protected:

        MapHtml5Render        m_vHtml5Renders;

    };
}

#endif //__JCHtml5RenderManager_H__

//-----------------------------END FILE--------------------------------
