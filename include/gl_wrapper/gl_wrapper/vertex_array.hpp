#pragma once

#include <gl_wrapper/vertex_buffer.hpp>

namespace GlWrapper
{

template <class T>
class VertexArray
{
public:
  VertexArray() : buffer_(nullptr) { glGenVertexArrays(1, &id_); }
  ~VertexArray() { glDeleteVertexArrays(1, &id_); }

  void set_buffer(VertexBuffer<T> & buffer)
  {
    bind();
    buffer_ = &buffer;
    buffer_->bind();
    buffer_->get_layout().activate();
  }

  void bind() const
  {
    if (buffer_ == nullptr) {
      return;
    }
    glBindVertexArray(id_);
    buffer_->bind();
    buffer_->get_layout().activate();
  }
  void unbind() const { glBindVertexArray(0); }

private:
  uint32_t id_;

  VertexBuffer<T> * buffer_;
};

};  // namespace GlWrapper
