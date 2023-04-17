#include "Paddle.hpp"

void Paddle::Init(glm::vec2 pos, glm::vec2 size, glm::vec4 color)
{
  this->speed = 1000.0f;
  this->pos = pos;
  this->size = size;
  this->color = color;
}

void Paddle::Reset()
{
  this->speed = 1000.0f;
  this->velocity = {0.0f, 0.0f};
  this->pos = {winWidth / 2.0f, winHeight - size.y * 2.0f};
}

void Paddle::Move(float dt)
{
  this->pos += this->velocity * this->speed * dt;
}

void Paddle::CheckCollision()
{
  if (this->pos.x <= 0.0f + this->size.x / 2.0f)
  {
    this->pos.x = 0.0f + this->size.x / 2.0f;
  }
  else if (this->pos.x >= winWidth - this->size.x / 2.0f)
  {
    this->pos.x = winWidth - this->size.x / 2.0f;
  }
}
