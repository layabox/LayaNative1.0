precision mediump float;
uniform float g_Alpha;
uniform sampler2D g_Tex0;
uniform vec4 g_TexRange;
varying vec2 texCoord;
void main()
{
   vec2 newTexCoord;
   newTexCoord.x = mod((texCoord.x-g_TexRange.x),g_TexRange.y)+g_TexRange.x;
   newTexCoord.y = mod((texCoord.y-g_TexRange.z),g_TexRange.w)+g_TexRange.z;
   gl_FragColor = texture2D(g_Tex0, newTexCoord);
   gl_FragColor*=g_Alpha;
#ifdef TRANSPARENT_MODE
   gl_FragColor*=0.5;
#endif
}