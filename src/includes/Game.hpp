#pragma once
#include <iostream>
#include "Renderer.hpp"
#include "GameObject.hpp"
#include <vector>
#include "Ball.hpp"
#include "Brick.hpp"

class Game
{
private:
  Renderer renderer;
  uint32_t Width, Height;

  float brickWidth, brickHeight;
  std::vector<Brick> bricks;
  std::vector<Ball> balls;

  GameObject player;

public:
  Game(uint32_t width, uint32_t height);
  ~Game();
  void Init();
  void Update(float dt);
  void ProcessInput(float dt, uint32_t leftKey, uint32_t rightKey, uint32_t spaceKey);
  void Render();
};