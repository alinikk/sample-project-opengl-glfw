#pragma once

#include<string>
#include<unordered_map>

struct ShaderSource;

class Shader
{
private:
  std::string m_FilePath;
  unsigned int m_RendererID;
  std::unordered_map<std::string, unsigned int> m_UniformLocationMap;
public:
  Shader(const std::string& filepath);
  ~Shader();

  void Bind() const;
  void Unbind() const;

  void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
  ShaderSource ParseShader(const std::string& filepath);
  unsigned int CompileShader(unsigned int type, const std::string& source);
  unsigned int CreateShader(const std::string& vertex_shader_source, const std::string& fragment_shader_source);
  int GetUniformLocation(const std::string& name);
};