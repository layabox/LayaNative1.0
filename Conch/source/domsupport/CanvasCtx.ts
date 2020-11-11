
///<reference path="./LayaConchRuntime.d.ts" />
interface ImageData {

}

class ColorUtil{
    static _COLOR_MAP:Object =  {"white": '#FFFFFF', "red": '#FF0000', "green": '#00FF00', "blue": '#0000FF', "black": '#000000', "yellow": '#FFFF00', 'gray': '#AAAAAA'};
    static getColorNum(str):number
    {
        if(!str)return 0;
        if(typeof(str)=="number")
        {
            if(str >= 0xFFFFFF ) return str;
            return str|0xFF000000;
        }
        var len:number;
        if(str[0]=="#")
        {
            var r:string=ColorUtil.getRGBByRGBStr(str);
            if(r){
                return parseInt(r,16);
            }
            else
                return NaN;
        }
        else if(str.indexOf("rgb")!=-1){
            return ColorUtil.getColNum(str,true);
        }
        else if(str.indexOf("hsl")!=-1){
            return ColorUtil.getColNum(str,false);
        }
        else
        {
            var c:string=ColorUtil._COLOR_MAP[str];
            return  c?ColorUtil.getColorNum(c):NaN;
        }
    }
    static RGBReg:RegExp=new RegExp("rgb([a]{0,1})\\s*[(]([\\d,.\\s-]*)?[)]");
    static HSLReg:RegExp=new RegExp("hsl([a]{0,1})\\s*[(]([\\d,.\\s-%]*)?[)]");
    
    static getColNum(c:string,isRgb:boolean):number
    {
        var  temp:string,i:number,s:number,result:Array<number>=[],st:number;
        var t:Array<string>=isRgb?ColorUtil.RGBReg.exec(c):ColorUtil.HSLReg.exec(c);
        if(t)
        {
            var r = t[2].split(",");
            i = 0; s = r.length;
            while (i < s) { result[i] =parseFloat(r[i]); i++;}
            var ta:Array<number>=result;
            if(!isRgb){
                ta=ColorUtil.hsl2rgb(result[0]/360,result[1]/100,result[2]/100);
            }
            if(i==3&&!t[1]){
                return 0xff000000+(ta[0]<<16)+(ta[1]<<8)+ta[2];
            }
            else if(i==4&&t[1]){
                return parseInt(Math.floor(0xff*result[3]).toString(16)+"000000",16)+(ta[0]<<16)+(ta[1]<<8)+ta[2];
            }
            return NaN;
        }
        else
        return NaN;
    }
    
    static  getRGBByRGBStr(str:string):string
	{
        str = str.substr(1);
        var i:number,len:number=str.length,result:string;
        switch (len) {
            case 3:
                result="ff";
                for(i=0;i<len;i++){
                    result+=str[i]+str[i];
                }
                break;
            case 6:
                result="ff"+str;
            default:
                break;
        }
        return result;
    }
    
    static toHexColor(color:number):string
    {
        if (color < 0 || isNaN(color))
            return null;
        if(color>0xff000000){
            color-=0xff000000;
        }
        else{
            return "rgba("+(color>>16&0xff)+","+(color>>8&0xff)+","+(color&0xff)+","+((color>>24&0xff)/0xff)+")";
        }
        var str:string = color.toString(16);
        while (str.length < 6)
            str = "0" + str;
        return "#" + str;
    }

    static  hue2rgb(p:number, q:number, t:number):number
    {
        if(t < 0) t += 1;
        if(t > 1) t -= 1;
        if(t < 1/6) return p + (q - p) * 6 * t;
        if(t < 1/2) return q;
        if(t < 2/3) return p + (q - p) * (2/3 - t) * 6;
        return p;
    }
    static  hsl2rgb(h:number, s:number, l:number):Array<number>
    {
        var r:number, g:number, b:number;
        if(s == 0){
            r = g = b = l; // achromatic
        }else{
            
            var q = l < 0.5 ? l * (1 + s) : l + s - l * s;
            var p = 2 * l - q;
            r = ColorUtil.hue2rgb(p, q, h + 1/3);
            g = ColorUtil.hue2rgb(p, q, h);
            b = ColorUtil.hue2rgb(p, q, h - 1/3);
        }
        return [Math.round(r * 255), Math.round(g * 255), Math.round(b * 255)];
    }
    
}

class ContextDataInfo{
       curpos:number;
       buffer:DataView;
       arrayBuffer:Uint8Array;
       constructor(){
           
       }
       private ru16(i:number):number
       {
         return this.buffer.getUint16(this.curpos+i,true);
       }
       
       private wu16(i:number,v:number):void
       {
           this.buffer.setUint16(this.curpos+i,v,true);
       }
       
       private ru32(i:number):number
       {
         return this.buffer.getUint32(this.curpos+i,true);
       }
       
       private wu32(i:number,v:number):void
       {
           this.buffer.setUint32(this.curpos+i,v,true);
       }
       
