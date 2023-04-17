#include "Game.hpp"
#include "ResourceManager.hpp"
#include "GameObject.hpp"
#include <GLFW/glfw3.h>
#include <random>
#include "TextRenderer.hpp"

Game::Game(uint32_t width, uint32_t height) : Width(width), Height(height), paddle(Paddle(renderer, width, height)), state(GAME_MENU), lives(1)
{
}
Game::~Game()
{
  ResourceManager::Clear();
  renderer.Clear();
}

void ResetBricks(Renderer &renderer, uint32_t width, uint32_t height, float brickWidth, float brickHeight, std::vector<Brick> &bricks)
{
  bricks.clear();

  float brickPadding = 15.0f;
  float borderPadding = 50.0f;

  uint32_t rows = 8;
  uint32_t columns = 10;

  glm::vec2 totalBrickSize = glm::vec2(brickWidth, brickHeight) + glm::vec2(brickPadding, brickPadding);
  glm::vec2 boardStartPos = {width - (columns * totalBrickSize.x), brickHeight + 25.0f};

  for (uint32_t i = 0; i < rows; i++)
  {
    uint32_t brickLives = i % 2 == 0 ? 1 : 2;

    for (uint32_t j = 0; j < columns; j++)
    {
      Brick brick(renderer, width, height);
      brick.Init({boardStartPos.x + (j * totalBrickSize.x), boardStartPos.y + (i * totalBrickSize.y)}, {brickWidth, brickHeight}, brickLives);

      bricks.push_back(brick);
    }
  }

  std::random_device rd;
  std::mt19937 gen(rd());

  for (uint32_t i = 0; i < 5; i++)
  {
    std::uniform_real_distribution<float> brickIndex(0, bricks.size());

    bricks[brickIndex(gen)].type = ADD_BALL;
  }
}

void Game::Init()
{
  ResourceManager::LoadShader("D:/Dev/Cpp/OpenGL/breakout/src/shaders/quad.vert", "D:/Dev/Cpp/OpenGL/breakout/src/shaders/quad.frag", "quad");
  ResourceManager::LoadShader("D:/Dev/Cpp/OpenGL/breakout/src/shaders/font.vert", "D:/Dev/Cpp/OpenGL/breakout/src/shaders/font.frag", "font");

  Renderer renderer;
  this->renderer = renderer;
  this->renderer.Init();

  TextRenderer::Get().Init();
  TextRenderer::Get().LoadCharacters();

  this->brickHeight = 25.0f;
  this->brickWidth = 100.0f;

  ResetBricks(renderer, Width, Height, brickWidth, brickHeight, bricks);

  float paddleWidth = 300.0f;
  float paddleHeight = 25.0f;

  paddle.Init({Width / 2.0f, Height - paddleHeight * 2.0f}, {paddleWidth, paddleHeight}, {0.8f, 0.2f, 0.3f, 1.0f});

  Ball ball(renderer, Width, Height);

  ball.Init(paddle.pos);
  balls.push_back(ball);
}

void Game::Update(float dt)
{
  glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);

  ResourceManager::GetShader("quad").Use().SetMatrix4("u_MVP", proj, false);
  ResourceManager::GetShader("font").Use().SetMatrix4("proj", proj, false);

  if (state == GAME_ACTIVE)
  {
    if (lives == 0 || bricks.size() == 0)
    {
      paddle.Reset();
      balls.clear();

      ResetBricks(renderer, Width, Height, brickWidth, brickHeight, bricks);

      Ball ball(renderer, Width, Height);
      ball.Init(paddle.pos);
      balls.push_back(ball);
      state = lives == 0 ? GAME_LOST : GAME_WON;
    }
  }

  if (state == GAME_ACTIVE)
  {
    for (uint32_t i = 0; i < balls.size(); i++)
    {
      balls[i].Move(dt);
    }

    paddle.Move(dt);
    paddle.CheckCollision();

    for (uint32_t i = 0; i < balls.size(); i++)
    {
      bool remove = balls[i].CheckCollision(paddle, lives);
      if (remove)
      {
        lives--;
        balls.erase(balls.begin() + i);
        continue;
      }
      for (uint32_t j = 0; j < bricks.size(); j++)
      {
        balls[i].CheckCollision(bricks[j]);
        if (bricks[j].lives <= 0)
        {
          if (bricks[j].type == ADD_BALL)
          {
            lives++;
            Ball ball(renderer, Width, Height);
            ball.Init(paddle.pos);
            balls.push_back(ball);
          }
          bricks.erase(bricks.begin() + j);
        }
      }
    }
  }
}

void Game::Render()
{
  ResourceManager::GetShader("quad").Use();

  renderer.BeginBatch();

  if (state == GAME_MENU)
  {
    renderer.DrawText("Start by pressing SPACE", {Width / 2.0f - 250.0f, Height / 2.0f + 50.0f}, 1.0f);
  }
  else if (state == GAME_LOST)
  {
    renderer.DrawText("You LOST!", {Width / 2.0f - 100.0f, Height / 2.0f + 50.0f}, 1.0f);
    renderer.DrawText("Play again by pressing SPACE", {Width / 2.0f - 250.0f, Height / 2.0f + 100.0f}, 1.0f);
  }
  else if (state == GAME_WON)
  {
    renderer.DrawText("You WON!", {Width / 2.0f - 100.0f, Height / 2.0f + 50.0f}, 1.0f);
    renderer.DrawText("Play again by pressing SPACE", {Width / 2.0f - 250.0f, Height / 2.0f + 100.0f}, 1.0f);
  }

  paddle.Draw();
  for (uint32_t i = 0; i < bricks.size(); i++)
  {
    bricks[i].Draw();
  }

  for (uint32_t i = 0; i < balls.size(); i++)
  {
    balls[i].Draw();
  }

  renderer.EndBatch();
  renderer.Flush();
}

void Game::ProcessInput(float dt, uint32_t leftKey, uint32_t rightKey, uint32_t spaceKey)
{
  if (state == GAME_ACTIVE)
  {
    if (leftKey == GLFW_PRESS)
    {
      paddle.velocity = {-1.0f, 0.0f};
    }
    else if (rightKey == GLFW_PRESS)
    {
      paddle.velocity = {1.0f, 0.0f};
    }
    else
    {
      paddle.velocity = {0.0f, 0.0f};
    }
  }

  if (state != GAME_ACTIVE)
  {
    if (spaceKey == GLFW_PRESS)
    {
      lives = 1;
      state = GAME_ACTIVE;
    }
  }
}
