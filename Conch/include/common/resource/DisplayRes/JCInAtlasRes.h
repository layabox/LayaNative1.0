/**
@file			JCInAtlasRes.h
@brief			
@author			James
@version		1.0
@date			2016_5_26
*/

#ifndef __JCInAtlasRes_H__
#define __JCInAtlasRes_H__

#include "JCDisplayRes.h"

namespace laya
{
    class JCAtlas;
	//在大图集中的一个资源。属于大图集的一部分。
	class JCInAtlasRes : public JCDisplayRes
	{
	public:

        JCInAtlasRes();

		virtual ~JCInAtlasRes();

	public:
		
        virtual void setAtlas(JCAtlas* pAtlas);

        virtual ENABLE_TYPE enable();

        virtual JCTexture* getTexture();

        virtual bool mergeBitmapDataToTexture(BitmapData* pBitmapData, BOARDER_TYPE nBoarderFlag,int x,int y);

        virtual bool isInAtlas();

    public:

		JCAtlas*					m_pAtlas;
	};
}
//------------------------------------------------------------------------------


#endif //__JCInAtlasRes_H__

//-----------------------------END FILE--------------------------------