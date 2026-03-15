precision mediump float;

varying vec3 v_color;

varying vec3 Vposition;

uniform vec3 cameraPosition;

uniform float smallR;

uniform float bigR;

uniform vec3 fogColor;	

void main()
{
    float d = distance(cameraPosition,Vposition);

    float alpha;
    if (d <= smallR)
        alpha = 0.0;
    if (d >= bigR)
        alpha = 1.0;
    if (d > smallR && d < bigR)
        alpha = (d - smallR) / (bigR - smallR);

    gl_FragColor=vec4(alpha * fogColor + (1.0 - alpha) * v_color , alpha);
}
