#type vertex
#version 450 core
			
layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec4 aColor;
layout (location = 2) in float aThickness;
layout (location = 3) in float aFade;

struct VertexOutput
{
	vec3  position;
	vec4  color;
	float thickness;
	float fade;
};

layout(location = 0) out VertexOutput outVertex;

layout (std140, binding = 0) uniform Camera
{
	mat4 projection;
};

void main()
{
	outVertex.position = aPosition;
	outVertex.color = aColor;
	outVertex.thickness = aThickness;
	outVertex.fade = aFade;
	gl_Position = projection * vec4(aPosition, 1.0);
}

#type pixel
#version 450 core
			
out vec4 pixelColor;

struct VertexOutput
{
	vec3  position;
	vec4  color;
	float thickness;
	float fade;
};

layout(location = 0) in VertexOutput inVertex;

layout(binding = 0) uniform sampler2D uTexture;

void main()
{
	//float distance = 1.0 - length(inVertex.position);
	//
	//vec3 color = vec3(smoothstep(0.0, inVertex.fade, distance));
	//color *= vec3(1.0 - smoothstep(inVertex.thickness, inVertex.thickness - inVertex.fade, distance));

	float distance = (1.0 - length(inVertex.position));	
	distance = step(0.0, distance); // if distance is greater than 0.0, make it 1.0

	pixelColor = vec4(distance, distance, distance, distance) * inVertex.color;
}