#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

const float amplitude = 2.0;
const float frequency = 4.0;
const float PI = 3.14159;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
  float distance = length(aPos);
  float effect = amplitude*sin(-PI*frequency+time);
 	if (effect>=0)
	{
		 gl_Position = projection*view*model*vec4(aPos.x+(32*effect),aPos.y+(effect*8), aPos.z+(effect*0.5),1);
	}
 if (effect<0)
	{
		 gl_Position = projection*view*model*vec4(aPos.x+(32*effect),aPos.y-(effect*8), aPos.z+(effect*0.5),1);
	}
  TexCoords=vec2(aTexCoords.x,aTexCoords.y);
}