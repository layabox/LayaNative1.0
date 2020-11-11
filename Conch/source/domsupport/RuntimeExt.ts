class ConchNode implements IBuffer{
    _nativeObj:conchNode2D;
    _graphics:ConchGraphics;
    id:number;
    constructor()
    {
        this._nativeObj=new conchNode2D();
        this.id=this._nativeObj.conchID;
    }
    setRootNode()
    {
        this._nativeObj.setRootNode();
    }
    
    setFPSNode()
    {
        this._nativeObj.setFPSNode();
    }
   
    /**
     * 插入子节点
     */
    addChildAt(c:ConchNode,i:number)
    {
        if(!c)return;
        var _:_Buffer=conch.bf;
        _.needsz(16);
        _.wu32(this.id);
        _.wu32(0);
        _.wu32(c.id);
        _.wu32(i);
    }
    /**
     * 删除节点
     */
    removeChild(c:ConchNode)
    {
        if(!c)return;
        _Buffer.oneInt(this,1,c.id);
    }
    
    /**
     * 设置宽高。
     * @param w 宽度。
     * @param h 高度。
     */
    size(w:number,h:number):void
    {
        _Buffer.point(this,2,w,h);
    }
    /**
   	 * 设置坐标位置。
     * @param x X 轴坐标。
     * @param y Y 轴坐标。
     */
    pos(x:number,y:number):void
    {
        _Buffer.point(this,3,x,y);
    }
    /**
     * 设置轴心点。
     * @param	x X轴心点。
     * @param	y Y轴心点。
     * @return	返回对象本身。
     */
    pivot(x:number,y:number):void
    {
        _Buffer.point(this,4,x,y);
    }
    
    /**
     * 设置缩放比例
     * @param x  X轴缩放
     */
    scale(x:number,y:number):void
    {
        _Buffer.point(this,5,x,y);
    }
    
    skew(x:number,y:number):void
    {
        _Buffer.point(this,6,x,y);
    }
    
    /**
     * 旋转
     */
    rotate(r:number):void
    {
        _Buffer.oneNum(this,7,r);
    }
     /**
     * 设置背景色 只有文字有
     */
    bgColor(bg:string):void
    {
        if(!bg)
        {
            this.removeType(0x81);
            return;
        }
        var n:number=ColorUtil.getColorNum(bg);
        if(isNaN(n))
        {
            console.error("bgColor:"+bg+"not  support");
            n = 0xFFFFFFFF;
        }
        _Buffer.oneInt(this,8,n);
    }
    
    /**
     * 设置font字体
     */
    font(str:string):void
    {
        var _:_Buffer=conch.bf;
        _.needsz(12);
        _.wu32(this.id);
        _.wu32(9); 
        _.wstring(str,true);
    }
    
    /**
     * 文字
     */
    text(d:any):void
    {
        //11
    }
    
    transform(a:number,b:number,c:number,d:number,tx:number,ty:number):void
    {
        _Buffer.setMatrix(this,11,a,b,c,d,tx,ty);
    }
    
    setTransform(a:number,b:number,c:number,d:number,tx:number,ty:number):void
    {
        _Buffer.setMatrix(this,12,a,b,c,d,tx,ty);
    }
    
    /**
     * alpha
     */
    alpha(a:number):void
    {
        _Buffer.oneNum(this,13,a);
    }
    
    /**
     * 
     */
    filter(r:number,g:number,b:number,gray:number):void
    {
        _Buffer.rect(this,14,r,g,b,gray);
    }
    

    /**
     * 是否显示
     */
    visible(b:boolean):void
    {
        _Buffer.oneInt(this,15,b?1:0);
    }
    
    /**
     * 设置blendMode
     */
    blendMode(v:string):void
    {
        if(!v)
        {
            this.removeType(0x08);
            return;
        }
        var d:number=CanvasRenderingContext2D.BLEND[v]||0;
        _Buffer.oneInt(this,16,d);
    }
    /**
     * 剪裁
     */
    scrollRect(x:number,y:number,w:number,h:number):void
    {
        _Buffer.rect(this,17,x,y,w,h);
    }
    
    /**
     * 遮罩
     */
    mask(node:ConchNode):void
    {
        var i:number=node?node.id:-1;
        _Buffer.oneInt(this,18,i);
    }
    
    /**
     * 设置graphics
     */
    graphics(g:ConchGraphics):void
    {
        var i:number=g?g.id:-1;
        _Buffer.oneInt(this,19,i);
    }
    
    
    custom(t:CanvasRenderingContext2D)
    {
        if(t==null)
        {
            this.removeType(0x400);
        }
        else
            _Buffer.oneInt(this,20,t.id);
    }
    
    removeType(t:number):void
    {
        /**
         * 下面这段代码虽然有点恶心
         * 但是这是为了兼容老的layaAir 
         */
        switch(t)
        {
            case 0x10:
             t=0x20;
             break;
            case 0x40:
             t=0x80;
             break;
            case 0x100:
             t=0x200;
             break;
            default:
             break;
        }
        _Buffer.oneInt(this,21,t);
    }
    
    cacheAs(t:number):void{
        _Buffer.oneInt(this,22,t);
    }
    
    border(b:string):void
    {
        if(!b)
        {
            this.removeType(0x82);
            return;
        }
        var bc:number=ColorUtil.getColorNum(b);
        if(isNaN(bc))
        {
            console.error("border："+b+"not support");
            bc = 0xFFFFFFFF;
        }
        _Buffer.oneInt(this,23,bc);
    }
    
    optimizeScrollRect(b:boolean):void
    {
        _Buffer.oneInt(this,24,b?1:0);
    }
    
    blurFilter(strength:number):void
    {
        _Buffer.oneNum(this,25,strength);
    }
    
    glowFilter(color:string, blur:number, offX:number, offY:number)
    {
        var c:number=ColorUtil.getColorNum(color);
        if(isNaN(c))
        {
            console.error("growFilter"+color+"not support");
            c = 0xFFFFFFFF;
        }
        var _:_Buffer=conch.bf;
        _.needsz(24);
        _.wu32(this.id);
        _.wu32(26);
        _.wu32(c);
        _.wf32(blur);
        _.wf32(offX);
        _.wf32(offY);
    }
    
    setFilterMatrix(mat:Float32Array,alpha:Float32Array)
    {
        if(!mat||!alpha)
        {
            this.removeType(0x20);
            return;      
        }
        var _=conch.bf;
        _.needsz(8);
        _.wu32(this.id);
        _.wu32(27);
        _.wab(mat,64);
        _.wab(alpha,16);
    } 
    
    repaint():void{
        var _=conch.bf;
        _.needsz(8);
        _.wu32(this.id);
        _.wu32(28);
    } 

    setZOrder(z:number):void{
        _Buffer.oneNum(this,29,z);
    } 

    updateZOrder():void{
        _Buffer.noParm(this,30);
    }

    drawToCanvas(canvas:HTMLCanvasElement,offsetX:number,offsetY:number):void
    {
        var _=conch.bf;
        _.needsz(8);
        _.wu32(this.id);
        _.wu32(31);
        _.wu32(canvas._ctx2d.id);
        _.wf32(offsetX);
        _.wf32(offsetY);
    }
}

