#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec3 vertexColor;
in layout(location = 2) mat4 transformationMatrix;

out vec3 theColor;
void main()
{
	vec4 p = vec4(position, 1.0);
	gl_Position = transformationMatrix*p;
	theColor = vertexColor;
}