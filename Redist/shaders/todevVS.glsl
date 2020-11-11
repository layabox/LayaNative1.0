attribute vec2 position;
attribute vec2 texcoord;
attribute vec2 texcoord2;
attribute vec4 color;
uniform vec4 g_DevInfo;
uniform mat4 g_WorldMat;
varying vec2 texCoord;
varying vec2 texCoord2;
varying vec4 oColor;
void main()
{
    vec4 tPos = vec4( position.x,position.y,0,1 );
    tPos = g_WorldMat * tPos;
    float cx = (tPos.x*2.0/g_DevInfo.x)-1.0;
    float cy = -(tPos.y*2.0/g_DevInfo.y)+1.0;
    gl_Position = vec4(cx, cy, 0.5, 1.0);
    texCoord.xy = texcoord;
    texCoord2 = texcoord2;
    oColor = color;
}
