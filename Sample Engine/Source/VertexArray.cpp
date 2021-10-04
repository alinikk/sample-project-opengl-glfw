#include "VertexArray.h" 
#include "Renderer.h"

VertexArray::VertexArray()
{
  GLDC(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
  GLDC(glDeleteVertexArrays(1, &m_RendererID));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout)
{
  vb.Bind();

  const auto& elements = layout.GetElements();
  unsigned int offset = 0;
  for (unsigned int i = 0; i < elements.size(); i++)
  {
    const auto& element = elements[i];
    GLDC(glEnableVertexAttribArray(i));
    GLDC(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void *)offset));
    offset += VertexBufferElement::GetSizeOfType(element.type) * element.count;
  }
}

void VertexArray::Bind() const
{
  GLDC(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
  GLDC(glBindVertexArray(0));
}
