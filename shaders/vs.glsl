// NOTE - Drawn VERY heavily from by Dr. Patterson's code for the Tech Art/Direction course. It is a very 
// simple shader though, so there wasn't much to change. 

#version 410 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 object_color;

out vec3 vertexColor;

void main()
{
    vec4 view_position = view * model * vec4(position, 1.0);
    gl_Position = projection * view_position;
    vertexColor = object_color;
}

