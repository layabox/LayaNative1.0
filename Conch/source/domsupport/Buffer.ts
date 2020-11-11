interface IBuffer{
    id:number;
}
class _Buffer{
    private ABSIZE = 20 * 1024;
    private _cmdBuf = new DataView(new ArrayBuffer(this.ABSIZE));
    
    private _cmdPos = new Uint32Array(this._cmdBuf.buffer,0,1);

    public  _byteArray:Uint8Array = new Uint8Array(this._cmdBuf.buffer);
    private _shortArray:Uint16Array=new Uint16Array(this._cmdBuf.buffer);
    private _intArray:Uint32Array=new Uint32Array(this._cmdBuf.buffer);
    private _floatArray:Float32Array=new Float32Array(this._cmdBuf.buffer);
    private _nativeobj:any;
    
    constructor(){
        this._nativeobj=conch;
       //conch.setCmdBuffer(this._byteArray.buffer);
    }
    needsz(d: number) {
        if (this._cmdPos[0] + d > this.ABSIZE) {
            this._nativeobj.flushRenderCommands();
        }
    }
    
    initPos(d:number){
        this._cmdPos[0] = d;
    }
    
    wu8(v: number) {
        this._byteArray[this._cmdPos[0]++]=v;
    }
    wu32(v: number) {
        this._intArray[(this._cmdPos[0]+=4)/4-1]=v;
    }
    wu16(v: number) {
        this._shortArray[(this._cmdPos[0]+=2)/2-1]=v;
    }
    wf32(v: number) {
        this._floatArray[(this._cmdPos[0]+=4)/4-1]=v;
    }
    
    wf32Rect(x:number,y:number,w:number,h:number){
        this.wf32(x);
        this.wf32(y);
        this.wf32(w);
        this.wf32(h);
    }
    wu64(value: number): void {
        var temp = value.toString(16);
        var len = temp.length;
        var pre, last;
        if (len > 8 && value > 0) {
            pre = parseInt(temp.substr(len - 8), 16);
            last = parseInt(temp.substr(0, len - 8), 16);
        }
        else {
            pre = value;
            last = 0;
        }
        this.wu32(pre);
        this.wu32(last);
    }

     wab(arraybuffer: ArrayBuffer | ArrayBufferView, length: number,offset?:number): void {
         offset=offset?offset:0;
        if (length > this.ABSIZE / 2) {//如果太大，则直接给c
            this.flush();//已经有的
            this._nativeobj.appendBuffer(arraybuffer,length,offset);//现在的
            return;
        }
        if ( length < 0)
            throw "wab error - Out of bounds";
            
        var i = length % 4;
        this.needsz(length + (4 - i));

        var uint8array;
        if (arraybuffer instanceof ArrayBuffer) 
        {
            uint8array = new Uint8Array(arraybuffer, offset, length);
        }
        else if (arraybuffer.buffer)
        {
            uint8array = new Uint8Array(arraybuffer.buffer, offset + arraybuffer.byteOffset,length);
        } 
        else throw "not arraybuffer/dataview ";
       
        this._byteArray.set(uint8array, this._cmdPos[0]);
        this._cmdPos[0] += length;

        if(i!=0)
        {
            for(var j=0;j<4-i;j++)
            {
               this.wu8(0);
            }
        }
    }
    //TODO 不要for
    wstring(str: string,is32:boolean): void {
        var ab = conch.strTobufer(str); //现在这个函数返回的已经是对齐的了
		var len = ab.byteLength;
        if (is32)
            this.wu32(len);
        else
            this.wu16(len);
        if (len == 0) {
            return;
        }
		this.wab(ab, len);
    }
    
     flush() {
        if (this._cmdPos[0]<=4)
            return;
        this._nativeobj.flushRenderCommands();
        //glperf.frameend();
    }
    
    static readString(bf:DataView,pos:number,len:number):string
    {
        len=bf.getUint16(pos,true);
        pos+=2;
        var value:string = "";
        var max:number = pos + len;
        var c:number, c2:number, c3:number;
        // utf8-encode
        while (pos < max) {
            c = bf.getUint8 (pos++);
            if (c < 0x80) {
                //if(c==0) flash的原生
                if (c !=0)
                {
                    value +=String.fromCharCode (c);
                }
            } else if (c < 0xE0) {
                value += String.fromCharCode (((c & 0x3F) << 6) | (bf.getUint8 (pos++) & 0x7F));
            } else if (c < 0xF0) {
                c2 = bf.getUint8 (pos++);
                value += String.fromCharCode (((c & 0x1F) << 12) | ((c2 & 0x7F) << 6) | (bf.getUint8 (pos++) & 0x7F));
            } else {
                c2 = bf.getUint8 (pos++);
                c3 = bf.getUint8 (pos++);
                value += String.fromCharCode (((c & 0x0F) << 18) | ((c2 & 0x7F) << 12) | ((c3 << 6) & 0x7F) | (bf.getUint8 (pos++) & 0x7F));
            }
        }	
        return value;
    }
    
    
    
    static setMatrix(that:IBuffer,fid:number,m11: number, m12: number, m21: number, m22: number, dx: number, dy: number):void
    {
        var _:_Buffer=conch.bf;
        _.needsz(32);
        _.wu32(that.id);
        _.wu32(fid);
        _.wf32(m11);
        _.wf32(m12);
        _.wf32(m21);
        _.wf32(m22);
        _.wf32(dx);
        _.wf32(dy);
    }
         
    static rect(that:IBuffer,fid:number,x: number, y: number, w: number, h: number):void
    {
        var _:_Buffer=conch.bf;
        _.needsz(24);
        _.wu32(that.id);
        _.wu32(fid);
        _.wf32Rect(x,y,w,h);
    }
    
    static noParm(that:IBuffer,fid:number):void
    {
        var _:_Buffer=conch.bf;
        _.needsz(8);
        _.wu32(that.id);
        _.wu32(fid);
    }
    
    static oneNum(that:IBuffer,fid:number,n:number):void
    {
        var _:_Buffer=conch.bf;
        _.needsz(12);
        _.wu32(that.id);
        _.wu32(fid);
        _.wf32(n);
    }
    
    static oneInt(that:IBuffer,fid:number,n:number):void
    {
        var _:_Buffer=conch.bf;
        _.needsz(12);
        _.wu32(that.id);
        _.wu32(fid);
        _.wu32(n);
    }
    
    static point(that:IBuffer,fid:number,x:number,y:number):void
    {
        var _:_Buffer=conch.bf;
        _.needsz(16);
        _.wu32(that.id);
        _.wu32(fid);
        _.wf32(x);
        _.wf32(y);
    }
    
    static wany(_:_Buffer,ab:any,t:number):void{
        switch(t)
        {
        case 0:
            _.needsz(4);
            var i = ab.byteLength;
            _.wu32(i);
            _.wab(ab, i);
            break;
        case 1:
            _.needsz(8);
            _.wu32(4);
            _.wu32(ab);
            break;
        case 2:
            _.needsz(8);
            _.wu32(4);
            _.wf32(ab);
            break;
        default:
            break;
        }
    }
}
conch.bf=new _Buffer();
conch.setCmdBuffer(conch.bf._byteArray.buffer);
conch.bf.initPos(4);