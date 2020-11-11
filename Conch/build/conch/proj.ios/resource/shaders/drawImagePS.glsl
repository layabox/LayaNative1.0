precision mediump float;
uniform sampler2D g_Tex0;
uniform float g_Alpha;
varying vec2 texCoord;
void main()
{
   gl_FragColor=texture2D(g_Tex0, texCoord.xy);
   gl_FragColor*=g_Alpha;
#ifdef TRANSPARENT_MODE
   gl_FragColor*=0.5;
#endif
}

