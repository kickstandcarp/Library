#include <stdexcept>
#include <algorithm>
#include <sstream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "opengl/window.hpp"



Window::Window(const std::string &title, const std::array<unsigned int, 2> &size, const bool show)
:   frame_buffer_name("window"),
	use_depth_test(false),
	use_blending(false),
    event_handler(size),
	target_frame_buffer_color_attachments({GL_COLOR_ATTACHMENT0})
{
	if (SDL_WasInit(SDL_INIT_VIDEO) != SDL_TRUE)
        SDL_Init(SDL_INIT_VIDEO);

    this->sdl_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, size[0], size[1], (show ? SDL_WINDOW_SHOWN : SDL_WINDOW_HIDDEN) | SDL_WINDOW_OPENGL);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	this->sdl_context = SDL_GL_CreateContext(this->sdl_window);

	SDL_GL_SetSwapInterval(1);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
		throw std::runtime_error("GLEW initialization failed");

	int major_version, minor_version;
	glGetIntegerv(GL_MAJOR_VERSION, &major_version);
	glGetIntegerv(GL_MINOR_VERSION, &minor_version);
	if ((GL_MAJOR_VERSION < 3) || (GL_MAJOR_VERSION >= 3 && GL_MINOR_VERSION < 3))
	{
		std::stringstream what;
		what << "OpenGL version " << GL_MAJOR_VERSION << "." << GL_MINOR_VERSION << ", minimum 3.3 required";
		throw std::runtime_error(what.str().c_str());
	}

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glGetError();

    this->shader = this->shaders.end();
	this->vertex_array = this->vertex_arrays.end();
	this->target_frame_buffer = this->frame_buffers.end();
}

Window::~Window()
{
	this->shaders.clear();
    this->vertex_arrays.clear();

	SDL_GL_DeleteContext(this->sdl_context);
    SDL_DestroyWindow(this->sdl_window);
}

std::array<unsigned int, 2> Window::get_size() const
{
    int w, h;
    SDL_GetWindowSize(this->sdl_window, &w, &h);
    return std::array<unsigned int, 2>{{static_cast<unsigned int>(w), static_cast<unsigned int>(h)}};
}

int Window::get_refresh_rate() const
{
    SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(this->sdl_window), &display_mode);
    return display_mode.refresh_rate;
}

float Window::get_refresh_time() const
{
    SDL_DisplayMode display_mode;
	SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(this->sdl_window), &display_mode);
	return 1.0f / static_cast<float>(display_mode.refresh_rate);
}

bool Window::get_use_depth_test() const
{
	return glIsEnabled(GL_DEPTH_TEST) == GL_TRUE;
}

bool Window::get_use_blending() const
{
	return glIsEnabled(GL_BLEND) == GL_TRUE;
}

std::tuple<BlendFactor, BlendFactor> Window::get_blend_factors() const
{
	int source, destination;
	glGetIntegerv(GL_BLEND_SRC, &source);
	glGetIntegerv(GL_BLEND_SRC, &destination);
	return std::make_tuple(static_cast<BlendFactor>(source), static_cast<BlendFactor>(destination));
}

glm::vec4 Window::get_clear_color() const
{
	glm::vec4 color;
	glGetFloatv(GL_COLOR_CLEAR_VALUE, glm::value_ptr(color));
	return color;
}

EventHandler& Window::get_event_handler()
{
    return this->event_handler;
}

std::vector<std::string> Window::get_shader_names() const
{
    std::vector<std::string> names;
    for (auto const &shader : this->shaders)
        names.push_back(shader.first);
    return names;
}

Shader& Window::get_shader(const std::string &name)
{
	if (this->shader == this->shaders.end() || this->shader->first != name)
	{
		this->shader = this->shaders.find(name);
		if (this->shader == this->shaders.end())
		{
			std::stringstream what;
			what << "invalid shader name: " << name;
			throw std::runtime_error(what.str());
		}
		else
			glUseProgram(this->shader->second.get_id());
	}

    return this->shader->second;
}

std::vector<std::string> Window::get_vertex_array_names() const
{
	std::vector<std::string> names;
	for (auto const &vertex_array : this->vertex_arrays)
		names.push_back(vertex_array.first);
	return names;
}

