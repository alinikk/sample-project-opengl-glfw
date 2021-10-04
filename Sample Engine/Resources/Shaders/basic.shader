#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

out vec4 vertex_color;

uniform vec4 color_0;
uniform vec4 color_1;
uniform vec4 color_2;

void main()
{
  vertex_color = vec4(0.0, 0.0, 0.0, 1.0);
  if (position.x == -0.5f)
  {
    vertex_color = color_1;
  }
  else if (position.x == 0.0f)
  {
    vertex_color = color_0;
  }
  else if (position.x == 0.5f)
  {
    vertex_color = color_2;
  }

  gl_Position = position;
};

#shader fragment
#version 330 core

in vec4 vertex_color;
out vec4 fragment_color;

void main()
{
  fragment_color = vertex_color;
};