class ConchGraphics implements IBuffer
{
    /**
     *对应graphics的drawTexture
     */
    
    /*
    drawTexture(tex:Texture, x:Number, y:Number, width:Number = 0, height:Number = 0, m:Matrix = null)
    {
        if (!width) width = tex.sourceWidth;
			if (!height) height = tex.sourceHeight;
			
			//处理透明区域裁剪
			x += tex.offsetX;
			y += tex.offsetY;
			width = width - tex.sourceWidth + tex.width;
			height = height - tex.sourceHeight + tex.height;
			width < 0 && (width = 0);
			height < 0 && (height = 0);
            var uv:Array = tex.uv, w:Number = tex.bitmap.width, h:Number = tex.bitmap.height;
		    if (m)	this.drawImage(tex.bitmap.source, uv[0] * w, uv[1] * h, (uv[2] - uv[0]) * w, (uv[5] - uv[3]) * h, x + tx, y + ty, width, height);
    }
    
    
    */
    private _cmds:Array<any> = null;
    private  _temp:Array<number>;
    private  _bounds:any;
    private  _rstBoundPoints:Array<number>;
    private  _sp:any;
    private  _one:any;
    _nativeObj:_conchGraphics;
    id:number;
    constructor()
    {
        this._nativeObj=new _conchGraphics();
        this.id=this._nativeObj.conchID;
    }
    
    private _addCmd(a:any)
    {
        if(!this._cmds)this._one=a;
        this._cmds=this._cmds||[];
        this._cmds.push(a);
    }
    _repaint():void{

    }
    clear():void
    {
        this._repaint();
       // this._sp=null;
        this._cmds&&(this._cmds.length=0);
        _Buffer.noParm(this,0);
    }
    
    destroy():void
    {
        this._sp=null;
        this._cmds=null;
        this._temp=null;
        this._bounds=null;
        this._rstBoundPoints=null;
        _Buffer.noParm(this,1);
    }
    
    alpha(a:number):void
    {
        _Buffer.oneNum(this,2,a);
    }
    
    rotate(angle:number,pivotX:number, pivotY:number):void
    {
        var _:_Buffer=conch.bf;
        _.needsz(20);
        _.wu32(this.id);
        _.wu32(3);
        _.wf32(angle);
        _.wf32(pivotX||0);
        _.wf32(pivotY||0);
        this._addCmd([3,angle,pivotX||0,pivotY||0]);
    }
    /**
     * pivotX 中心点偏移
     * pivotY
     */
    transform(mat:any,pivotX:number,pivotY:number):void
    {
        _Buffer.setMatrix(this,4,mat.a,mat.b,mat.c,mat.d,mat.tx-(pivotX||0),mat.ty-pivotY||0);
        this._addCmd([4,mat,pivotX||0,pivotY||0]);    
    }
    
    scale(x:number,y:number,pivotX:number,pivotY:number):void
    {
        _Buffer.rect(this,5,x,y,pivotX||0,pivotY||0);
        this._addCmd([5,x,y,pivotX||0,pivotY||0]);
    }
    
    translate(x:number,y:number):void
    {
        _Buffer.point(this,6,x,y);
        this._addCmd([6,x,y]);
    }
    
    save():void
    {
        _Buffer.noParm(this,7);
        this._addCmd([7]);
    }
    
    restore():void
    {
        _Buffer.noParm(this,8);
        this._addCmd([8]);
    }
    
    clipRect(x:number,y:number,w:number,h:number):void
    {
        _Buffer.rect(this,9,x,y,w,h);
    }
    /**
     * c++  int
     */
    blendMode(s:string):void
    {
       _Buffer.oneInt(this,10, CanvasRenderingContext2D.BLEND[s]||0); 
    }
    
    fillText(text:string,x:number,y:number,font:string,color:string,textAlign:string,decorationLine:number):void
    {
        var d:number=ColorUtil.getColorNum(color);
        if(isNaN(d)){
            console.error("graphic fillText color :"+color+" not support");
            d = 0xFFFFFFFF;
        }
        if( decorationLine > 0 )
		{
			//先加描边的值 和 color  
			//再增加线的描述 和 color
			font += " " + 0 + " " + "#000000" + " " + decorationLine + " " + color;
		}
        var _:_Buffer=conch.bf;
        _.needsz(28);
        _.wu32(this.id);
        _.wu32(11);              
        _.wf32(x);
        _.wf32(y);
        _.wu32(d);
        _.wu32(CanvasRenderingContext2D.TEXTALIGN[textAlign]||0);
        _.wstring(font,true);
        _.needsz(4);
        _.wstring(text,true);  
    }
    
    fillBorderText(text:string, x:number, y:number, font:string, fillColor:string, borderColor:string, lineWidth:number, textAlign:string,decorationLine:number):void
    {
        if (borderColor)
		{
            font += " " + 1 + " " + borderColor;
			if( decorationLine > 0 )
			{
				font += " " + decorationLine + " " + fillColor;
			}
		}
		else
		{
			if( decorationLine > 0 )
			{
				font += " " + 0 + " " + "#000000" + " " + decorationLine + " " + fillColor;
			}
		}
        //再调用这个接口的时候，下划线传入的值为0，因为在这都处理了
        this.fillText(text,x,y,font,fillColor,textAlign,0);
    }
    
    private _fands(fillColor:string, lineColor:string, lineWidth:number)
    {
        var _:_Buffer=conch.bf;
        lineWidth=lineWidth||1;
        var fd=-1,ld=-1,flag=1;
        if(fillColor)
        {
            fd=ColorUtil.getColorNum(fillColor);
            if(isNaN(fd)){
                console.error("graphics drawRect fillcolor:"+fillColor+" not support");
                fd=0xFFFFFFFF;
            }
        }
        else{
            flag=-1;
        }
      
        if(lineColor)
        {
            ld=ColorUtil.getColorNum(lineColor);
            if(isNaN(ld))
            {
                console.error("graphics drawRect lineColor"+lineColor+" not support");
                ld = 0xFFFFFFFF;
            }
        }
        else
        {
            if(flag>0)
                flag=-2;
            else
                flag=-3;
        }
        _.needsz(16);
        _.wf32(lineWidth);
        _.wu32(fd);
        _.wu32(ld);
        _.wu32(flag);
    }
    
    drawRect(x:number, y:number, width:number, height:number, fillColor:string, lineColor:string, lineWidth:number)
    {
        var offset:number = lineColor ? 0.5 : 0;
        _Buffer.rect(this,13,x+offset,y+offset,width,height);
        this._fands(fillColor,lineColor,lineWidth);
        this._addCmd([13,x+offset,y+offset,width,height]);
    }
    