       private rf32(i:number):number
       {
         return this.buffer.getFloat32(this.curpos+i,true);
       }
       
       private wf32(i:number,v:number):void
       {
           this.buffer.setFloat32(this.curpos+i,v,true);
       }
       
       get fillStyle():number
       {
           return this.ru32(0); 
       }
       set fillStyle(t:number)
       {
           this.wu32(0,t); 
       }
       
       get storkeStyle():number
       {
           return this.ru32(4);
       }
       
       set storkeStyle(t:number)
       {
           this.wu32(4,t);
       }
      
       get lineWidth():number
       {
           return this.rf32(8);
       }
       
       set lineWidth(t:number)
       {
           this.wf32(8,t);
       }
       
       get textBaseLineType():number
       {
           return this.ru16(12);
       }
       
       set textBaseLineType(t:number)
       {
           this.wu16(12,t);
       }
      
       get textAlignType():number
       {
           return this.ru16(14);
       }
       
       set textAlignType(t:number)
       {
           this.wu16(14,t);
       }
       
       get blendType():number
       {
           return this.ru16(16);
       }
       
       set blendType(t:number)
       {
           this.wu16(16,t);
       }
       
       get repetition():number
       {
           return this.ru16(18);
       }
       
       set repetition(v:number)
       {
           this.wu16(18,v);
       }
       
       get shadowColor():number
       {
           return this.ru32(20);
       }
       
       set shadowColor(t:number)
       {
           this.wu32(20,t);
       }
       
       get shadowBlur():number
       {
           return this.rf32(24);
       }
       
       set shadowBlur(t:number)
       {
           this.wf32(24,t);
       }
       
       get shadowOffsetX():number
       {
           return this.rf32(28);
       }
       
       set shadowOffsetX(t:number)
       {
           this.wf32(28,t);
       }
       
       get shadowOffsetY():number
       {
           return this.rf32(32);
       }
       
       set shadowOffsetY(t:number)
       {
           this.wf32(32,t);
       }
       
       get lineCap():number
       {
           return this.ru16(36);
       }
       
       set lineCap(t:number)
       {
           this.wu16(36,t);
       }
       
       get lineJoin():number
       {
           return this.ru16(38);
       }
       
       set lineJoin(t:number)
       {
           this.wu16(38,t);
       }
       
       get miterLimit():number
       {
           return this.rf32(40);
       }
       
       set miterLimit(t:number)
       {
           this.wf32(40,t);
       }
             
       get alpha():number
       {
           return this.rf32(44);
       }
       
       set alpha(t:number)
       {
           this.wf32(44,t);
       }
       
       get font():string
       {
           return _Buffer.readString(this.buffer,this.curpos+48,64);   
       }
       
       set font(f:string)
       {
           if(!f)f="";
           var pos=48;
           var ab:ArrayBuffer=conch.strTobufer(f);
           var size:number=ab.byteLength;
           if(size+2>64){
               console.error("font不能超过64字符");
               return;
           }
           this.wu16(pos,size);
           pos+=2;
           var uint8array = new Uint8Array(ab, 0, size);
           this.arrayBuffer.set(uint8array,this.curpos+pos);
           pos+=size;
           for (var i:number= size+2;i < 64; ++i) {
                this.buffer.setUint8(pos, 0);
                pos++;
           }
       }
       
       get fillType():number
       {
           return this.ru16(112);
       }
       set fillType(t:number)
       {
           this.wu16(112,t);
       }
       
       get storkeType():number
       {
           return this.ru16(114);
       }
       
       set storkeType(t:number)
       {
           this.ru16(114);
       }
       
       get x():number
       {
           return this.ru32(116);
       }
       set x(t:number){
           this.wu32(116,t);
       }
       
       get y():number
       {
           return this.ru32(120);
       }
       set y(t:number){
           this.wu32(120,t);
       }
       
       get w():number
       {
           return this.ru32(124);
       }
       set w(t:number){
           this.wu32(124,t);
       }
       
       get h():number
       {
           return this.ru32(128);
       }
       set h(t:number){
           this.wu32(128,t);
       }
       
       init(){
           this.fillStyle=0xffffffff;
           this.lineWidth=1;
           this.alpha=1;
       }
}

class ContextSaveData{
    curData:ContextDataInfo;
    datasize:number;
    sizeofData:number;
    constructor(){
        this.curData=new ContextDataInfo();
        this.sizeofData=132;
        this.datasize=20*this.sizeofData;
        this.curData.buffer=new DataView(new ArrayBuffer(this.datasize));
        this.curData.arrayBuffer=new Uint8Array(this.curData.buffer.buffer);
        this.curData.curpos=0;
        this.curData.init();
    }
    
