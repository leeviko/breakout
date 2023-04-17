#pragma once

#include "GameObject.hpp"

class Paddle : public GameObject
{
public:
  glm::vec2 velocity;
  float speed;

  using GameObject::GameObject;

  void Reset();
  void Init(glm::vec2 pos, glm::vec2 size, glm::vec4 color);
  void Move(float dt);
  void CheckCollision();
};
