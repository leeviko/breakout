#version 330 core
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec2 a_TexCoords;
layout (location = 2) in float a_TexIndex;

out vec2 v_TexCoords;
out float v_TexIndex;

uniform mat4 proj;

void main()
{
  v_TexCoords = a_TexCoords;
  v_TexIndex = a_TexIndex;
  gl_Position = proj * vec4(a_Pos, 1.0);
}
