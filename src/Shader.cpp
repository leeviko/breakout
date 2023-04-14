#include "Shader.hpp"
#include "Renderer.hpp"

Shader &Shader::Use()
{
  GLCall(glUseProgram(m_ProgramId));
  return *this;
}

void Shader::Compile(const char *vertexSource, const char *fragmentSource)
{
  uint32_t sVertex, sFragment;

  sVertex = glCreateShader(GL_VERTEX_SHADER);
  GLCall(glShaderSource(sVertex, 1, &vertexSource, NULL));
  GLCall(glCompileShader(sVertex));
  checkCompileErrors(sVertex, "VERTEX");

  sFragment = glCreateShader(GL_FRAGMENT_SHADER);
  GLCall(glShaderSource(sFragment, 1, &fragmentSource, NULL));
  GLCall(glCompileShader(sFragment));
  checkCompileErrors(sFragment, "FRAGMENT");

  m_ProgramId = glCreateProgram();
  GLCall(glAttachShader(m_ProgramId, sVertex));
  GLCall(glAttachShader(m_ProgramId, sFragment));
  GLCall(glLinkProgram(m_ProgramId));
  checkCompileErrors(m_ProgramId, "PROGRAM");

  GLCall(glDeleteShader(sVertex));
  GLCall(glDeleteShader(sFragment));
}

void Shader::SetFloat(const char *name, float value, bool useShader)
{
  if (useShader)
    Use();
  glUniform1f(glGetUniformLocation(m_ProgramId, name), value);
}
void Shader::SetInteger(const char *name, int value, bool useShader)
{
  if (useShader)
    Use();
  glUniform1i(glGetUniformLocation(m_ProgramId, name), value);
}
void Shader::SetIntegerv(const char *name, int value[32], bool useShader)
{
  if (useShader)
    Use();
  glUniform1iv(glGetUniformLocation(m_ProgramId, name), 32, value);
}
void Shader::SetVector2f(const char *name, float x, float y, bool useShader)
{
  if (useShader)
    Use();
  glUniform2f(glGetUniformLocation(m_ProgramId, name), x, y);
}
void Shader::SetVector2f(const char *name, const glm::vec2 &value, bool useShader)
{
  if (useShader)
    Use();
  glUniform2f(glGetUniformLocation(m_ProgramId, name), value.x, value.y);
}
void Shader::SetVector3f(const char *name, float x, float y, float z, bool useShader)
{
  if (useShader)
    Use();
  glUniform3f(glGetUniformLocation(m_ProgramId, name), x, y, z);
}
void Shader::SetVector3f(const char *name, const glm::vec3 &value, bool useShader)
{
  if (useShader)
    Use();
  glUniform3f(glGetUniformLocation(m_ProgramId, name), value.x, value.y, value.z);
}
void Shader::SetVector4f(const char *name, float x, float y, float z, float w, bool useShader)
{
  if (useShader)
    Use();
  glUniform4f(glGetUniformLocation(m_ProgramId, name), x, y, z, w);
}
void Shader::SetVector4f(const char *name, const glm::vec4 &value, bool useShader)
{
  if (useShader)
    Use();
  glUniform4f(glGetUniformLocation(m_ProgramId, name), value.x, value.y, value.z, value.w);
}
void Shader::SetMatrix4(const char *name, const glm::mat4 &matrix, bool useShader)
{
  if (useShader)
    Use();
  glUniformMatrix4fv(glGetUniformLocation(m_ProgramId, name), 1, false, glm::value_ptr(matrix));
}

void Shader::checkCompileErrors(unsigned int object, std::string type)
{
  int success;
  char infoLog[1024];
  if (type != "PROGRAM")
  {
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if (!success)
    {
      glGetShaderInfoLog(object, 1024, NULL, infoLog);
      std::cout << "| ERROR::SHADER: Compile-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
    }
  }
  else
  {
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if (!success)
    {
      glGetProgramInfoLog(object, 1024, NULL, infoLog);
      std::cout << "| ERROR::Shader: Link-time error: Type: " << type << "\n"
                << infoLog << "\n -- --------------------------------------------------- -- "
                << std::endl;
    }
  }
}
