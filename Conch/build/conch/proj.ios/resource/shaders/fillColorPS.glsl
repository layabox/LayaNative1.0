precision mediump float;
uniform float g_Alpha;
varying vec4 oColor;
void main()
{
   gl_FragColor=vec4(oColor.xyz*g_Alpha,g_Alpha);
#ifdef TRANSPARENT_MODE
   gl_FragColor*=0.5;
#endif
}

