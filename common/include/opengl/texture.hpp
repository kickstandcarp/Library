#ifndef OPENGL_TEXTURE_HPP
#define OPENGL_TEXTURE_HPP

#include <vector>
#include <array>
#include <GL/glew.h>
#include <glm/glm.hpp>

enum class TextureFormat : GLenum { r = GL_RED, rgba = GL_RGBA, depth = GL_DEPTH_COMPONENT }; 
enum class TextureInterpolation : GLenum { nearest = GL_NEAREST, linear = GL_LINEAR }; 
enum class TextureWrap : GLenum { clamp = GL_CLAMP_TO_EDGE, mirror = GL_MIRRORED_REPEAT, repeat = GL_REPEAT }; 

class Texture
{
    public:
        Texture(const std::vector<float> &data, const std::array<unsigned int, 2> &size, const TextureFormat format, const TextureInterpolation interpolation, const TextureWrap wrap);
        ~Texture();

		unsigned int				get_id() const;
		unsigned int				get_length() const;
		std::array<unsigned int, 2>	get_size() const;
		std::array<unsigned int, 3>	get_shape() const;
		std::vector<float>			get_data() const;

		void						set_data(const std::vector<float> &data, const std::array<unsigned int, 2> &size);
		void						set_texture_unit(const unsigned int index);

        friend void                 swap(Texture &a, Texture &b);

	private:
		unsigned int				num_channels() const;
		int                         internal_format() const;

		unsigned int                id;

		std::array<unsigned int, 3>	shape;
		TextureFormat				format;
};

#endif
