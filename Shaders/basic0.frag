#version 330 core

in vec2 frag;

#define PW 1.0/SW

uniform sampler2D tex;
uniform float SW;
uniform float SH;
uniform float dT;

uniform int Init;

out vec4 FragColor;

float fRules(float f)
{
	if(f == 100.0 || f == 11.0 || f == 10.0 || f == 1.0) return 1.0;
	return 0.0;

}

float fCellularAutomata()
{
	if(gl_FragCoord.x == 0.5 || gl_FragCoord.x == SW-0.5) return 0.0;
	float result = fRules(texture2D(tex, vec2(frag.x-PW, frag.y)).r + texture2D(tex, frag).r + texture2D(tex, vec2(frag.x+PW, frag.y)).r);
	return result;
}

void main()
{
	vec2 speed = vec2(0.1, 0.0);
	vec4 c0 = texture2D(tex, vec2(gl_FragCoord.x/SW, (gl_FragCoord.y-1)/SH));
	if(gl_FragCoord.y == 0.5)
	{
		if(Init == 0)
		{
			if (gl_FragCoord.r == SW/2.0 + 0.5) c0 = vec4(1.0);
			//if (gl_FragCoord.r == 0.5) c0 = vec4(1.0);
		}
		else
		{
			c0 = vec4(fCellularAutomata());
		}
	}

	gl_FragColor = c0;
}