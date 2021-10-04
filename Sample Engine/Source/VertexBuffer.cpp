#include "VertexBuffer.h"
#include "Renderer.h"

VertexBuffer::VertexBuffer(const void* data, unsigned int size)
{
  GLDC(glGenBuffers(1, &m_RendererID));
  GLDC(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
  GLDC(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}

VertexBuffer::~VertexBuffer()
{
  GLDC(glDeleteBuffers(1, &m_RendererID));
}

void VertexBuffer::Bind() const
{
  GLDC(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

void VertexBuffer::Unbind() const
{
  GLDC(glBindBuffer(GL_ARRAY_BUFFER, 0));
}
