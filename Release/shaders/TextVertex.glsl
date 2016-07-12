//Vertex
#version 330 core

layout(location = 0) in vec2 in_position;
layout(location = 1) in vec2 vertexUV;

out vec2 UV;

void main()
{
	//vec2 vertexPosition_homoneneousspace = in_position - vec2(600, 400); // [0..800][0..600] -> [-400..400][-300..300]
	//vertexPosition_homoneneousspace /= vec2(600, 400);
	//gl_Position = vec4(vertexPosition_homoneneousspace, 0, 1);
	gl_Position = vec4(in_position, 0, 1);
	UV = vertexUV;
}
