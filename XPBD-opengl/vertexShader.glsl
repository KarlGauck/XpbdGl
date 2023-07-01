#version 330 core

#define PI 3.1415926535

layout (location = 0) in vec2 vertexPos;
layout (location = 1) in vec2 offset;
layout (location = 2) in vec2 scale;
layout (location = 3) in float rotation;

uniform float VIEW_WIDTH;
uniform float VIEW_HEIGHT;
void main()
{
	float sinus = sin(rotation);
	float cosinus = cos(rotation);
	vec2 realVertexPos = vertexPos;
	realVertexPos.x = vertexPos.x * cosinus - vertexPos.y * sinus;
	realVertexPos.y = vertexPos.x * sinus + vertexPos.y * cosinus;

	vec2 pos = (realVertexPos * scale);
	pos += offset;
	pos.x /= VIEW_WIDTH;
	pos.y /= VIEW_HEIGHT;
	gl_Position = vec4(pos, 0.0f, 1.0f);
}
