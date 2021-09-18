#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLDC(x) \
  glClearErrors();\
  x;\
  ASSERT(glLogErrors(#x, __FILE__, __LINE__))

static void
glClearErrors()
{
  while (glGetError());
}

static bool
glLogErrors(const char* function, const char* file, int line)
{
  bool no_errors = true;
  while (GLenum error = glGetError())
  {
    printf("[OpenGL Error] (%u): %s in %s:%d", error, function, file, line);
    no_errors = false;
  }

  return no_errors;
}

struct ShaderSource
{
  std::string vertex_shader_source;
  std::string fragment_shader_source;
};

static ShaderSource
parse_shader(const std::string& filepath)
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

static GLuint
compile_new_shader(GLenum type, const std::string& source)
{
  GLDC(GLuint id = glCreateShader(type);)
  const GLchar* src = source.c_str();
  GLDC(glShaderSource(id, 1, &src, NULL);)
  GLDC(glCompileShader(id);)

  GLint result;
  GLDC(glGetShaderiv(id, GL_COMPILE_STATUS, &result);)
  if (result == GL_FALSE)
  {
    GLint length;
    GLDC(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);)
    GLchar *message = (GLchar *)alloca(length * sizeof(GLchar));
    GLDC(glGetShaderInfoLog(id, length, &length, message);)
    
    printf("Failed to compile %s shader!\n %s\n", type == GL_VERTEX_SHADER ? "vertex" : "fragment", message);
    GLDC(glDeleteShader(id);)
    return 0;
  }

  return id;
}

static GLuint
create_new_shader(const std::string& vertex_shader_source, const std::string& fragment_shader_source)
{
  GLDC(GLuint program = glCreateProgram();)
  GLuint vertex_shader   = compile_new_shader(GL_VERTEX_SHADER,   vertex_shader_source);
  GLuint fragment_shader = compile_new_shader(GL_FRAGMENT_SHADER, fragment_shader_source);

  GLDC(glAttachShader(program, vertex_shader);)
  GLDC(glAttachShader(program, fragment_shader);)

  GLDC(glLinkProgram(program);)
  GLDC(glValidateProgram(program);)

  GLDC(glDeleteShader(vertex_shader);)
  GLDC(glDeleteShader(fragment_shader);)

  return program;
}

int main(void)
{
  GLFWwindow* window;

  /* Initialize the library */
  if (!glfwInit())
    return -1; 

  /* Create a windowed mode window and its OpenGL context */
  window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
  if (!window)
  {
    glfwTerminate();
    return -1;
  }

  /* Make the window's context current */
  glfwMakeContextCurrent(window);

  /* Set swap interval, i.e. the number of screen updates to wait after swapping */
  glfwSwapInterval(1);

  GLenum err = glewInit();
  if (err != GLEW_OK)
  {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
  }

  printf("%s\n", glGetString(GL_VERSION));

  /* Outside the loop */

  GLfloat positions[8] =
    { -0.5f, -0.5f,
       0.5f, -0.5f,
       0.5f,  0.5f,
      -0.5f,  0.5f };

  GLuint indices[6] =
    { 0, 1, 2,
      2, 3, 0 };

  GLuint vbo;
  GLDC(glGenBuffers(1, &vbo);)
  GLDC(glBindBuffer(GL_ARRAY_BUFFER, vbo);)
  GLDC(glBufferData(GL_ARRAY_BUFFER, 8 * sizeof(GLfloat), &positions, GL_STATIC_DRAW);)

  GLDC(glEnableVertexAttribArray(0);)
  GLDC(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), 0);)

  GLuint ibo;
  GLDC(glGenBuffers(1, &ibo);)
  GLDC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);)
  GLDC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(GLuint), &indices , GL_STATIC_DRAW);)

  ShaderSource source = parse_shader("Resources/Shaders/basic.shader");

  GLuint shader = create_new_shader(source.vertex_shader_source, source.fragment_shader_source);
  GLDC(glUseProgram(shader);)

  GLDC(GLint u_location = glGetUniformLocation(shader, "uniform_color");)
  if (u_location != -1)
  {
    GLDC(glUniform4f(u_location, 1.0f, 0.0f, 1.0f, 1.0f);)
  }

  /* Loop until the user closes the window */
  while (!glfwWindowShouldClose(window))
  {
    /* Render here */
    glClear(GL_COLOR_BUFFER_BIT);

    GLDC(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);)

    /* Swap front and back buffers */
    glfwSwapBuffers(window);

    /* Poll for and process events */
    glfwPollEvents();
  }

  GLDC(glDeleteProgram(shader);)

  glfwTerminate();
  return 0;
}