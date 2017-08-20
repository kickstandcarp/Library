#ifndef REPR_OPENGL_REPR_HPP
#define REPR_OPENGL_REPR_HPP

#include <string>
#include "opengl/window.hpp"
#include "opengl/shader.hpp"
#include "opengl/vertex_array.hpp"
#include "opengl/texture.hpp"
#include "opengl/frame_buffer.hpp"

std::string	window_repr(const Window &window);
std::string	shader_repr(const Shader &shader);
std::string	vertex_array_repr(const VertexArray &vertex_array);
std::string	texture_repr(const Texture &texture);
std::string	frame_buffer_repr(const FrameBuffer &frame_buffer);

#endif
