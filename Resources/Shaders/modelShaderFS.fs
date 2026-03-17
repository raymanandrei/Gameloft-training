precision mediump float;

varying vec2 v_uv;

uniform sampler2D u_texture_0;

uniform samplerCube u_texture_1;

varying vec3 Vposition;

varying vec3 v_normW;

uniform vec3 cameraPosition;

uniform mat4 matrixModel;

uniform float smallR;

uniform float bigR;

uniform vec3 fogColor;	

void main()
{    
    vec3 vectCam = Vposition;

    vec3 dirReflect = reflect(normalize(vectCam),normalize(v_normW));

    vec4 culoare = textureCube(u_texture_1, dirReflect);

    float d = distance(cameraPosition,Vposition);

    float alpha;
    if (d <= smallR)
        alpha = 0.0;
    if (d >= bigR)
        alpha = 1.0;
    if (d > smallR && d < bigR)
        alpha = (d - smallR) / (bigR - smallR);

    vec4 colorTexture = texture2D(u_texture_0,v_uv);

    if (colorTexture.a < 0.1) {
        discard;
    }

    gl_FragColor =  vec4(alpha * fogColor + (1.0 - alpha) * (0.5 * colorTexture.xyz  + 0.5 * culoare.xyz),alpha);
}
