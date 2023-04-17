#pragma once

#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include "MyMath.hpp"
#include <map>

struct TextVertex
{
  glm::vec3 Position;
  glm::vec2 TexCoords;
  float TexIndex;
};

struct Character
{
  uint32_t TextureID;
  glm::vec2 Size;
  glm::vec2 Bearing;
  FT_Pos Advance;
};

class TextRenderer
{
private:
  FT_Library m_Ft;
  FT_Face m_Face;

public:
  std::map<char, Character> Characters;

  TextRenderer(const TextRenderer &) = delete;

  static TextRenderer &Get();

  bool Init();
  void LoadCharacters();

private:
  TextRenderer() {}
};