#type vertex
#version 450 core
			
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

struct VertexOutput
{
	vec4 color;
	vec2 texCoord;
};

layout(location = 0) out VertexOutput outVertex;

layout (std140, binding = 0) uniform Camera
{
	mat4 projection;
};

void main()
{	
	outVertex.color = aColor;
	outVertex.texCoord = aTexCoord;
	gl_Position = projection * vec4(aPosition, 1.0);
}

#type pixel
#version 450 core
			
out vec4 pixelColor;

struct VertexOutput
{
	vec4 color;
	vec2 texCoord;
};

layout(location = 0) in VertexOutput inVertex;

layout(binding = 0) uniform sampler2D uTexture;

void main()
{
	pixelColor = texture(uTexture, inVertex.texCoord) * vec4(inVertex.color);
}