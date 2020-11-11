precision mediump float;
uniform sampler2D g_Tex0;
uniform vec4 u_color;       //rgb,strength
uniform vec4 u_blurInfo;   //blurw,blurh,texw,texh
uniform vec4 u_blur_off;    //blurx,blury,offx,offy
uniform float g_Alpha;
varying vec2 texCoord;

vec4 blur(){
    const float blurw = 10.0;
    const float floatIterationTotalTime = 100.0;
    vec4 vec4Color = vec4(0.);
    
    vec2 halfsz=u_blurInfo.xy/u_blurInfo.zw;    //参照as的：相当于最左偏移为 u_blurInfo.x个像素，即整个过滤框大小
    vec2 startpos=texCoord-halfsz-u_blur_off.zw/u_blurInfo.zw;  //原来的是/blurw 感觉应该不对
    vec2 ctexcoord = startpos;
    vec2 step = halfsz*2./blurw;  //参照as的：每次走几个像素，相当于每次走过滤框宽度的1/5
    
    for(float i = 0.0;i<=blurw; ++i){
        ctexcoord.x=startpos.x;
        for(float j = 0.0;j<=blurw; ++j){
            vec4Color += texture2D(g_Tex0, ctexcoord);
            ctexcoord.x+=step.x;
        }
        ctexcoord.y+=step.y;
    }
    return vec4Color/floatIterationTotalTime;
}

/*
 * as的代码
*/
vec4 blur1(){
    const float blurw = 10.0;
    const float floatIterationTotalTime = 100.0;
    
    vec4 vec4Color = vec4(0.0,0.0,0.0,0.0);
    vec2 vec2FilterDir =  u_blur_off.zw/u_blurInfo.zw;
    vec2 vec2FilterOff = vec2(u_blurInfo.x/u_blurInfo.z/blurw * 2.0,u_blurInfo.y/u_blurInfo.w/blurw * 2.0);
    vec2 vec2Off = vec2(0.0,0.0);
    float floatOff = blurw/2.0;
    for(float i = 0.0;i<=blurw; ++i){
        for(float j = 0.0;j<=blurw; ++j){
            vec2Off = vec2(vec2FilterOff.x * (i - floatOff),vec2FilterOff.y * (j - floatOff));
            vec4Color += texture2D(g_Tex0, texCoord - vec2FilterDir + vec2Off);
        }
    }
    return vec4Color/floatIterationTotalTime;
}

void main(){
   vec4 col = blur();
   gl_FragColor=vec4(u_color.rgb*col.a*u_color.w,col.a*u_color.w);
   gl_FragColor*=g_Alpha;
#ifdef TRANSPARENT_MODE
   gl_FragColor*=0.5;
#endif
}