    save():boolean
    {
        this.curData.curpos+=this.sizeofData;
        if(this.curData.curpos>=this.datasize)
        {
            var enlargeSize:number=20*this.sizeofData;
            var dv:DataView=new DataView(new ArrayBuffer(this.datasize+enlargeSize));
            var ab:Uint8Array=new Uint8Array(dv.buffer);
            ab.set(this.curData.arrayBuffer,0);
            this.datasize+=enlargeSize;
            this.curData.buffer=dv;
            this.curData.arrayBuffer=ab;
        }
        var temp:Uint8Array=new Uint8Array(this.curData.buffer.buffer,this.curData.curpos-this.sizeofData, this.sizeofData);
        this.curData.arrayBuffer.set(temp,this.curData.curpos);
        return true;
    }
    
    restore():boolean
    {
        if(this.curData.curpos>0)
        {
           this.curData.curpos-=this.sizeofData;
           return true;
        }
        else
        {
            throw "save and restore don't make a pair.";
        }   
    }
}

class CanvasPattern{
    x:number;
    y:number;
    w:number;
    h:number;
    image:number;
    repetition:number;
    constructor(img?:number,r?:number,x?:number,y?:number,w?:number,h?:number){
       if(img!==undefined)
       {
           this.image=img;
           this.x=x;
           this.y=y;
           this.w=w;
           this.h=h;
           this.repetition=r;
       } 
    }   
}

class CanvasGradient{
    x0:number;
    y0:number;
    x1:number;
    y1:number;
    addColorStop(fs:string,color:string)
    {
        
    }
}

class CanvasRenderingContext2D implements IBuffer{// extends context{
    private canvas:HTMLCanvasElement;
    static LINECAP:any=["butt","round","square"];
    static TEXTALIGN:any=["start","left","center","end","right"];
    static TEXTBASELINE:any=["alphabetic","top","hanging","middle","ideographic","bottom"];
    static LINEJOIN:any=["miter","round","bevel"];
    static BLEND:any=["source-over","source-atop","source-in","source-out","destination-over","destination-atop","destination-in","destination-out","lighter","copy",'xor',
                      'normal','add','multiply','screen','overlay','light','mask'];
    static DRAWIMAGES:Float32Array=new Float32Array(0);
    static skinMeshShader={
         conch_id:undefined,
         id:1025,
         vs:`attribute vec2 position;
                        attribute vec2 texcoord;
                        attribute vec4 color;
                        uniform vec4 g_DevInfo;
                        uniform mat4 mmat;
                        varying vec2 v_texcoord;
                        varying vec4 v_color;
                        void main() {
                        vec4 pos=mmat*vec4(position.x,position.y,0,1 );
                        gl_Position = vec4((pos.x/g_DevInfo.x-0.5)*2.0,(0.5-pos.y/g_DevInfo.y)*2.0,pos.z,1.0);
                        v_color = color;
                        v_color.rgb *= v_color.a;
                        v_texcoord = texcoord;  
                        }`,
           ps:`precision mediump float;
            varying vec2 v_texcoord;
            varying vec4 v_color;
            uniform sampler2D texture;
            uniform float g_Alpha;
            void main() {
                vec4 t_color = texture2D(texture, v_texcoord);
                gl_FragColor = t_color * v_color;
				gl_FragColor *= g_Alpha;
            }`
    };
    static particleShader={
        conch_id:undefined,
        id:1024,
        ps:`precision mediump float;
        varying vec4 v_Color;
        varying vec2 v_TextureCoordinate;
        uniform sampler2D texture;

        void main()
        {	
            gl_FragColor=texture2D(texture,v_TextureCoordinate)*v_Color;
            gl_FragColor.rgb*=v_Color.a;
        }
        `,
        vs:`
        attribute vec4 a_CornerTextureCoordinate;
attribute vec3 a_Position;
attribute vec3 a_Velocity;
attribute vec4 a_StartColor;
attribute vec4 a_EndColor;
attribute vec3 a_SizeRotation;
attribute vec2 a_Radius;
attribute vec4 a_Radian;
attribute float a_AgeAddScale;
attribute float a_Time;

varying vec4 v_Color;
varying vec2 v_TextureCoordinate;

uniform  float u_CurrentTime;
uniform float u_Duration;
uniform float u_EndVelocity;
uniform vec3 u_Gravity;

 uniform vec2 size;
 uniform mat4 mmat;

vec4 ComputeParticlePosition(in vec3 position, in vec3 velocity,in float age,in float normalizedAge)
{

   float startVelocity = length(velocity);
   float endVelocity = startVelocity * u_EndVelocity;

   float velocityIntegral = startVelocity * normalizedAge +(endVelocity - startVelocity) * normalizedAge *normalizedAge/2.0;
   
   vec3 addPosition = normalize(velocity) * velocityIntegral * u_Duration; 
   addPosition += u_Gravity * age * normalizedAge;
   
   float radius=mix(a_Radius.x, a_Radius.y, normalizedAge);
   float radianHorizontal =mix(a_Radian.x,a_Radian.z,normalizedAge);
   float radianVertical =mix(a_Radian.y,a_Radian.w,normalizedAge);
   
   float r =cos(radianVertical)* radius;
   addPosition.y += sin(radianVertical) * radius;
	
   addPosition.x += cos(radianHorizontal) *r;
   addPosition.z += sin(radianHorizontal) *r;
  
   addPosition.y=-addPosition.y;
   position+=addPosition;
    return  vec4(position,1.0);
}

float ComputeParticleSize(in float startSize,in float endSize, in float normalizedAge)
{    
    float size = mix(startSize, endSize, normalizedAge);
	 return size;
}

mat2 ComputeParticleRotation(in float rot,in float age)
{    
    float rotation =rot * age;
    float c = cos(rotation);
    float s = sin(rotation);
    return mat2(c, -s, s, c);
}

vec4 ComputeParticleColor(in vec4 startColor,in vec4 endColor,in float normalizedAge)
{
	vec4 color=mix(startColor,endColor,normalizedAge);
    color.a *= normalizedAge * (1.0-normalizedAge) * (1.0-normalizedAge) * 6.7;
   
    return color;
}

void main()
{
   float age = u_CurrentTime - a_Time;
   age *= 1.0 + a_AgeAddScale;
   float normalizedAge = clamp(age / u_Duration,0.0,1.0);
   gl_Position = ComputeParticlePosition(a_Position, a_Velocity, age, normalizedAge);
   float pSize = ComputeParticleSize(a_SizeRotation.x,a_SizeRotation.y, normalizedAge);
   mat2 rotation = ComputeParticleRotation(a_SizeRotation.z, age);
   mat4 mat=mmat;
   gl_Position=vec4((mat*gl_Position).xy,0.0,1.0);
   gl_Position.xy += (rotation*a_CornerTextureCoordinate.xy) * pSize*vec2(mat[0][0],mat[1][1]);
   gl_Position=vec4((gl_Position.x/size.x-0.5)*2.0,(0.5-gl_Position.y/size.y)*2.0,0.0,1.0);
   v_Color = ComputeParticleColor(a_StartColor,a_EndColor, normalizedAge);
   v_TextureCoordinate =a_CornerTextureCoordinate.zw;
}


        `
    }