    drawCircle(x:number, y:number, radius:number, fillColor:string, lineColor:string, lineWidth:number):void
    {
        var offset:number = lineColor ? 0.5 : 0;
        var _:_Buffer=conch.bf;
        _.needsz(20);
        _.wu32(this.id);
        _.wu32(14);
        _.wf32(x+offset);
        _.wf32(y+offset);
        _.wf32(radius);
        this._fands(fillColor,lineColor,lineWidth);
        this._addCmd([14,x+offset,y+offset,radius]);
    }
    
    drawPie(x:number, y:number, radius:number, startAngle:number, endAngle:number, fillColor:string, lineColor:string, lineWidth:number):void 
    {
        var offset:number = lineColor ? 0.5 : 0;
        var _:_Buffer=conch.bf;
        _.needsz(28);
        _.wu32(this.id);
        _.wu32(15);
        _.wf32(x+offset);
        _.wf32(y+offset);
        _.wf32(radius);
        _.wf32(startAngle);
        _.wf32(endAngle);
        this._fands(fillColor,lineColor,lineWidth);
        this._addCmd([15,x+offset,y+offset,radius,startAngle,endAngle]);
    }
    
    drawImageM(img,sx:number,sy:number,sw:number,sh:number,dx:number,dy:number,dw:number,dh:number,mat:any,alpha:number):void
    {
        alpha = alpha ? alpha : 1.0;
        var sz:number=mat?76:52;
        var fid:number=mat?17:16;
        var _:_Buffer=conch.bf;
        _.needsz(sz);
        //img有可能是image，有可能是canvas
        var bImage:Boolean = img instanceof HTMLImageElement;
        _.wu32(this.id);
        _.wu32(fid);
        _.wu32(bImage?img.imgId:img._ctx2d.id);
        _.wf32Rect(sx,sy,sw,sh);
        _.wf32Rect(dx,dy,dw,dh);
        _.wu32(bImage?0:1);
        _.wf32(alpha);
        if(mat)
        {
            _.wf32Rect(mat.a,mat.b,mat.c,mat.d);
            _.wf32(mat.tx);
            _.wf32(mat.ty);
            this._addCmd([17,dx,dy,dw,dh,mat,img]);
        }
        else{
            this._addCmd([16,dx,dy,dw,dh,img]);
        }
    }
   
    
    drawPoly(x:number,y:number,points:number[],fillColor:string,lineColor:string,lineWidth:number):void
    {
        var _:_Buffer=conch.bf,pl:number=points.length,i:number;
        _.needsz(pl*4+36);
        _.wu32(this.id);
        _.wu32(18);
        _.wu32(pl);
        _.wf32(x);
        _.wf32(y);
        this._fands(fillColor,lineColor,lineWidth);
        for(i=0;i<pl;i++)
        {
            _.wf32(points[i]);
        }
        this._addCmd([18,x,y,points]);
    }
    
    drawPath(x:number, y:number, paths:Array<Array<any>>,brush:any, pen:any):void 
    {
       var _:_Buffer=conch.bf,pl:number=paths.length,i:number,bl:number=0,plen:number=0,fc:number,sc:number,ab:ArrayBuffer,bfl:number=0;
       if(brush)
       {
           bl=1;
           fc=ColorUtil.getColorNum(brush.fillStyle);
           if(isNaN(fc))
           {
               console.error("brush fillStyle is not support"+brush.fillStyle);
               fc = 0xFFFFFFFF;
           }
       }
       if(pen)
       {
           plen=5;
           sc=ColorUtil.getColorNum(pen.strokeStyle);
           if(isNaN(sc))
           {
               console.error("pen strokeStyle is not support"+pen.strokeStyle);
               sc = 0xFFFFFFFF;
           }
       }
       if(pl>0)
       {
           ab=new ArrayBuffer(pl*24);
           var dv:DataView=new DataView(ab);
           /*function wf32(n:number)
           {
               dv.setFloat32(bfl,n,true);
               bfl+=4;
           }
           function wu32(n:number)
           {
               dv.setUint32(bfl,n,true);
               bfl+=4;
           }*/
            for (var i:number=0;i < pl;i++)
            {
                var path=paths[i];
                switch (path[0]){
                    case "moveTo":
                        dv.setUint32(bfl,0,true);bfl+=4;//wu32(0);
                        dv.setFloat32(bfl,path[1],true);bfl+=4;//wf32(path[1]);
                        dv.setFloat32(bfl,path[2],true);bfl+=4;//wf32(path[2]);
                        break ;
                    case "lineTo":
                        dv.setUint32(bfl,1,true);bfl+=4;//wu32(1);
                        dv.setFloat32(bfl,path[1],true);bfl+=4;//wf32(path[1]);
                        dv.setFloat32(bfl,path[2],true);bfl+=4;//wf32(path[2]);
                        break ;
                    case "arcTo":
                        dv.setUint32(bfl,2,true);bfl+=4;//wu32(2);
                        dv.setFloat32(bfl,path[1],true);bfl+=4;//wf32(path[1]);
                        dv.setFloat32(bfl,path[2],true);bfl+=4;//wf32(path[2]);
                        dv.setFloat32(bfl,path[3],true);bfl+=4;//wf32(path[3]);
                        dv.setFloat32(bfl,path[4],true);bfl+=4;//wf32(path[4]);
                        dv.setFloat32(bfl,path[5],true);bfl+=4;//wf32(path[5]);
                        break ;
                    case "closePath":
                        dv.setUint32(bfl,3,true);bfl+=4;//wu32(3);
                        break ;
                    }
            };
       }
       else return;
        _.needsz((bl+plen+7)*4+bfl);
        _.wu32(this.id);
        _.wu32(19);
        _.wf32(x+0.5);
        _.wf32(y+0.5);
        _.wu32(bl);
        _.wu32(plen);
        _.wu32(bfl);       
        _.wab(ab,bfl);
        if(bl>0)
        {
            _.wu32(fc);
        }
        
        if(plen>0)
        {
            _.wu32(sc);
            _.wu32(CanvasRenderingContext2D.LINEJOIN[pen.lineJoin]||0);
            _.wu32(CanvasRenderingContext2D.LINECAP[pen.lineCap]||0);
            _.wf32(pen.lineWidth||1);
            _.wf32(pen.miterLimit);
        }
        this._addCmd([19,x+0.5,y+0.5,paths]);
    }
    
    drawLine(x1:number, y1:number, x2:number, y2:number, lineColor:string, lineWidth:number):void
    {
        var lc=ColorUtil.getColorNum(lineColor);
        if(isNaN(lc))
        {
            console.error("drawLine lineColor:"+lineColor+"not support");
            lc = 0xFFFFFFFF;
        }
        var _:_Buffer=conch.bf;
        x1+=0.5;y1+=0.5;x2+=0.5;y2+=0.5;
        _.needsz(32);
        _.wu32(this.id);
        _.wu32(20);
        _.wf32Rect(x1,y1,x2,y2);
        _.wf32(lineWidth||1);
        _.wu32(lc);
        this._addCmd([20,x1,y1,x2,y2]);
    }
    
