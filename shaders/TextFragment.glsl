//Fragment
#version 330 core

in vec2 UV;

out vec4 out_color;

uniform sampler2D TextureSampler;

void main()
{
	vec4 texel = texture(TextureSampler, UV);
	if (texel.x < 0.5 && texel.y < 0.5 && texel.z < 0.5)
		discard;
	out_color = vec4(1, 1, 1, 1);
}
