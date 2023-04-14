#include "Ball.hpp"
#include <random>

void Ball::Init(glm::vec2 playerPos, uint32_t winWidth, uint32_t winHeight)
{
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<float> xPos(-1.0f, 1.0f);
  std::uniform_real_distribution<float> speed(200.0f, 600.0f);

  float randX = xPos(gen);

  this->winWidth = winWidth;
  this->winHeight = winHeight;
  this->speed = speed(gen);
  this->velocity = {randX, 1.0f};
  this->size = {25.0f, 25.0f};
  this->color = {1.0f, 1.0f, 1.0f, 1.0f};
  this->pos = {playerPos.x, playerPos.y - 200.0f};
}
void Ball::Move(float dt)
{
  glm::vec2 normalizedVel = glm::normalize(this->velocity);

  this->pos += normalizedVel * this->speed * dt;
}

void Ball::CheckCollision(const GameObject &paddle)
{
  glm::vec2 borderNormal = {0.0f, 0.0f};
  glm::vec2 reflectedVel;

  if (pos.x < 0.0f)
  {
    borderNormal = {-1.0f, 0.0f};
  }
  else if (pos.x + size.x > winWidth)
  {
    borderNormal = {1.0f, 0.0f};
  }

  else if (pos.y < 0.0f)
  {
    borderNormal = {0.0f, 1.0f};
  }

  float left = paddle.pos.x - paddle.size.x / 2;

  bool collisionX = pos.x + size.x >= left && left + paddle.size.x >= pos.x;
  bool collisionY = pos.y + size.y >= paddle.pos.y && paddle.pos.y + paddle.size.y >= pos.y;

  if (collisionX && collisionY)
  {
    borderNormal = {0.0f, -1.0f};
  }

  if (borderNormal.x == 0.0f && borderNormal.y == 0.0f)
  {
    return;
  }

  reflectedVel = glm::reflect(this->velocity, borderNormal);

  this->pos += reflectedVel;
  this->velocity = reflectedVel;
}
