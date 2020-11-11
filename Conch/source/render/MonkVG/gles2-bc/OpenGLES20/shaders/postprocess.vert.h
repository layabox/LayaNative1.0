unsigned char postprocess_vert[] = R"shi(


// Attributes
attribute vec4 a_position;
attribute vec2 a_texCoord;

// Varyings
varying vec2 v_texCoord;

void main() 
{
	gl_Position = u_modelViewProjectionMatrix * a_position;
	v_texCoord = a_texCoord;	
}  
)shi";
unsigned int postprocess_vert_len = 209;
