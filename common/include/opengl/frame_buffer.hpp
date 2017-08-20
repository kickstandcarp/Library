#ifndef OPENGL_FRAME_BUFFER_HPP
#define OPENGL_FRAME_BUFFER_HPP

#include <vector>
#include <array>
#include <GL/glew.h>
#include "opengl/texture.hpp"

class FrameBuffer
{
    public:
		FrameBuffer(const std::array<unsigned int, 2> &size, const unsigned int num_color_attachments, const bool has_depth_attachment);
        ~FrameBuffer();

		unsigned int				get_id() const;
		std::array<unsigned int, 2>	get_size() const;
		unsigned int				get_num_color_attachments() const;
		bool						get_has_depth_attachment() const;
		Texture&					get_color_texture(const unsigned int color_attachment);
		Texture&					get_depth_texture();

    private:
        unsigned int				id;

		std::array<unsigned int, 2>	size;
		unsigned int				num_color_attachments;

		std::vector<Texture>		textures;
};

#endif
