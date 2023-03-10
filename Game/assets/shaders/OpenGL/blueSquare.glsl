#type vertex
#version 450 core
			
layout(location = 0) in vec2 aPosition;

uniform mat4 uModel;
uniform mat4 uProjection;

void main()
{				
	gl_Position = uProjection * uModel * vec4(aPosition, -1.0, 1.0);
}

#type pixel
#version 450 core
			
out vec4 color;
uniform vec4 uColor;

void main()
{
	color = vec4(uColor);
}