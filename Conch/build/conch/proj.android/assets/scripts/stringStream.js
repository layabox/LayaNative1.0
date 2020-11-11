
if( !conch.m_sStreamBuffer )
{
    conch.m_sStreamBuffer = [];
    conch.m_sStreamBuffer.index = 0;
}

ConchNode=function()
{
	var pCNode = null;
    var conchThisID = 0;
    var tmp=[];
	this.init=function()
	{
		this.pCNode = new _ConchNode();
		this.conchThisID = this.pCNode.conchThisID;
	}
	this.setClipChild=function( bClip )
	{
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [0,this.conchThisID,bClip ? 1 : 0];//.push( "0" + '\4' + this.conchThisID + '\4' + bClip ? 1 : 0 );
	}
	this.setClip=function( scrollX,scrollY )
	{
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [1,this.conchThisID,scrollX,scrollY];//.push( "1" + '\4' + this.conchThisID + '\4' + scrollX + '\4' + scrollY );
	}		
	this.pos=function( x,y )
	{
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [2,this.conchThisID,x,y];//.push( "2" + '\4' + this.conchThisID + '\4' + x + '\4' + y );
	}
	this.size=function( x,y )
	{
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [3,this.conchThisID,x,y];//.push( "3" + '\4' + this.conchThisID + '\4' + x + '\4' + y );
	}
	this.scale2d=function( x,y )
	{
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [4,this.conchThisID,x,y];//push( "4" + '\4' + this.conchThisID + '\4' + x + '\4' + y );
	}
	this.scale2dEx=function( x,y )
	{
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [5,this.conchThisID,x,y];//push( "5" + '\4' + this.conchThisID + '\4' + x + '\4' + y );
	}
	this.rotate2d=function( r )
	{
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [6,this.conchThisID,r];//push( "6" + '\4' + this.conchThisID + '\4' + r );
	}
	this.bgcolor=function( color )
	{
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [7,this.conchThisID,color];//push( "7" + '\4' + this.conchThisID + '\4' + color );
	}
	this.bgimg=function( p_pImage,p_nRepeat )
	{
		if( p_pImage.conchThisID != null )
	        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [8,this.conchThisID,p_pImage.conchThisID,p_nRepeat];//push( "8" + '\4' + this.conchThisID + '\4' + p_pImage.conchThisID + '\4' + p_nRepeat );
	}
	this.bgimgPos=function( x,y )
	{
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [9,this.conchThisID,x,y];//push( "9" + '\4' + this.conchThisID + '\4' + x + '\4' + y );
    }
	this.image=function(p_pImage,bNeedNodeSize )
	{
		if(!p_pImage)
			return;
		//这个应该是canvas
		if( p_pImage.isCanvas )
		{
			conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [46,this.conchThisID,p_pImage.getThisID()];//push( "46" + '\4' + this.conchThisID + '\4' + p_pImage.getThisID() );
		}
		else
		{
			//TODO  p_pImage 可能是canvas
			if( arguments.length >= 2 )
			{
				conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [11,this.conchThisID,p_pImage.conchThisID,bNeedNodeSize];//push( "11" + '\4' + this.conchThisID + '\4' + p_pImage.conchThisID + '\4' + bNeedNodeSize );
			}
			else
			{
				conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [10,this.conchThisID,p_pImage.conchThisID];//push( "10" + '\4' + this.conchThisID + '\4' + p_pImage.conchThisID );
			}
		}
	}
	this.bitmapdataContent=function( p_pBitmapdataContent )
	{
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [12,this.conchThisID,p_pBitmapdataContent.conchThisID];//push( "12" + '\4' + this.conchThisID + '\4' + p_pBitmapdataContent.conchThisID );
	}
	this.border=function( color )
	{
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [13,this.conchThisID,color];//push( "13" + '\4' + this.conchThisID + '\4' + color );
	}
	this.text=function( words )
	{
		conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [14,this.conchThisID,words.conchThisID];//push( "14" + '\4' + this.conchThisID + '\4' + words.conchThisID );
	}
	this.textStr=function( p_sText,x,y )
	{
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [15,this.conchThisID,p_sText,x,y];//push( "15" + '\4' + this.conchThisID + '\4' + p_sText + '\4' + x + '\4' + y );
	}
	this.clip=function( bClip )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [16,this.conchThisID,(bClip? 1 : 0)];//push( "16" + '\4' + this.conchThisID + '\4' + (bClip? 1 : 0) );
    }
	this.flip=function( nFlipType )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [17,this.conchThisID,nFlipType];//push( "17" + '\4' + this.conchThisID + '\4' + nFlipType );
    }
	this.padding=function( left,top,right,bottom )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [18,this.conchThisID,left,top,right,bottom];//push( "18" + '\4' + this.conchThisID + '\4' + left + '\4' + top + '\4' + right + '\4' + bottom );
    }
	this.margin=function(left,top,right,bottom )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [19,this.conchThisID,left,top,right,bottom];//push( "19" + '\4' + this.conchThisID + '\4' + left + '\4' + top + '\4' + right + '\4' + bottom );
    }
	this.scroll=function( x,y )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [20,this.conchThisID,x,y];//push( "20" + '\4' + this.conchThisID + '\4' + x + '\4' + y );
    }
	this.alpha=function( p_fAlpha )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [21,this.conchThisID,p_fAlpha];//push( "21" + '\4' + this.conchThisID + '\4' + p_fAlpha );
    }
	this.filter=function( r,g,b,gray )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [22,this.conchThisID,r,g,b,gray];//push( "22" + '\4' + this.conchThisID + '\4' + r + '\4' + g + '\4' + b + '\4' + gray );
    }
	this.globalCompositeOperation=function( p_nType )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [23,this.conchThisID,p_nType];//push( "23" + '\4' + this.conchThisID + '\4' + p_nType );
    }
	this.setOrigin=function( x,y,xtype,ytype )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [24,this.conchThisID,x,y,xtype,ytype];//push( "24" + '\4' + this.conchThisID + '\4' + x + '\4' + y + '\4' + xtype + '\4' + ytype );
    }
	this.color=function( p_sColor )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [25,this.conchThisID,p_sColor];//push( "25" + '\4' + this.conchThisID + '\4' + p_sColor );
    }
	this.setToCanvas=function( p_pCanvas )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [26,this.conchThisID,p_pCanvas.getThisID()];//push( "26" + '\4' + this.conchThisID + '\4' + p_pCanvas.getThisID() );
    }
	this.quote=function( p_pNode )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [27,this.conchThisID,p_pNode.conchThisID];//push( "27" + '\4' + this.conchThisID + '\4' + p_pNode.conchThisID );
    }
	this.matrix=function( m11,m12,m21,m22,m31,m32 )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [28,this.conchThisID,m11,m12,m21,m22,m31,m32];//push( "28" + '\4' + this.conchThisID + '\4' + m11 + '\4' + m12 + '\4' + m21 + '\4' + m22 + '\4' + m31 + '\4' + m32 );
    }
	this.font=function( p_sFont )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [29,this.conchThisID,p_sFont];//push( "29" + '\4' + this.conchThisID + '\4' + p_sFont );
    }
	this.zIndex=function( p_nIndex )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [30,this.conchThisID,p_nIndex];//push( "30" + '\4' + this.conchThisID + '\4' + p_nIndex );
    }
	this.position=function( p_nType )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [31,this.conchThisID,p_nType];//push( "31" + '\4' + this.conchThisID + '\4' + p_nType );
    }
	this.reset=function( p_nID,p_pNode )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [32,this.conchThisID,p_nID,p_pNode.conchThisID];//push( "32" + '\4' + this.conchThisID + '\4' + p_nID + '\4' + p_pNode.conchThisID );
    }
	this.show=function( p_bShow )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [33,this.conchThisID,p_bShow?1:0];//push( "33" + '\4' + this.conchThisID + '\4' + (p_bShow ? 1 : 0 ));
    }
	this.destroy=function()
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [34,this.conchThisID];//push( "34" + '\4' + this.conchThisID );
    }
	this.destroyAllChild=function()
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [35,this.conchThisID];//push( "35" + '\4' + this.conchThisID );
    }
	this.swap=function( p_pNode1,p_pNode2 )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [36,this.conchThisID,p_pNode1.conchThisID,p_pNode2.conchThisID];//push( "36" + '\4' + this.conchThisID + '\4' + p_pNode1.conchThisID + '\4' + p_pNode2.conchThisID );
    }
	this.removeAt=function( p_nIndex,p_pNode,p_nSize )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [37,this.conchThisID,p_nIndex,p_pNode.conchThisID,p_nSize ];//push( "37" + '\4' + this.conchThisID + '\4' + p_nIndex + '\4' + p_pNode.conchThisID + '\4' + p_nSize );
    }
	this.insert=function( p_pNode,p_nIndex,p_nSize )
    {
		if( p_pNode )
		{
			conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [38,this.conchThisID,p_pNode.conchThisID,p_nIndex,p_nSize];//push( "38" + '\4' + this.conchThisID + '\4' + p_pNode.conchThisID + '\4' + p_nIndex + '\4' + p_nSize );
		}
    }
	this.forceInsert=function( p_pNode,p_nIndex )
    {
		if( p_pNode )
		{
			conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [39,this.conchThisID,p_pNode.conchThisID,p_nIndex];//push( "39" + '\4' + this.conchThisID + '\4' + p_pNode.conchThisID + '\4' + p_nIndex );
		}
    }
	this.forceRemoveAt=function( p_nIndex )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [40,this.conchThisID,p_nIndex];//push( "40" + '\4' + this.conchThisID + '\4' + p_nIndex );
    }
	this.virtualBitmap=function( p_pVirtualBitmap )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [41,this.conchThisID,p_pVirtualBitmap.conchThisID];//push( "41" + '\4' + this.conchThisID + '\4' + p_pVirtualBitmap.conchThisID );
    }
	this.boneAnimation=function( p_pBoneAnimation )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [42,this.conchThisID,p_pBoneAnimation.conchThisID];//push( "42" + '\4' + this.conchThisID + '\4' + p_pBoneAnimation.conchThisID );
    }
	this.zDepth=function( p_fDepth )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [43,this.conchThisID,p_fDepth];//push( "43" + '\4' + this.conchThisID + '\4' + p_fDepth );
    }
	this.setHideText=function( p_bHideText )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [44,this.conchThisID,p_bHideText];//push( "44" + '\4' + this.conchThisID + '\4' + p_bHideText );
    }
	this.setPassword=function( p_bPassword )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [45,this.conchThisID,p_bPassword];//push( "45" + '\4' + this.conchThisID + '\4' + p_bPassword );
    }
	this.init();
};
boneAnimation=function()
{
    var pCBoneAnimation = null;
    var conchThisID = 0;
    this.init=function()
    {
        this.pCBoneAnimation = new ConchBoneAnimation();
        this.conchThisID = this.pCBoneAnimation.conchThisID;
    }
    this.setTextureGroup=function( p_pTextureGroup )
    {
        this.pCBoneAnimation.setTextureGroup( p_pTextureGroup );
    }
    this.setAnimTemplate=function( p_pAnimTemplate )
    {
        this.pCBoneAnimation.setAnimTemplate( p_pAnimTemplate );
    }
    this.setAnimationInfo=function( p_nX,p_nY,p_nAnimationIndex,p_nFrameIndex )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [47,this.conchThisID,p_nX,p_nY,p_nAnimationIndex,p_nFrameIndex];//push( "47" + '\4' + this.conchThisID + '\4' + p_nX + '\4' + p_nY + '\4' + p_nAnimationIndex + '\4' + p_nFrameIndex );
    }
    this.init();
}
boneAnimation.prototype=
{
    set x( p_nX )
    {
        this.pCBoneAnimation.x = p_nX;
    },
    get x()
    {
        return pCBoneAnimation.y;
    },
    set y( p_nY )
    {
        this.pCBoneAnimation.y = p_nY;
    },
    get y()
    {
        return this.pCBoneAnimation.y;
    },
    set animationIndex( p_nIndex )
    {
        this.pCBoneAnimation.animationIndex = p_nIndex;
    },
    get animationIndex()
    {
        return this.pCBoneAnimation.animationIndex;
    },
    set frameIndex( p_nIndex )
    {
        this.pCBoneAnimation.frameIndex = p_nIndex;
    },
    get frameIndex()
    {
        return this.pCBoneAnimation.frameIndex;
    }
}
ConchCanvas=function()
{
    var m_pCCanvas = null;
    var m_pContext = null;
    this.init=function()
    {
        this.m_pCCanvas = new _ConchCanvas();
    }
    this.getThisID=function()
    {
        return this.m_pCCanvas.getThisID();
    }
    this.isCanvas=function()
    {
        return true;
    }
    this.setSize=function( w,h )
    {
        this.m_pCCanvas.setSize( w,h );
    }
    this.getContext=function( type )
    {
        if( type == "2d" && !this.m_pContext )
        {
            this.m_pContext = new context();
            this.m_pContext.m_pCContext = this.m_pCCanvas.getContext( type );
            this.m_pContext.conchThisID = this.m_pContext.m_pCContext.conchThisID;
        }
        return this.m_pContext;
    }
    this.init();
}
ConchCanvas.prototype=
{
    set width( w )
    {
        this.m_pCCanvas.width = w;
    },
    get width()
    {
        return this.m_pCCanvas.width;
    },
    set height( w )
    {
        this.m_pCCanvas.height = w;
    },
    get height()
    {
        return this.m_pCCanvas.height;
    },
    set style( p_pStyle )
    {
        this._style = p_pStyle;
    },
    get style()
    {
        return this._style;
    }
}
context=function()
{
    var m_pCContext = null;
    var conchThisID = 0;
    this.clearRect=function( x,y,w,h )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [48,this.conchThisID,x,y,w,h];//push( "48" + '\4' + this.conchThisID + '\4' + x + '\4' + y + '\4' + w + '\4' + h );
    }
    this.fillRect=function( x,y,w,h )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [49,this.conchThisID,x,y,w,h];//push( "49" + '\4' + this.conchThisID + '\4' + x + '\4' + y + '\4' + w + '\4' + h );
    }
    this.translate=function( x,y )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [50,this.conchThisID,x,y];//push( "50" + '\4' + this.conchThisID + '\4' + x + '\4' + y);
    }
    this.save=function()
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [51,this.conchThisID];//push( "51" + '\4' + this.conchThisID );
    }
    this.restore=function()
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [52,this.conchThisID];//push( "52" + '\4' + this.conchThisID );
    }
    this.beginPath=function()
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [53,this.conchThisID];//push( "53" + '\4' + this.conchThisID );
    }
    this.clip=function()
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [54,this.conchThisID];//push( "54" + '\4' + this.conchThisID );
    }
    this.rect=function( x,y,w,h )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [55,this.conchThisID,x,y,w,h];//push( "55" + '\4' + this.conchThisID + '\4' + x + '\4' + y + '\4' + w + '\4' + h );
    }
    this.strokeRect=function( x,y,w,h )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [56,this.conchThisID,x,y,w,h];//push( "56" + '\4' + this.conchThisID + '\4' + x + '\4' + y + '\4' + w + '\4' + h );
    }
    this.fill=function()
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [57,this.conchThisID];//push( "57" + '\4' + this.conchThisID );
    }
    this.stroke=function()
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [58,this.conchThisID];//push( "58" + '\4' + this.conchThisID );
    }
    this.moveTo=function(x,y)
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [59,this.conchThisID,x,y];//push( "59" + '\4' + this.conchThisID + '\4' + x + '\4' + y );
    }
    this.closePath=function()
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [60,this.conchThisID];//push( "60" + '\4' + this.conchThisID );
    }
    this.lineTo=function(x,y)
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [61,this.conchThisID,x,y];//push( "61" + '\4' + this.conchThisID + '\4' + x + '\4' + y );
    }
    this.quadraticCurveTo=function( x,y,w,h )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [62,this.conchThisID,x,y,w,h];//push( "62" + '\4' + this.conchThisID + '\4' + x + '\4' + y + '\4' + w + '\4' + h );
    }
    this.bezierCurveTo=function( x,y,x1,y1,ex,ey )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [63,this.conchThisID,x,y,x1,y1,ex,ey];//push( "63" + '\4' + this.conchThisID + '\4' + x + '\4' + y + '\4' + x1 + '\4' + y1 + '\4' + ex + '\4' + ey );
    }
    this.arc=function( x,y,r,sa,ea,b )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [64,this.conchThisID,x,y,r,sa,ea,(b?1:0)];//push( "64" + '\4' + this.conchThisID + '\4' + x + '\4' + y + '\4' + r + '\4' + sa + '\4' + ea + '\4' + (b?1:0) );
    }
    this.arcTo=function( x,y,x1,y1,nr )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [65,this.conchThisID,x,y,x1,y1,nr];//push( "65" + '\4' + this.conchThisID + '\4' + x + '\4' + y + '\4' + x1 + '\4' + y1 + '\4' + nr);
    }
    this.scale=function(x,y)
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [66,this.conchThisID,x,y];//push( "66" + '\4' + this.conchThisID + '\4' + x + '\4' + y );
    }
    this.rotate=function(x)
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [67,this.conchThisID,x];//push( "67" + '\4' + this.conchThisID + '\4' + x );
    }
    this.transform=function( a,b,c,d,e,f )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [68,this.conchThisID,a,b,c,d,e,f];//push( "68" + '\4' + this.conchThisID + '\4' + a + '\4' + b + '\4' + c + '\4' + d + '\4' + e + '\4' + f );
    }
    this.setTransform=function( a,b,c,d,e,f )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [69,this.conchThisID,a,b,c,d,e,f];//push( "69" + '\4' + this.conchThisID + '\4' + a + '\4' + b + '\4' + c + '\4' + d + '\4' + e + '\4' + f );
    }
    this.fillText=function( src,x,y,w )
    {
        if( arguments.length<4 )
            conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [70,this.conchThisID,src,x,y];//push( "70" + '\4' + this.conchThisID + '\4' + src + '\4' + x + '\4' + y );
        else
            conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [71,this.conchThisID,src,x,y,w];//push( "71" + '\4' + this.conchThisID + '\4' + src + '\4' + x + '\4' + y + '\4' + w );
    }
    this.strokeText=function( src,x,y,w )
    {
        if( arguments.length<4 )
            conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [72,this.conchThisID,src,x,y];//push( "72" + '\4' + this.conchThisID + '\4' + src + '\4' + x + '\4' + y );
        else
            conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [73,this.conchThisID,src,x,y,w];//push( "73" + '\4' + this.conchThisID + '\4' + src + '\4' + x + '\4' + y + '\4' + w );
    }
    this.drawImage=function( pSrc,x,y,w,h,x1,y1,w1,h1 )
    {
        if( pSrc.isCanvas )
        {
            if( arguments.length == 3 ){
                conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [101,this.conchThisID,pSrc.getThisID(),x,y];//push( "101" + '\4' + this.conchThisID + '\4' + pSrc.conchThisID + '\4' + x + '\4' + y );
            }else if(arguments.length == 5){
                conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [102,this.conchThisID,pSrc.getThisID(),x,y,w,h];//push( "102" + '\4' + this.conchThisID + '\4' + pSrc.conchThisID + '\4' + x + '\4' + y + '\4' + w + '\4' + h );
            }else if(arguments.length == 9){
                conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [103,this.conchThisID,pSrc.getThisID(),x,y,w,h,x1,y1,w1,h1];//push( "103" + '\4' + this.conchThisID + '\4' + pSrc.conchThisID + '\4' + x + '\4' + y + '\4' + w + '\4' + h+ '\4' + x1 + '\4' + y1 + '\4' + w1 + '\4' + h1 );
            }
        }
        else
        {
            if( arguments.length == 3 ){
                conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [74,this.conchThisID,pSrc.conchThisID,x,y];//push( "74" + '\4' + this.conchThisID + '\4' + pSrc.conchThisID + '\4' + x + '\4' + y );
            }else if(arguments.length == 5){
                conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [75,this.conchThisID,pSrc.conchThisID,x,y,w,h];//push( "75" + '\4' + this.conchThisID + '\4' + pSrc.conchThisID + '\4' + x + '\4' + y + '\4' + w + '\4' + h );
            }else if(arguments.length == 9){
                conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [76,this.conchThisID,pSrc.conchThisID,x,y,w,h,x1,y1,w1,h1];//push( "76" + '\4' + this.conchThisID + '\4' + pSrc.conchThisID + '\4' + x + '\4' + y + '\4' + w + '\4' + h+ '\4' + x1 + '\4' + y1 + '\4' + w1 + '\4' + h1 );
            }
        }
    }
    this.setFilter=function( r,g,b,a )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [77,this.conchThisID,r,g,b,a];//push( "77" + '\4' + this.conchThisID + '\4' + r + '\4' + g + '\4' + b + '\4' + a );
    }
    this.setFilterToC=function( r,g,b,a )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [77,this.conchThisID,r,g,b,a];//push( "77" + '\4' + this.conchThisID + '\4' + r + '\4' + g + '\4' + b + '\4' + a );
    }
    this.isPointInPath=function(x,y)
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [78,this.conchThisID,x,y];//push( "78" + '\4' + this.conchThisID + '\4' + x + '\4' + y );
    }
    this.measureText=function(src)
    {
        //conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [45,this.conchThisID,p_bPassword];//push( "79" + '\4' + this.conchThisID + '\4' + src );
        onCommitStream();
        return this.m_pCContext.measureText(src);
    }
    this.getImageData=function( x,y,w,h )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [80,this.conchThisID,x,y,w,h];//push( "80" + '\4' + this.conchThisID + '\4' + x + '\4' + y + '\4' + w + '\4' + h );
    }
    this.disableClip=function( b )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [81,this.conchThisID,b?1:0];//push( "81" + '\4' + this.conchThisID + '\4' + (b?1:0) );
    }
    
    this.setCanvas=function( canvas )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [82,this.conchThisID,canvas.conchThisID];//push( "82" + '\4' + this.conchThisID + '\4' + canvas.conchThisID );
    }
    this.createPattern=function( canvas,src )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [83,this.conchThisID,canvas.conchThisID,src];//push( "83" + '\4' + this.conchThisID + '\4' + canvas + '\4' + src );
    }
    this.createLinearGradient=function( x,y,w,h )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [84,this.conchThisID,x,y,w,h];//push( "84" + '\4' + this.conchThisID + '\4' + x + '\4' + y + '\4' + w + '\4' + h );
    }
    this.createRadialGradient=function( x,y,r,ex,ey,er )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [85,this.conchThisID,x,y,r,ex,ey,er];//push( "85" + '\4' + this.conchThisID + '\4' + x + '\4' + y + '\4' + r + '\4' + ex + '\4' + ey + '\4' + er );
    }
    //TODO
}
context.prototype =
{
    set globalAlpha( alpha )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [86,this.conchThisID,alpha];//push( "86" + '\4' + this.conchThisID + '\4' + alpha );
    },
    get globalAlpha()
    {
        //TODO
        return this.m_pCContext.globalAlpha;
    },
    set shadowColor( color )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [87,this.conchThisID,color];//push( "87" + '\4' + this.conchThisID + '\4' + color );
    },
    get shadowColor()
    {
        return this.m_pCContext.shadowColor;
    },
    set shadowBlur( blur )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [88,this.conchThisID,blur];//push( "88" + '\4' + this.conchThisID + '\4' + blur );
    },
    get shadowBlur()
    {
        return this.m_pCContext.shadowBlur;
    },
    set shadowOffsetX( ox )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [89,this.conchThisID,ox];//push( "89" + '\4' + this.conchThisID + '\4' + ox );
    },
    get shadowOffsetX()
    {
        return this.m_pCContext.shadowOffsetX;
    },
    set shadowOffsetY( oy )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [90,this.conchThisID,oy];//push( "90" + '\4' + this.conchThisID + '\4' + oy );
    },
    get shadowOffsetY()
    {
        return this.m_pCContext.shadowOffsetY;
    },
    set lineCap( cap )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [91,this.conchThisID,cap];//push( "91" + '\4' + this.conchThisID + '\4' + cap);
    },
    get lineCap()
    {
        return this.m_pCContext.lineCap;
    },
    set lineJoin( join )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [92,this.conchThisID,join];//push( "92" + '\4' + this.conchThisID + '\4' + join );
    },
    get lineJoin()
    {
        return this.m_pCContext.lineJoin;
    },
    set lineWidth( w )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [93,this.conchThisID,w];//push( "93" + '\4' + this.conchThisID + '\4' + w );
    },
    get lineWidth()
    {
        return this.m_pCContext.lineWidth;
    },
    set miterLimit( limit )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [94,this.conchThisID,limit];//push( "94" + '\4' + this.conchThisID + '\4' + limit );
    },
    get miterLimit()
    {
        return this.m_pCContext.miterLimit;
    },
    set font( f )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [95,this.conchThisID,f];//push( "95" + '\4' + this.conchThisID + '\4' + f );
    },
    get font()
    {
        return this.m_pCContext.font;
    },
    set textAlign( a )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [96,this.conchThisID,a];//push( "96" + '\4' + this.conchThisID + '\4' + a );
    },
    get textAlign()
    {
        return this.m_pCContext.textAlign;
    },
    set textBaseline( b )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [97,this.conchThisID,b];//push( "97" + '\4' + this.conchThisID + '\4' + b );
    },
    get textBaseline()
    {
        return this.m_pCContext.textBaseline;
    },
    set globalCompositeOperation( o )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [98,this.conchThisID,o];//push( "98" + '\4' + this.conchThisID + '\4' + o );
    },
    get globalCompositeOperation()
    {
        return this.m_pCContext.globalCompositeOperation;
    },
    get width()
    {
        return this.m_pCContext.width;
    },
    get height()
    {
        return this.m_pCContext.height;
    },
    set fillStyle( s )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [99,this.conchThisID,s];//push( "99" + '\4' + this.conchThisID + '\4' + s );
    },
    get fillStyle()
    {
        return this.m_pCContext.fillStyle;
    },
    set strokeStyle( s )
    {
        conch.m_sStreamBuffer[conch.m_sStreamBuffer.index++] = [100,this.conchThisID,s];//push( "100" + '\4' + this.conchThisID + '\4' + s );
    },
    get strokeStyle()
    {
        return this.m_pCContext.strokeStyle;
    }
}
function onCommitStream()
{
    //console.log("onCommitStream = "+conch.m_sStreamBuffer.length);    conch.commitStream( '1' );
    	if( conch.m_sStreamBuffer.length > 0 ){
    		conch.commitStream( '1' );
            conch.m_sStreamBuffer.index = 0;
    		conch.m_sStreamBuffer.length = 0;
    	}
}
window.GetStreamBuffer = function()
{
    return conch.m_sStreamBuffer;
}
conch.createNode = function()
{
	return new ConchNode();
}
conch.setRootNode = function( pNode )
{
	conch._setRootNode( pNode.pCNode );
}
conch.setLogoImageNode = function( pNode )
{
    conch._setLogoImageNode( pNode.pCNode );
}
conch.setOnCommitStream( onCommitStream );