    static PATTERN:any=["repeat","repeat-x","repeat-y","no-repeat"];
    private _saveData:ContextSaveData;
    private _nativeObj:context;
    id:number;
    constructor() {
        this._nativeObj=new context();
        this._saveData=new ContextSaveData();
        this.id=this._nativeObj.conchID;
        /*document.addEventListener('frameend',(e:Event)=>{
            //如果是可见的canvas，则要flush
            if(this.canvas && this.canvas.parentNode){
                conch.bf.flush();
            }
        });*/
    }
    
    setSize(w:number,h:number)
    {
        if(!isNaN(w)&&!isNaN(h))
        {
            this._nativeObj.setSize(w,h);
        }
    }
    /**
     *  CANVAS_TYPE_MAIN=0,                   //主画布
        CANVAS_TYPE_RENDER_TARGET=1,          //GPU的画布
        CANVAS_TYPE_MEMORY=2                  //假画布,内存中的只保存指令
     */
    setCanvasType(type:number):void{
        var _:_Buffer=conch.bf;
        _.needsz(16)
        _.wu32(0);
        _.wu32(9);
        _.wu32(this.id);
        _.wu32(type);
    }
    
    setCanvasClip(b:boolean):void{
        var _:_Buffer=conch.bf;
        _.needsz(16)
        _.wu32(0);
        _.wu32(17);
        _.wu32(this.id);
        _.wu32(b?1:0);
    }
   
    save():void
    {
        _Buffer.noParm(this,0);
        this._saveData.save();
    }
    
    restore():void
    {
        _Buffer.noParm(this,1);
        this._saveData.restore();
    }
    
    beginPath():void
    {
        _Buffer.noParm(this,2);
    }
    
    drawImage(image: HTMLImageElement | HTMLCanvasElement, offsetX: number, offsetY: number, width?: number, height?: number, canvasOffsetX?: number, canvasOffsetY?: number, canvasImageWidth?: number, canvasImageHeight?: number): void
    {
        var n:number=arguments.length,_:_Buffer=conch.bf;
        if(image instanceof HTMLImageElement)
        {
            switch (n) {
                case 3:
                    _.needsz(20);
                    _.wu32(this.id);
                    _.wu32(3);
                    break;
                case 5:
                    _.needsz(28);
                    _.wu32(this.id);
                    _.wu32(4);
                    break;
                case 9:
                    _.needsz(44);
                    _.wu32(this.id);
                    _.wu32(5);
                default:
                    break;
            }
            _.wu32(image._nativeObj.conchImgId);
        }
        else if(image instanceof HTMLCanvasElement)
        {
            switch (n) {
                case 3:
                    _.needsz(20);
                    _.wu32(this.id);
                    _.wu32(35);
                    break;
                case 5:
                    _.needsz(28);
                    _.wu32(this.id);
                    _.wu32(36);
                    break;
                case 9:
                    _.needsz(44);
                    _.wu32(this.id);
                    _.wu32(37);
                default:
                    break;
            }
            _.wu32(image._ctx2d._nativeObj.conchID);
        }
        else{
            throw "不支持改对象的drawImage";
        }
        
        _.wf32(offsetX);
        _.wf32(offsetY);
        if(n>3)
        {
            _.wf32(width);
            _.wf32(height);
        }
        if(n>5)
        {
            _.wf32(canvasOffsetX);
            _.wf32(canvasOffsetY);
            _.wf32(canvasImageWidth);
            _.wf32(canvasImageHeight);
        }
    }
    
