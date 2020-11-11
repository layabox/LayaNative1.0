
#ifndef _LAYA_RECT_GEOMETRY2_H__
#define _LAYA_RECT_GEOMETRY2_H__

namespace laya{
	/*
		二维Rect，min在左上角，max在右下角
	    0-----1
		|     |
		3-----2
	*/
	class RectGeometry2{
	public:
		struct Vertex{
			float x,y,u,v,u1,v1;
		};
		bool m_bShow;
		float nx1,ny1;			//缺省的起点。对应vertRect[0].xy
		float nWidth,nHeight;	//框高，可以为负的，负表示进行水平或者垂直镜像了。
		Vertex  vertRect[4];
		//------------------------------------------------------------------------------
		RectGeometry2(){
			nx1=ny1=nWidth=nHeight=0;
			m_bShow = true;
		}
		
		void copy( const RectGeometry2& src ){
			*this = src;
		}
		
		//TODO 颜色
		void create(float x1, float y1, float w, float h, float tx1, float ty1, float tx2, float ty2,float tx3, float ty3, float tx4, float ty4 )
		{
			m_bShow = true;
			nx1 = x1;
			ny1 = y1;
			nWidth = w;
			nHeight = h;

			float x2=x1+w;
			float y2=y1+h;

			vertRect[ 0].x = x1;
			vertRect[ 0].y = y1;
			vertRect[ 0].u = tx1;
			vertRect[ 0].v = ty1;
			vertRect[ 0].u1 = tx3;
			vertRect[ 0].v1 = ty3;

			vertRect[ 1].x = x2;
			vertRect[ 1].y = y1;
			vertRect[ 1].u = tx2;
			vertRect[ 1].v = ty1;
			vertRect[ 1].u1 = tx4;
			vertRect[ 1].v1 = ty3;

			vertRect[ 2].x = x2;
			vertRect[ 2].y = y2;
			vertRect[ 2].u = tx2;
			vertRect[ 2].v = ty2;
			vertRect[ 2].u1 = tx4;
			vertRect[ 2].v1 = ty4;

			vertRect[ 3].x = x1;
			vertRect[ 3].y = y2;
			vertRect[ 3].u = tx1;
			vertRect[ 3].v = ty2;
			vertRect[ 3].u1 = tx3;
			vertRect[ 3].v1 = ty4;
		}

		//------------------------------------------------------------------------------
		void applyTranslate( float tx, float ty){
			if(tx==0&&ty==0)
				return;
			for( int i = 0; i < 4; i++ ){
				vertRect[i].x+=tx;
				vertRect[i].y+=ty;
			}
			nx1 += tx;	//x
			ny1 += ty;	//y
		}
		//------------------------------------------------------------------------------
		/*
			创建一个带有纹理坐标的Rect
			@param x1 Rect的左上角位置
			@param y1 Rect的左上角文职
			@param w 	Rect的宽度
			@param h 	Rect的高度
			@param tx1,ty1 左上角的纹理坐标
			@param tx2,ty2 右下角的纹理坐标
		*/
		void createForUV( float x1, float y1, float w, float h, float tx1, float ty1, float tx2, float ty2,float tx3, float ty3, float tx4, float ty4 )
		{
			m_bShow = true;
			nx1 = x1;
			ny1 = y1;
			nWidth = w;
			nHeight = h;
			float x2= x1+w;
			float y2= y1+h;
			
			vertRect[0].x = x1;
			vertRect[0].y = y1;
			vertRect[0].u = tx1;
			vertRect[0].v = ty1;
			vertRect[0].u1 = tx3;
			vertRect[0].v1 = ty3;

			vertRect[1].x = x2;
			vertRect[1].y = y1;
			vertRect[1].u = tx2;
			vertRect[1].v = ty1;
			vertRect[1].u1 = tx4;
			vertRect[1].v1 = ty3;
			
			vertRect[2].x = x2;
			vertRect[2].y = y2;
			vertRect[2].u = tx2;
			vertRect[2].v = ty2;
			vertRect[2].u1 = tx4;
			vertRect[2].v1 = ty4;
			
			vertRect[3].x = x1;
			vertRect[3].y = y2;
			vertRect[3].u = tx1;
			vertRect[3].v = ty2;
			vertRect[3].u1 = tx3;
			vertRect[3].v1 = ty4;
		}
		
