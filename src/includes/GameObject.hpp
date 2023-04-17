#pragma once
#include "MyMath.hpp"
#include "Renderer.hpp"

class GameObject
{
public:
  Renderer *renderer;
  uint32_t winWidth, winHeight;
  glm::vec2 pos, size, velocity;
  glm::vec4 color;

  GameObject(Renderer &renderer, uint32_t winWidth, uint32_t winHeight);
  ~GameObject();

  void Init(glm::vec2 pos, glm::vec2 size, glm::vec4 color);
  void Draw();
};