//void main() {
//	gl_FragColor = vec4(gl_FragCoord.z,gl_FragCoord.z,gl_FragCoord.z,1.0);
//	//gl_FragColor = vec4(1.0,0.0,0.0,1.0);
//}

uniform float fNear;
uniform float fFar;

void main()
{
    float fDepth = gl_FragCoord.z / gl_FragCoord.w;
    float fColor = 1.0 - smoothstep(fNear, fFar, fDepth);
    gl_FragColor = vec4(vec3(fColor), 1.0);
}