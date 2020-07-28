#Shader Vertex
#version 430 core
layout(location = 0) in vec2 a_Position;

void main()
{
	gl_Position = vec4(a_Position, 0.0, 1.0);
}

#Shader Fragment
#version 430 core
precision highp float;
out vec4 fragment;
uniform float u_Scale;
uniform vec2 u_Offset;
uniform vec2 u_WindowSize;
uniform float u_Degree;

const int iterations = 100;

vec2 complex_mul(vec2 z1, vec2 z2)
{
    float real = z1.x * z2.x - z1.y * z2.y;
    float imaginary = z1.y * z2.x + z1.x * z2.y;
    return vec2(real, imaginary);
}

vec2 complex_exp(vec2 z, float exp)
{
    float mag = sqrt(dot(z, z));
    float theta = atan(z.y, z.x);
    mag = pow(mag, exp);
    theta *= exp;
    return vec2(mag * cos(theta), mag * sin(theta));
}

void main()
{
    vec2 c = u_Scale * (gl_FragCoord.xy - 0.5 * u_WindowSize) + u_Offset;
    fragment = vec4(0.0, 0.0, 0.0, 1.0);

    vec2 p = vec2(0.0, 0.0);
    for (int i = 0; i < iterations; i++)
    {
        p = complex_exp(p, u_Degree) + c;

        if (dot(p, p) > 4.0)
        {
			//This is a coloring algorithm I found to be appealing. Written in HSV, many functions will work.
            float colorRegulator = float(i-1)-log(((log(dot(p,p)))/log(2.0)))/log(2.0);
			vec3 color = vec3(0.95 + .012*colorRegulator , 1.0, .2+.4*(1.0+sin(.3*colorRegulator)));
            vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	        vec3 m = abs(fract(color.xxx + K.xyz) * 6.0 - K.www);
	        fragment.rgb = color.z * mix(K.xxx, clamp(m - K.xxx, 0.0, 1.0), color.y);
            break;
        }
    }
}