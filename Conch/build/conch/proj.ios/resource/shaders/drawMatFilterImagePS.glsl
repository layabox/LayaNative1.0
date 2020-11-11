precision mediump float;
uniform sampler2D g_Tex0;
uniform mat4 g_colorMat;
uniform vec4 g_colorAlpha;
uniform float g_Alpha;
varying vec2 texCoord;
void main(){
   gl_FragColor = texture2D(g_Tex0, texCoord.xy);
   gl_FragColor *= g_Alpha;
   mat4 alphaMat = g_colorMat;
   alphaMat[0][3] *= gl_FragColor.a;
   alphaMat[1][3] *= gl_FragColor.a;
   alphaMat[2][3] *= gl_FragColor.a;
   gl_FragColor =  gl_FragColor * alphaMat;
   gl_FragColor += g_colorAlpha/255.0 * gl_FragColor.a;
#ifdef TRANSPARENT_MODE
   gl_FragColor*=0.5;
#endif
}