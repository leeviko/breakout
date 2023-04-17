#pragma once
#include "GameObject.hpp"
#include "Paddle.hpp"
#include "Brick.hpp"

class Ball : public GameObject
{
public:
  float speed;

  using GameObject::GameObject;

  void Init(glm::vec2 playerPos);
  void Move(float dt);

  bool CheckCollision(const Paddle &paddle, uint32_t &lives);
  void CheckCollision(Brick &brick);
};
