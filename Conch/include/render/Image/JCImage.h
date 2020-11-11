/**
@file			JCImage.h
@brief			
@author			James
@version		1.0
@date			2016_5_13
*/

#ifndef __JCImage_H__
#define __JCImage_H__

#include <resource/DisplayRes/JCDisplayRes.h>
#include <resource/JCFileResManager.h>

namespace laya
{
    class JCAtlasManager;
    class JCResManager;
    class JCImageManager;
    /**
        * @brief
    */
    class JCImage
    {
    public:

        /** @brief构造函数
        */
        JCImage();

        /** @brief析构函数
        */
        ~JCImage();

        int getWidth();

        int getHeight();

        int getFormat();
        
        char* getImageData();

        ENABLE_TYPE enable();

        bool gpuRestoreRes( JCDisplayRes* pDisplayRes );

        void setManager(JCAtlasManager* pAltasManager,JCResManager* pTextureManager,JCFileResManager* pFileResManager,JCImageManager* pImageManager);

        void setImageID( int nID );

        void releaseTexture();

        bool enableBitmapData();

    public:
        static int          s_nMisoperationWarningTime;
        static int          s_nPushAtlasLimitSize;
        bool                m_bEnableMerageInAtlas;
        int64_t             m_nTouchTime;
        BitmapData          m_kBitmapData;
        std::string         m_sUrl;
        JCDisplayRes*       m_pDisplayRes;
        JCAtlasManager*     m_pAltasManager;
        JCResManager*       m_pTextureManager;
        JCFileResManager*   m_pFileResManager;
        JCImageManager*     m_pImageManager;
        bool                m_bPushBitmapData;      //是否为pushBitmapData的，正常的url为false，如果是js调用的pushBitmapData为true
    private:
        int                 m_nID;
        double              m_nReleaseTime;
    };
}
#endif //__JCImage_H__

//-----------------------------END FILE--------------------------------