    private _drawPs(fid:number,x:number,y:number,points:Array<number>,lineColor:string,lineWidth:number):void
    {
        var lc=ColorUtil.getColorNum(lineColor),pl:number=points.length;
        if(isNaN(lc))
        {
            console.error("_drawPs lineColor:"+lineColor+"not support");
            lc = 0xFFFFFFFF;
        }
         var _:_Buffer=conch.bf;
         _.needsz(28+pl*4);
         _.wu32(this.id);
         _.wu32(fid);
         _.wf32(x+0.5);
         _.wf32(y+0.5);
         _.wf32(lineWidth||1);
         _.wu32(lc);
         _.wu32(pl);
         for(var i:number=0;i<pl;i++)
         {
             _.wf32(points[i]);
         }
    }
    drawLines(x:number,y:number,points:Array<number>,lineColor:string,lineWidth:number):void
    {
         this._drawPs(21,x,y,points,lineColor,lineWidth);
    }
    
    drawCurves(x:number,y:number,points:Array<number>,lineColor:string,lineWidth:number):void
    {
        this._drawPs(22,x,y,points,lineColor,lineWidth);
        this._addCmd([22,x+0.5,y+0.5,points]);
    }
    
    replaceText(text:string):boolean
    {
        var _:_Buffer=conch.bf;
        _.needsz(12);
        _.wu32(this.id);
        _.wu32(28);
        _.wstring(text, true);
        return true;
    }
    
    replaceTextColor(c:string):void
    {
        var fc=ColorUtil.getColorNum(c);
        if(isNaN(fc))
        {
            console.error("replaceTextColor:"+c+"not support");
            fc=0xFFFFFFFF;
        }
        _Buffer.oneInt(this,23,fc);
    }
    
    _fillImage(pat:CanvasPattern,x:number,y:number,fx:number,fy:number,w:number,h:number)
    {
         var _:_Buffer=conch.bf;
         _.needsz(56);
         _.wu32(this.id);
         _.wu32(24);
         _.wu32(pat.image);
         _.wu32(pat.repetition);
         _.wf32Rect(pat.x,pat.y,pat.w,pat.h);

         switch(pat.repetition)
         {
             case 0:
             break;
             case 1:
              if(fy>0)
              {
                  if(pat.h-fy<h)
                  {
                      h=pat.h-fy;
                  }
              }
              else
              {
                  y-=fy;
                  if(pat.h-fy>h)
                  {
                      h=h+fy;
                  }
                  else
                  {
                      h=pat.h;
                  }
                  fy=0;
              }
              break;
             case 2:
              if(fx>0)
              {
                  if(pat.w-fx<w)
                   w=pat.w-fx;
              }
              else
              {
                  x-=fx;
                  if(pat.w-fx>w)
                  {
                      w=w+fx;
                  }
                  else
                  {
                    w=pat.w;
                  }
                  fx=0;
              }
              break;
             default:
              break;
         }
         _.wf32Rect(x,y,fx,fy);
         _.wf32(w);
         _.wf32(h);
         this._addCmd([24,x+fx,y+fy,w,h]);
    }
    
     setSkinMesh(ib:any/*IndexBuffer*/,vb:any/*VertexBuffer*/,eleNum:number,iStart:number,texture:any,mat:any/*Matrix*/){
        var img:HTMLImageElement=texture.source;
        var _:_Buffer=conch.bf;
        if(ib.conch_id===undefined||!ib._upload)
        {
            ib.conch_id=CanvasRenderingContext2D.ID++;
            ib.conchRef = new ArrayBufferRef();
            ib.conchRef.id = ib.conch_id;
            _.needsz(16);
            _.wu32(0);
            _.wu32(11);
            _.wu32(ib.conch_id);
            _.wu32(ib.length*2);
            ib.buffer=new Uint16Array(ib);
            _.wab(ib.buffer,ib.length*2);
            ib._upload=true;
        }
        
        if(vb.conch_id===undefined||!vb._upload)
        {
            vb.conch_id=CanvasRenderingContext2D.ID++;
            vb.conchRef = new ArrayBufferRef();
            vb.conchRef.id = vb.conch_id;
            _.needsz(16);
            _.wu32(0);
            _.wu32(11);
            _.wu32(vb.conch_id);
            _.wu32(vb.byteLength);
            _.wab(vb,vb.byteLength);
            vb._upload=true;
        }
        
        var shader=CanvasRenderingContext2D.skinMeshShader;
        if(shader.conch_id===undefined)
        {
            _.needsz(16);
            _.wu32(0);
            _.wu32(12);
            _.wu32(shader.id);
            _.wstring(shader.vs,true);
            _.needsz(4);
            _.wstring(shader.ps,true);
            shader.conch_id=shader.id;
        }
        _.needsz(68);
        _.wu32(this.id);
        _.wu32(25);
        _.wu32(ib.conch_id);
        _.wu32(vb.conch_id);
        _.wu32(1);
        _.wu32(eleNum);
        _.wu32(iStart);
        _.wu32(img.imgId);
        _.wu32(shader.id);
        var uv=texture.uv;
        _.wf32(mat.a);
        _.wf32(mat.b);
        _.wf32(mat.c);
        _.wf32(mat.d);
        _.wf32(mat.tx);
        _.wf32(mat.ty);
        _.wf32(0);
        _.wf32(0);
    }
    
    drawParticle(par:any)
    {
        var _:_Buffer=conch.bf;
        _.needsz(20);
        _.wu32(this.id);
        _.wu32(26);
        _.wu32(par?par.id:-1);
    }

    drawImageS(img,sx:number,sy:number,sw:number,sh:number,dx:number,dy:number,dw:number,dh:number,pos:Float32Array|Array<number>)
    {
        var _:_Buffer=conch.bf;
        var len:number;
        if(!(pos instanceof Float32Array))
        {
            if(CanvasRenderingContext2D.DRAWIMAGES.length<pos.length)
            {
                CanvasRenderingContext2D.DRAWIMAGES=new Float32Array(pos);

            }
            else 
            {
                CanvasRenderingContext2D.DRAWIMAGES.set(pos);
            }
            len=pos.length*4;
            pos=CanvasRenderingContext2D.DRAWIMAGES;
        }
        len= len||(<Float32Array>pos).byteLength;
        _.needsz(52);
        _.wu32(this.id);
        _.wu32(27);
        //img有可能是image，有可能是canvas
        var bImage:Boolean = img instanceof HTMLImageElement;
        _.wu32(bImage?img.imgId:img._ctx2d.id);
        _.wf32(sx);
        _.wf32(sy);
        _.wf32(sw);
        _.wf32(sh);
        _.wf32(dx);
        _.wf32(dy);
        _.wf32(dw);
        _.wf32(dh);
        _.wu32(bImage?0:1);
        _.wu32(len);
        _.wab(<Float32Array>pos,len);
    }
    
    
    
}
window["ConchNode"]=ConchNode;
window["ConchGraphics"]=ConchGraphics;

/**
 * name
 */

