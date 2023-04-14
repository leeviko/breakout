#pragma once
#include "GameObject.hpp"

enum BrickType
{
  REGULAR,
  ADD_BALL
};

class Brick : public GameObject
{
public:
  uint32_t lives;
  BrickType type;

  using GameObject::GameObject;

  void Init(glm::vec2 pos, glm::vec2 size, uint32_t lives, BrickType type = REGULAR);
  void Draw();
};
