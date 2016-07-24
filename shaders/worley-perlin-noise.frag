#version 150 core

uniform float iGlobalTime;
uniform vec3  iResolution;
uniform vec4  iMouse;
uniform int   iFrame;

in vec2 fragCoord;
out vec4 fragColor;

// copy from https://www.shadertoy.com/view/4l2GzW
float r(float n)
{
 	return fract(cos(n*89.42)*343.42);
}
vec2 r(vec2 n)
{
 	return vec2(r(n.x*23.62-300.0+n.y*34.35),r(n.x*45.13+256.0+n.y*38.89)); 
}
float worley(vec2 n,float s)
{
    float dis = 2.0;
    for(int x = -1;x<=1;x++)
    {
        for(int y = -1;y<=1;y++)
        {
            vec2 p = floor(n/s)+vec2(x,y);
            float d = length(r(p)+vec2(x,y)-fract(n/s));
            if (dis>d)
            {
             	dis = d;   
            }
        }
    }
    return 1.0 - dis;
	
}

// copy from https://www.shadertoy.com/view/4sc3z2

#define MOD3 vec3(.1031,.11369,.13787)

vec3 hash33(vec3 p3)
{
	p3 = fract(p3 * MOD3);
    p3 += dot(p3, p3.yxz+19.19);
    return -1.0 + 2.0 * fract(vec3((p3.x + p3.y)*p3.z, (p3.x+p3.z)*p3.y, (p3.y+p3.z)*p3.x));
}
float perlin_noise(vec3 p)
{
    vec3 pi = floor(p);
    vec3 pf = p - pi;
    
    vec3 w = pf * pf * (3.0 - 2.0 * pf);
    
    return 	mix(
        		mix(
                	mix(dot(pf - vec3(0, 0, 0), hash33(pi + vec3(0, 0, 0))), 
                        dot(pf - vec3(1, 0, 0), hash33(pi + vec3(1, 0, 0))),
                       	w.x),
                	mix(dot(pf - vec3(0, 0, 1), hash33(pi + vec3(0, 0, 1))), 
                        dot(pf - vec3(1, 0, 1), hash33(pi + vec3(1, 0, 1))),
                       	w.x),
                	w.z),
        		mix(
                    mix(dot(pf - vec3(0, 1, 0), hash33(pi + vec3(0, 1, 0))), 
                        dot(pf - vec3(1, 1, 0), hash33(pi + vec3(1, 1, 0))),
                       	w.x),
                   	mix(dot(pf - vec3(0, 1, 1), hash33(pi + vec3(0, 1, 1))), 
                        dot(pf - vec3(1, 1, 1), hash33(pi + vec3(1, 1, 1))),
                       	w.x),
                	w.z),
    			w.y);
}
void main()
{
    
    float dis = (1.0+perlin_noise(vec3(fragCoord.xy/iResolution.xy, iGlobalTime*0.05)*8.0)) 
        * (1.0+(worley(fragCoord.xy, 32.0)+
        0.5*worley(2.0*fragCoord.xy,32.0) +
        0.25*worley(4.0*fragCoord.xy,32.0) ));
	fragColor = vec4(vec3(dis/4.0),1.0);
}