conch["platCallBack"]=function(ret)
{
    var objid,m,rs,c,rJSON;
    if(ret==null)return;
    objid=ret.objId;
    m=ret.mName;
    c=ret.cName;
    rs=ret.v;
    var platC:IPlatForm;
    if(objid!=-1){
        platC=PlatformObj.objMap[objid];
    }
    else{
        platC=PlatformClass.clsMap[c];
    }
    if(platC){
        var callback:Function= platC.callBackList[m];
        callback&&callback(rs);
    }
}
interface IPlatForm
{
    callBackList:Object;
    call(methodName,p1?:any,p2?:any,p3?:any,p4?:any,p5?:any,p6?:any,p7?:any,p8?:any,p9?:any);
    callWithBack(callback,methodName,p1?:any,p2?:any,p3?:any,p4?:any,p5?:any,p6?:any,p7?:any,p8?:any,p9?:any):void
    
}
class PlatformBase implements IPlatForm{
    callBackList:Object;
    className:string;
    objid:number;
    call(methodName,p1?:any,p2?:any,p3?:any,p4?:any,p5?:any,p6?:any,p7?:any,p8?:any,p9?:any)
    {
        var a:Array<any>= Array.prototype.slice.call(arguments);
        a.shift();
        try{
            var rs=JSON.parse(conch.callMethod(this.objid,true,this.className,methodName,JSON.stringify(a))||"{}");
            return rs.v;
        }
        catch(e)
        {
            return null;
        }
    }
    
    callWithBack(callback,methodName,p1?:any,p2?:any,p3?:any,p4?:any,p5?:any,p6?:any,p7?:any,p8?:any,p9?:any):void
    {
        this.callBackList[methodName]=callback;
        var a:Array<any>= Array.prototype.slice.call(arguments);
        a.splice(0,2)
        try{
            var rs=JSON.parse(conch.callMethod(this.objid,false,this.className,methodName,JSON.stringify(a))||"{}");
            return rs.v;
        }
        catch(e)
        {
            return null;
        }
    }
}

class PlatformClass extends PlatformBase {
    static clsMap:Object={};
    constructor(clsName:string)
    {
        super();
        this.objid=-1;
        this.className=clsName;
        this.callBackList={};
        PlatformClass.clsMap[clsName]=this;
    }
    static createClass(clsName:string):PlatformClass
    {
        return PlatformClass.clsMap[clsName]||new PlatformClass(clsName);
    }
    
    newObject(p1?:any,p2?:any,p3?:any,p4?:any,p5?:any,p6?:any,p7?:any,p8?:any,p9?:any):PlatformObj
    {
        var a:Array<any>= Array.prototype.slice.call(arguments);
        a.splice(0,0,"<init>");
        var rs=new PlatformObj(this);
        rs.init.apply(rs,a);
        return rs;
    }
}
class PlatformObj extends PlatformBase{
    static objMap:Object={};
    static objNum:number=0;
    cls:PlatformClass;
    callBackList:Object;
    constructor(cls:PlatformClass)
    {
        super();
        this.cls=cls;
        this.className=cls.className;
        this.callBackList={};
    }
    init()
    {
        this.objid=PlatformObj.objNum++;
        this.call.apply(this,arguments)||-1;
        PlatformObj.objMap[this.objid]=this;
    }
}
class ConchFps extends ConchNode{
    color:string;
    fontstr:string;
    fontSize:number;
    fps:number;
    vertex:number;
    drawCall:number;
    countNode:number;
    _repaint:boolean;
    left:ConchNode;
    textAligh:string;
    
    constructor(color:string,fontSize:number,x:number,y:number)
    {
        super();
        this.pos(x,y);
        this.color=color;
        this.fontSize=fontSize;
        this.fontstr="normal 100 "+fontSize+"px Arial";
        this._repaint=false;
        this.fps=this.vertex=this.drawCall=this.countNode=0;
        this.textAligh="left";
        this.left=new ConchNode();
        this.addChildAt(this.left,0);
        var g=new ConchGraphics();
        this._graphics=g;
        g.fillText("FPS",2,2,this.fontstr,color,this.textAligh,0);
        g.fillText("Vertex",2,fontSize+10,this.fontstr,color,this.textAligh,0);
        g.fillText("DrawCall",2,fontSize*2+20,this.fontstr,color,this.textAligh,0);
        g.fillText("Sprite",2,fontSize*3+30,this.fontstr,color,this.textAligh,0);
        this.graphics(g);
        this.left.pos(fontSize*5,0);
        this.left._graphics=new ConchGraphics();
        this.left.graphics(this.left._graphics);
        var g=this.left._graphics;
        g.fillText("0",0,2,this.fontstr,this.color,this.textAligh,0);
        g.fillText("0",0,this.fontSize+10,this.fontstr,this.color,this.textAligh,0);
        g.fillText("0",0,this.fontSize*2+20,this.fontstr,this.color,this.textAligh,0);
        g.fillText("0",0,this.fontSize*3+30,this.fontstr,this.color,this.textAligh,0);
        this.setFPSNode();
        this.size(this.fontSize*7+10,this.fontSize*4+40);
        this.bgColor("rgba(150,150,150,0.8)");
    }
    preVetex:number;
    preFps:number;
    preDrawCall:number;
    render(f:number)
    {
        var t = conch.getCountVertext();
        var d = conch.getCountGroup();
        var countNode = conch.getCountNode();
        if(this.preVetex!=undefined)
        {
            d=d-2;
            var last=(''+ this.preVetex+this.preFps+this.preDrawCall).length;
            t-=(last*4+72);//17*4+4
            if(d<0)d=0;
            if(t<0)t=0;
        }
        this.preVetex=t;
        this.preDrawCall=d;
        this.preFps=f;
        if(this.countNode != countNode)
        {
            this.countNode = countNode;
            this._repaint = true;
        }
        if(this.vertex!=t)
        {
            this.vertex=t;
            this._repaint=true;
        }
        if(this.fps!=f)
        {
            this.fps=f;
            this._repaint=true;
        }
        if(this.drawCall!=d)
        {
            this.drawCall=d;
            this._repaint=true;
        }
        if(this._repaint)
        {
            var g=this.left._graphics;
            g.clear();
            g.fillText(this.fps+"",0,2,this.fontstr,this.color,this.textAligh,0);
            g.fillText(this.vertex+"",0,this.fontSize+10,this.fontstr,this.color,this.textAligh,0);
            g.fillText(this.drawCall+"",0,this.fontSize*2+20,this.fontstr,this.color,this.textAligh,0);
            g.fillText(this.countNode+"",0,this.fontSize*3+30,this.fontstr,this.color,this.textAligh,0);
            
        }
    }
}
window["PlatformClass"]=PlatformClass;
window["PlatformObj"]=PlatformObj;

/**
 * 直接加载缓存中的某个文件
 * @param cache
 * @param relUrl  相对路径，从/开始。例如 '/test.html'
 */
