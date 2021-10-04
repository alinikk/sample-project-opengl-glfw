#include "Shader.h"

#include <stdio.h>
//#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

struct ShaderSource
{
  std::string vertex_shader_source;
  std::string fragment_shader_source;
};

Shader::Shader(const std::string& filepath)
  : m_FilePath(filepath), m_RendererID(0)
{
  ShaderSource source = ParseShader(filepath);
  m_RendererID = CreateShader(source.vertex_shader_source, source.fragment_shader_source);
}

Shader::~Shader()
{
  GLDC(glDeleteProgram(m_RendererID));
}

ShaderSource Shader::ParseShader(const std::string& filepath)
{
  std::ifstream stream(filepath);

  enum class ShaderType
  {
    NONE = -1, VERTEX = 0, FRAGMENT = 1
  };

  std::string line;
  std::stringstream ss[2];
  ShaderType type = ShaderType::NONE;
  while (getline(stream, line))
  {
    if (line.find("#shader") != std::string::npos)
    {
      if (line.find("vertex") != std::string::npos)
      {
        type = ShaderType::VERTEX;
      }
      else if (line.find("fragment") != std::string::npos)
      {
        type = ShaderType::FRAGMENT;
      }
    }
    else
    {
      ss[(int)type] << line << '\n';
    }
  }

  return { ss[0].str(), ss[1].str() };
}

unsigned int Shader::CompileShader(unsigned int type, const std::string& source)
{
  GLDC(GLuint id = glCreateShader(type));
  const GLchar* src = source.c_str();
  GLDC(glShaderSource(id, 1, &src, NULL));
  GLDC(glCompileShader(id));

  GLint result;
  GLDC(glGetShaderiv(id, GL_COMPILE_STATUS, &result));
  if (result == GL_FALSE)
  {
    GLint length;
    GLDC(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
    GLchar* message = (GLchar*)alloca(length * sizeof(GLchar));
    GLDC(glGetShaderInfoLog(id, length, &length, message));

    printf("Failed to compile %s shader!\n %s\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment", message);
    GLDC(glDeleteShader(id));
    return 0;
  }

  return id;
}

unsigned int Shader::CreateShader(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
{
  GLDC(GLuint program = glCreateProgram());
  GLuint vertex_shader = CompileShader(GL_VERTEX_SHADER, vertex_shader_source);
  GLuint fragment_shader = CompileShader(GL_FRAGMENT_SHADER, fragment_shader_source);

  GLDC(glAttachShader(program, vertex_shader));
  GLDC(glAttachShader(program, fragment_shader));

  GLDC(glLinkProgram(program));
  GLDC(glValidateProgram(program));

  GLDC(glDeleteShader(vertex_shader));
  GLDC(glDeleteShader(fragment_shader));

  return program;
}


void Shader::Bind() const
{
  GLDC(glUseProgram(m_RendererID));
}

void Shader::Unbind() const
{
  GLDC(glUseProgram(0));
}

void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
{
  GLDC(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

int Shader::GetUniformLocation(const std::string& name)
{
  if (m_UniformLocationMap.find(name) != m_UniformLocationMap.end())
    return m_UniformLocationMap[name];
  
  GLDC(int location = glGetUniformLocation(m_RendererID, name.c_str()));
  if (location == -1)
    printf("Warning: uniform \"%s\" doesn't exist (yet...?).", name.c_str());
  
  m_UniformLocationMap[name] = location;
  return location;
}
