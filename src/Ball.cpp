#include "Ball.hpp"
#include <random>

void Ball::Init(glm::vec2 playerPos)
{
  std::random_device rd;
  std::mt19937 gen(rd());

  std::uniform_real_distribution<float> xPos(-0.5f, 0.5f);
  std::uniform_real_distribution<float> speed(250.0f, 500.0f);

  float randX = xPos(gen);

  this->speed = speed(gen);
  this->velocity = {randX, 1.0f};
  this->size = {25.0f, 25.0f};
  this->color = {1.0f, 1.0f, 1.0f, 1.0f};
  this->pos = {playerPos.x, playerPos.y - 250.0f};
}
void Ball::Move(float dt)
{
  glm::vec2 normalizedVel = glm::normalize(this->velocity);
  this->pos += normalizedVel * this->speed * dt;
}

bool Ball::CheckCollision(const Paddle &paddle, uint32_t &lives)
{
  glm::vec2 borderNormal = {0.0f, 0.0f};
  glm::vec2 reflectedVel;

  float ballLeft = pos.x - size.x / 2.0f;

  if (ballLeft < 0.0f)
  {
    borderNormal = {-1.0f, 0.0f};
  }
  else if (ballLeft + size.x > winWidth)
  {
    borderNormal = {1.0f, 0.0f};
  }

  else if (pos.y < 0.0f)
  {
    borderNormal = {0.0f, 1.0f};
  }
  else if (pos.y > winHeight)
  {
    return true;
  }

  float paddleLeft = paddle.pos.x - paddle.size.x / 2.0f;

  bool collisionX = ballLeft + size.x >= paddleLeft && paddleLeft + paddle.size.x >= ballLeft;
  bool collisionY = pos.y + size.y >= paddle.pos.y && paddle.pos.y + paddle.size.y >= pos.y;

  if (collisionX && collisionY)
  {
    borderNormal = {0.0f, -1.0f};
  }

  if (borderNormal.x == 0.0f && borderNormal.y == 0.0f)
  {
    return false;
  }

  reflectedVel = glm::reflect(this->velocity, borderNormal);

  this->pos += reflectedVel;
  this->velocity = reflectedVel;
  return false;
}

void Ball::CheckCollision(Brick &brick)
{
  float brickLeft = brick.pos.x - brick.size.x / 2.0f;
  float ballLeft = pos.x - size.x / 2.0f;

  bool collisionX = ballLeft + size.x >= brickLeft && brickLeft + brick.size.x >= ballLeft;
  bool collisionY = pos.y + size.y >= brick.pos.y && brick.pos.y + brick.size.y >= pos.y;
  if (collisionX && collisionY)
  {
    if (brick.lives > 0)
    {
      brick.lives--;
    }

    if (brick.lives == 0)
    {
      return;
    }

    glm::vec2 reflectedVel = glm::reflect(this->velocity, {0.0f, 1.0f});

    this->pos += reflectedVel;
    this->velocity = reflectedVel;
  }
}