function loadRawCache(cache:AppCache, relUrl:string, encode:'raw'|'utf8'):string|ArrayBuffer{
    var cpath = cache.getCachePath();
    var relFile = relUrl; 
    var id =  new Uint32Array([cache.hashstr(relFile)])[0].toString(16);
    var fn = cpath+'/files/'+id.substr(0,2)+'/'+id.substr(2);
    var ab = fs_readFileSync(fn);
    if(ab){
        var content = new Uint8Array(ab,48);
        //content就是文件内容
        //如果需要字符串
        if(encode==='utf8'){
            var strCont = String.fromCharCode.apply(null,content);
            return strCont
        }
        else
            return content.buffer;
    }  
    return null;
}


class ConchParticleTemplate2D implements IBuffer
{
    id:number;
    _tex:any;
    _nativeObj:_conchParticleTemplate2D;
    constructor()
    {
        this._nativeObj=new _conchParticleTemplate2D();
        this.id=this._nativeObj.conchID;
        //this.initSetting(setting);
        this.setShader();   
    }
    
    set settings(st:any)
    {
        var _:_Buffer=conch.bf;
        _.needsz(32);
        _.wu32(this.id);
        _.wu32(0);
        _.wu32(216);
        _.wu32(st.textureCount);
        _.wu32(st.maxPartices);
        _.wu32(st.blendState === 7 ? 7 : st.blendState + 11);
        _.wu32(st.useEndRadian?1:0);
        _.wu32(st.colorComponentInter?1:0);
        _.wu32(st.disableColor?1:0);

        var temp=new Float32Array([
            st.duration,//18
            st.ageAddScale,//7.58
            st.emitterVelocitySensitivity,//1
            st.minStartSize,//0
            st.maxStartSize,//0
            st.minEndSize,//65.35
            st.maxEndSize,//20.29
            st.minHorizontalVelocity,//0.1
            st.maxHorizontalVelocity,//-15
            st.minVerticalVelocity,//0.1
            st.maxVerticalVelocity,//-10
            st.endVelocity,//0.1
            st.minRotateSpeed,//6.28
            st.maxRotateSpeed,//2
            st.minStartRadius,//93.81
            st.maxStartRadius,//92.2
            st.minEndRadius,//75.5
            st.maxEndRadius,//86.64
            st.minHorizontalStartRadian,//0
            st.maxHorizontalStartRadian,//0
            st.minVerticalStartRadian,//0
            st.maxVerticalStartRadian,//0
            st.minHorizontalEndRadian,//0
            st.maxHorizontalEndRadian,//0
            st.minVerticalEndRadian,//11.14
            st.maxVerticalEndRadian,//20.42
            st.gravity[0],//0
            st.gravity[1],//0
            st.gravity[2],//0
            st.minStartColor[0],//1
            st.minStartColor[1],//0
            st.minStartColor[2],//0
            st.minStartColor[3],//1
            
            st.maxStartColor[0],//1
            st.maxStartColor[1],//0
            st.maxStartColor[2],//0
            st.maxStartColor[3],//1
            
            st.minEndColor[0],//0
            st.minEndColor[1],//1
            st.minEndColor[2],//0
            st.minEndColor[3],//1
            
            st.maxEndColor[0],//0
            st.maxEndColor[1],//1
            st.maxEndColor[2],//0
            st.maxEndColor[3],//1
            
            st.positionVariance[0],
            st.positionVariance[1],
            st.positionVariance[2],
        ]);
        _.wab(temp,192);
    }
    
    setShader()
    {
        var _:_Buffer=conch.bf;
        var shader=CanvasRenderingContext2D.particleShader;
        if(shader.conch_id==undefined)
        {
            shader.conch_id=shader.id;
            _.needsz(16);
            _.wu32(0);
            _.wu32(12);
            _.wu32(shader.conch_id);
            _.wstring(shader.vs,true);
            _.needsz(4);
            _.wstring(shader.ps,true);
        }
    }
    get texture():any{
        return this._tex;
    }
    set texture(tex:any)
    {
        this._tex=tex;
        _Buffer.oneInt(this,1,tex.source.imgId);
    }
    
    dispose()
    {
        
    }
    
    play()
    {
        _Buffer.noParm(this,2);
    }
    
    stop()
    {
        _Buffer.noParm(this,3);
    }
}
window["ConchParticleTemplate2D"]=ConchParticleTemplate2D;

// class ConchByte
// {
//     private _u8d_:Uint8Array;
//     private _d_:DataView;
//     private _length:number;
//     private _xd_:boolean;
//     private _pos_:number;
//     constructor(data:ArrayBuffer){
//         this._xd_=true;
//         this._pos_=0;
//         this._u8d_ = new Uint8Array(data);
//         this._d_ = new DataView(this._u8d_.buffer);
//         this._length = this._d_.byteLength;
//     }
    
//     get pos():number
//     {
//         return this._pos_;
//     }
    
//     set pos(p:number)
//     {
//         this._pos_=p;
//     }
//      /**
//      * 在指定字节偏移量位置处读取 Float32 值。
//      * @return Float32 值。
//      */
// 	getFloat32():number {
// 			var v:number = this._d_.getFloat32(this._pos_, this._xd_);
// 			this._pos_ += 4;
// 			return v;
// 	}
    
//     /**
// 		 * 在当前字节偏移量位置处读取 Uint16 值。
// 		 * @return Uint16 值。
//    */
// 	getUint16():number {
//         var us:number = this._d_.getUint16(this._pos_, this._xd_);
//         this._pos_ += 2;
//         return us;
// 	}
    
//     /**
//      * 在当前字节偏移量位置处读取 Int16 值。
//      * @return Int16 值。
//      */
// 	getInt16():number {
//         var us:number = this._d_.getInt16(this._pos_, this._xd_);
//         this._pos_ += 2;
//         return us;
//     }
//     /**
//     *在当前字节偏移量位置处读取 Int32 值。
//     *@return Int32 值。
//     */
//     getInt32():number{
//         var float=this._d_.getInt32(this._pos_,this._xd_);
//         this._pos_+=4;
//         return float;
//     }
    
//     /**
//     *在当前字节偏移量位置处读取 Uint8 值。
//     *@return Uint8 值。
//     */
//     getUint8=function(){
//         return this._d_.getUint8(this._pos_++);
//     }
    
    
// }
// class ConchNodeNew extends ConchNode{
//     _childs:Array<ConchNodeNew>;
//     zOrder:number;
//     parentNode:ConchNodeNew;
//     addChild(child:ConchNodeNew):ConchNodeNew
//     {
//         this._childs=this._childs||[];
//         this.addChildAt(child,this._childs.length);
//         this._childs.push(child);
//         child.parentNode=this;
//         return child;
//     }
    
//     loadImage(url:string):void
//     {
//         this._graphics=this._graphics||new ConchGraphics();
//         var img:HTMLImageElement=<HTMLImageElement>document.createElement("image");
//         var that=this;
//         img.onload=function(e)
//         {
//             var w=img.width,h=img.height;
//             that._graphics.drawImageM(img,0,0,w,h,0,0,w,h,null);
//         }
//         img.src=url;
//         this.graphics(this._graphics);
//     }
    
