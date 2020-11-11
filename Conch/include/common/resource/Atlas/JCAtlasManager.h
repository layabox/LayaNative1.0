/**
@file			JCAtlasManager.h
@brief			
@author			James
@version		1.0
@date			2016_5_26
*/

#ifndef __JCAtlasManager_H__
#define __JCAtlasManager_H__

#include <vector>
#include "JCAtlas.h"
#include "../../imageLib/JCImageRW.h"

namespace laya
{
    class JCAtlasManager
    {
    public:

        JCAtlasManager( int nWidth, int nHeight, float nGridSize,int nMaxTexNum );

	    ~JCAtlasManager();

	    bool setAtlasParam( int nWidth, int nHeight, float nGridSize,int nMaxTexNum );

	    bool Initialize();

	    void setInAltasResValues( JCInAtlasRes* pInAltasRes,int nOffsetX,int nOffsetY,int nAtlasNum, BitmapData* pBitmapData);

	    bool pushData( JCInAtlasRes* pInAltasRes,BitmapData* pBitmapData );

	    bool toGarbageCollection();

	    void freeGLResource();

        void resetAtlas();

    public:

        int                     m_nMaxAtlasNum;
        int                     m_nWidth;
        int                     m_nHeight;
        float                   m_nGridSize;
        int                     m_nGridNumX;
        int                     m_nGridNumY;
        bool                    m_bInit;
        int                     m_nCurAtlasIndex;
	    std::vector<JCAtlas*>   m_vAtlasArray;
	    bool                    m_bNeedUploadToGPU;
	    bool                    m_bSetAtlasParam;//是否可以再修改参数，比如大图合集的宽、高、个数，这个设置只能在开始的时候才能使用，游戏中是不可以的

    };
}
//------------------------------------------------------------------------------


#endif //__JCAtlasManager_H__

//-----------------------------END FILE--------------------------------