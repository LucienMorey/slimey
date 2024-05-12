#pragma once

#include <GL/glew.h>

#include <cstdint>
#include <stdexcept>
#include <vector>

namespace GlWrapper
{
struct VertexBufferElement
{
  uint32_t type;
  uint32_t count;
  uint8_t normalised;

  static size_t get_size_of_type(uint32_t type)
  {
    switch (type) {
      case GL_FLOAT:
        return 4U;
      default:
        throw std::invalid_argument("unimplemented type size check");
        return 0;
    }
  }
};

class VertexBufferLayout
{
public:
  VertexBufferLayout() : elements_({}), stride_(0) {}
  ~VertexBufferLayout() {}

  template <typename T>
  VertexBufferLayout & append(const uint32_t count)
  {
    throw std::invalid_argument("unimplemented type size check");
    return *this;
  }

  uint32_t get_stride() const { return stride_; }
  std::vector<VertexBufferElement> get_elements() const { return elements_; }

  void activate() const
  {
    uint32_t index = 0;
    uint32_t offset = 0;
    for (const auto & element : elements_) {
      glEnableVertexAttribArray(index);
      glVertexAttribPointer(
        index, element.count, element.type, element.normalised, stride_, (void *)offset);
      offset += element.count * VertexBufferElement::get_size_of_type(element.type);
      index++;
    }
  }

private:
  std::vector<VertexBufferElement> elements_;
  uint32_t stride_;
};

template <>
VertexBufferLayout & VertexBufferLayout::append<float>(const uint32_t count)
{
  elements_.push_back({GL_FLOAT, count, false});
  stride_ += count * VertexBufferElement::get_size_of_type(GL_FLOAT);
  return *this;
}

};  // namespace GlWrapper
