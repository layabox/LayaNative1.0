/**
@file			JCAtlas.h
@brief			
@author			James
@version		1.0
@date			2016_5_26
*/

#ifndef __JCAtlas_H__
#define __JCAtlas_H__

#include "JCAtlasGrid.h"
#include "../../imageLib/JCImageRW.h"
#include "../DisplayRes/JCInAtlasRes.h"
#include "../DisplayRes/JCTexture.h"
#include <map>
#include <functional>

namespace laya
{
	class JCAtlas:public JCAtlasGrid
	{
    public:

        typedef std::map<int, std::function<void(JCAtlas*)>> MapFunctions;

	public:
		
        JCAtlas( int nGridNumX, int nGridNumY, int nWidth, int nHeight );
		
        ~JCAtlas();

		bool mergeBitmapData( BitmapData* pBitmapData, BOARDER_TYPE nBoarderFlag,int offsetX, int offsetY );
		
        void clear();
		
        void destroy();

		void pushInAtlasRes( JCInAtlasRes* pInAtlasRes );
		
        bool removeInAtlasRes( JCInAtlasRes* pInAtlasRes );

        void setFunction(int nIndex, std::function<void(JCAtlas*)> pFunction);

        void removeFunction(int nIndex );
	
    public:

        MapFunctions                m_vMapFunctions;

		JCTexture*                  m_pTexture;

		std::vector<JCInAtlasRes*>  m_vInAtlasRes;

	};
}
//------------------------------------------------------------------------------


#endif //__JCAtlas_H__

//-----------------------------END FILE--------------------------------