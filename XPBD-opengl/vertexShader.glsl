#version 330 core

#define PI 3.1415926535

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 offset;
layout (location = 2) in vec2 scale;
layout (location = 3) in vec2 rot; // complex number for transformation
layout (location = 4) in vec4 color;

layout (location = 0) out vec4 outcolor;

uniform float VIEW_WIDTH;
uniform float VIEW_HEIGHT;
void main()
{
	vec2 pos = (vertexPos * scale);
	pos = vec2(pos.x*rot.x - pos.y*rot.y, pos.x*rot.y + pos.y*rot.x);
	pos += offset;
	pos.x /= VIEW_WIDTH;
	pos.y /= VIEW_HEIGHT;
	gl_Position = vec4(pos, 0.0f, 1.0f);
	outcolor = color;
}
