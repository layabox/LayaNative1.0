precision mediump float;
uniform sampler2D g_Tex0;
uniform sampler2D g_Tex1;
uniform float g_Alpha;
varying vec2 texCoord1;
varying vec2 texCoord2;
void main()
{
    vec4 color1 = texture2D(g_Tex0, texCoord1.xy);
    vec4 color2 = texture2D(g_Tex1, texCoord2.xy);//mask
    gl_FragColor=vec4(color1.xyz,color1.w*color2.r);
    gl_FragColor*=g_Alpha;
}

