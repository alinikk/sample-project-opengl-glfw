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