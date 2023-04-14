#pragma once
#include "MyMath.hpp"
#include "Renderer.hpp"

class GameObject
{
public:
  Renderer *renderer;
  glm::vec2 pos, size, velocity;
  glm::vec4 color;

  GameObject(Renderer &renderer);
  ~GameObject();

  void Init(glm::vec2 pos, glm::vec2 size, glm::vec4 color);
  void Draw();
};