#pragma once

#include <GL/glew.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLDC(x) \
  glClearErrors();\
  x;\
  ASSERT(glLogErrors(#x, __FILE__, __LINE__))

void glClearErrors();
bool glLogErrors(const char* function, const char* file, int line);