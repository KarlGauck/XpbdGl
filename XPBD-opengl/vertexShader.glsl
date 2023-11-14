#version 330 core

#define PI 3.1415926535

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 offset;
layout (location = 2) in vec2 scale;
layout (location = 3) in vec2 rot; // complex number for transformation
layout (location = 4) in vec4 color;

layout (location = 0) out vec4 outcolor;

uniform vec2 viewportSize;
uniform vec2 viewportOffset;
uniform vec2 viewportRotation;

vec2 rotate(vec2 point, vec2 c)
{
	return vec2(point.x*c.x - point.y*c.y, point.x*c.y + point.y*c.x);
}

void main()
{
	vec2 pos = (vertexPos * scale);
	pos = rotate(pos, rot);
	pos += offset;
	pos = rotate(pos, viewportRotation);
	pos += viewportOffset;
	pos.x /= viewportSize.x;
	pos.y /= viewportSize.y;
	gl_Position = vec4(pos, 0.0f, 1.0f);
	outcolor = color;
}


