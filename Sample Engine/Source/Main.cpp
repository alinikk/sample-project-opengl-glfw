#include <stdio.h>
//#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"

#define PI 3.14159265358

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

  {
    /* Outside the loop */

    GLfloat positions[12] =
    { -0.5f, -0.5f,
       0.0f, -0.5f,
       0.5f, -0.5f,
       0.5f,  0.5f,
       0.0f,  0.5f,
      -0.5f,  0.5f };

    GLuint indices[12] =
    { 0, 4, 5,
      0, 1, 4,
      1, 3, 4,
      1, 2, 3 };

    VertexArray va;

    VertexBuffer vb(positions, 12 * sizeof(GLfloat));
    
    VertexBufferLayout layout;
    layout.Push<float>(2);
    va.AddBuffer(vb, layout);

    IndexBuffer ib(indices, 12);

    Shader shader("Resources/Shaders/basic.shader");
    shader.Bind();
    shader.SetUniform4f("color_0", 0.0f, 1.0f, 0.0f, 0.0f);
    shader.SetUniform4f("color_1", 0.0f, 0.0f, 0.0f, 0.0f);
    shader.SetUniform4f("color_2", 0.0f, 0.0f, 0.0f, 0.0f);

    va.Unbind();
    vb.Unbind();
    ib.Unbind();
    shader.Unbind();

    float f = 0.0f;
    float t = 0.0f;
    float dt = 0.05f;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
      /* Render here */
      GLDC(glClear(GL_COLOR_BUFFER_BIT));

      shader.Bind();

      if (t > 1.0f)
      {
        t = 2.0f - t;
        dt *= -1.0f;
      }
      else if (t < 0.0f)
      {
        t = -t;
        dt *= -1.0f;
      }

      f = cos(t * PI / 2 + 0.5f);


      // -- Draw Call Happening -- //

      shader.SetUniform4f("color_1", f, 0.0f, 1.0f - f, 1.0f);
      shader.SetUniform4f("color_2", 1.0f - f, 0.0f, f, 1.0f);

      vb.Bind();
      ib.Bind();
      GLDC(glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, NULL));

      t += dt;

      /* Swap front and back buffers */
      glfwSwapBuffers(window);

      /* Poll for and process events */
      glfwPollEvents();
    }
  }

  glfwTerminate();
  return 0;
}