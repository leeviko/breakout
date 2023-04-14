#include "Brick.hpp"

void Brick::Init(glm::vec2 pos, glm::vec2 size, uint32_t lives, BrickType type)
{
  this->lives = lives;
  this->pos = pos;
  this->size = size;
  this->type = type;
}

void Brick::Draw()
{
  glm::vec4 color = {0.2f, 0.4f, 0.8f, 1.0f};

  if (lives > 1)
  {
    color = {0.8f, 0.4f, 0.2f, 1.0f};
  }

  if (type == ADD_BALL)
  {
    color = {0.4f, 0.8f, 0.2f, 1.0f};
  }
  this->color = color;

  renderer->DrawQuad(this->pos, this->size, this->color);
}
