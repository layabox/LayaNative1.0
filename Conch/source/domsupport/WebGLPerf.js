'use strict';
class _WebGLTexture {

}

class _WebGLCtx {
    constructor(glperf) {
        this.glperf = glperf;
        this.textures = [];
        this.textureSlot = new Map();
        this.activetex = null;
    }

    //texture
    activeTexture(texture) {
        this.activetex = texture;
    }
    bindTexture(target, texture) {
        this.textureSlot.set(target, texture);
    }
    compressedTexImage2D(target, level, internalformat, width, height, border, data) {
        alert('compressedTexImage2D');
    }
    compressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, data) {
        alert('compressedTexSubImage2D');
    }
    copyTexImage2D(target, level, internalformat, x, y, width, height, border) {
        alert('copyTexImage2D');
    }
    copyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height) {
        alert('copyTexSubImage2D');
    }
    createTexture() {
        var tex = new _WebGLTexture();
        this.textures.push(tex);
        return tex;
    }
    deleteTexture(texture) {
        alert('deleteTexture');
        var p = this.textures.indexOf(texture);
        if (p >= 0) {
            this.textures.splice(p, 1);
        }
    }
    getTexParameter(target, pname) {

    }
    texImage2D() {
        if (arguments.length > 6)
            this.texImage2D_9.apply(this, arguments);
        else
            this.texImage2D_6.apply(this, arguments);
    }
    texImage2D_9(target, level, internalformat, width, height, border, format, type, pixels) {
        alert('texImage2D pixel');
    }
    texImage2D_6(target, level, internalformat, format, type, obj) {
        //HTMLImageElement, HTMLCanvasElement, HTMLVideoElement, ImageData
        //if (obj instanceof HTMLImageElement) {
        //
        //} else {
        //debugger;
        //}
    }
    texParameterf(target, pname, param) {

    }
    texParameteri(target, pname, param) {

    }
    texSubImage2D() {
        if (arguments.length > 6)
            this.texSubImage2D_9.apply(this, arguments);
        else
            this.texSubImage2D_6.apply(this, arguments);
    }
    texSubImage2D_9(target, level, xoffset, yoffset, width, height, format, type, pixels) {
        this.glperf.texSubImgNum++;
    }
    texSubImage2D_6(target, level, xoffset, yoffset, format, type, obj) {
        //HTMLImageElement, HTMLCanvasElement, HTMLVideoElement, ImageData
        this.glperf.texSubImgNum++;
    }

    drawElements(mode, count, type, offset) {
        this.glperf.drawnum++;
    }
}

class WebglPerf {
    constructor() {
        this.curfrm = 0;
        this.drawnum = 0;
        this.texImgNum = 0;
        this.texSubImgNum = 0;
        this.mygl = new _WebGLCtx(this);
    }

    init(gl) {
        //注意，这里要考虑是class还是function，如果是class则需要__proto__
        var ps = Object.getOwnPropertyNames(gl.__proto__);
        ps.forEach((v, i, arr) => {
            var mygl = this.mygl;
            if (typeof (gl[v]) == 'function' && v != 'constructor') {
                var old = gl[v];
                //上面定义了就要先调用
                /*
                if(this.mygl[v]){
                    gl[v] = function(){
                        var r = mygl[v].apply(mygl,arguments);
                        var rr = old.apply(gl,arguments);
                        //如果有返回值要调用
                        if(r) return rr;
                    };
                }
                */
                //全部加上打印
                if (v != 'wf32' && v != 'wu32' && v != 'wu16' && v != 'needsz') {
                    gl[v] = function () {
                        console.log('function:' + v);
                        var rr = old.apply(gl, arguments);
                        //如果有返回值要调用
                        return rr;
                    };
                }
            }
        });
    }
    frameend() {
        this.report();
        this.drawnum = 0;
        this.texImgNum = 0;
        this.texSubImgNum = 0;
    }
    report() {
        console.log('=================================================================');
        console.log(`${this.curfrm++}
        drawcall:${this.drawnum} 
        texnum:${this.mygl.textures.length}
        texsubImg:${this.texSubImgNum}
        `);
    }
}

exports = new WebglPerf();
