#ifndef IMAGE_GAUSSIAN_FILTER_HPP
#define IMAGE_GAUSSIAN_FILTER_HPP

#include <string>
#include "opengl/window.hpp"

class GaussianFilter
{
	public:
		GaussianFilter(Window &window);
        ~GaussianFilter();

        void            apply(Window &window);

		float			size, width, amplitude;

        bool            blend;
        BlendFactor     source_blend_factor, destination_blend_factor;

        std::string     source_frame_buffer_name, intermediate_frame_buffer_name, destination_frame_buffer_name;
        unsigned int    source_color_attachment, intermediate_color_attachment, destination_color_attachment;

    private:
        std::string		shader_1_name, shader_2_name;
		std::string		vertex_array_name;
};

#endif
