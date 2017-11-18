#ifndef IMAGE_GAUSSIAN_FILTER_HPP
#define IMAGE_GAUSSIAN_FILTER_HPP

#include <string>
#include "opengl/window.hpp"

class GaussianFilter
{
    public:
        GaussianFilter(Window &window);
        ~GaussianFilter();

        void            apply(Window &window) const;

        float           size, width, amplitude;

        bool            blend;
        BlendFactor     source_blend_factor, destination_blend_factor;

        std::string     source_frame_buffer_name, intermediate_frame_buffer_name, destination_frame_buffer_name;
        unsigned int    source_color_attachment_index, intermediate_color_attachment_index, destination_color_attachment_index;

        std::string     column_shader_name, row_shader_name;
        std::string     vertex_array_name;
};

void					initialize_window_gaussian_filter(Window &window, const std::string &column_shader_name="gaussian_filter_column", const std::string &row_shader_name="gaussian_filter_row", const std::string &vertex_array_name="gaussian_filter_vertex_array");

#endif
