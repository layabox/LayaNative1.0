
///<reference path="e:/tsdefination/node/node.d.ts" />
import * as fs from 'fs';

class ctx {
    FT_getExtension = 0;
    FT_createBuffer = 1;
    FT_bindBuffer = 2;
    FT_bufferData_vector = 3;
    FT_bufferData_size = 4;
    FT_createShader = 5;
    FT_shaderSource = 6;
    FT_compileShader = 7;
    FT_getShaderParameter = 8;
    FT_createProgram = 9;
    FT_attachShader = 10
    FT_linkProgram = 11;
    FT_getProgramParameter = 12;
    FT_useProgram = 13;
    FT_getUniformLocation = 14;
    FT_getAttribLocation = 15;
    FT_disable = 16;
    FT_enable = 17;
    FT_colorMask = 18;
    FT_viewport = 19;
    FT_clear = 20;
    FT_clearColor = 21;
    FT_createTexture = 22;
    FT_bindTexture = 23;
    FT_pixelStorei = 24;
    FT_texImage2D_06 = 25;
    FT_texImage2D_09 = 26;
    FT_texSubImage2D = 27;
    FT_texParameterf = 28;
    FT_texParameteri = 29;
    FT_activeTexture = 30;
    FT_getShaderInfoLog = 31;
    FT_uniform1f = 32;
    FT_uniform1i = 33;
    FT_uniform2f = 34;
    FT_uniform2i = 35;
    FT_uniform3f = 36;
    FT_uniform3i = 37;
    FT_uniform4f = 38;
    FT_uniform4i = 39;
    FT_uniform1fv = 40;
    FT_uniform1iv = 41;
    FT_uniform2fv = 42;
    FT_uniform2iv = 43;
    FT_uniform3fv = 44;
    FT_uniform3iv = 45;
    FT_uniform4fv = 46;
    FT_uniform4iv = 47;
    FT_uniformMatrix2fv = 48;
    FT_uniformMatrix3fv = 49;
    FT_uniformMatrix4fv = 50;
    FT_vertexAttribPointer = 51;
    FT_enableVertexAttribArray = 52;
    FT_blendFunc = 53;
    FT_bufferSubData = 54;
    FT_drawElements = 55;
    FT_deleteTexture = 56;
    FT_drawArrays = 57;
    FT_bindAttribLocation = 58;
    FT_cullFace = 59;
    FT_createFramebuffer = 60;
    FT_getParameter = 61;
    FT_bindFramebuffer = 62;
    FT_depthFunc = 63;
    FT_depthMask = 64;
    FT_depthRange = 65;
    FT_stencilFunc = 66;
    FT_stencilMask = 67;
    FT_stencilOp = 68;
    FT_deleteBuffer = 69;
    FT_deleteFramebuffer = 70;
    FT_deleteProgram = 71;
    FT_deleteRenderbuffer = 72;
    FT_deleteShader = 73;
    FT_isBuffer = 74;
    FT_disableVertexAttribArray = 75;
    FT_scissor = 76;
    FT_generateMipmap = 77;
    FT_framebufferTexture2D = 78;
    FT_checkFramebufferStatus = 79;
    FT__bindShaderFetchRT = 80;
    FT__renderMesh1RT = 81;
    FT_setTextureInvalidFunc = 82;
    FT_FINISH = 0xffffffff;
    
    TYPE_IMAGE = 0;
    TYPE_CANVAS = 1;
    TYPE_MEMORY_CANVAS = 2;
    TYPE_TEXT_CANVAS = 3;
    constructor(){}
}

var dt = new ctx();

var file = 'E:/conch4/trunk/Conch4/source/domsupport/WebGL.ts';
var fc = fs.readFileSync(file,'utf8');

for(var v in dt){
    console.log(v+','+dt[v]);
    fc = fc.replace(new RegExp('WebGLRenderingContext.'+v+'\\)','g'),`/*WebGLRenderingContext.${v}*/${dt[v]})`);
    fc = fc.replace(new RegExp('WebGLRenderingContext.'+v+'\\,','g'),`/*WebGLRenderingContext.${v}*/${dt[v]},`);
}

fs.writeFileSync('E:/conch4/trunk/Conch4/source/domsupport/WebGL1.ts',fc);