    rect(x: number, y: number, w: number, h: number):void
    {
        _Buffer.rect(this,6,x,y,w,h);
    }
       
    clip(fillRule?: string):void
    {
        _Buffer.noParm(this,7);
    }
    
    scale(x: number, y: number):void
    {
        _Buffer.point(this,8,x,y);
    }
        
    rotate(angle: number):void
    {
        _Buffer.oneNum(this,9,angle);
    }
    
    translate(x: number, y: number):void
    {
        _Buffer.point(this,10,x,y);
    }
    
    transform(m11: number, m12: number, m21: number, m22: number, dx: number, dy: number):void
    {
        _Buffer.setMatrix(this,11,m11,m12,m21,m22,dx,dy);
    }
    
    setTransform(m11: number, m12: number, m21: number, m22: number, dx: number, dy: number):void
    {
        _Buffer.setMatrix(this,12,m11,m12,m21,m22,dx,dy);
    }

    
    set globalAlpha(v: number) 
    {        
        _Buffer.oneNum(this,13,v);
        this._saveData.curData.alpha=v;
    }
    get globalAlpha(): number { return this._saveData.curData.alpha; }
    
    clearRect(x: number, y: number, w: number, h: number):void
    {
        _Buffer.rect(this,14,x,y,w,h);
    }
    
    closePath():void
    {
        _Buffer.noParm(this,15);
    }
    
    
    set fillStyle(fs: string | CanvasGradient | CanvasPattern) {
        var _:_Buffer=conch.bf,
            s:ContextDataInfo=this._saveData.curData;;
        if(typeof(fs)=="string")
        {
            var d=ColorUtil.getColorNum(<string>fs);
            if(isNaN(d))
            {
                console.error("fillStyle 不支持color:"+fs);
                d = 0xFFFFFFFF;
            }
            _Buffer.oneInt(this,16,d);
            s.fillType=0;
            s.fillStyle=d;
        }
        else if(fs instanceof CanvasPattern)
        {
           //<CanvasPattern>fs.
           var imgid:number=(<CanvasPattern>fs).image,
               x:number=(<CanvasPattern>fs).x,
               y:number=(<CanvasPattern>fs).y,
               w:number=(<CanvasPattern>fs).w,
               h:number=(<CanvasPattern>fs).h,
               rt:number=(<CanvasPattern>fs).repetition;
           _.needsz(32);
           _.wu32(this.id);
           _.wu32(47);
           _.wu32(imgid);
           _.wu32(rt);
           _.wf32Rect(x,y,w,h);
           s.fillType=1;
           s.fillStyle=imgid;
           s.repetition=rt;
           s.x=x;
           s.y=y;
           s.w=w;
           s.h=h;
        }
        else
           console.error("fillStyle 不支持此方式:"+fs);
    }
    get fillStyle(): string | CanvasGradient | CanvasPattern {
        var s:ContextDataInfo=this._saveData.curData,t:number=s.fillType;
        if(t==0)
            return ColorUtil.toHexColor(s.fillStyle);
        else{
            return new CanvasPattern(s.fillStyle,s.repetition,s.x,s.y,s.w,s.h);
        }
    }
       
    fillRect(x: number, y: number, w: number, h: number):void{
        _Buffer.rect(this,17,x,y,w,h);
    }
    
    set strokeStyle(v: string | CanvasGradient | CanvasPattern) {
       if(typeof(v)=="string")
        {
            var d=ColorUtil.getColorNum(<string>v);
            if(isNaN(d))
            {
                console.error( "strokeStyle 不支持color:"+v );
                d = 0xFFFFFFFF;
            }
            _Buffer.oneInt(this,18,d);
            this._saveData.curData.storkeStyle=d;
        }
        else
        {
            throw("strokeStyle Not Support yet");
        }
    }
    get strokeStyle(): string | CanvasGradient | CanvasPattern {
        var d=this._saveData.curData.storkeStyle;
        return ColorUtil.toHexColor(d);
    }
    
        
    stroke():void
    {
        _Buffer.noParm(this,19);
    }
    
    strokeRect(x: number, y: number, w: number, h: number):void
    {
       _Buffer.rect(this,20,x,y,w,h);
    }
    
