attribute vec3 a_posL;
attribute vec3 a_color;
varying vec3 v_color;

void main()
{
    // Eroarea era aici: trebuia să atribui valoarea lui gl_Position
    gl_Position = vec4(a_posL, 1.0);
    
    // Setezi culoarea care va fi trimisă către Fragment Shader
    v_color = vec3(0.0, 0.0, 1.0);
}