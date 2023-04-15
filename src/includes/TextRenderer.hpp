#pragma once

#pragma once
#include <ft2build.h>
#include FT_FREETYPE_H
#include <string>
#include "MyMath.hpp"
#include <map>

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
  std::map<char, Character> Characters;
  FT_Library m_Ft;
  FT_Face m_Face;

public:
  TextRenderer();
  ~TextRenderer();

  bool Init();
  void LoadCharacters();
  void RenderText(std::string text);
};