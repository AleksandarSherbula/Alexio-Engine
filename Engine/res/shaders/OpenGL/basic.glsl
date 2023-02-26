#type vertex
#version 450 core
			
layout(location = 0) in vec2 aPosition;
layout(location = 1) in vec4 aColor;

out vec2 vPosition;
out vec4 vColor;

void main()
{
	vPosition = aPosition;
	vColor = aColor;
	gl_Position = vec4(aPosition, 0.0, 1.0);
}

#type pixel
#version 450 core
			
layout(location = 0) out vec4 color;

in vec2 vPosition;
in vec4 vColor;

void main()
{
	color = vColor;
}