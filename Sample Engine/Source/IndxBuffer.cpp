#include "IndexBuffer.h"
#include "Renderer.h"

IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count)
{
  ASSERT(sizeof(unsigned int) == sizeof(GLuint));
  GLDC(glGenBuffers(1, &m_RendererID));
  GLDC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
  GLDC(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer()
{
  GLDC(glDeleteBuffers(1, &m_RendererID));
}

void IndexBuffer::Bind() const
{
  GLDC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

void IndexBuffer::Unbind() const
{
  GLDC(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}
