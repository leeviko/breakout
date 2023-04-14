#include "Renderer.hpp"
#include <glad/glad.h>
#include <array>
#include "ResourceManager.hpp"

void GLClearError()
{
  while (glGetError() != GL_NO_ERROR)
    ;
}
bool GLLogCall(const char *function, const char *file, int line)
{
  while (GLenum error = glGetError())
  {
    std::cout << "[OpenGL Error] (" << error << ")"
              << " " << file << ":" << line << std::endl;
    return false;
  }
  return true;
}

struct RendererData
{
  uint32_t QuadVA = 0;
  uint32_t QuadVB = 0;
  uint32_t QuadIB = 0;

  uint32_t WhiteTextureId = 0;
  uint32_t WhiteTextureSlot = 0;

  uint32_t TextureIndex = 1;

  Vertex *QuadBuffer = nullptr;
  Vertex *QuadBufferPtr = nullptr;

  uint32_t IndexCount = 0;

  glm::vec4 QuadVertexPositions[4];

  std::array<uint32_t, MaxTextureSlots> TextureSlots;
};

RendererData s_Data;

Renderer::Renderer()
{
}
Renderer::~Renderer()
{
}

void Renderer::Init()
{
  s_Data.QuadBuffer = new Vertex[MaxVertices];

  GLCall(glGenVertexArrays(1, &s_Data.QuadVA));
  GLCall(glGenBuffers(1, &s_Data.QuadVB));

  GLCall(glBindVertexArray(s_Data.QuadVA));

  GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
  GLCall(glBufferData(GL_ARRAY_BUFFER, MaxVertices * sizeof(Vertex), nullptr, GL_DYNAMIC_DRAW));

  GLCall(glEnableVertexAttribArray(0));
  GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, Position)));

  GLCall(glEnableVertexAttribArray(1));
  GLCall(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, Color)));

  GLCall(glEnableVertexAttribArray(2));
  GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, TexCoords)));

  GLCall(glEnableVertexAttribArray(3));
  GLCall(glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void *)offsetof(Vertex, TexIndex)));

  uint32_t indices[MaxIndices];

  uint32_t offset = 0;
  for (uint32_t i = 0; i < MaxIndices; i += 6)
  {
    indices[i + 0] = 0 + offset;
    indices[i + 1] = 1 + offset;
    indices[i + 2] = 2 + offset;
    indices[i + 3] = 2 + offset;
    indices[i + 4] = 3 + offset;
    indices[i + 5] = 0 + offset;

    offset += 4;
  }

  GLCall(glGenBuffers(1, &s_Data.QuadIB));
  GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, s_Data.QuadIB));
  GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));

  GLCall(glGenTextures(1, &s_Data.WhiteTextureId));
  GLCall(glBindTexture(GL_TEXTURE_2D, s_Data.WhiteTextureId));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
  GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

  uint32_t color = 0xffffffff;
  GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, &color));

  s_Data.TextureSlots[0] = s_Data.WhiteTextureId;
  for (int i = 1; i < MaxTextureSlots; i++)
  {
    s_Data.TextureSlots[i] = 0;
  }

  int samplers[MaxTextureSlots];
  for (uint32_t i = 0; i < MaxTextureSlots; i++)
  {
    samplers[i] = i;
  }

  ResourceManager::GetShader("quad").Use().SetIntegerv("u_Textures", samplers, false);

  s_Data.QuadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
  s_Data.QuadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
  s_Data.QuadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
  s_Data.QuadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};

  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Renderer::BeginBatch()
{
  s_Data.QuadBufferPtr = s_Data.QuadBuffer;
}

void Renderer::EndBatch()
{
  GLsizeiptr size = (uint8_t *)s_Data.QuadBufferPtr - (uint8_t *)s_Data.QuadBuffer;
  GLCall(glBindBuffer(GL_ARRAY_BUFFER, s_Data.QuadVB));
  GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, s_Data.QuadBuffer));
}

void Renderer::Flush()
{
  for (uint32_t i = 0; i < s_Data.TextureIndex; i++)
  {
    GLCall(glActiveTexture(GL_TEXTURE0 + i));
    GLCall(glBindTexture(GL_TEXTURE_2D, s_Data.TextureSlots[i]));
  }

  GLCall(glBindVertexArray(s_Data.QuadVA));
  GLCall(glDrawElements(GL_TRIANGLES, s_Data.IndexCount, GL_UNSIGNED_INT, 0));

  s_Data.IndexCount = 0;
  s_Data.TextureIndex = 1;

  GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void Renderer::DrawQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color)
{
  if (s_Data.IndexCount >= MaxIndices)
  {
    EndBatch();
    Flush();
    BeginBatch();
  }

  glm::mat4 transform = glm::translate(glm::mat4(1.0f), {pos.x, pos.y, 0.0f}) * glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

  float textureIndex = 0.0f;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[0];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {0.0f, 0.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[1];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {1.0f, 0.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[2];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {1.0f, 1.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.QuadBufferPtr->Position = transform * s_Data.QuadVertexPositions[3];
  s_Data.QuadBufferPtr->Color = color;
  s_Data.QuadBufferPtr->TexCoords = {0.0f, 1.0f};
  s_Data.QuadBufferPtr->TexIndex = textureIndex;
  s_Data.QuadBufferPtr++;

  s_Data.IndexCount += 6;
}

void Renderer::Clear()
{
  GLCall(glDeleteBuffers(1, &s_Data.QuadVA));
  GLCall(glDeleteBuffers(1, &s_Data.QuadVB));
  GLCall(glDeleteBuffers(1, &s_Data.QuadIB));

  GLCall(glDeleteTextures(1, &s_Data.WhiteTextureId));

  delete[] s_Data.QuadBuffer;
}