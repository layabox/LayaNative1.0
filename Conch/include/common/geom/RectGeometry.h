
#ifndef _LAYA_RECT_GEOMETRY_H__
#define _LAYA_RECT_GEOMETRY_H__

namespace laya{
	/*
		二维Rect，min在左上角，max在右下角
	    0-----1
		|     |
		3-----2
	*/
	class RectGeometry
    {
	public:
		struct VertexInfo
        {
            //如果需要颜色的 话，就用w
            float x, y, u, v;
            float r,g,b,a;//
		};
		RectGeometry()
        {
            m_nX =m_nY= m_nWidth = m_nHeight =0;
			m_bShow = true;
		}
		void copy( const RectGeometry& src )
        {
			*this = src;
		}
		void create(float x1, float y1, float w, float h, float tx1, float ty1, float tx2, float ty2 )
		{
			m_bShow = true;
			m_nX = x1;
			m_nY = y1;
            m_nWidth = w;
            m_nHeight = h;
			float x2=x1+w;
			float y2=y1+h;
			m_pVertexRect[0].x = x1;
			m_pVertexRect[0].y = y1;
			m_pVertexRect[0].u = tx1;
			m_pVertexRect[0].v = ty1;

			m_pVertexRect[1].x = x2;
			m_pVertexRect[1].y = y1;
			m_pVertexRect[1].u = tx2;
			m_pVertexRect[1].v = ty1;

			m_pVertexRect[2].x = x2;
			m_pVertexRect[2].y = y2;
			m_pVertexRect[2].u = tx2;
			m_pVertexRect[2].v = ty2;

			m_pVertexRect[3].x = x1;
			m_pVertexRect[3].y = y2;
			m_pVertexRect[3].u = tx1;
			m_pVertexRect[3].v = ty2;
		}
		void applyTranslate( float tx, float ty)
        {
			if(tx==0&&ty==0)return;
			for( int i = 0; i < 4; i++ )
            {
				m_pVertexRect[i].x+=tx;
				m_pVertexRect[i].y+=ty;
			}
			m_nX += tx;	//x
			m_nY += ty;	//y
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
		void createForUV( float x1, float y1, float w, float h, float tx1, float ty1, float tx2, float ty2)
		{
			m_bShow = true;
			m_nX = x1;
			m_nY = y1;
            m_nWidth = w;
            m_nHeight = h;
			float x2= x1+w;
			float y2= y1+h;
			m_pVertexRect[0].x = x1;
			m_pVertexRect[0].y = y1;
			m_pVertexRect[0].u = tx1;
			m_pVertexRect[0].v = ty1;
			m_pVertexRect[1].x = x2;
			m_pVertexRect[1].y = y1;
			m_pVertexRect[1].u = tx2;
			m_pVertexRect[1].v = ty1;
			m_pVertexRect[2].x = x2;
			m_pVertexRect[2].y = y2;
			m_pVertexRect[2].u = tx2;
			m_pVertexRect[2].v = ty2;
			m_pVertexRect[3].x = x1;
			m_pVertexRect[3].y = y2;
			m_pVertexRect[3].u = tx1;
			m_pVertexRect[3].v = ty2;
		}
		void createForColor( float x1, float y1, float w,  float h, float r, float g, float b)
		{
			m_bShow = true;
			m_nX = x1;
			m_nY = y1;
            m_nWidth = w;
            m_nHeight = h;
			float x2 = x1+w;
			float y2 = y1+h;
			m_pVertexRect[0].x = x1;
			m_pVertexRect[0].y = y1;
			m_pVertexRect[0].r = r;
			m_pVertexRect[0].g = g;
			m_pVertexRect[0].b = b;
			m_pVertexRect[1].x = x2;
			m_pVertexRect[1].y = y1;
			m_pVertexRect[1].r = r;
			m_pVertexRect[1].g = g;
			m_pVertexRect[1].b = b;
			m_pVertexRect[2].x = x2;
			m_pVertexRect[2].y = y2;
			m_pVertexRect[2].r = r;
			m_pVertexRect[2].g = g;
			m_pVertexRect[2].b = b;
			m_pVertexRect[3].x = x1;
			m_pVertexRect[3].y = y2;
			m_pVertexRect[3].r = r;
			m_pVertexRect[3].g = g;
			m_pVertexRect[3].b = b;
		}
		void createForLine( float x1, float y1, float x2, float y2, float x3 ,float y3, float x4, float y4, float r, float g, float b )
		{
			m_bShow = true;
            //TODO 裁剪没用做呢
			m_nX = x1;
			m_nY = y1;
            m_nWidth = x3-x1;
            m_nHeight = y3-y1;
			m_pVertexRect[0].x = x1;
			m_pVertexRect[0].y = y1;
			m_pVertexRect[0].r = r;
			m_pVertexRect[0].g = g;
			m_pVertexRect[0].b = b;
			m_pVertexRect[1].x = x2;
			m_pVertexRect[1].y = y2;
			m_pVertexRect[1].r = r;
			m_pVertexRect[1].g = g;
			m_pVertexRect[1].b = b;
			m_pVertexRect[2].x = x3;
			m_pVertexRect[2].y = y3;
			m_pVertexRect[2].r = r;
			m_pVertexRect[2].g = g;
			m_pVertexRect[2].b = b;
			m_pVertexRect[3].x = x4;
			m_pVertexRect[3].y = y4;
			m_pVertexRect[3].r = r;
			m_pVertexRect[3].g = g;
			m_pVertexRect[3].b = b;
		}
        void createForClip(float x1, float y1, float w, float h)
        {
            m_bShow = true;
            m_nX = x1;
            m_nY = y1;
            m_nWidth = w;
            m_nHeight = h;
            float x2 = x1 + w;
            float y2 = y1 + h;
            m_pVertexRect[0].x = x1;
            m_pVertexRect[0].y = y1;
            m_pVertexRect[1].x = x2;
            m_pVertexRect[1].y = y1;
            m_pVertexRect[2].x = x2;
            m_pVertexRect[2].y = y2;
            m_pVertexRect[3].x = x1;
            m_pVertexRect[3].y = y2;
        }
		void ScaleUV(float x, float y)
        {
			for( int i=0; i<4; i++ )
            {
				m_pVertexRect[ i].u*=x;
				m_pVertexRect[ i].v*=y;
			}
		}
		void OffUV(float x, float y)
        {
			for( int i=0; i<4; i++ )
            {
				m_pVertexRect[ i].u+=x;
				m_pVertexRect[ i].v+=y;
			}
		}
		void applyMatrix(float m11,float m12, float m21, float m22, float m31, float m32)
        {
			for( int i=0; i<4; i++ )
            {
				float cx = m_pVertexRect[i].x;
				float cy = m_pVertexRect[i].y;
				float rx = cx*m11 + cy*m21 + m31;
				float ry = cx*m12 + cy*m22 + m32;
				m_pVertexRect[i].x = rx;
				m_pVertexRect[i].y = ry;
			}
            m_nWidth *= m11;
            m_nHeight *= m22;
			m_nX=m_pVertexRect[0].x;
			m_nY=m_pVertexRect[0].y;
		}
		void applyMatrix(float* m )
        {
			for( int i=0; i<4; i++ )
            {
				float& cx = m_pVertexRect[i].x;
				float& cy = m_pVertexRect[i].y;
				float rx = cx*m[0] + cy*m[2] + m[4];
				float ry = cx*m[1] + cy*m[3] + m[5];
				cx = rx;
				cy = ry;
			}
            m_nWidth *= m[0];
            m_nHeight *= m[3];
			m_nX=m_pVertexRect[0].x;
			m_nY=m_pVertexRect[0].y;
		}
		void SwapVert( int a, int b )
		{
			VertexInfo tmp = m_pVertexRect[a];
			m_pVertexRect[a]=m_pVertexRect[b];
			m_pVertexRect[b]=tmp;
		}
		//------------------------------------------------------------------------------
		/*
			整理一下。保证0在左上角，2在右下角
		*/
		void Normalize()
		{
			if(m_nWidth<0)
			{
				SwapVert(0,1);
				SwapVert(3,2);
				m_nX = m_pVertexRect[0].x;
                m_nWidth =-m_nWidth;
			}
			if(m_nHeight<0)
			{
				SwapVert(0,3);
				SwapVert(1,2);
				m_nY = m_pVertexRect[0].y;
                m_nHeight =-m_nHeight;
			}
		}
		void TransToDevice(float viewW, float viewH)
		{
			for( int i=0; i<4; i++ )
			{
				float& cx = m_pVertexRect[ i].x;
				float& cy = m_pVertexRect[ i].y;

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
		static bool clipWithRect(RectGeometry* p_pGeo, float p_ClipL, float p_ClipT, float p_ClipW, float p_ClipH, bool p_bClipTexcoord )
		{
			if(p_pGeo->m_nWidth <=0 || p_pGeo->m_nHeight <=0)
				p_pGeo->Normalize();
        
			if( p_pGeo->m_nWidth <=0 || p_pGeo->m_nHeight <=0 )
				return false;
			float cr = p_ClipL+p_ClipW;
			float cb = p_ClipT+p_ClipH;
			if( p_pGeo->m_nX>cr || p_pGeo->m_nY>cb )
				return false;
			float sr = (p_pGeo->m_nX + p_pGeo->m_nWidth);
			float sb = (p_pGeo->m_nY + p_pGeo->m_nHeight);
			if( sr<p_ClipL || sb<p_ClipT )
				return false;
			float sx=(p_pGeo->m_nX);		//裁剪后的左边
			if( p_pGeo->m_nX<p_ClipL ) sx=p_ClipL;
			float sy=(p_pGeo->m_nY);		//裁剪后的上边
			if( p_pGeo->m_nY<p_ClipT ) sy = p_ClipT;
			float ex = sr;	//裁剪后的右边
			if( sr>cr) ex=cr;
			float ey = sb;	//裁剪后的下边
			if( sb>cb) ey=cb;
        
			float w = ex-sx;
			float h = ey-sy;
        
			float texl=p_pGeo->m_pVertexRect[0].u;	//[0].u
			float text=p_pGeo->m_pVertexRect[0].v;  //[0].v
			float texr=p_pGeo->m_pVertexRect[1].u;	//[1].u
			float texb=p_pGeo->m_pVertexRect[3].v;	//[3].v
			if( p_bClipTexcoord )
			{
				float texw=p_pGeo->m_pVertexRect[1].u-texl;
				float texh=p_pGeo->m_pVertexRect[3].v-text;
				//如果水平被裁剪了
				if(w<p_pGeo->m_nWidth)
				{
					float dx = sx-p_pGeo->m_nX;
					texl += dx*texw/p_pGeo->m_nWidth;
					texw *= w/p_pGeo->m_nWidth;
				}
				//如果垂直被裁剪了
				if(h<p_pGeo->m_nHeight)
				{
					float dy = sy-p_pGeo->m_nY;
					text += dy*texh/p_pGeo->m_nHeight;
					texh *= h/p_pGeo->m_nHeight;
				}
				texr = texl+texw;
				texb = text+texh;
			}
            //TODO 颜色
			p_pGeo->create(sx,sy,w,h,texl,text,texr,texb);
			return true;
		}
    public:
        bool            m_bShow;            //是否显示
        float           m_nX;               //x 缺省的起点。对应m_pVertexRect[0].xy
        float           m_nY;			    //y 缺省的起点。对应m_pVertexRect[0].xy
        float           m_nWidth;           //宽
        float           m_nHeight;	        //高     宽高可以为负的，负表示进行水平或者垂直镜像了
        VertexInfo      m_pVertexRect[4];   //四边形的4个点
	};
}

#endif