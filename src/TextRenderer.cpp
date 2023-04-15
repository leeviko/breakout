#include "TextRenderer.hpp"
#include "Renderer.hpp"
#include <glad/glad.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <iostream>

TextRenderer::TextRenderer()
{
}
TextRenderer::~TextRenderer()
{
}
bool TextRenderer::Init()
{
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  {
    std::cout << "ERROR::FREETYPE: Could not init FreeType library" << std::endl;
    return false;
  }
  FT_Face face;
  if (FT_New_Face(ft, "D:/Dev/Cpp/OpenGL/breakout/src/assets/fonts/Poppins.ttf", 0, &face))
  {
    std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
    return false;
  }
  m_Ft = ft;
  m_Face = face;
  FT_Set_Pixel_Sizes(face, 0, 48);
  return true;
}
void TextRenderer::LoadCharacters()
{
  GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1));
  for (unsigned char c = 0; c < 128; c++)
  {
    if (FT_Load_Char(m_Face, c, FT_LOAD_RENDER))
    {
      std::cout << "ERROR::FREETYPE: Failed to load Glyph" << std::endl;
      continue;
    }

    uint32_t texture;
    GLCall(glGenTextures(1, &texture));
    GLCall(glBindTexture(GL_TEXTURE_2D, texture));
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, m_Face->glyph->bitmap.buffer));

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    Character character = {
        texture,
        {m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows},
        {m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top},
        m_Face->glyph->advance.x};

    Characters.insert(std::pair<char, Character>(c, character));
  }

  FT_Done_Face(m_Face);
  FT_Done_FreeType(m_Ft);
}
