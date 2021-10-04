#include "Renderer.h"

#include <stdio.h>

void glClearErrors()
{
  while (glGetError());
}

bool glLogErrors(const char* function, const char* file, int line)
{
  bool no_errors = true;
  while (GLenum error = glGetError())
  {
    printf("[OpenGL Error] (%u): %s in %s:%d", error, function, file, line);
    no_errors = false;
  }

  return no_errors;
}

void Renderer::Clear() const
{
  GLDC(glClear(GL_COLOR_BUFFER_BIT));
}

void Renderer::Draw(const VertexArray& va, const IndexBuffer& ib, const Shader& shader) const
{
  shader.Bind();
  va.Bind();
  ib.Bind();
  GLDC(glDrawElements(GL_TRIANGLES, ib.GetCount(), GL_UNSIGNED_INT, NULL));
}