		//------------------------------------------------------------------------------
		void ScaleUV(float x, float y){
			for( int i=0; i<4; i++ ){
				vertRect[ i].u*=x;
				vertRect[ i].v*=y;
				vertRect[ i].u1*=x;
				vertRect[ i].v1*=y;
			}
		}
		//------------------------------------------------------------------------------
		void OffUV(float x, float y){
			for( int i=0; i<4; i++ ){
				vertRect[ i].u+=x;
				vertRect[ i].v+=y;
				vertRect[ i].u1+=x;
				vertRect[ i].v1+=y;
			}
		}
		//------------------------------------------------------------------------------
		void applyMatrix(float m11,float m12, float m21, float m22, float m31, float m32){
			for( int i=0; i<4; i++ ){
				float cx = vertRect[ i].x;
				float cy = vertRect[ i].y;

				float rx = cx*m11 + cy*m21 + m31;
				float ry = cx*m12 + cy*m22 + m32;

				vertRect[ i].x = rx;
				vertRect[ i].y = ry;
			}
			//TODO 有问题么?			
			nWidth*=m11;
			nHeight*=m22;
			nx1=vertRect[0].x;
			ny1=vertRect[0].y;
		}

		void applyMatrix(float* m ){
			for( int i=0; i<4; i++ ){
				float& cx = vertRect[ i].x;
				float& cy = vertRect[ i].y;

				float rx = cx*m[0] + cy*m[2] + m[4];
				float ry = cx*m[1] + cy*m[3] + m[5];

				cx = rx;
				cy = ry;
			}
			//TODO 有问题么?			
			nWidth*=m[0];
			nHeight*=m[3];
			nx1=vertRect[0].x;
			ny1=vertRect[0].y;
		}
		
		void SwapVert( int p_a, int p_b )
		{
			Vertex tmp = vertRect[p_a];
			vertRect[p_a]=vertRect[p_b];
			vertRect[p_b]=tmp;
		}

		//------------------------------------------------------------------------------
		/*
			整理一下。保证0在左上角，2在有下家
		*/
		void Normalize()
		{
			if( nWidth<0)
			{
				SwapVert(0,1);
				SwapVert(3,2);
				nx1 = vertRect[0].x;
				nWidth=-nWidth;
			}
        
			if( nHeight<0)
			{
				SwapVert(0,3);
				SwapVert(1,2);
				ny1 = vertRect[0].y;
				nHeight=-nHeight;
			}
		}
		
		void applyOnlyTranslate( int p_nX, int p_nY)
		{
			
		}
		
		/*
			如果翻转了，需要修改nWidth和nHeight并保证为正的。需要修改nx1,ny1
		*/
		void applyOnlyScale( float p_nX, float p_nY)
		{
			
		}
		//------------------------------------------------------------------------------
		/*
			这个不会修改nWidth和nHeight
		*/
		void TransToDevice(float viewW, float viewH)
		{
			for( int i=0; i<4; i++ )
			{
				float& cx = vertRect[ i].x;
				float& cy = vertRect[ i].y;

				cx = 2.0f*cx/viewW-1.0f;
				cy = 1.0f-2.0f*cy/viewH;
			}
		}

