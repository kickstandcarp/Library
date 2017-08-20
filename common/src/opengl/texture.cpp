#include <stdexcept>

#include "opengl/texture.hpp"



Texture::Texture(const std::vector<float> &data, const std::array<unsigned int, 2> &size, const TextureFormat format, const TextureInterpolation interpolation, const TextureWrap wrap)
:	format(format)
{
	this->shape = {{size[1], size[0], this->num_channels()}};

	int bound_texture_id;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_texture_id);

	glGenTextures(1, &this->id);
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, static_cast<GLenum>(interpolation));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, static_cast<GLenum>(interpolation));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, static_cast<GLenum>(wrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, static_cast<GLenum>(wrap));
	glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format(), size[0], size[1], 0, static_cast<GLenum>(format), GL_FLOAT, data.size() == 0 ? NULL : data.data());
	glBindTexture(GL_TEXTURE_2D, bound_texture_id);
}

Texture::~Texture()
{
	glDeleteTextures(1, &this->id);
}

unsigned int Texture::get_id() const
{
	return this->id;
}

std::array<unsigned int, 2> Texture::get_size() const
{
	return std::array<unsigned int, 2>{{this->shape[1], this->shape[0]}};
}

std::array<unsigned int, 3> Texture::get_shape() const
{
	return this->shape;
}

std::vector<float> Texture::get_data() const
{
	int bound_texture_id;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_texture_id);

	std::vector<float> data(this->shape[0]*this->shape[1]*this->shape[2]);
	glBindTexture(GL_TEXTURE_2D, this->id);
	glGetTexImage(GL_TEXTURE_2D, 0, static_cast<GLenum>(this->format), GL_FLOAT, data.data());
	glBindTexture(GL_TEXTURE_2D, bound_texture_id);
	
	return data;
}

void Texture::set_data(const std::vector<float> &data, const std::array<unsigned int, 2> &size)
{
	int bound_texture_id;
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &bound_texture_id);

	this->shape = {{size[1], size[0], this->num_channels()}};
	glBindTexture(GL_TEXTURE_2D, this->id);
	glTexImage2D(GL_TEXTURE_2D, 0, this->internal_format(), size[0], size[1], 0, static_cast<GLenum>(this->format), GL_FLOAT, data.data());
	glBindTexture(GL_TEXTURE_2D, bound_texture_id);
}

void Texture::set_texture_unit(const unsigned int index)
{
	glActiveTexture(GL_TEXTURE0 + index);
	glBindTexture(GL_TEXTURE_2D, this->id);
}

unsigned int Texture::num_channels() const
{
	switch (this->format)
	{
		case TextureFormat::rgba:
			return 4;
			break;
		case TextureFormat::depth:
			return 1;
			break;
		default:
			throw std::runtime_error("unknown texture format");
	}
}

int Texture::internal_format() const
{
	switch (this->format)
	{
		case TextureFormat::rgba:
			return GL_RGBA32F;
			break;
		case TextureFormat::depth:
			return GL_DEPTH_COMPONENT32F;
			break;
		default:
			throw std::runtime_error("unknown texture format");
	}
}
