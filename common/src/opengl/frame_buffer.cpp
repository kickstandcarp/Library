#include <stdexcept>
#include <utility>
#include <sstream>
#include <numeric>

#include "opengl/frame_buffer.hpp"
#include "opengl/texture.hpp"



FrameBuffer::FrameBuffer(const std::array<unsigned int, 2> &size, const unsigned int num_color_attachments, const bool has_depth_attachment)
:	size(size),
	num_color_attachments(num_color_attachments)
{
	int bound_frame_buffer_id;
	glGetIntegerv(GL_FRAMEBUFFER_BINDING, &bound_frame_buffer_id);

	glGenFramebuffers(1, &this->id);
	glBindFramebuffer(GL_FRAMEBUFFER, this->id);

	this->textures.reserve(num_color_attachments + (has_depth_attachment ? 1 : 0));
	for (unsigned int i = 0; i < num_color_attachments; i++)
	{
		this->textures.emplace_back(std::vector<float>(), size, TextureFormat::rgba, TextureInterpolation::nearest, TextureWrap::clamp);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, this->textures.back().get_id(), 0);
	}
	if (has_depth_attachment)
	{
		this->textures.emplace_back(std::vector<float>(), size, TextureFormat::depth, TextureInterpolation::nearest, TextureWrap::clamp);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, this->textures.back().get_id(), 0);
	}

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		throw std::runtime_error("frame buffer initialization error");

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBindFramebuffer(GL_FRAMEBUFFER, bound_frame_buffer_id);
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

std::array<unsigned int, 2> FrameBuffer::get_size() const
{
	return this->size;
}

unsigned int FrameBuffer::get_num_color_attachments() const
{
	return this->num_color_attachments;
}

bool FrameBuffer::get_has_depth_attachment() const
{
	return this->num_color_attachments != this->textures.size();
}

Texture& FrameBuffer::get_color_texture(const unsigned int color_attachment)
{
	if (color_attachment < this->num_color_attachments)
		return this->textures[color_attachment];
	else
	{
		std::stringstream what;
		what << "frame buffer has " << this->num_color_attachments << " color attachment" << (this->num_color_attachments == 1 ? "" : "s");
		throw std::runtime_error(what.str());
	}
}

Texture& FrameBuffer::get_depth_texture()
{
	if (this->get_has_depth_attachment())
		return this->textures.back();
	else
		throw std::runtime_error("frame buffer does not have depth attachment");
}
