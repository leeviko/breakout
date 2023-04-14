#pragma once
#include "GameObject.hpp"
#include "Brick.hpp"

class Ball : public GameObject
{
public:
  float speed;
  uint32_t winWidth, winHeight;

  using GameObject::GameObject;

  void Init(glm::vec2 playerPos, uint32_t winWidth, uint32_t winHeight);
  void Move(float dt);

  void CheckCollision(const GameObject &paddle);
  void CheckCollision(Brick &brick);
};
