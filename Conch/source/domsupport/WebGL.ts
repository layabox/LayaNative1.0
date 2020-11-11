
interface WebGLContextAttributes {
    alpha?: boolean;
    depth?: boolean;
    stencil?: boolean;
    antialias?: boolean;
    premultipliedAlpha?: boolean;
    preserveDrawingBuffer?: boolean;
}

///////////////////////////////////////

class ProgramLocationTable{
    FADELOC_BASE:number=50000;
    _obj:Object={};
    getFadeLocation(fadeProgramID:number, name:string):number 
    {
        var key = fadeProgramID + "-" + name;
        var fadeID = this._obj[key];
        if (fadeID == undefined) {
            fadeID = this.FADELOC_BASE;
            this.FADELOC_BASE++;
            this._obj[key] = fadeID;
        }
        return fadeID;
    }
}

class WebGLRenderingContext {
    /*
    static FT_getExtension = 0;
    static FT_createBuffer = 1;
    static FT_bindBuffer = 2;
    static FT_bufferData_vector = 3;
    static FT_bufferData_size = 4;
    static FT_createShader = 5;
    static FT_shaderSource = 6;
    static FT_compileShader = 7;
    static FT_getShaderParameter = 8;
    static FT_createProgram = 9;
    static FT_attachShader = 10
    static FT_linkProgram = 11;
    static FT_getProgramParameter = 12;
    static FT_useProgram = 13;
    static FT_getUniformLocation = 14;
    static FT_getAttribLocation = 15;
    static FT_disable = 16;
    static FT_enable = 17;
    static FT_colorMask = 18;
    static FT_viewport = 19;
    static FT_clear = 20;
    static FT_clearColor = 21;
    static FT_createTexture = 22;
    static FT_bindTexture = 23;
    static FT_pixelStorei = 24;
    static FT_texImage2D_06 = 25;
    static FT_texImage2D_09 = 26;
    static FT_texSubImage2D = 27;
    static FT_texParameterf = 28;
    static FT_texParameteri = 29;
    static FT_activeTexture = 30;
    static FT_getShaderInfoLog = 31;
    static FT_uniform1f = 32;
    static FT_uniform1i = 33;
    static FT_uniform2f = 34;
    static FT_uniform2i = 35;
    static FT_uniform3f = 36;
    static FT_uniform3i = 37;
    static FT_uniform4f = 38;
    static FT_uniform4i = 39;
    static FT_uniform1fv = 40;
    static FT_uniform1iv = 41;
    static FT_uniform2fv = 42;
    static FT_uniform2iv = 43;
    static FT_uniform3fv = 44;
    static FT_uniform3iv = 45;
    static FT_uniform4fv = 46;
    static FT_uniform4iv = 47;
    static FT_uniformMatrix2fv = 48;
    static FT_uniformMatrix3fv = 49;
    static FT_uniformMatrix4fv = 50;
    static FT_vertexAttribPointer = 51;
    static FT_enableVertexAttribArray = 52;
    static FT_blendFunc = 53;
    static FT_bufferSubData = 54;
    static FT_drawElements = 55;
    static FT_deleteTexture = 56;
    static FT_drawArrays = 57;
    static FT_bindAttribLocation = 58;
    static FT_cullFace = 59;
    static FT_createFramebuffer = 60;
    static FT_getParameter = 61;
    static FT_bindFramebuffer = 62;
    static FT_depthFunc = 63;
    static FT_depthMask = 64;
    static FT_depthRange = 65;
    static FT_stencilFunc = 66;
    static FT_stencilMask = 67;
    static FT_stencilOp = 68;
    static FT_deleteBuffer = 69;
    static FT_deleteFramebuffer = 70;
    static FT_deleteProgram = 71;
    static FT_deleteRenderbuffer = 72;
    static FT_deleteShader = 73;
    static FT_isBuffer = 74;
    static FT_disableVertexAttribArray = 75;
    static FT_scissor = 76;
    static FT_generateMipmap = 77;
    static FT_framebufferTexture2D = 78;
    static FT_checkFramebufferStatus = 79;
    static FT_createRenderbuffer=80;
    static FT_bindRenderbuffer=81;
    static FT_renderbufferStorage=82;
    static FT_framebufferRenderbuffer=83;
    static FT_blendFuncSeparate=84;
    static FT_frontFace=85;
    static FT_getActiveAttrib=86;
    static FT_getActiveUniform=87;
    static FT_FINISH = 0xffffffff;
    
    static TYPE_IMAGE = 0;
    static TYPE_CANVAS = 1;
    static TYPE_MEMORY_CANVAS = 2;
    static TYPE_TEXT_CANVAS = 3;
    */
    private _nativeObj:WebGLContext;
    private id:number;
    drawingBufferWidth = getInnerWidth();// screen.width;
    drawingBufferHeight = getInnerHeight();// screen.height;
    currentTextureID = 0;
    private viewportx=0;
    private viewporty=0;
    private viewportw=0;
    private viewporth=0;
    private _i:number=0;
    private _fadeArray=[];
    private curFBO:number=0;
    __priv_canvas: HTMLCanvasElement;    //項目中也會設置 _canvas,所以改個名字
    constructor() {
        this._nativeObj=new WebGLContext();
        this.id=this._nativeObj.conchID;
        this._fadeArray.push(0);
        //glperf.init(this);
       /* document.addEventListener('frameend',(e:Event)=>{
            //如果是可见的canvas，则要flush
            if(this.__priv_canvas && this.__priv_canvas.parentNode){
               conch.bf.flush();
            }
        });*/
    }
    
    private createFadeID():number {
        for( var i:number=0,n:number=this._fadeArray.length;i<n;i++)
        {
            if(this._fadeArray[i]==-1)
            {
                this._fadeArray[i]=i;
                return i;
            }
        }
        var nID:number = ++this._i;
        this._fadeArray.push(nID);
        return nID;
    }

    private deleteFadeID(nID:number){
        this._fadeArray[nID] = -1;
    }


    getContextAttributes(): WebGLContextAttributes {
        return {};
    }

    onCssTransform(){
        //不需要first
        if( this.__priv_canvas ){
            this.viewport(this.viewportx,this.viewporty,this.viewportw,this.viewporth);
        }
    }
    width = 0;
    height = 0;
    _locTable = new ProgramLocationTable();
  
    //是否丢失上下文
    isContextLost():boolean{
        return false;
    }
    