		//------------------------------------------------------------------------------
		/*
			用一个AABB对矩形进行裁剪。要求矩形本身没有进行旋转（可以是被缩放过的）<br>
			会直接修改裁剪的矩形<br>
			当返回false的时候，传入的p_pGeo不会被修改。
			@param p_pGeo 要裁剪的对象
			@param p_ClipRect 裁剪区域
			@param p_bClipTexcoord 是否裁剪纹理坐标。true裁剪
			@return true表示矩形和裁剪区域有交集。
			@TODO 这里有int和float的转换的问题。
     
			测试：
			对于被翻转的Rect有问题么
			缩放后的裁剪测试
			*/
		static bool clipWithRect(RectGeometry2* p_pGeo, float p_ClipL, float p_ClipT, float p_ClipW, float p_ClipH, bool p_bClipTexcoord )
		{
			if(p_pGeo->nWidth<=0 || p_pGeo->nHeight<=0)
				p_pGeo->Normalize();
        
			if( p_pGeo->nWidth<=0 || p_pGeo->nHeight<=0 )
				return false;
			float cr = p_ClipL+p_ClipW;
			float cb = p_ClipT+p_ClipH;
			if( p_pGeo->nx1>cr || p_pGeo->ny1>cb )
				return false;
			float sr = (p_pGeo->nx1 + p_pGeo->nWidth);
			float sb = (p_pGeo->ny1 + p_pGeo->nHeight);
			if( sr<p_ClipL || sb<p_ClipT )
				return false;
			float sx=(p_pGeo->nx1);		//裁剪后的左边
			if( p_pGeo->nx1<p_ClipL ) sx=p_ClipL;
			float sy=(p_pGeo->ny1);		//裁剪后的上边
			if( p_pGeo->ny1<p_ClipT ) sy = p_ClipT;
			float ex = sr;	//裁剪后的右边
			if( sr>cr) ex=cr;
			float ey = sb;	//裁剪后的下边
			if( sb>cb) ey=cb;
        
			float w = ex-sx;
			float h = ey-sy;
			
			float texl=p_pGeo->vertRect[0].u;		//[0].u
			float text=p_pGeo->vertRect[0].v;		//[0].v
			float texr=p_pGeo->vertRect[1].u;		//[1].u
			float texb=p_pGeo->vertRect[3].v;		//[3].v
			if( p_bClipTexcoord )
			{
				float texw=p_pGeo->vertRect[1].u-texl;
				float texh=p_pGeo->vertRect[3].v-text;
            
				//如果水平被裁剪了
				if(w<p_pGeo->nWidth)
				{
					float dx = sx-p_pGeo->nx1;
					texl += dx*texw/p_pGeo->nWidth;
					texw *= w/p_pGeo->nWidth;
				}
				//如果垂直被裁剪了
				if(h<p_pGeo->nHeight)
				{
					float dy = sy-p_pGeo->ny1;
					text += dy*texh/p_pGeo->nHeight;
					texh *= h/p_pGeo->nHeight;
				}
				texr = texl+texw;
				texb = text+texh;
			}

			float texl2=p_pGeo->vertRect[0].u1;		//[0].u
			float text2=p_pGeo->vertRect[0].v1;		//[0].v
			float texr2=p_pGeo->vertRect[1].u1;		//[1].u
			float texb2=p_pGeo->vertRect[3].v1;		//[3].v
			if( p_bClipTexcoord )
			{
				float texw=p_pGeo->vertRect[1].u1-texl2;
				float texh=p_pGeo->vertRect[3].v1-text2;
            
				//如果水平被裁剪了
				if(w<p_pGeo->nWidth)
				{
					float dx = sx-p_pGeo->nx1;
					texl2 += dx*texw/p_pGeo->nWidth;
					texw *= w/p_pGeo->nWidth;
				}
				//如果垂直被裁剪了
				if(h<p_pGeo->nHeight)
				{
					float dy = sy-p_pGeo->ny1;
					text2 += dy*texh/p_pGeo->nHeight;
					texh *= h/p_pGeo->nHeight;
				}
            
				texr2 = texl2+texw;
				texb2 = text2+texh;
			}
			p_pGeo->create(sx,sy,w,h,texl,text,texr,texb,texl2,text2,texr2,texb2);
			return true;
		}
	};
}

#endif