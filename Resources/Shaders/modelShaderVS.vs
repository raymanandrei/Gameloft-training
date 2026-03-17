precision mediump float;

attribute vec3 a_posL;

attribute vec3 a_color;

varying vec3 v_color;

attribute vec2 a_uv;

varying vec2 v_uv;

uniform mat4 MVP;

uniform mat4 matrixModel;

varying vec3 Vposition;

varying vec3 v_normW;

void main()
{
	vec4 posL = vec4(a_posL, 1.0);

	Vposition = (matrixModel * posL).xyz;

	v_normW = normalize(Vposition);

	v_color = a_color;

	v_uv = a_uv;
	
	gl_Position = MVP * posL;
}
   