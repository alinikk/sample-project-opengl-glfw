#pragma once

#include <GL/glew.h>

#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLDC(x) \
  glClearErrors();\
  x;\
  ASSERT(glLogErrors(#x, __FILE__, __LINE__))

void glClearErrors();
bool glLogErrors(const char* function, const char* file, int line);

class Renderer
{
public:
  void Clear() const;
  void Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const;
};