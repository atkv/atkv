#version 330 core

layout(location = 0) in vec3 vertex_pos;
layout(location = 1) in vec3 vertex_col;

uniform mat4 P;
uniform mat4 M;
uniform mat4 V;

out vec3 color;

void main(){
  gl_Position = P * V * M * vec4(vertex_pos,1.0);
  color       = vertex_col;
}
