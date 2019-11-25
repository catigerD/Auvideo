#version 300 es

layout(location = 0) in vec4 vertexcoord;
layout(location = 1) in vec4 texcoord;
out vec4 v_texcoord;
void main(){
    gl_Position = vertexcoord;
    v_texcoord = texcoord;
}