precision mediump float;
varying vec2 v_uv;
uniform sampler2D u_texture;
varying vec3 v_color;
void main()
{
    //vec4 color = texture2D(u_texture,v_uv);
    //if (color.a < 0.1) {
    //    discard;
    //}
    gl_FragColor = v_color;
}
