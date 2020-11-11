precision mediump float;
uniform sampler2D g_Tex0;
uniform vec4 g_Filter;     //x 1现在没用, y 1-饱和度 z 饱和度/3
uniform vec4 g_TexRange;   //r,g,b表示rgb的亮度调整，w表示饱和度的调整。w从-1到1（0到1？）
uniform float g_Alpha;
varying vec2 texCoord;
void main()
{
   vec4 color = texture2D(g_Tex0, texCoord.xy);
   float avg = dot(color.xyz, g_Filter.zzz);
   color.rgb = (avg + g_TexRange.xyz*color.rgb*g_Filter.y ); 
   color.a *= g_Filter.x;
   gl_FragColor = color;
   gl_FragColor *= g_Alpha;
#ifdef TRANSPARENT_MODE
   gl_FragColor*=0.5;
#endif
}