VertexArray& Window::get_vertex_array(const std::string &name)
{
	if (this->vertex_array == this->vertex_arrays.end() || this->vertex_array->first != name)
	{
		this->vertex_array = this->vertex_arrays.find(name);
		if (this->vertex_array == this->vertex_arrays.end())
		{
			std::stringstream what;
			what << "invalid vertex array name: " << name;
			throw std::runtime_error(what.str());
		}
		else
			glBindVertexArray(this->vertex_array->second.get_id());
	}

	return this->vertex_array->second;
}

std::vector<std::string> Window::get_texture_names() const
{
	std::vector<std::string> names;
	for (auto const &texture : this->textures)
		names.push_back(texture.first);
	return names;
}

Texture& Window::get_texture(const std::string &name)
{
	auto texture = this->textures.find(name);
	if (texture == this->textures.end())
	{
		std::stringstream what;
		what << "invalid texture name: " << name;
		throw std::runtime_error(what.str());
	}
	else
		return texture->second;
}

std::vector<std::string> Window::get_frame_buffer_names() const
{
	std::vector<std::string> names;
	for (auto const &frame_buffer : this->frame_buffers)
		names.push_back(frame_buffer.first);
	return names;
}

FrameBuffer& Window::get_frame_buffer(const std::string &name)
{
	auto frame_buffer = this->frame_buffers.find(name);
	if (frame_buffer == this->frame_buffers.end())
	{
		std::stringstream what;
		what << "invalid frame buffer name: " << name;
		throw std::runtime_error(what.str());
	}
	else
		return frame_buffer->second;
}

void Window::set_use_depth_test(const bool use_depth_test)
{
	if (this->use_depth_test == use_depth_test)
		return;

	if (use_depth_test)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);

	this->use_depth_test = use_depth_test;
}

void Window::set_use_blending(const bool use_blending)
{
	if (this->use_blending == use_blending)
		return;

	if (use_blending)
		glEnable(GL_BLEND);
	else
		glDisable(GL_BLEND);
	
	this->use_blending = use_blending;
}

void Window::set_blend_factors(const BlendFactor source, const BlendFactor destination)
{
	glBlendFunc(static_cast<GLenum>(source), static_cast<GLenum>(destination));
}

void Window::set_clear_color(const glm::vec4 &color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void Window::set_target_frame_buffer(const std::string &name, const std::vector<unsigned int> &color_attachment_indices)
{
    if (name == this->frame_buffer_name)
    {
        if (this->target_frame_buffer == this->frame_buffers.end() && color_attachment_indices.size() == 1 && color_attachment_indices[0] == 0)
            return;
        else if (color_attachment_indices.size() != 1 || (color_attachment_indices.size() >= 1 && color_attachment_indices[0] != 0))
            throw std::runtime_error("frame buffer has 1 color attachment");
        else
        {
            this->target_frame_buffer = this->frame_buffers.end();
            this->target_frame_buffer_color_attachments = {GL_COLOR_ATTACHMENT0};
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
        }
    }
    else
    {
        std::vector<GLenum> color_attachments(color_attachment_indices.size());
        for (unsigned int i = 0; i < color_attachment_indices.size(); i++)
            color_attachments[i] = GL_COLOR_ATTACHMENT0 + color_attachment_indices[i];

        if (this->target_frame_buffer != this->frame_buffers.end() && this->target_frame_buffer->first == name && this->target_frame_buffer_color_attachments == color_attachments)
            return;

        auto frame_buffer = this->frame_buffers.find(name);
        if (frame_buffer == this->frame_buffers.end())
        {
            std::stringstream what;
            what << "invalid frame buffer name: " << name;
            throw std::runtime_error(what.str());
        }
        else if (*std::max_element(color_attachment_indices.begin(), color_attachment_indices.end()) > frame_buffer->second.get_num_color_attachments()-1)
        {
            std::stringstream what;
            what << "frame buffer has " << frame_buffer->second.get_num_color_attachments() << " color attachment" << (frame_buffer->second.get_num_color_attachments() == 1 ? "" : "s");
            throw std::runtime_error(what.str());
        }
        else
        {
            this->target_frame_buffer = frame_buffer;
            this->target_frame_buffer_color_attachments = color_attachments;
            glBindFramebuffer(GL_FRAMEBUFFER, frame_buffer->second.get_id());
			glDrawBuffers(color_attachments.size(), color_attachments.data());
        }
    }
}

void Window::add_shader(const std::string &name, const std::vector<std::pair<std::string, ShaderType> > &shaders)
{
    this->shaders.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(shaders));
}

