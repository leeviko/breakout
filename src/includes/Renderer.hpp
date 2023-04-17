#pragma once
#include <iostream>
#include "MyMath.hpp"
#include "Macros.hpp"

void GLClearError();
bool GLLogCall(const char *function, const char *file, int line);

static const uint32_t MaxQuads = 1000;
static const uint32_t MaxVertices = MaxQuads * 4;
static const uint32_t MaxIndices = MaxQuads * 6;
static const uint32_t MaxTextureSlots = 32;

struct Vertex
{
  glm::vec3 Position;
  glm::vec4 Color;
  glm::vec2 TexCoords;
  float TexIndex;
};

class Renderer
{
public:
  Renderer();
  ~Renderer();

  void Init();
  void BeginBatch();
  void EndBatch();
  void Flush();

  void DrawQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color);
  void DrawText(const std::string &text, const glm::vec2 &pos, float scale);
  void Clear();
};
