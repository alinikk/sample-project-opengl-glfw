#shader vertex
#version 330 core

layout(location = 0) in vec4 position;

out vec4 vertex_color;

void main()
{
  vertex_color = vec4(0.0, 0.0, 0.0, 1.0);
  if (position.x == -0.5f)
  {
    vertex_color.r = 1.0;
  }
  else if (position.x == 0.0f)
  {
    vertex_color.g = 1.0;
  }
  else if (position.x == 0.5f)
  {
    vertex_color.b = 1.0;
  }

  gl_Position = position;
};

#shader fragment
#version 330 core

in vec4 vertex_color;
out vec4 fragment_color;

uniform vec4 uniform_color;

void main()
{
  fragment_color = vertex_color;
};