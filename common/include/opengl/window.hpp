#ifndef OPENGL_WINDOW_HPP
#define OPENGL_WINDOW_HPP

#include <memory>
#include <utility>
#include <string>
#include <map>
#include <vector>
#include <array>
#include <tuple>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>
#include "opengl/shader.hpp"
#include "opengl/vertex_array.hpp"
#include "opengl/texture.hpp"
#include "opengl/frame_buffer.hpp"
#include "event/event_handler.hpp"

enum class BlendFactor : GLenum { one = GL_ONE, source_alpha = GL_SRC_ALPHA, one_minus_source_alpha = GL_ONE_MINUS_SRC_ALPHA }; 

class Window
{
    public:
        Window(const std::string &title, const std::array<unsigned int, 2> &size, const bool is_shown);
		~Window();

		std::array<unsigned int, 2>                     get_size() const;
		int                                             get_refresh_rate() const;
        float                                           get_refresh_time() const;
		bool											get_use_depth_test()  const;
		bool											get_use_blending() const;
		std::tuple<BlendFactor, BlendFactor>			get_blend_factors() const;
		glm::vec4										get_clear_color() const;
		bool											get_discard_rasterizer() const;
		EventHandler&                                   get_event_handler();
		std::vector<std::string>                        get_shader_names() const;
        Shader&                                         get_shader(const std::string &name);
		std::vector<std::string>                        get_vertex_array_names() const;
		VertexArray&			                        get_vertex_array(const std::string &name);
		std::vector<std::string>                        get_texture_names() const;
		Texture&				                        get_texture(const std::string &name);
		std::vector<std::string>                        get_frame_buffer_names() const;
        FrameBuffer&                                    get_frame_buffer(const std::string &name);

		void											set_use_depth_test(const bool use_depth_test);
		void											set_use_blending(const bool use_blending);
		void											set_blend_factors(const BlendFactor source, const BlendFactor destination);
		void											set_discard_rasterizer(const bool discard_rasterizer);
		void											set_clear_color(const glm::vec4 &color);
		void											set_target_frame_buffer(const std::string &name, const std::vector<unsigned int> &color_attachment_indices);

		void                                            add_shader(const std::string &name, const std::vector<std::pair<std::string, ShaderType> > &shaders, const std::vector<std::string> &transform_feedback_varying_names=std::vector<std::string>());
		void                                            add_vertex_array(const std::string &name, const DrawMode draw_mode);
		void											add_texture(const std::string &name, const std::vector<float> &data, const std::array<unsigned int, 2> &size, const TextureFormat format, const TextureInterpolation interpolation, const TextureWrap wrap);
		void                                            add_frame_buffer(const std::string &name, const std::array<unsigned int, 2> &size, const unsigned int num_color_attachments, const bool has_depth_attachment);

		void											remove_shader(const std::string &name);
		void											remove_vertex_array(const std::string &name);
		void											remove_texture(const std::string &name);
		void											remove_frame_buffer(const std::string &name);

		void											copy_frame_buffer(const std::string &source_name, const unsigned int source_color_attachment, const std::string &destination_name, const unsigned int destination_color_attachment);

		void                                            clear(const bool color=true, const bool depth=true);
		void                                            draw();

        std::tuple<bool, std::string>                   validate() const;

		std::string										window_frame_buffer_name;

    private:
		SDL_Window*                                     sdl_window;
		SDL_GLContext                                   sdl_context;

        std::array<unsigned int, 2>                     size;
        unsigned int                                    refresh_rate;
        float                                           refresh_time;
		bool											use_depth_test, use_blending, discard_rasterizer;
        std::tuple<BlendFactor, BlendFactor>            blend_factors;
        glm::vec4                                       clear_color;

        EventHandler                                    event_handler;

		std::map<std::string, Shader>                   shaders;
        std::map<std::string, Shader>::iterator         shader;

		std::map<std::string, VertexArray>              vertex_arrays;
		std::map<std::string, VertexArray>::iterator    vertex_array;

		std::map<std::string, Texture>                  textures;
		
		std::map<std::string, FrameBuffer>              frame_buffers;
		std::map<std::string, FrameBuffer>::iterator    target_frame_buffer;
		std::vector<GLenum>                             target_frame_buffer_color_attachments;
};

#endif
