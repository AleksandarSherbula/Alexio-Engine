#type vertex
#version 460 core
			
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vColor;
out vec2 vTexCoord;

layout (std140, binding = 0) uniform Camera
{
	mat4 projection;
};

void main()
{	
	vColor = aColor;
	vTexCoord = aTexCoord;
	gl_Position = projection * vec4(aPosition, 0.0, 1.0);
}

#type pixel
#version 460 core
			
out vec4 color;

in vec4 vColor;
in vec2 vTexCoord;

layout(binding = 0) uniform sampler2D uTexture;

void main()
{
	color = texture(uTexture, vTexCoord) * vec4(vColor);
}