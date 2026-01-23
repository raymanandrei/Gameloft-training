precision mediump float;
varying vec2 v_uv;
uniform sampler2D u_texture;
void main()
{
    vec4 color = texture2D(u_texture,v_uv);
   // gl_FragColor = color;
    if (color.a < 0.1) {
        discard;
    }
    gl_FragColor =vec4(v_uv, 0.0, 1.0);
}