    private _drawText(fid:number,text: string, x: number, y: number, maxWidth?: number)
    {
        if(!text)return;
        var _:_Buffer=conch.bf,j:number=(maxWidth===undefined)?0:1;
        _.needsz(20+j*4);
        _.wu32(this.id);
        _.wu32(fid+j);
        _.wf32(x);
        _.wf32(y);
        j&&_.wu32(maxWidth)
        _.wstring(text,true);
    }
    
    strokeText(text: string, x: number, y: number, maxWidth?: number) 
    {
        this._drawText(21,text,x,y,maxWidth);
    }
    
    fillText(text: string, x: number, y: number, maxWidth?: number) 
    {
        this._drawText(23,text,x,y,maxWidth);
    }
   
    fill(fillRule?: string):void
    {
        _Buffer.noParm(this,25);
    }
    
    set lineCap(v: string) 
    {
        var d:number=CanvasRenderingContext2D.LINECAP[v]||0;
        _Buffer.oneInt(this,26,d);
        this._saveData.curData.lineCap=d;
        
    }
    get lineCap(): string 
    {
        return CanvasRenderingContext2D.LINECAP[this._saveData.curData.lineCap]||"butt";
    }
    
    set lineWidth(v: number) 
    {
       _Buffer.oneNum(this,27,v);
       this._saveData.curData.lineWidth=v;
    }
    get lineWidth(): number 
    {
        return this._saveData.curData.lineWidth;
    }
    
    set lineJoin(v: string) {
        var d:number=CanvasRenderingContext2D.LINEJOIN[v]||0;
        _Buffer.oneInt(this,28,d);
        this._saveData.curData.lineJoin=d;
    }
    get lineJoin(): string {
        return CanvasRenderingContext2D.LINEJOIN[this._saveData.curData.lineJoin]||"miter";
    }
    
    moveTo(x: number, y: number):void
    {  
       _Buffer.point(this,29,x,y);
    }
    
    lineTo(x: number, y: number):void
    {
       _Buffer.point(this,30,x,y);
    }
    
    arc(x: number, y: number, radius: number, startAngle: number, endAngle: number, anticlockwise?: boolean):void
    {
        var _:_Buffer=conch.bf;
        _.needsz(32);
        _.wu32(this.id);
        _.wu32(31);
        _.wf32(x);
        _.wf32(y);
        _.wf32(radius);
        _.wf32(startAngle);
        _.wf32(endAngle);
        _.wu32(anticlockwise?1:0);
    }
    
    arcTo(x1: number, y1: number, x2: number, y2: number, radius: number):void
    {
        var _:_Buffer=conch.bf;
        _.needsz(28);
        _.wu32(this.id);
        _.wu32(32);
        _.wf32(x1);
        _.wf32(y1);
        _.wf32(x2);
        _.wf32(y2);
        _.wf32(radius);
    }
    

    
    bezierCurveTo(cp1x: number, cp1y: number, cp2x: number, cp2y: number, x: number, y: number):void
    {
        _Buffer.setMatrix(this,33,cp1x, cp1y, cp2x, cp2y, x, y);
    }
    
        
    quadraticCurveTo(cpx: number, cpy: number, x: number, y: number):void
    {
        _Buffer.rect(this,34,cpx,cpy,x,y);
    }
    
    set font(f: string) 
    {
        var _:_Buffer=conch.bf;
        _.needsz(12);
        _.wu32(this.id);
        _.wu32(38);
        _.wstring(f,true);
        this._saveData.curData.font=f;
    }
    get font(): string 
    {
        return this._saveData.curData.font;
    }
        
    set globalCompositeOperation(v: string) 
    {
        var b:number=CanvasRenderingContext2D.BLEND[v]||0;
        _Buffer.oneInt(this,39,b);
        this._saveData.curData.blendType=b;
    }
    
    get globalCompositeOperation(): string 
    {
        return CanvasRenderingContext2D.BLEND[this._saveData.curData.blendType]||"source-over";
    }
    
    set textBaseline(v: string) 
    {
        var d:number=CanvasRenderingContext2D.TEXTBASELINE[v]||0;
        _Buffer.oneInt(this,40,d);
        this._saveData.curData.textBaseLineType=d;
    }
    
    get textBaseline(): string 
    {
        return CanvasRenderingContext2D.TEXTBASELINE[this._saveData.curData.textBaseLineType]||"alphabetic";
    }
    
    set textAlign(v: string) 
    {
        var d:number=CanvasRenderingContext2D.TEXTALIGN[v]||0;
        _Buffer.oneInt(this,41,d);
        this._saveData.curData.textAlignType=d;
    }
    get textAlign(): string 
    {
        return CanvasRenderingContext2D.TEXTALIGN[this._saveData.curData.textAlignType]||"start";
    }
    
    set miterLimit(v: number) 
    {
        _Buffer.oneNum(this,42,v);
        this._saveData.curData.miterLimit=v;
    }
    
