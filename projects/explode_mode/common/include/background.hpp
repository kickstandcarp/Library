#ifndef EXPLODE_MODE_BACKGROUND_HPP
#define EXPLODE_MODE_BACKGROUND_HPP

#include <string>
#include "clock.hpp"
#include "opengl/window.hpp"
#include "opengl/camera.hpp"

class Background
{
	public:
		Background(const float radius, const float height, const unsigned int num_vertices, Window &window, const std::string &vertex_array_name);
		virtual ~Background();

        std::string     get_vertex_array_name() const;

        void            step(const Clock &clock, Window &window);

        void            draw(const Camera &camera, Window &window) const;

    private:
        float           radius, height;

        std::string     vertex_array_name;
        std::string     step_shader_name, draw_shader_name;
};

void                    initialize_window_background(Window &window, const std::string &step_shader_name="background_step", const std::string &draw_shader_name="background_draw");

#endif
