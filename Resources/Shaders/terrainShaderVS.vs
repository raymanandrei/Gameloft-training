attribute vec3 a_posL;
attribute vec3 a_color;
attribute vec2 a_uv;
attribute vec2 a_uv2;

varying vec2 v_uv;
varying vec2 v_uv2;

varying vec3 v_color;

uniform mat4 MVP;

uniform sampler2D u_texture_3;//Blend
uniform vec3 u_height;

void main()
{
	vec4 c_blend = texture2D(u_texture_3,a_uv);
	vec4 posL = vec4(a_posL, 1.0);
	v_uv = a_uv;
	v_uv2 = a_uv2;

	vec4 pos_nou = vec4(a_posL,1.0);
	pos_nou.y += c_blend.r * u_height.r + c_blend.g * u_height.g + c_blend.b * u_height.b;

	gl_Position = MVP * pos_nou;
}