    get miterLimit(): number 
    {
        return this._saveData.curData.miterLimit;
    }
    
    set shadowOffsetX(v: number) 
    {
        _Buffer.oneNum(this,43,v);
        this._saveData.curData.shadowOffsetX=v;
    }
    get shadowOffsetX(): number 
    {
        return this._saveData.curData.shadowOffsetX;
    }
    set shadowOffsetY(v: number) 
    {
        _Buffer.oneNum(this,44,v);
        this._saveData.curData.shadowOffsetY=v;
    }
    get shadowOffsetY(): number 
    {
        return this._saveData.curData.shadowOffsetY;
    }
    
    set shadowColor(v: string) 
    {
       var n=ColorUtil.getColorNum(v);
       if(isNaN(n))
       {
          console.error( "shadowColor 不支持color:"+v );
          n=0xFFFFFFFF;
       }
       _Buffer.oneInt(this,45,n);
       this._saveData.curData.shadowColor=n;
    }
    
    get shadowColor(): string 
    {
        return ColorUtil.toHexColor(this._saveData.curData.shadowColor);
    }
    
    set shadowBlur(v: number) 
    {
       _Buffer.oneNum(this,46,v);
       this._saveData.curData.shadowBlur=v;
    }
    
    get shadowBlur(): number 
    {
        return this._saveData.curData.shadowBlur;
    }
    
    setFilter(r:number,g:number,b:number,gray:number):void
    {
       _Buffer.rect(this,48,r,g,b,gray);
    }
    
    fillBorderText(text:string, x:number, y:number, font:string, fillColor:string, borderColor:string, lineWidth:number, textAlign:string):void 
    {
        var _:_Buffer=conch.bf;
        var fc:number=ColorUtil.getColorNum(fillColor);
        var bc:number=ColorUtil.getColorNum(borderColor);
        var ta:number=CanvasRenderingContext2D.TEXTALIGN[textAlign]||0;
        if(isNaN(fc))
        {
            console.error("fillBorderText fillcolor或者borderColor不符合规范"+fillColor+","+borderColor);
            fc = 0xFFFFFFFF;
        }
        if(isNaN(bc))
        {
            console.error("fillBorderText fillcolor或者borderColor不符合规范"+fillColor+","+borderColor);
            bc = 0xFFFFFFFF;
        }
        var cur:ContextDataInfo=this._saveData.curData;
        _.needsz(36);
        _.wu32(this.id);
        _.wu32(49);
        _.wf32(x);
        _.wf32(y);
        _.wf32(lineWidth);
        _.wu32(fc);
        _.wu32(bc);
        _.wu32(ta);
        _.wstring(font,true);
        _.needsz(4);
        _.wstring(text,true);
        cur.lineWidth=lineWidth;
        cur.font=font;
        cur.fillStyle=fc;
        cur.textAlignType=ta;
        cur.textBaseLineType=2;
    }
    
    clipRect(x:number,y:number,w:number,h:number)
    {
        _Buffer.rect(this,50,x,y,w,h);
    }
    
    createLinearGradient(x0: number, y0: number, x1: number, y1: number):CanvasGradient{
        //this._rect(49,x0,y0,x1,y1);
        return null;
    }
    
    clear():void{
        
    }
    static ID:number=0;
    setSkinMesh(x:number,y:number,ib:any/*IndexBuffer*/,vb:any/*VertexBuffer*/,eleNum:number,iStart:number,texture:any,mat:any/*Matrix*/){
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
            _.wab(ib,ib.length*2);
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
        _.wu32(53);
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
        _.wf32(x);
        _.wf32(y);
    }
    
    drawParticle(x:number,y:number,par:any)
    {
         if(par.texture==null||par.texture.source==null)return;
         var _:_Buffer=conch.bf;
         _.needsz(20);
         _.wu32(this.id);
         _.wu32(54);
         _.wu32(par.id);
         _.wf32(x);
         _.wf32(y);
    }
    
