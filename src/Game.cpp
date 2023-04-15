#include "Game.hpp"
#include "ResourceManager.hpp"
#include "GameObject.hpp"
#include <GLFW/glfw3.h>
#include <random>
#include "TextRenderer.hpp"

Game::Game(uint32_t width, uint32_t height) : Width(width), Height(height), player(GameObject(renderer)), state(GAME_MENU)
{
}
Game::~Game()
{
  ResourceManager::Clear();
  renderer.Clear();
}

void Game::Init()
{
  ResourceManager::LoadShader("D:/Dev/Cpp/OpenGL/breakout/src/shaders/quad.vert", "D:/Dev/Cpp/OpenGL/breakout/src/shaders/quad.frag", "quad");

  Renderer renderer;
  this->renderer = renderer;
  this->renderer.Init();

  TextRenderer textRenderer;
  this->textRenderer = textRenderer;
  textRenderer.Init();
  textRenderer.LoadCharacters();

  this->brickHeight = 25.0f;
  this->brickWidth = 100.0f;

  float brickPadding = 15.0f;
  float borderPadding = 50.0f;

  uint32_t rows = 8;
  uint32_t columns = 10;

  glm::vec2 totalBrickSize = glm::vec2(brickWidth, brickHeight) + glm::vec2(brickPadding, brickPadding);
  glm::vec2 boardStartPos = {Width - (columns * totalBrickSize.x), brickHeight + 25.0f};

  for (uint32_t i = 0; i < rows; i++)
  {
    uint32_t brickLives = i % 2 == 0 ? 1 : 2;

    for (uint32_t j = 0; j < columns; j++)
    {
      Brick brick(renderer);
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

  float playerWidth = 150.0f;
  float playerHeight = 25.0f;

  player.Init({Width / 2.0f, Height - playerHeight * 2.0f}, {playerWidth, playerHeight}, {0.8f, 0.2f, 0.3f, 1.0f});

  Ball ball(renderer);

  ball.Init(player.pos, Width, Height);
  balls.push_back(ball);
}

void Game::Update(float dt)
{
  glm::mat4 proj = glm::ortho(0.0f, static_cast<float>(Width), static_cast<float>(Height), 0.0f, -1.0f, 1.0f);

  ResourceManager::GetShader("quad").Use();
  ResourceManager::GetShader("quad").SetMatrix4("u_MVP", proj, false);

  for (uint32_t i = 0; i < balls.size(); i++)
  {
    balls[i].Move(dt);
  }

  for (uint32_t i = 0; i < balls.size(); i++)
  {
    balls[i].CheckCollision(player);

    for (uint32_t j = 0; j < bricks.size(); j++)
    {
      balls[i].CheckCollision(bricks[j]);
      if (bricks[j].lives <= 0)
      {
        if (bricks[j].type == ADD_BALL)
        {
          Ball ball(renderer);
          ball.Init(player.pos, Width, Height);
          balls.push_back(ball);
        }
        bricks.erase(bricks.begin() + j);
      }
    }
  }
}
void Game::Render()
{
  ResourceManager::GetShader("quad").Use();

  renderer.BeginBatch();

  player.Draw();
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
  if (leftKey == GLFW_PRESS)
  {
    player.velocity = {-1.0f, 0.0f};

    player.pos += player.velocity * 500.0f * dt;

    if (player.pos.x <= 0.0f + player.size.x / 2.0f)
    {
      player.pos.x = 0.0f + player.size.x / 2.0f;
    }
  }
  else if (rightKey == GLFW_PRESS)
  {
    player.velocity = {1.0f, 0.0f};

    player.pos += player.velocity * 500.0f * dt;
    if (player.pos.x >= Width - player.size.x / 2.0f)
    {
      player.pos.x = Width - player.size.x / 2.0f;
    }
  }
  if (spaceKey == GLFW_PRESS)
  {
    Ball ball(renderer);
    ball.Init(player.pos, Width, Height);
    balls.push_back(ball);
  }
}