    getShaderPrecisionFormat(a:number,b:number):any{
        return {
            precision: 23,
            rangeMax: 127,
            rangeMin: 127
        };
    }
    


    getExtension(name:string) {
        conch.bf.needsz(8);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_getExtension*/0);
    }

    createBuffer() {
        var fadeID = this.createFadeID();
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_createBuffer*/1);
        conch.bf.wu32(fadeID);
        return fadeID;
    }

    bindBuffer(target:number, buffer:number) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_bindBuffer*/2);
        conch.bf.wu32(target);
        conch.bf.wu32(buffer);
    }

    flush() {
        conch.bf.flush();
        //glperf.frameend();
    }

    bufferData(target:number, sizeOrArray:number|ArrayBuffer, usage:number) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        if (typeof (arguments[1]) === 'number') {
            conch.bf.wu32(/*WebGLRenderingContext.FT_bufferData_size*/4);
            conch.bf.wu32(arguments[0]);  //target
            conch.bf.wu32(arguments[2]);	//usage		
            conch.bf.wu32(arguments[1]);	//size
        }
        else {
            conch.bf.wu32(/*WebGLRenderingContext.FT_bufferData_vector*/3);
            conch.bf.wu32(arguments[0]);  //target
            conch.bf.wu32(arguments[2]);	//usage		

            var length = arguments[1].byteLength;
            if ((length % 4) != 0) {
                alert("bufferData,len is not 4 align,length=" + length);
            }
            conch.bf.wu32(length);	//length
            conch.bf.wab(arguments[1], length);	//buffer
        }
    }

    createShader(type) {
        var fadeID = this.createFadeID();
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_createShader*/5);
        conch.bf.wu32(type);
        conch.bf.wu32(fadeID);
        return fadeID;
    }

    shaderSource(shader, source) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_shaderSource*/6);
        conch.bf.wu32(shader);
        conch.bf.wstring(source, true);
    }

    compileShader(shader) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_compileShader*/7);
        conch.bf.wu32(shader);
    }

    getShaderParameter(shader, pname) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_getShaderParameter*/8);
        conch.bf.wu32(shader);
        conch.bf.wu32(pname);
        return true;
    }

    createProgram() {
        var fadeID = this.createFadeID();
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_createProgram*/9);
        conch.bf.wu32(fadeID);
        return fadeID;
    }

    attachShader(program, shader) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_attachShader*/10);
        conch.bf.wu32(program);
        conch.bf.wu32(shader);
    }

    linkProgram(program) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_linkProgram*/11);
        conch.bf.wu32(program);
    }

    getProgramParameter(program, pname) {
        return 0;
    }

    useProgram(program) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_useProgram*/13);
        conch.bf.wu32(program);
    }

    getUniformLocation(program, name) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_getUniformLocation*/14);
        conch.bf.wu32(program);
        var fadeLoc = this._locTable.getFadeLocation(program, name);
        conch.bf.wu32(fadeLoc);
        conch.bf.wstring(name, true);
        return fadeLoc;
    }

    getAttribLocation(program, name) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_getAttribLocation*/15);
        conch.bf.wu32(program);
        var fadeLoc = this._locTable.getFadeLocation(program, name);
        conch.bf.wu32(fadeLoc);
        conch.bf.wstring(name, true);
        return fadeLoc;
    }

    disable(cap) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_disable*/16);
        conch.bf.wu32(cap);
    }

    enable(cap) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_enable*/17);
        conch.bf.wu32(cap);
    }

    colorMask(red, green, blue, alpha) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_colorMask*/18);
        conch.bf.wu8(red ? 1 : 0);
        conch.bf.wu8(green ? 1 : 0);
        conch.bf.wu8(blue ? 1 : 0);
        conch.bf.wu8(alpha ? 1 : 0);
    }

    viewport(x:number, y:number, width:number, height:number) {
        this.viewportx=x;
        this.viewporty=y;
        this.viewportw=width;
        this.viewporth=height;
        /**
         * TODO 先简化处理transform。只考虑缩放因素。
         */
       if (!this.curFBO&&this.__priv_canvas._hasTransform) {
            var t = this.__priv_canvas._tranform;
            x += t[4]; y += t[5];
            width *= t[0]; height *= t[3];
        }
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_viewport*/19);
        conch.bf.wu16(x);
        conch.bf.wu16(y);
        conch.bf.wu16(width);
        conch.bf.wu16(height);
    }

    clear(mask:number) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_clear*/20);
        conch.bf.wu32(mask);
    }

    clearColor(red:number, green:number, blue:number, alpha:number) {
        conch.bf.needsz(24);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_clearColor*/21);
        conch.bf.wf32(red);
        conch.bf.wf32(green);
        conch.bf.wf32(blue);
        conch.bf.wf32(alpha);
    }

    createTexture(invalidCallback, restoreCallback) {
        var fadeID = this.createFadeID();
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_createTexture*/22);
        conch.bf.wu32(fadeID);
        return fadeID;
    }

    bindTexture(target, texture) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_bindTexture*/23);
        conch.bf.wu32(target);
        conch.bf.wu32(texture);
        this.currentTextureID = texture;
    }

    pixelStorei(pname, param) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_pixelStorei*/24);
        conch.bf.wu32(pname);
        conch.bf.wu32(param);
    }

    texImage2D(target, level, internalformat, width, height, border, format, type, pixels) {
        if (arguments.length == 9) {
            this.texImage2DEx(false, target, level, internalformat, width, height, border, format, type, pixels);
        }
        else if (arguments.length == 6) {
            this.texImage2DEx(false, target, level, internalformat, width, height, border);
        }
    }

    texImage2DEx(premultiplyAlpha, target, level, internalformat, width, height, border, format?, type?, pixels?) {
        if (arguments.length == 10) {
            conch.bf.needsz(42);
            conch.bf.wu32(this.id);
            conch.bf.wu32(/*WebGLRenderingContext.FT_texImage2D_09*/26);
            conch.bf.wu32(arguments[0+1]);  //target
            conch.bf.wu32(arguments[1+1]);	//level
            conch.bf.wu32(arguments[2+1]);	//internalformat
            conch.bf.wu32(arguments[3+1]);	//width
            conch.bf.wu32(arguments[4+1]);	//height
            conch.bf.wu32(arguments[5+1]);	//border
            conch.bf.wu32(arguments[6+1]);	//format
            conch.bf.wu32(arguments[7+1]);	//type
            conch.bf.wu32(arguments[0] ? 1 : 0);	//premultiplyAlpha
            var argobj = arguments[5+1] && arguments[8+1]._nativeObj;
            if (argobj) {
                conch.bf.needsz(8);
                if (argobj && argobj.isCanvas && argobj.isCanvas()) {
                    alert("texSubImage2D:禁止2D和3D混合了 " + (new Error())['stack']);
                    conch.bf.wu32(/*WebGLRenderingContext.TYPE_CANVAS*/1);
                }
                else if (argobj.isMemoryCanvas && argobj.isMemoryCanvas()) {
                    conch.bf.wu32(/*WebGLRenderingContext.TYPE_MEMORY_CANVAS*/2);
                    argobj.setTargetTextureID(this.currentTextureID);
                }
                else if (argobj.isTextCanvas && argobj.isTextCanvas()) {
                    conch.bf.wu32(/*WebGLRenderingContext.TYPE_TEXT_CANVAS*/3);
                    argobj.setTargetTextureID(this.currentTextureID);
                }
                else {
                    conch.bf.wu32(/*WebGLRenderingContext.TYPE_IMAGE*/0);
                }
                //this.wu32(arguments[8].conchImgId);
                conch.bf.wu32(argobj.conchImgId);
            }
            else {
                conch.bf.needsz(12);
                conch.bf.wu32(/*WebGLRenderingContext.TYPE_IMAGE*/0);
                //this.wu32(0);	//Image
                conch.bf.wu32(-1);
            }

        }
        else if (arguments.length == 7) {
            conch.bf.needsz(38)
            conch.bf.wu32(this.id);
            conch.bf.wu32(/*WebGLRenderingContext.FT_texImage2D_06*/25);
            conch.bf.wu32(arguments[0+1]);  //target
            conch.bf.wu32(arguments[1+1]);	//level
            conch.bf.wu32(arguments[2+1]);	//internalformat
            conch.bf.wu32(arguments[3+1]);	//format
            conch.bf.wu32(arguments[4+1]);	//type
            conch.bf.wu32(arguments[0] ? 1 : 0);	//premultiplyAlpha
            var argobj = arguments[5+1] && arguments[5+1]._nativeObj;
            if (argobj) {

                if (argobj && argobj.isCanvas && argobj.isCanvas()) {
                    alert("texSubImage2D:禁止2D和3D混合了 " + (new Error())['stack']);
                    conch.bf.wu32(/*WebGLRenderingContext.TYPE_CANVAS*/1);
                }
                else if (argobj.isMemoryCanvas && argobj.isMemoryCanvas()) {
                    conch.bf.wu32(/*WebGLRenderingContext.TYPE_MEMORY_CANVAS*/2);
                    argobj.setTargetTextureID(this.currentTextureID);
                }
                else if (argobj.isTextCanvas && argobj.isTextCanvas()) {
                    conch.bf.wu32(/*WebGLRenderingContext.TYPE_TEXT_CANVAS*/3);
                    argobj.setTargetTextureID(this.currentTextureID);
                }
                else {
                    conch.bf.wu32(/*WebGLRenderingContext.TYPE_IMAGE*/0);
                }
                //conch.bf.wu32(arguments[5].conchImgId);
                conch.bf.wu32(argobj.conchImgId);
            }
            else {
                conch.bf.wu32(/*WebGLRenderingContext.TYPE_IMAGE*/0);
                //conch.bf.wu32(0);	//Image
                conch.bf.wu32(-1);
            }
        }
        //alert("<OUT>  texImage2D");
    }

    texSubImage2D(target, level, xoffset, yoffset, format, type, source) {
        if (arguments.length == 6) {
            this.texSubImage2DEx(false, target, level, xoffset, yoffset, format, type);
        }
        else if (arguments.length == 7) {
            this.texSubImage2DEx(false, target, level, xoffset, yoffset, format, type, source);
        }
    }

    texSubImage2DEx(premultiplyAlpha, target, level, xoffset, yoffset, format, type, source?) {
        conch.bf.needsz(42);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_texSubImage2D*/27);
        conch.bf.wu32(arguments[0+1]);  //target
        conch.bf.wu32(arguments[1+1]);	//level
        conch.bf.wu32(arguments[2+1]);	//xoffset
        conch.bf.wu32(arguments[3+1]);	//yoffset
        conch.bf.wu32(arguments[4+1]);	//format
        conch.bf.wu32(arguments[5+1]);	//type
        conch.bf.wu32(arguments[0] ? 1 : 0);	//premultiplyAlpha
        var argobj = arguments[6+1] && (arguments[6+1]._nativeObj || arguments[6+1]);
        if (argobj) {
            if (argobj && argobj.isCanvas && argobj.isCanvas()) {
                var e = new Error('dd');
                alert("texSubImage2D:禁止2D和3D混合了 " + (<any>e).stack);
                conch.bf.wu32(/*WebGLRenderingContext.TYPE_CANVAS*/1);
            }
            else if (argobj.isMemoryCanvas && argobj.isMemoryCanvas()) {
                conch.bf.wu32(/*WebGLRenderingContext.TYPE_MEMORY_CANVAS*/2);
                argobj.setTargetTextureID(this.currentTextureID);
            }
            else if (argobj.isTextCanvas && argobj.isTextCanvas()) {
                conch.bf.wu32(/*WebGLRenderingContext.TYPE_TEXT_CANVAS*/3);
                argobj.setTargetTextureID(this.currentTextureID);
            }
            else {
                conch.bf.wu32(/*WebGLRenderingContext.TYPE_IMAGE*/0);
            }
            //conch.bf.wu32(arguments[6].conchImgId);
            conch.bf.wu32(argobj.conchImgId);
        }
        else {
            conch.bf.wu32(/*WebGLRenderingContext.TYPE_IMAGE*/0);
            //conch.bf.wu32(0);	//Image
            conch.bf.wu32(-1);
        }
    }

    texParameterf(target, pname, param) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_texParameterf*/28);
        conch.bf.wu32(target);
        conch.bf.wu32(pname);
        conch.bf.wf32(param);
    }

    texParameteri(target, pname, param) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_texParameteri*/29);
        conch.bf.wu32(target);
        conch.bf.wu32(pname);
        conch.bf.wu32(param);
    }

    activeTexture(texture) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_activeTexture*/30);
        conch.bf.wu32(texture);
    }

    getShaderInfoLog(shader) {
        return "unknown log";
    }

    uniform1f(location, x) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_uniform1f*/32);
        conch.bf.wu32(location);
        conch.bf.wf32(x);
    }

    uniform1i(location, x) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_uniform1i*/33);
        conch.bf.wu32(location);
        conch.bf.wu32(x);
    }

    uniform2f(location, x, y) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_uniform2f*/34);
        conch.bf.wu32(location);
        conch.bf.wf32(x);
        conch.bf.wf32(y);
    }

    uniform2i(location, x, y) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_uniform2i*/35);
        conch.bf.wu32(location);
        conch.bf.wu32(x);
        conch.bf.wu32(y);
    }

    uniform3f(location, x, y, z) {
        conch.bf.needsz(24);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_uniform3f*/36);
        conch.bf.wu32(location);
        conch.bf.wf32(x);
        conch.bf.wf32(y);
        conch.bf.wf32(z);
    }

    uniform3i(location, x, y, z) {
        conch.bf.needsz(24);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_uniform3i*/37);
        conch.bf.wu32(location);
        conch.bf.wu32(x);
        conch.bf.wu32(y);
        conch.bf.wu32(z);
    }

    uniform4f(location, x, y, z, w) {
        conch.bf.needsz(28);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_uniform4f*/38);
        conch.bf.wu32(location);
        conch.bf.wf32(x);
        conch.bf.wf32(y);
        conch.bf.wf32(z);
        conch.bf.wf32(w);
    }

    uniform4i(location, x, y, z, w) {
        conch.bf.needsz(28);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_uniform4i*/39);
        conch.bf.wu32(location);
        conch.bf.wu32(x);
        conch.bf.wu32(y);
        conch.bf.wu32(z);
        conch.bf.wu32(w);
    }
    uniform1fv(location, v) {
        this.uniformxv(/*WebGLRenderingContext.FT_uniform1fv*/40, location, v);
    }
    uniform1iv(location, v) {
        this.uniformxv(/*WebGLRenderingContext.FT_uniform1iv*/41, location, v);
    }
    uniform2fv(location, v) {
        this.uniformxv(/*WebGLRenderingContext.FT_uniform2fv*/42, location, v);
    }
    uniform2iv(location, v) {
        this.uniformxv(/*WebGLRenderingContext.FT_uniform2iv*/43, location, v);
    }
    uniform3fv(location, v) {
        this.uniformxv(/*WebGLRenderingContext.FT_uniform3fv*/44, location, v);
    }
    uniform3iv(location, v) {
        this.uniformxv(/*WebGLRenderingContext.FT_uniform3iv*/45, location, v);
    }
    uniform4fv(location, v) {
        this.uniformxv(/*WebGLRenderingContext.FT_uniform4fv*/46, location, v);
    }
    uniform4iv(location, v) {
        this.uniformxv(/*WebGLRenderingContext.FT_uniform4iv*/47, location, v);
    }
    uniformxv(cmd, location, v) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(cmd);
        conch.bf.wu32(location);
        var length = v.byteLength;
        conch.bf.wu32(length);//byte length
        conch.bf.wab(v, length);
    }
    _uniformMatrixfv_x(cmd, location, transpose, values, number) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(cmd);
        conch.bf.wu32(location);
        conch.bf.wu32(transpose);
        if (values.byteLength == null)
        {
            var len=values.length;
            conch.bf.wu32(len*4);
            conch.bf.needsz(len*4);
            for(var i=0;i<len;i++){
                conch.bf.wf32(values[i]);
            }
            return;
        }

        var length = values.byteLength;
        if ((length % 4) != 0) {
            alert("_uniformMatrixfv_x- len is not 4 align!");
        }

        conch.bf.wu32(length);  //byte length
        conch.bf.wab(values, length);
    }

    uniformMatrix2fv(location, transpose, values, number) {
        this._uniformMatrixfv_x(/*WebGLRenderingContext.FT_uniformMatrix2fv*/48, location, transpose, values, number);
    }

    uniformMatrix3fv(location, transpose, values, number) {
        this._uniformMatrixfv_x(/*WebGLRenderingContext.FT_uniformMatrix3fv*/49, location, transpose, values, number);
    }

    uniformMatrix4fv(location, transpose, values, number) {
        this._uniformMatrixfv_x(/*WebGLRenderingContext.FT_uniformMatrix4fv*/50, location, transpose, values, number);
    }

    vertexAttribPointer(index, size, type, normalized, stride, offset) {
        conch.bf.needsz(32);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_vertexAttribPointer*/51);
        conch.bf.wu32(index);
        conch.bf.wu32(size);
        conch.bf.wu32(type);
        conch.bf.wu32(normalized);
        conch.bf.wu32(stride);
        conch.bf.wu32(offset);
    }

    enableVertexAttribArray(index) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_enableVertexAttribArray*/52);
        conch.bf.wu32(index);
    }

    blendFunc(sfactor, dfactor) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_blendFunc*/53);
        conch.bf.wu16(sfactor);
        conch.bf.wu16(dfactor);
    }

    bufferSubData(target:number, offset:number, data:ArrayBufferView) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_bufferSubData*/54);
        conch.bf.wu32(target);
        conch.bf.wu32(offset);

        var length = data.byteLength;
        // if ((length % 4) != 0) {
        //     alert("bufferSubData- length is not 4 align!");
        // }

        conch.bf.wu32(length);
        conch.bf.wab(data, length,0);
    }

    drawElements(mode, count, type, offset) {
        conch.bf.needsz(24);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_drawElements*/55);
        conch.bf.wu32(mode);
        conch.bf.wu32(count);
        conch.bf.wu32(type);
        conch.bf.wu32(offset);
    }

    deleteTexture(texture) {
        this.deleteFadeID(texture);
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_deleteTexture*/56);
        conch.bf.wu32(texture);
    }

    drawArrays(mode, first, count) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_drawArrays*/57);
        conch.bf.wu32(mode);
        conch.bf.wu32(first);
        conch.bf.wu32(count);
    }

    bindAttribLocation(program, index, name) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_bindAttribLocation*/58);
        conch.bf.wu32(program);
        conch.bf.wu32(index);
        conch.bf.wstring(name, true);
    }

    cullFace(mode) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_cullFace*/59);
        conch.bf.wu32(mode);
    }

    createFramebuffer() {
        var fadeID = this.createFadeID();
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_createFramebuffer*/60);
        conch.bf.wu32(fadeID);
        return fadeID;
    }

    getParameter(pname) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_getParameter*/61);
        conch.bf.wu32(pname);
        var fadeValue = 0;
        if (pname == this.FRAMEBUFFER_BINDING) {
            fadeValue = this.curFBO;// this.createFadeID();
        }
        conch.bf.wu32(fadeValue);
        return fadeValue;
    }

    bindFramebuffer(target, framebuffer) {
        this.curFBO = framebuffer;
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_bindFramebuffer*/62);
        conch.bf.wu32(target);
        conch.bf.wu32(framebuffer);
    }

    depthFunc(func) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_depthFunc*/63);
        conch.bf.wu32(func);
    }

    depthMask(flag) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_depthMask*/64);
        conch.bf.wu32(flag);
    }

    depthRange(zNear, zFar) {
        conch.bf.needsz(16);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_depthRange*/65);
        conch.bf.wf32(zNear);
        conch.bf.wf32(zFar);
    }

    stencilFunc(func, ref, mask) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_stencilFunc*/66);
        conch.bf.wu32(func);
        conch.bf.wu32(ref);
        conch.bf.wu32(mask);
    }

    stencilMask(mask) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_stencilMask*/67);
        conch.bf.wu32(mask);
    }

    stencilOp(fail, zfail, zpass) {
        conch.bf.needsz(20);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_stencilOp*/68);
        conch.bf.wu32(fail);
        conch.bf.wu32(zfail);
        conch.bf.wu32(zpass);
    }

    deleteBuffer(buffer) {
        this.deleteFadeID(buffer);
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_deleteBuffer*/69);
        conch.bf.wu32(buffer);
    }

    deleteFramebuffer(framebuffer) {
        this.deleteFadeID(framebuffer);
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_deleteFramebuffer*/70);
        conch.bf.wu32(framebuffer);
    }

    deleteProgram(program) {
        this.deleteFadeID(program);
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_deleteProgram*/71);
        conch.bf.wu32(program);
    }

    deleteRenderbuffer(renderbuffer) {
        this.deleteFadeID(renderbuffer);
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_deleteRenderbuffer*/72);
        conch.bf.wu32(renderbuffer);
    }

    deleteShader(shader) {
        this.deleteFadeID(shader);
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_deleteShader*/73);
        conch.bf.wu32(shader);
    }

    isBuffer(buffer) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_isBuffer*/74);
        conch.bf.wu32(buffer);
        return true;
    }

    disableVertexAttribArray(index) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_disableVertexAttribArray*/75);
        conch.bf.wu32(index);
    }

    scissor(x, y, width, height) {
        /**
         * TODO 先简化处理transform。只考虑缩放因素。
         */
        if (!this.curFBO && this.__priv_canvas._hasTransform) {
            var t = this.__priv_canvas._tranform;
            width *= t[0]; height *= t[3];
            x = x * t[0] + t[4];
            y = y * t[3] + t[5];
        }
        conch.bf.needsz(24);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_scissor*/76);
        conch.bf.wu32(x);
        conch.bf.wu32(y);
        conch.bf.wu32(width);
        conch.bf.wu32(height);
    }

    generateMipmap(target) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_generateMipmap*/77);
        conch.bf.wu32(target);
    }

    framebufferTexture2D(target, attachment, textarget, texture, level) {
        conch.bf.needsz(28);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_framebufferTexture2D*/78);
        conch.bf.wu32(target);
        conch.bf.wu32(attachment);
        conch.bf.wu32(textarget);
        conch.bf.wu32(texture);
        conch.bf.wu32(level);
    }

    checkFramebufferStatus(target) {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_checkFramebufferStatus*/79);
        conch.bf.wu32(target);
        if (target == this.FRAMEBUFFER) {
            return this.FRAMEBUFFER_COMPLETE;
        }
        return 0;
    }
    
    createRenderbuffer(){
        var _=conch.bf;
        var fadeID = this.createFadeID();
        _.needsz(12);
        _.wu32(this.id);
        _.wu32(/*WebGLRenderingContext.FT_createRenderbuffer*/80);
        _.wu32(fadeID);
        return fadeID;
    }
    
    bindRenderbuffer(target:number, buffer:number){
        var _=conch.bf;
        _.needsz(16);
        _.wu32(this.id);
        _.wu32(/*WebGLRenderingContext.FT_bindRenderbuffer*/81);
        _.wu32(target);
        _.wu32(buffer);
    }
    
    renderbufferStorage(target:number,internalformat:number,width:number,height:number)
    {
        var _=conch.bf;
        _.needsz(24);
        _.wu32(this.id);
        _.wu32(/*WebGLRenderingContext.FT_renderbufferStorage*/82);
        _.wu32(target);
        _.wu32(internalformat);
        _.wu32(width);
        _.wu32(height);
    }
    
    framebufferRenderbuffer(target,attachment,renderbuffertarget,buffer)
    {
        var _=conch.bf;
        _.needsz(24);
        _.wu32(this.id);
        _.wu32(/*WebGLRenderingContext.FT_framebufferRenderbuffer*/83);
        _.wu32(target);
        _.wu32(attachment);
        _.wu32(renderbuffertarget);
        _.wu32(buffer);
    }
    
    blendFuncSeparate( srcRGB,  dstRGB,  srcAlpha, dstAlpha)
    {
        var _=conch.bf;
        _.needsz(16);
        _.wu32(this.id);
        _.wu32(/*WebGLRenderingContext.FT_blendFuncSeparate*/84);
        _.wu16(srcRGB);
        _.wu16(dstRGB);
        _.wu16(srcAlpha);
        _.wu16(dstAlpha);
    }

    frontFace(mode) 
    {
        conch.bf.needsz(12);
        conch.bf.wu32(this.id);
        conch.bf.wu32(/*WebGLRenderingContext.FT_frontFace*/85);
        conch.bf.wu32(mode);
    }

    getProgramParameterEx(vs:String,ps:String,define:String,pname:Number)
    {
        return this._nativeObj.getProgramParameterEx(vs,ps,define,pname);
    }

    getActiveAttribEx(vs:String,ps:String,define:String,index:Number)
    {
        return this._nativeObj.getActiveAttribEx(vs,ps,define,index);
    }

    getActiveUniformEx(vs:String,ps:String,define:String,index:Number)
    {
        return this._nativeObj.getActiveUniformEx(vs,ps,define,index);
    } 
    

    DEPTH_BUFFER_BIT = 0x00000100;
    STENCIL_BUFFER_BIT = 0x00000400;
    COLOR_BUFFER_BIT = 0x00004000;

    /* BeginMode */
    POINTS = 0x0000;
    LINES = 0x0001;
    LINE_LOOP = 0x0002;
    LINE_STRIP = 0x0003;
    TRIANGLES = 0x0004;
    TRIANGLE_STRIP = 0x0005;
    TRIANGLE_FAN = 0x0006;

    /* AlphaFunction (not supported in ES20) */
    /*      NEVER */
    /*      LESS */
    /*      EQUAL */
    /*      LEQUAL */
    /*      GREATER */
    /*      NOTEQUAL */
    /*      GEQUAL */
    /*      ALWAYS */

    /* BlendingFactorDest */
    ZERO = 0;
    ONE = 1;
    SRC_COLOR = 0x0300;
    ONE_MINUS_SRC_COLOR = 0x0301;
    SRC_ALPHA = 0x0302;
    ONE_MINUS_SRC_ALPHA = 0x0303;
    DST_ALPHA = 0x0304;
    ONE_MINUS_DST_ALPHA = 0x0305;

    /* BlendingFactorSrc */
    /*      ZERO */
    /*      ONE */
    DST_COLOR = 0x0306;
    ONE_MINUS_DST_COLOR = 0x0307;
    SRC_ALPHA_SATURATE = 0x0308;
    /*      SRC_ALPHA */
    /*      ONE_MINUS_SRC_ALPHA */
    /*      DST_ALPHA */
    /*      ONE_MINUS_DST_ALPHA */

    /* BlendEquationSeparate */
    FUNC_ADD = 0x8006;
    BLEND_EQUATION = 0x8009;
    BLEND_EQUATION_RGB = 0x8009;   /* same as BLEND_EQUATION */
    BLEND_EQUATION_ALPHA = 0x883D;

    /* BlendSubtract */
    FUNC_SUBTRACT = 0x800A;
    FUNC_REVERSE_SUBTRACT = 0x800B;

    /* Separate Blend Functions */
    BLEND_DST_RGB = 0x80C8;
    BLEND_SRC_RGB = 0x80C9;
    BLEND_DST_ALPHA = 0x80CA;
    BLEND_SRC_ALPHA = 0x80CB;
    CONSTANT_COLOR = 0x8001;
    ONE_MINUS_CONSTANT_COLOR = 0x8002;
    CONSTANT_ALPHA = 0x8003;
    ONE_MINUS_CONSTANT_ALPHA = 0x8004;
    BLEND_COLOR = 0x8005;

    /* Buffer Objects */
    ARRAY_BUFFER = 0x8892;
    ELEMENT_ARRAY_BUFFER = 0x8893;
    ARRAY_BUFFER_BINDING = 0x8894;
    ELEMENT_ARRAY_BUFFER_BINDING = 0x8895;

    STREAM_DRAW = 0x88E0;
    STATIC_DRAW = 0x88E4;
    DYNAMIC_DRAW = 0x88E8;

    BUFFER_SIZE = 0x8764;
    BUFFER_USAGE = 0x8765;

    CURRENT_VERTEX_ATTRIB = 0x8626;

    /* CullFaceMode */
    FRONT = 0x0404;
    BACK = 0x0405;
    FRONT_AND_BACK = 0x0408;

    /* DepthFunction */
    /*      NEVER */
    /*      LESS */
    /*      EQUAL */
    /*      LEQUAL */
    /*      GREATER */
    /*      NOTEQUAL */
    /*      GEQUAL */
    /*      ALWAYS */

    /* EnableCap */
    /* TEXTURE_2D */
    CULL_FACE = 0x0B44;
    BLEND = 0x0BE2;
    DITHER = 0x0BD0;
    STENCIL_TEST = 0x0B90;
    DEPTH_TEST = 0x0B71;
    SCISSOR_TEST = 0x0C11;
    POLYGON_OFFSET_FILL = 0x8037;
    SAMPLE_ALPHA_TO_COVERAGE = 0x809E;
    SAMPLE_COVERAGE = 0x80A0;

    /* ErrorCode */
    //NO_ERROR                       = 0;
    INVALID_ENUM = 0x0500;
    INVALID_VALUE = 0x0501;
    INVALID_OPERATION = 0x0502;
    OUT_OF_MEMORY = 0x0505;

    /* FrontFaceDirection */
    CW = 0x0900;
    CCW = 0x0901;

    /* GetPName */
    LINE_WIDTH = 0x0B21;
    ALIASED_POINT_SIZE_RANGE = 0x846D;
    ALIASED_LINE_WIDTH_RANGE = 0x846E;
    CULL_FACE_MODE = 0x0B45;
    FRONT_FACE = 0x0B46;
    DEPTH_RANGE = 0x0B70;
    DEPTH_WRITEMASK = 0x0B72;
    DEPTH_CLEAR_VALUE = 0x0B73;
    DEPTH_FUNC = 0x0B74;
    STENCIL_CLEAR_VALUE = 0x0B91;
    STENCIL_FUNC = 0x0B92;
    STENCIL_FAIL = 0x0B94;
    STENCIL_PASS_DEPTH_FAIL = 0x0B95;
    STENCIL_PASS_DEPTH_PASS = 0x0B96;
    STENCIL_REF = 0x0B97;
    STENCIL_VALUE_MASK = 0x0B93;
    STENCIL_WRITEMASK = 0x0B98;
    STENCIL_BACK_FUNC = 0x8800;
    STENCIL_BACK_FAIL = 0x8801;
    STENCIL_BACK_PASS_DEPTH_FAIL = 0x8802;
    STENCIL_BACK_PASS_DEPTH_PASS = 0x8803;
    STENCIL_BACK_REF = 0x8CA3;
    STENCIL_BACK_VALUE_MASK = 0x8CA4;
    STENCIL_BACK_WRITEMASK = 0x8CA5;
    VIEWPORT = 0x0BA2;
    SCISSOR_BOX = 0x0C10;
    /*      SCISSOR_TEST */
    COLOR_CLEAR_VALUE = 0x0C22;
    COLOR_WRITEMASK = 0x0C23;
    UNPACK_ALIGNMENT = 0x0CF5;
    PACK_ALIGNMENT = 0x0D05;
    MAX_TEXTURE_SIZE = 0x0D33;
    MAX_VIEWPORT_DIMS = 0x0D3A;
    SUBPIXEL_BITS = 0x0D50;
    RED_BITS = 0x0D52;
    GREEN_BITS = 0x0D53;
    BLUE_BITS = 0x0D54;
    ALPHA_BITS = 0x0D55;
    DEPTH_BITS = 0x0D56;
    STENCIL_BITS = 0x0D57;
    POLYGON_OFFSET_UNITS = 0x2A00;
    /*      POLYGON_OFFSET_FILL */
    POLYGON_OFFSET_FACTOR = 0x8038;
    TEXTURE_BINDING_2D = 0x8069;
    SAMPLE_BUFFERS = 0x80A8;
    SAMPLES = 0x80A9;
    SAMPLE_COVERAGE_VALUE = 0x80AA;
    SAMPLE_COVERAGE_INVERT = 0x80AB;

    /* GetTextureParameter */
    /*      TEXTURE_MAG_FILTER */
    /*      TEXTURE_MIN_FILTER */
    /*      TEXTURE_WRAP_S */
    /*      TEXTURE_WRAP_T */

    COMPRESSED_TEXTURE_FORMATS = 0x86A3;

    /* HintMode */
    DONT_CARE = 0x1100;
    FASTEST = 0x1101;
    NICEST = 0x1102;

    /* HintTarget */
    GENERATE_MIPMAP_HINT = 0x8192;

    /* DataType */
    BYTE = 0x1400;
    UNSIGNED_BYTE = 0x1401;
    SHORT = 0x1402;
    UNSIGNED_SHORT = 0x1403;
    INT = 0x1404;
    UNSIGNED_INT = 0x1405;
    FLOAT = 0x1406;

    /* PixelFormat */
    DEPTH_COMPONENT = 0x1902;
    ALPHA = 0x1906;
    RGB = 0x1907;
    RGBA = 0x1908;
    LUMINANCE = 0x1909;
    LUMINANCE_ALPHA = 0x190A;

    /* PixelType */
    /*      UNSIGNED_BYTE */
    UNSIGNED_SHORT_4_4_4_4 = 0x8033;
    UNSIGNED_SHORT_5_5_5_1 = 0x8034;
    UNSIGNED_SHORT_5_6_5 = 0x8363;

    /* Shaders */
    FRAGMENT_SHADER = 0x8B30;
    VERTEX_SHADER = 0x8B31;
    MAX_VERTEX_ATTRIBS = 0x8869;
    MAX_VERTEX_UNIFORM_VECTORS = 0x8DFB;
    MAX_VARYING_VECTORS = 0x8DFC;
    MAX_COMBINED_TEXTURE_IMAGE_UNITS = 0x8B4D;
    MAX_VERTEX_TEXTURE_IMAGE_UNITS = 0x8B4C;
    MAX_TEXTURE_IMAGE_UNITS = 0x8872;
    MAX_FRAGMENT_UNIFORM_VECTORS = 0x8DFD;
    SHADER_TYPE = 0x8B4F;
    DELETE_STATUS = 0x8B80;
    LINK_STATUS = 0x8B82;
    VALIDATE_STATUS = 0x8B83;
    ATTACHED_SHADERS = 0x8B85;
    ACTIVE_UNIFORMS = 0x8B86;
    ACTIVE_ATTRIBUTES = 0x8B89;
    SHADING_LANGUAGE_VERSION = 0x8B8C;
    CURRENT_PROGRAM = 0x8B8D;

    /* StencilFunction */
    NEVER = 0x0200;
    LESS = 0x0201;
    EQUAL = 0x0202;
    LEQUAL = 0x0203;
    GREATER = 0x0204;
    NOTEQUAL = 0x0205;
    GEQUAL = 0x0206;
    ALWAYS = 0x0207;

    /* StencilOp */
    /*      ZERO */
    KEEP = 0x1E00;
    REPLACE = 0x1E01;
    INCR = 0x1E02;
    DECR = 0x1E03;
    INVERT = 0x150A;
    INCR_WRAP = 0x8507;
    DECR_WRAP = 0x8508;

    /* StringName */
    VENDOR = 0x1F00;
    RENDERER = 0x1F01;
    VERSION = 0x1F02;

    /* TextureMagFilter */
    NEAREST = 0x2600;
    LINEAR = 0x2601;

    /* TextureMinFilter */
    /*      NEAREST */
    /*      LINEAR */
    NEAREST_MIPMAP_NEAREST = 0x2700;
    LINEAR_MIPMAP_NEAREST = 0x2701;
    NEAREST_MIPMAP_LINEAR = 0x2702;
    LINEAR_MIPMAP_LINEAR = 0x2703;

    /* TextureParameterName */
    TEXTURE_MAG_FILTER = 0x2800;
    TEXTURE_MIN_FILTER = 0x2801;
    TEXTURE_WRAP_S = 0x2802;
    TEXTURE_WRAP_T = 0x2803;

    /* TextureTarget */
    TEXTURE_2D = 0x0DE1;
    TEXTURE = 0x1702;

    TEXTURE_CUBE_MAP = 0x8513;
    TEXTURE_BINDING_CUBE_MAP = 0x8514;
    TEXTURE_CUBE_MAP_POSITIVE_X = 0x8515;
    TEXTURE_CUBE_MAP_NEGATIVE_X = 0x8516;
    TEXTURE_CUBE_MAP_POSITIVE_Y = 0x8517;
    TEXTURE_CUBE_MAP_NEGATIVE_Y = 0x8518;
    TEXTURE_CUBE_MAP_POSITIVE_Z = 0x8519;
    TEXTURE_CUBE_MAP_NEGATIVE_Z = 0x851A;
    MAX_CUBE_MAP_TEXTURE_SIZE = 0x851C;

    /* TextureUnit */
    TEXTURE0 = 0x84C0;
    TEXTURE1 = 0x84C1;
    TEXTURE2 = 0x84C2;
    TEXTURE3 = 0x84C3;
    TEXTURE4 = 0x84C4;
    TEXTURE5 = 0x84C5;
    TEXTURE6 = 0x84C6;
    TEXTURE7 = 0x84C7;
    TEXTURE8 = 0x84C8;
    TEXTURE9 = 0x84C9;
    TEXTURE10 = 0x84CA;
    TEXTURE11 = 0x84CB;
    TEXTURE12 = 0x84CC;
    TEXTURE13 = 0x84CD;
    TEXTURE14 = 0x84CE;
    TEXTURE15 = 0x84CF;
    TEXTURE16 = 0x84D0;
    TEXTURE17 = 0x84D1;
    TEXTURE18 = 0x84D2;
    TEXTURE19 = 0x84D3;
    TEXTURE20 = 0x84D4;
    TEXTURE21 = 0x84D5;
    TEXTURE22 = 0x84D6;
    TEXTURE23 = 0x84D7;
    TEXTURE24 = 0x84D8;
    TEXTURE25 = 0x84D9;
    TEXTURE26 = 0x84DA;
    TEXTURE27 = 0x84DB;
    TEXTURE28 = 0x84DC;
    TEXTURE29 = 0x84DD;
    TEXTURE30 = 0x84DE;
    TEXTURE31 = 0x84DF;
    ACTIVE_TEXTURE = 0x84E0;

    /* TextureWrapMode */
    REPEAT = 0x2901;
    CLAMP_TO_EDGE = 0x812F;
    MIRRORED_REPEAT = 0x8370;

    /* Uniform Types */
    FLOAT_VEC2 = 0x8B50;
    FLOAT_VEC3 = 0x8B51;
    FLOAT_VEC4 = 0x8B52;
    INT_VEC2 = 0x8B53;
    INT_VEC3 = 0x8B54;
    INT_VEC4 = 0x8B55;
    BOOL = 0x8B56;
    BOOL_VEC2 = 0x8B57;
    BOOL_VEC3 = 0x8B58;
    BOOL_VEC4 = 0x8B59;
    FLOAT_MAT2 = 0x8B5A;
    FLOAT_MAT3 = 0x8B5B;
    FLOAT_MAT4 = 0x8B5C;
    SAMPLER_2D = 0x8B5E;
    SAMPLER_CUBE = 0x8B60;

    /* Vertex Arrays */
    VERTEX_ATTRIB_ARRAY_ENABLED = 0x8622;
    VERTEX_ATTRIB_ARRAY_SIZE = 0x8623;
    VERTEX_ATTRIB_ARRAY_STRIDE = 0x8624;
    VERTEX_ATTRIB_ARRAY_TYPE = 0x8625;
    VERTEX_ATTRIB_ARRAY_NORMALIZED = 0x886A;
    VERTEX_ATTRIB_ARRAY_POINTER = 0x8645;
    VERTEX_ATTRIB_ARRAY_BUFFER_BINDING = 0x889F;

    /* Read Format */
    IMPLEMENTATION_COLOR_READ_TYPE = 0x8B9A;
    IMPLEMENTATION_COLOR_READ_FORMAT = 0x8B9B;

    /* Shader Source */
    COMPILE_STATUS = 0x8B81;

    /* Shader Precision-Specified Types */
    LOW_FLOAT = 0x8DF0;
    MEDIUM_FLOAT = 0x8DF1;
    HIGH_FLOAT = 0x8DF2;
    LOW_INT = 0x8DF3;
    MEDIUM_INT = 0x8DF4;
    HIGH_INT = 0x8DF5;

    /* Framebuffer Object. */
    FRAMEBUFFER = 0x8D40;
    RENDERBUFFER = 0x8D41;

    RGBA4 = 0x8056;
    RGB5_A1 = 0x8057;
    RGB565 = 0x8D62;
    DEPTH_COMPONENT16 = 0x81A5;
    STENCIL_INDEX = 0x1901;
    STENCIL_INDEX8 = 0x8D48;
    DEPTH_STENCIL = 0x84F9;

    RENDERBUFFER_WIDTH = 0x8D42;
    RENDERBUFFER_HEIGHT = 0x8D43;
    RENDERBUFFER_INTERNAL_FORMAT = 0x8D44;
    RENDERBUFFER_RED_SIZE = 0x8D50;
    RENDERBUFFER_GREEN_SIZE = 0x8D51;
    RENDERBUFFER_BLUE_SIZE = 0x8D52;
    RENDERBUFFER_ALPHA_SIZE = 0x8D53;
    RENDERBUFFER_DEPTH_SIZE = 0x8D54;
    RENDERBUFFER_STENCIL_SIZE = 0x8D55;

    FRAMEBUFFER_ATTACHMENT_OBJECT_TYPE = 0x8CD0;
    FRAMEBUFFER_ATTACHMENT_OBJECT_NAME = 0x8CD1;
    FRAMEBUFFER_ATTACHMENT_TEXTURE_LEVEL = 0x8CD2;
    FRAMEBUFFER_ATTACHMENT_TEXTURE_CUBE_MAP_FACE = 0x8CD3;

    COLOR_ATTACHMENT0 = 0x8CE0;
    DEPTH_ATTACHMENT = 0x8D00;
    STENCIL_ATTACHMENT = 0x8D20;
    DEPTH_STENCIL_ATTACHMENT = 0x821A;

    NONE = 0;

    FRAMEBUFFER_COMPLETE = 0x8CD5;
    FRAMEBUFFER_INCOMPLETE_ATTACHMENT = 0x8CD6;
    FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT = 0x8CD7;
    FRAMEBUFFER_INCOMPLETE_DIMENSIONS = 0x8CD9;
    FRAMEBUFFER_UNSUPPORTED = 0x8CDD;

    FRAMEBUFFER_BINDING = 0x8CA6;
    RENDERBUFFER_BINDING = 0x8CA7;
    MAX_RENDERBUFFER_SIZE = 0x84E8;

    INVALID_FRAMEBUFFER_OPERATION = 0x0506;

    /* WebGL-specific enums */
    UNPACK_FLIP_Y_WEBGL = 0x9240;
    UNPACK_PREMULTIPLY_ALPHA_WEBGL = 0x9241;
    CONTEXT_LOST_WEBGL = 0x9242;
    UNPACK_COLORSPACE_CONVERSION_WEBGL = 0x9243;
    BROWSER_DEFAULT_WEBGL = 0x9244;

}

//var glperf = <{ frameend: () => void, init: (gl: WebGLRenderingContext) => void }>require('./webglPerf');


