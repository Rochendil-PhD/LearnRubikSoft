//Vertex
#version 330 core

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

void main()
{
	gl_Position = vec4(in_position, 0, 1);
	UV = vertexUV;
}
