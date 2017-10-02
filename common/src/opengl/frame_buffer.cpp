#include <stdexcept>
#include <utility>

#include "opengl/frame_buffer.hpp"
#include "opengl/texture.hpp"


#include <iostream>


FrameBuffer::FrameBuffer(const std::array<unsigned int, 2> &size, const std::vector<TextureFormat> &color_attachment_texture_formats, const bool has_depth_attachment)
:   num_color_attachments(color_attachment_texture_formats.size())
{
	int bound_frame_buffer_id;
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &bound_frame_buffer_id);

	glGenFramebuffers(1, &this->id);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, this->id);

	this->textures.reserve(num_color_attachments + (has_depth_attachment ? 1 : 0));
	for (unsigned int i = 0; i < color_attachment_texture_formats.size(); i++)
	{
		this->textures.emplace_back(std::vector<float>(), size, color_attachment_texture_formats[i], TextureInterpolation::nearest, TextureWrap::clamp);
		this->textures.back().set_data(std::vector<float>(this->textures.back().get_length(), 0.0f), this->textures.back().get_size());
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->textures.back().get_id(), 0);
	}
	if (has_depth_attachment)
	{
		this->textures.emplace_back(std::vector<float>(), size, TextureFormat::depth, TextureInterpolation::nearest, TextureWrap::clamp);
		this->textures.back().set_data(std::vector<float>(this->textures.back().get_length(), 0.0f), this->textures.back().get_size());
		glFramebufferTexture2D(GL_READ_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->textures.back().get_id(), 0);
	}

	glBindFramebuffer(GL_READ_FRAMEBUFFER, bound_frame_buffer_id);
}

FrameBuffer::~FrameBuffer()
{
	this->textures.clear();

	glDeleteFramebuffers(1, &this->id);
}

unsigned int FrameBuffer::get_id() const
{
	return this->id;
}

unsigned int FrameBuffer::get_num_color_attachments() const
{
	return this->num_color_attachments;
}

bool FrameBuffer::get_has_depth_attachment() const
{
	return this->num_color_attachments != this->textures.size();
}

Texture& FrameBuffer::get_color_texture(const unsigned int color_attachment_index)
{
	if (color_attachment_index < this->num_color_attachments)
		return this->textures[color_attachment_index];
	else
        throw std::runtime_error("invalid frame buffer color attachment");
}

Texture& FrameBuffer::get_depth_texture()
{
	if (this->get_has_depth_attachment())
		return this->textures.back();
	else
		throw std::runtime_error("invalid frame buffer depth attachment");
}

void FrameBuffer::swap_color_texture(const unsigned int color_attachment_index, Texture &texture)
{
	int bound_draw_frame_buffer_id;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &bound_draw_frame_buffer_id);

    glFramebufferTexture2D(static_cast<unsigned int>(bound_draw_frame_buffer_id) == this->id ? GL_DRAW_FRAMEBUFFER : GL_READ_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + color_attachment_index, GL_TEXTURE_2D, texture.get_id(), 0);
	swap(this->textures[color_attachment_index], texture);
}

void FrameBuffer::copy(const unsigned int color_attachment_index)
{
	std::array<unsigned int, 2> size = this->textures[color_attachment_index].get_size();
	glReadBuffer(GL_COLOR_ATTACHMENT0 + color_attachment_index);
    glBlitFramebuffer(0, 0, size[0], size[1], 0, 0, size[0], size[1], GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

bool FrameBuffer::complete() const
{
    return glCheckFramebufferStatus(GL_READ_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE;
}
