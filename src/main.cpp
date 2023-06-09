#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "Game.hpp"

static const uint32_t SCREEN_WIDTH = 1270;
static const uint32_t SCREEN_HEIGHT = 720;

int main()
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", NULL, NULL);
  if (window == NULL)
  {
    std::cout << "Failed to create GLFW window" << std::endl;
    glfwTerminate();
    return -1;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
  {
    std::cout << "Failed to initialize GLAD" << std::endl;
    glfwTerminate();
    return -1;
  }

  Game Breakout(SCREEN_WIDTH, SCREEN_HEIGHT);

  Breakout.Init();

  float deltaTime = 0.0f;
  float lastFrame = 0.0f;
  GLCall(glClearColor(0.1f, 0.1f, 0.1f, 1.0f));
  GLCall(glEnable(GL_BLEND));
  GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
  while (!glfwWindowShouldClose(window))
  {
    GLCall(glClear(GL_COLOR_BUFFER_BIT));

    float currFrame = glfwGetTime();
    deltaTime = currFrame - lastFrame;
    lastFrame = currFrame;

    uint32_t leftKey = glfwGetKey(window, GLFW_KEY_LEFT);
    uint32_t rightKey = glfwGetKey(window, GLFW_KEY_RIGHT);
    uint32_t spaceKey = glfwGetKey(window, GLFW_KEY_SPACE);

    Breakout.ProcessInput(deltaTime, leftKey, rightKey, spaceKey);

    Breakout.Update(deltaTime);
    Breakout.Render();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwTerminate();
  return 0;
}