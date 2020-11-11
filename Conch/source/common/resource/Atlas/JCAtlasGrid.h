/**
@file			JCAtlasGrid.h
@brief			
@author			James
@version		1.0
@date			2016_5_26
*/

#ifndef __JCAtlasGrid_H__
#define __JCAtlasGrid_H__

#include <vector>

namespace laya
{
    typedef unsigned int uint;
	class MergeFillInfo
	{
	public:
		int m_nX;
		int m_nY;
	};
	class JCAtlasGrid
	{
	protected:
		struct TexMergeCellInfo//一个格子的信息
		{
			int m_bType;
			int m_nSuccessionWidth;	    //连续的宽度
			int m_nSuccessionHeight;	//连续的高度
			TexMergeCellInfo()
            {
				m_bType= 0;
				m_nSuccessionWidth= 0;
				m_nSuccessionHeight= 0;
			}
		};
		//------------------------------------------------------------------------------
		struct TexRowInfo
        {
			int m_nSpaceCount;  //此行总空白数
		};
		//------------------------------------------------------------------------------
		struct TexMergeTexSize
        {
			int m_nWidth;
			int m_nHeight;
		};

	public:
		int m_nWidth;
		int m_nHeight ;
		int m_nTexCount;
		TexMergeTexSize m_FailSize;//>=这个大小的格子是无法插入的
		std::vector<TexRowInfo> m_vRowInfo;
		std::vector<TexMergeCellInfo> m_vCells;
		//------------------------------------------------------------------------------
        JCAtlasGrid(uint p_nWidth=0,uint p_nHeight=0 )
		{
			m_nWidth = 0;
			m_nHeight = 0;
			m_nTexCount= 0;
			_init( p_nWidth,p_nHeight );
		}
		//------------------------------------------------------------------
		bool AddTex( int p_nType, int p_nWidth, int p_nHeight,MergeFillInfo& p_pResultFillInfo )
		{
			//调用获得应该放在哪 返回值有三个。。bRet是否成功，nX x位置，nY y位置
			bool pResult = _get( p_nWidth, p_nHeight,p_pResultFillInfo );
			//判断如果没有找到合适的位置，则直接返回失败
			if ( pResult == false ) 
			{
				return pResult;
			}
			//根据获得的x,y填充
			_fill( p_pResultFillInfo.m_nX,p_pResultFillInfo.m_nY,p_nWidth,p_nHeight,p_nType );
			
			m_nTexCount++;
			
			//返回是否成功，以及X位置和Y位置
			return pResult;
		}
		//这个函数清空格子上的标记，并且清空vector
		bool reset()
		{
			_release();
			if (m_nWidth==0 ) return false;
			m_vCells.resize(m_nWidth * m_nHeight );
			m_vRowInfo.resize(m_nHeight);
			_clear();
			return true;
		}
	private:
		//------------------------------------------------------------------------------
		void _release()
		{
			m_vCells.clear();
			m_vRowInfo.clear();
		}
		//------------------------------------------------------------------------------
		bool _init( uint p_nWidth, uint p_nHeight)
		{
			m_nWidth = p_nWidth;
			m_nHeight = p_nHeight;
			_release();
			if (m_nWidth==0 ) return false;
			m_vCells.resize(m_nWidth * m_nHeight );
			m_vRowInfo.resize(m_nHeight);
			_clear();
			return true;
		}
		//------------------------------------------------------------------
		bool _get( int p_nW, int p_nH,MergeFillInfo& p_pResultFillInfo )
		{
			if( p_nW >= m_FailSize.m_nWidth && p_nH >= m_FailSize.m_nHeight )
			{
				return false;
			}
			//定义返回的x,y的位置
			int rx = -1;
			int ry = -1;
			//为了效率先保存临时变量
			int nWidth = m_nWidth;
			int nHeight = m_nHeight;
			//定义一个变量为了指向 m_pCells
			std::vector<TexMergeCellInfo>& pCellBox = m_vCells;
			
			//遍历查找合适的位置
			for( int y = 0 ; y < nHeight; y++ )
			{
				//如果该行的空白数 小于 要放入的宽度返回
				if( m_vRowInfo[y].m_nSpaceCount < p_nW ) continue;
				for( int x = 0 ; x < nWidth; )
				{
					if ( pCellBox[ y * nWidth + x ].m_bType != 0 || pCellBox[ y * nWidth + x ].m_nSuccessionWidth < p_nW || pCellBox[ y * nWidth + x ].m_nSuccessionHeight < p_nH )
					{
						x += pCellBox[y * nWidth + x].m_nSuccessionWidth;
						continue;
					}
					rx = x;
					ry = y;
					for( int xx = 0 ; xx < p_nW; xx++ )
					{
						if ( pCellBox[y * nWidth + x + xx].m_nSuccessionHeight < p_nH )
						{
							rx=-1;
							break;
						}
					}
					if( rx < 0 )
					{
						x += pCellBox[y * nWidth + x].m_nSuccessionWidth;
						continue;
					}
					p_pResultFillInfo.m_nX = rx;
					p_pResultFillInfo.m_nY = ry;
					return true;
				}
			}
			return false;
		}
		//------------------------------------------------------------------
		void _fill( int p_nX, int p_nY, int p_nW, int p_nH, int p_nType)
		{
			//定义一些临时变量
			int nWidth = m_nWidth;
			int nHeghit = m_nHeight;
			//代码检查
			_check( (p_nX+p_nW) <= nWidth && (p_nY+p_nH) <= nHeghit );
			
			//填充
			for( int yy= p_nY; yy < (p_nH+p_nY); ++yy )
			{
				_check( m_vRowInfo[yy].m_nSpaceCount >= p_nW );
				m_vRowInfo[yy].m_nSpaceCount -= p_nW;
				for( int xx= 0 ; xx < p_nW; xx++ )
				{
					_check( m_vCells[p_nX + yy * nWidth + xx].m_bType == 0);
					m_vCells[p_nX + yy * nWidth + xx].m_bType = p_nType;
					m_vCells[p_nX + yy * nWidth + xx].m_nSuccessionWidth = p_nW;
					m_vCells[p_nX + yy * nWidth + xx].m_nSuccessionHeight = p_nH;
				}
			}
			//调整我左方相邻空白格子的宽度连续信息描述
			if( p_nX>0 )
			{
				for( int yy=0 ; yy < p_nH ; ++yy )
				{
					int s = 0;
					for( int xx = p_nX-1; xx >= 0; --xx,++s )
					{
						if ( m_vCells[(p_nY + yy) * nWidth + xx].m_bType != 0 ) break;
					}
					for( int xx = s; xx>0 ; --xx )
					{
						m_vCells[(p_nY + yy) * nWidth + p_nX - xx].m_nSuccessionWidth = xx;
						_check( xx>0 );
					}
				}
			}
			//调整我上方相邻空白格子的高度连续信息描述
			if( p_nY > 0 )
			{
				for( int xx=p_nX ; xx < (p_nX+p_nW) ; ++xx )
				{
					int s = 0;
					for( int yy=p_nY-1 ; yy >= 0 ; --yy, s++ )
					{
						if( m_vCells[ xx+ yy* nWidth ].m_bType!=0 ) break;
					}
					for( int yy = s; yy>0 ; --yy )
					{
						m_vCells[ xx+ (p_nY-yy) * nWidth ].m_nSuccessionHeight = yy;
						_check( yy>0 );
					}
				}
			}
		}
		void _check( bool p_bRet)
		{
			if ( p_bRet == false )
			{
				//LayaMethod.alert( "xtexMerger 错误啦" );
			}
		}
		//------------------------------------------------------------------
		void _clear()
		{
			m_nTexCount = 0;
			for  (int y= 0; y < m_nHeight; y++) 
			{
				m_vRowInfo[y].m_nSpaceCount = m_nWidth;
			}
			for (int i= 0; i < m_nHeight; i++) 
			{
				for (int j= 0; j < m_nWidth; j++)
				{
					TexMergeCellInfo& pCellbox= m_vCells[ i * m_nWidth + j];
					pCellbox.m_bType = 0;
					pCellbox.m_nSuccessionWidth = m_nWidth - j;
					pCellbox.m_nSuccessionHeight = m_nWidth - i;
				}
			}
			m_FailSize.m_nWidth = m_nWidth+1;
			m_FailSize.m_nHeight = m_nHeight+1;
		}
	};
}
//------------------------------------------------------------------------------

#endif //__JCAtlasGrid_H__

//-----------------------------END FILE--------------------------------