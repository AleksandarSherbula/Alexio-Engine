#type vertex
#version 460 core
			
layout (location = 0) in vec2 aPosition;
layout (location = 1) in vec4 aColor;

out vec4 vColor;
uniform mat4 uModel;
uniform mat4 uProjection;

layout (std140, binding = 0) uniform Camera
{
	mat4 projection;
};

void main()
{	
	vColor = aColor;
	gl_Position = projection * vec4(aPosition, 0.0, 1.0);
}

#type pixel
#version 460 core
			
out vec4 color;

in vec4 vColor;

void main()
{
	color = vColor;
}