//     removeChildNew(node:ConchNodeNew):ConchNodeNew
//     {
//         if(!this._childs)return null;
//         var i:number=this._childs.indexOf(node);
//         if(i!=-1)
//         {
//             this._childs.splice(i,1);
//             this.removeChild(node);
//             node.parentNode=null;
//         }
//         return node;
//     }
    
//     removeSelf():ConchNodeNew
//     {
//         this.parentNode&&this.parentNode.removeChildNew(this);
//         return this;
//     }
//     updateZOrder():void{
//         if ( (!this._childs) || this._childs.length < 2) return;
//             var c:ConchNodeNew = this._childs[0];
// 			var i:number = 1, sz:number = this._childs.length;
// 			var z:number = c.zOrder||0, low:number, high:number, mid:number, zz:number;
// 			for (i = 1; i < sz; i++) {
// 				c = this._childs[i];
// 				if (!c) continue;
// 				if ((z = c.zOrder||0) < 0) z = c.zOrder||0;
// 				if (z < this._childs[i - 1].zOrder||0)//如果z小于前面，找到z>=的位置插入
// 				{
// 					mid = low = 0;
// 					high = i - 1;
// 					while (low <= high) {
// 						mid = (low + high) >>> 1;
// 						if (!this._childs[mid]) break;//这里有问题
// 						zz = this._childs[mid].zOrder||0;
// 						if (zz < 0) zz = this._childs[mid].zOrder||0;
// 						if (zz < z)
// 							low = mid + 1;
// 						else if (zz > z)
// 							high = mid - 1;
// 						else break;
// 					}
// 					if (z > (this._childs[mid].zOrder||0)) mid++;
// 					var f = c.parentNode;
// 					this._childs.splice(i, 1);
//                		this._childs.splice(mid, 0, c);
// 					if (f )
// 					{
// 						f.removeChild(c);
// 						f.addChildAt(c, mid);
// 					}
// 				}
// 			}
        
//     }
//     removeChildren():void{
//         if(this._childs&&this._childs.length>0)
//         {
//             var len=this._childs.length;
//             var childs=this._childs.splice(0,len);
//             this._childs.length=0;
//             for(var i=0;i<len;i++)
//             {
//                 this.removeChild(childs[i]);
//             }
            
//         }
//     }
    
// }

// class ConchLoadingView extends ConchNodeNew{
//     mainCanvas:HTMLCanvasElement;
//     w:number;
//     h:number;
//     mc:ConchMovieClip;
//     n:ConchNodeNew;
//     bar:ConchNodeNew;
//     p:number;
//     constructor(){
//         super();
//         this.p=0;
//         this.initSwf();
//         this.setLoadingView();
//     }
//     initSwf()
//     {
//         var mc=this.mc=new ConchMovieClip(null);
//         mc.load("http://10.10.30.29:8866/res/swf/5.swf",this.onloaded.bind(this));
//         this.bgColor("#ffffff");
//         var n=this.n=new ConchNodeNew();
//         var base=this.createMBar("http://10.10.30.29:8866/res/swf/2.png",400);
//         this.bar=this.createMBar("http://10.10.30.29:8866/res/swf/1.png",1);
//         var lbar=new ConchNodeNew();
//         lbar.addChild(base);
//         lbar.addChild(this.bar);
//         n.addChild(mc);
//         n.addChild(lbar);
//         this.addChild(n);
//         this.onresize();
//         document.addEventListener("resize",this.onresize.bind(this));
//     }
    
//     onresize()
//     {
//         if(!this.mainCanvas)
//         {
//             this.mainCanvas=document.createElement("canvas");
//             this.mainCanvas.getContext("conch");
//         }         
//         this.w=window.innerWidth;
//         this.h=window.innerHeight;
//         this.mainCanvas.setSize(this.w,this.h);
//         this.size(this.w,this.h);  
//         var a=480;
//         var b=640;
//         var sx=this.w/a;
//         var sy=this.h/b;
//         var scale=Math.min(sx,sy);
//         var x=(this.w- scale*a)/2;
//         var y=(this.h- scale*b)/2;
//         if(this.n)
//         {
//             this.n.scale (scale,scale);
//             this.n.pos(x,y);
//             var lbar=this.n._childs[1];
//             if(lbar)
//             {
//                 lbar.pos(40,b/1.5);
//             }
//         }
//     }
    
//     onloaded()
//     {
//         var _t=this;
//         var i=0;
        
//         document.addEventListener("mousedown",function(e) {
//               if(i>1)i=i-1;
//             _t.loading(i);
//             i+=0.02;
//         });
//     }
    
//     createMBar(src,w:number):ConchNodeNew
//     {
//         var t=new ConchNodeNew();
//         var i=document.createElement("img");
//         i.onload=function(e) {
//             var g=t._graphics=new ConchGraphics();
//             t.graphics(t._graphics);
//             g["img"]=i;
//             var p=new CanvasPattern(i.imgId,1,0,0,i.width,i.height);
//             g["pat"]=p;
//             g._fillImage(p,0,0,0,0,w,i.height);
//         }
//         i.src=src;
//         return t;
//     }
    
//     loading(pre:number)
//     {
//         console.log(">>>>>>>>>>>"+pre);
//         if((1-pre)<0.000001)
//         {
//             alert("xxxxx");
//             this.mc.stop();
//             this.mc.removeChildren();
//             return;
//         }
//         if(this.p!=pre)
//         {
//             this.p=pre;
//             if(this.bar&&this.bar._graphics)
//             {
//                 var g=this.bar._graphics;
//                 g.clear();
//                 g._fillImage(g["pat"],0,0,0,0,pre*400,g["img"].height);
//             }
//         } 
//     }
// }

// class ConchMovieClip extends ConchNodeNew{
//     basePath:string;
//     private _playing:boolean;
//     private _data:ConchByte;
//     private _ids:Object;
//     private _start:number;
//     interval:number;
//     private _curIndex:number;
//     private _playIndex:number;
//     private _preIndex:number;
//     private _ended:boolean;
//     loop:boolean;
//     private _count:number;
//     private _Pos:number;
//     private _idOfSprite:Array<ConchNodeNew>;
//     private _movieClipList:Array<ConchMovieClip>;
//     static _ValueList:Array<string>=["x", "y", "width", "height", "scaleX", "scaleY", "rotation", "alpha"];
//     _parentMovieClip:ConchMovieClip;
//     constructor(parentMovieClip:ConchMovieClip){
//         super();
//         this._ids={};
//         this._idOfSprite=[];
//         this._reset(true);
//         this._start=0;
//         this._ended== true;
//         this._Pos=0;
//         this._parentMovieClip = parentMovieClip;
//         if (!parentMovieClip) {
//             this._movieClipList = [this];
//             //on(Event.DISPLAY, this, _onDisplay);
//             //on(Event.UNDISPLAY, this, _onDisplay);
//         } else {
//             this._movieClipList = parentMovieClip._movieClipList;
//             this._movieClipList.push(this);
//         }
//     }
//     start():void{
//         setInterval(this.updates.bind(this),this.interval)
//     }
    