void Window::add_vertex_array(const std::string &name, const DrawMode draw_mode)
{
	this->vertex_arrays.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(draw_mode));
}

void Window::add_texture(const std::string &name, const std::vector<float> &data, const std::array<unsigned int, 2> &size, const TextureFormat format, const TextureInterpolation interpolation, const TextureWrap wrap)
{
	this->textures.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(data, size, format, interpolation, wrap));
}

void Window::add_frame_buffer(const std::string &name, const std::array<unsigned int, 2> &size, const unsigned int num_color_attachments, const bool has_depth_attachment)
{
	if (name == this->frame_buffer_name)
		throw std::runtime_error("invalid frame buffer name");

	this->frame_buffers.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(size, num_color_attachments, has_depth_attachment));
}

void Window::remove_shader(const std::string &name)
{
	if (this->shader->first == name)
		this->shader = this->shaders.end();
	this->shaders.erase(name);
}

void Window::remove_vertex_array(const std::string &name)
{
	if (this->vertex_array->first == name)
		this->vertex_array = this->vertex_arrays.end();
	this->vertex_arrays.erase(name);
}

void Window::remove_texture(const std::string &name)
{
	this->textures.erase(name);
}

void Window::remove_frame_buffer(const std::string &name)
{
	if (this->target_frame_buffer->first == name)
	{
		this->target_frame_buffer = this->frame_buffers.end();
		this->target_frame_buffer_color_attachments = {GL_COLOR_ATTACHMENT0};
	}
	this->frame_buffers.erase(name);
}

void Window::copy_frame_buffer(const std::string &source_name, const unsigned int source_color_attachment, const std::string &destination_name, const unsigned int destination_color_attachment)
{
	unsigned int source_frame_buffer_id = source_name == this->frame_buffer_name ? 0 : this->frame_buffers.at(source_name).get_id(), destination_frame_buffer_id = destination_name == this->frame_buffer_name ? 0 : this->frame_buffers.at(destination_name).get_id();
	std::array<unsigned int, 2> source_size = source_frame_buffer_id == 0 ? this->get_size() : this->frame_buffers.at(source_name).get_size(), destination_size = destination_frame_buffer_id == 0 ? this->get_size() : this->frame_buffers.at(destination_name).get_size();
	
	glBindFramebuffer(GL_READ_FRAMEBUFFER, source_frame_buffer_id);
	if (source_frame_buffer_id != 0)
		glReadBuffer(GL_COLOR_ATTACHMENT0 + source_color_attachment);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, destination_frame_buffer_id);
	if (destination_frame_buffer_id != 0)
		glDrawBuffer(GL_COLOR_ATTACHMENT0 + destination_color_attachment);
	glBlitFramebuffer(0, 0, source_size[0], source_size[1], 0, 0, destination_size[0], destination_size[1], GL_COLOR_BUFFER_BIT | ((source_frame_buffer_id == 0 || destination_frame_buffer_id == 0) ? 0 : GL_DEPTH_BUFFER_BIT), GL_NEAREST);

    if (this->target_frame_buffer == this->frame_buffers.end())
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
	else
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->target_frame_buffer->second.get_id());
        glDrawBuffers(this->target_frame_buffer_color_attachments.size(), this->target_frame_buffer_color_attachments.data());
	}
}

void Window::clear(const bool color, const bool depth)
{
	if (!color && !depth)
		return;

	glClear((color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0));
}

void Window::draw()
{
    if (this->target_frame_buffer != this->frame_buffers.end())
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

	SDL_GL_SwapWindow(this->sdl_window);

    if (this->target_frame_buffer != this->frame_buffers.end())
    {
        glBindFramebuffer(GL_FRAMEBUFFER, this->target_frame_buffer->second.get_id());
        glDrawBuffers(this->target_frame_buffer_color_attachments.size(), this->target_frame_buffer_color_attachments.data());
    }

    GLenum err = glGetError();
    if (err != GL_NO_ERROR)
        throw std::runtime_error("opengl error");
}
