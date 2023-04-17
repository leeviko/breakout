#version 330 core
layout (location = 0) out vec4 Color;

in vec2 v_TexCoords;
in float v_TexIndex;

uniform sampler2D u_Textures[32];

void main()
{
  int index = int(v_TexIndex);
  
  vec4 sampled = vec4(1.0, 1.0, 1.0, texture(u_Textures[index], v_TexCoords).r);

  Color = sampled;
}