//     /**
//      * @private
//      * 清理。
//      */
//     private _clear():void {
//         this.stop();
//         this._idOfSprite.length = 0;
//         if (!this._parentMovieClip) {
//             //Laya.timer.clear(this, updates);
//             var i:number, len:number;
//             len = this._movieClipList.length;
//             for (i = 0; i < len; i++) {
//                 if (this._movieClipList[i] != this)
//                     this._movieClipList[i]._clear();
//             }
//             this._movieClipList.length = 0;
//         }		
//         this.removeChildren();
//         this._graphics= null;
//         this._parentMovieClip = null;
//     }
    
//     load(src:string,cb)
//     {
//         var a:XMLHttpRequest=new XMLHttpRequest();
//         this.basePath=src.split(".swf")[0]+"/image/";
//         this._clear();
//         a.responseType="arraybuffer";
//         this._movieClipList=[this];
//         var that:ConchMovieClip=this;
//         a.onload=function(e)
//         {
//             that._initData(a.response);
//             that.start();
//             cb&&cb();
//         }
//         a.open("GET",src,false);
//         a.send(null);
//     }
//     /**
// 		 * 停止播放动画。
// 		 */
// 	stop():void {
// 		this._playing = false;
// 	}
    
//     private _initData(data)
//     {
//         this._data=new ConchByte(data);
//         var i:number, len:number =this._data.getUint16();
// 			for (i = 0; i < len; i++) this._ids[this._data.getInt16()] = this._data.getInt32();
// 		this.interval = 1000 / this._data.getUint16();
//         this._setData(this._data, this._ids[32767]);
//         this._initState();
//         this.play(0,true);
//     }
	
//     private _setData(data:ConchByte, start:number):void {
//         this._data = data;
//         this._start = start + 3;
//     }
    
//     private  _initState():void {
//         this._reset(true);
//         this._ended = false;
//         var preState:boolean = this._playing;
//         this._playing = false;
//         this._curIndex = 0;
//         while (!this._ended) 
//         {
//             this._parse(++this._curIndex);
//         }
//         this._playing = preState;
//     }
    
//     private _reset(rm:boolean):void {
//         if (rm && this._curIndex != 1) this.removeChildren();
//         this._preIndex=this._curIndex = -1;
//         this._Pos = this._start;
//     }
    
//      /**
//      * 播放动画。
//      * @param	index 帧索引。
//      */
//     play(index:number, loop:boolean):void {
//         this.loop = loop;
//         this._playing = true;
//         if (this._data)
//             this._displayFrame(index);		
//     }
//     private _displayFrame(frameIndex:number):void {
//         if (frameIndex != -1) {
//             if (this._curIndex > frameIndex) this._reset(true);
//             this._parse(frameIndex);
//         }
//     }
    
//     _update():void
//     {
//         if (!this._data) return;
//         if (!this._playing) return;
//         this._playIndex++;
//         if (this._playIndex >= this._count) {
//             if (!this.loop) {
//                 this._playIndex--;
//                 this.stop();
//                 return;
//             }
//             this._playIndex = 0;
//         }
//         this._parse(this._playIndex);
//     }
    
//     /**@private 更新时间轴*/
// 	updates():void {
//         if (this._parentMovieClip) return;
//         var i:number, len:number;
//         len = this._movieClipList.length;
//         for (i = 0; i < len; i++) {
//             this._movieClipList[i]&&this._movieClipList[i]._update();
//         }
//     }
//     private _parse(frameIndex:number):void
//     {
//         var curChild:ConchNodeNew = this;
//         var mc:ConchMovieClip, sp:ConchNodeNew, key:number, type:number, tPos:number, ttype:number, ifAdd:boolean = false;
//         var _idOfSprite:Array<ConchNodeNew> = this._idOfSprite, _data:ConchByte = this._data, eStr:String;
//         if (this._ended) this._reset(true);
//         _data.pos = this._Pos;
//         this._ended = false;
//         this._playIndex = frameIndex;
//         if (this._curIndex > frameIndex&&frameIndex<this._preIndex)
//         {
//             this._reset(true);
//             _data.pos = this._Pos;
//         }
//         while ((this._curIndex <= frameIndex) && (!this._ended)) {
// 			type = _data.getUint16();
//             switch (type) {
//                 case 12:
//                     key=_data.getUint16();
//                     tPos=this._ids[_data.getUint16()];
//                     this._Pos=_data.pos;
//                     _data.pos=tPos;
//                     if ((ttype=_data.getUint8())==0){
//                         var pid=_data.getUint16();
//                         sp=this._idOfSprite[key];
// 							if (!sp){
// 								sp=_idOfSprite[key]=new ConchNodeNew();
// 								var spp=new ConchNodeNew();
// 								spp.loadImage(this.basePath+pid+".png");
// 								sp.addChild(spp);
// 								spp.size(_data.getFloat32(),_data.getFloat32());
// 								spp.transform(_data.getFloat32(), _data.getFloat32(),_data.getFloat32(), _data.getFloat32(), _data.getFloat32(), _data.getFloat32());
// 							}
// 							sp.alpha(1);
// 							}else if (ttype==1){
// 							mc=<ConchMovieClip>_idOfSprite[key];
// 							if (!mc){
// 								_idOfSprite[key]=mc=new ConchMovieClip(this);
// 								mc.interval=this.interval;
// 								mc._ids=this._ids;
// 								mc.basePath=this.basePath;
// 								mc._setData(_data,tPos);
// 								mc._initState();
// 								mc.play(0,true);
// 							}
// 							mc.alpha(1);
                        
//                     }
//                     _data.pos=this._Pos;
//                     break;
//                     case 3:
//                     (this.addChild(_idOfSprite[_data.getUint16()])).zOrder=_data.getUint16();
//                     ifAdd=true;
//                     break;
//                     case 4:
//                     _idOfSprite[_data.getUint16()].removeSelf();
//                     break;
//                     case 7:
//                     sp=_idOfSprite[ _data.getUint16()];
//                     sp.transform(_data.getFloat32(),_data.getFloat32(),_data.getFloat32(),_data.getFloat32(),_data.getFloat32(),_data.getFloat32());
//                     break ;
//                     case 10:
//                     _idOfSprite[ _data.getUint16()].alpha(_data.getFloat32());
//                     break;
//                     case 99:
//                     this._curIndex=_data.getUint16();
//                     ifAdd && this.updateZOrder();
//                     break ;
//                     case 100:
//                       	this._count=this._curIndex+1;
// 						this._ended=true;
// 						this._reset(false);
// 						break ;
//                 default:
//                     break;
//             }
//         } 
//         this._Pos=_data.pos;
//     }
// }


// window["startLoading"]=function()
// {
//     window["loadingview"]=new ConchLoadingView();
// }