    _setIBVB(x:number, y:number, ib:any/*IndexBuffer*/, vb:any/*VertexBuffer*/, numElement:number, 
        mat:{a:number,b:number,c:number,d:number,tx:number,ty:number}/*Matrix*/, 
        shader:any/*Shader*/, shaderValues:any/*Value2D*/, startIndex:number, offset:number):void
    {
        //XYUVRGBA 1
        //XYUV 3
        var vertType = vb._vertType?vb._vertType:1;
        var _:_Buffer=conch.bf;
        var imgid:number=-1;
        for(var i:number=0;i<2;i++)
        {
            var bf=(i==0?ib:vb);
            if(bf.conch_id===undefined||bf._upload)
            {
                bf.conch_id=bf.id;
                bf.conchRef = new ArrayBufferRef();
                bf.conchRef.id = bf.conch_id;
                _.needsz(16);
                _.wu32(0);
                _.wu32(11);
                _.wu32(bf.conch_id);
                _.wu32(bf.bufferLength);
                _.wab(bf._buffer,bf.bufferLength);
                bf._upload=false;
            }
        }
        if (shaderValues.textureHost){
			  //this.drawImage(shaderValues.textureHost.source,200,0);
              var img:HTMLImageElement=shaderValues.textureHost.source;
              if(img){
                  imgid=img.imgId;
              }
		}
        if(imgid==-1)
            return;
        if(shader.conch_id===undefined)
        {
            _.needsz(16);
            _.wu32(0);
            _.wu32(12);
            _.wu32(shader.id);
            _.wstring(shader._vs,true);
            _.needsz(4);
            _.wstring(shader._ps,true);
            shader.conch_id=shader.id;
        }
        _.needsz(72);
        _.wu32(this.id);
        _.wu32(51);
        _.wu32(ib.id);
        _.wu32(vb.id);
        _.wu32(vertType);
        _.wu32(shader.id);
        _.wu32(imgid);
        _.wu32(startIndex);
        _.wu32(offset);
        _.wu32(numElement);
        _.wf32(x);
        _.wf32(y);
        if(mat){
            _.wf32(mat.a);_.wf32(mat.b);
            _.wf32(mat.c);_.wf32(mat.d);
            _.wf32(mat.tx);_.wf32(mat.ty);
        }else{
            _.wf32(1.0);_.wf32(0.0);
            _.wf32(0.0);_.wf32(1.0);
            _.wf32(0.0);_.wf32(0.0);
        }
    }
    //20
    setFilterMatrix(mat:Float32Array,alpha:Float32Array)
    {
        if(!mat||!alpha)
        {
            return;      
        }
        var _=conch.bf;
        _.needsz(8);
        _.wu32(this.id);
        _.wu32(52);
        _.wab(mat,64);
        _.wab(alpha,16);
    }
    toBase64(type:string, encoderOptions:number, callback:(data:string)=>void)
    {
        this._nativeObj.setToBase64Callback(callback);
        var _:_Buffer=conch.bf;
        _.needsz(16);
        _.wu32(this.id);
        _.wu32(56);
        _.wf32(encoderOptions);
        _.wstring(type,true);
    }
    getPixels(x: number, y: number, w: number, h: number, callback:(data:ArrayBuffer, width:number, height:number)=>void)
    {
        this._nativeObj.setGetPixelsCallback(callback);
        var _:_Buffer=conch.bf;
        _.needsz(24);
        _.wu32(this.id);
        _.wu32(55);
        _.wf32(x);
        _.wf32(y);
        _.wf32(w);
        _.wf32(h);
    }
    isPointInPath(x: number, y: number, fillRule?: string):boolean{
        return true;
    }
    
    measureText(text: string):TextMetrics{
        return this._nativeObj.measureText(this._saveData.curData.font,text);
    }

    disableClip(b:boolean):void{
        
    }
    
    createPattern(image: HTMLImageElement, repetition: string,x?:number,y?:number,w?:number,h?:number):CanvasPattern
    {
        var r:CanvasPattern=new CanvasPattern();
        r.image=image._nativeObj.conchImgId;
        r.repetition=CanvasRenderingContext2D.PATTERN[repetition]||0;
        if(x!==undefined)
        {
            r.x=x;
            r.y=y;
            r.w=w;
            r.h=h;
        }else{
            r.x=r.y=0;
            r.w=image.width;r.h=image.height;
        }
        return r;
    }
    
    set lineDashOffset(v: number) {
    }
    get lineDashOffset(): number {
        return 0;
    }
    
    createImageData(imageDataOrSw: number | ImageData, sh?: number): ImageData {
        return null;
    }
    //createPattern(image: HTMLImageElement | HTMLCanvasElement | HTMLVideoElement, repetition: string): CanvasPattern;
    createRadialGradient(x0: number, y0: number, r0: number, x1: number, y1: number, r1: number): CanvasGradient {
        return null;
    }
    getImageData(sx: number, sy: number, sw: number, sh: number): ImageData {
        return null;
    }
    getLineDash(): number[] {
        return null;
    }
    putImageData(imagedata: ImageData, dx: number, dy: number, dirtyX?: number, dirtyY?: number, dirtyWidth?: number, dirtyHeight?: number): void {

    }
    setLineDash(segments: number[]): void {

    }
}

function _init(v:Array<string>){
    for(var i=0,sz=v.length;i<sz;i++)
    {
        v[v[i]]=i;
    }    
}
_init(CanvasRenderingContext2D.LINECAP);
_init(CanvasRenderingContext2D.TEXTALIGN);
_init(CanvasRenderingContext2D.TEXTBASELINE);
_init(CanvasRenderingContext2D.LINEJOIN);
_init(CanvasRenderingContext2D.BLEND);
_init(CanvasRenderingContext2D.PATTERN);
window["CanvasRenderingContext2D"]=CanvasRenderingContext2D;