#include <stdexcept>
#include <algorithm>
#include <sstream>

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "opengl/window.hpp"
#include "opengl/shader_support.hpp"



Window::Window(const std::string &title, const std::array<unsigned int, 2> &size, const bool show)
:   window_frame_buffer_name("window"),
    size(size),
    use_depth_test(false),
    use_blending(false),
    blend_factors(std::make_tuple(BlendFactor::one, BlendFactor::zero)),
    clear_color(glm::vec4(0.0f)),
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


    SDL_DisplayMode display_mode;
    SDL_GetCurrentDisplayMode(SDL_GetWindowDisplayIndex(this->sdl_window), &display_mode);

    this->refresh_rate = display_mode.refresh_rate;
    this->refresh_time = 1.0f / static_cast<float>(display_mode.refresh_rate);

    this->shader = this->shaders.end();
    this->vertex_array = this->vertex_arrays.end();
    this->frame_buffer = this->frame_buffers.end();
    this->target_frame_buffer = this->frame_buffers.end();
}

Window::~Window()
{
    this->shaders.clear();
    this->vertex_arrays.clear();
	this->frame_buffers.clear();
	this->textures.clear();

    SDL_GL_DeleteContext(this->sdl_context);
    SDL_DestroyWindow(this->sdl_window);
}

std::array<unsigned int, 2> Window::get_size() const
{
    return this->size;
}

int Window::get_refresh_rate() const
{
    return this->refresh_rate;
}

float Window::get_refresh_time() const
{
    return this->refresh_time;
}

bool Window::get_use_depth_test() const
{
    return this->use_depth_test;
}

bool Window::get_use_blending() const
{
    return this->use_blending;
}

std::tuple<BlendFactor, BlendFactor> Window::get_blend_factors() const
{
    return this->blend_factors;
}

glm::vec4 Window::get_clear_color() const
{
    return this->clear_color;
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
            throw std::runtime_error("invalid shader name");
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
              throw std::runtime_error("invalid vertex array name");
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
        throw std::runtime_error("invalid texture name");
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
    if (this->frame_buffer == this->frame_buffers.end() || this->frame_buffer->first != name)
    {
        this->frame_buffer = this->frame_buffers.find(name);
        if (this->frame_buffer == this->frame_buffers.end())
              throw std::runtime_error("invalid frame buffer name");
        else
            glBindFramebuffer(GL_READ_FRAMEBUFFER, this->frame_buffer->second.get_id());
    }

    return this->frame_buffer->second;
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
    if (std::get<0>(this->blend_factors) == source && std::get<1>(this->blend_factors) == destination)
        return;

    glBlendFunc(static_cast<GLenum>(source), static_cast<GLenum>(destination));
    this->blend_factors = std::make_tuple(source, destination);
}

void Window::set_clear_color(const glm::vec4 &color)
{
    if (this->clear_color == color)
        return;

    glClearColor(color.r, color.g, color.b, color.a);
    this->clear_color = color;
}

void Window::set_target_frame_buffer(const std::string &name, const std::vector<unsigned int> &color_attachment_indices)
{
    if (name == this->window_frame_buffer_name)
    {
        if (this->target_frame_buffer == this->frame_buffers.end() && color_attachment_indices.size() == 1 && color_attachment_indices[0] == 0)
            return;
        else if (color_attachment_indices.size() != 1 || (color_attachment_indices.size() >= 1 && color_attachment_indices[0] != 0))
            throw std::runtime_error("invalid frame buffer color attachment");
        else
        {
            this->target_frame_buffer = this->frame_buffers.end();
            this->target_frame_buffer_color_attachments = {GL_COLOR_ATTACHMENT0};
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
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
            throw std::runtime_error("invalid frame buffer name");
        else if (*std::max_element(color_attachment_indices.begin(), color_attachment_indices.end()) > frame_buffer->second.get_num_color_attachments()-1)
            throw std::runtime_error("invalid frame buffer color attachment");
        else
        {
            this->target_frame_buffer = frame_buffer;
            this->target_frame_buffer_color_attachments = color_attachments;
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, frame_buffer->second.get_id());
            glDrawBuffers(color_attachments.size(), color_attachments.data());
        }
    }
}

void Window::add_shader(const std::string &name, const std::vector<std::pair<std::string, ShaderType> > &shaders, const std::vector<std::string> &transform_feedback_varying_names)
{
    this->shaders.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(shaders, transform_feedback_varying_names));
}

void Window::add_vertex_array(const std::string &name, const DrawMode draw_mode)
{
    this->vertex_arrays.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(draw_mode));
}

void Window::add_texture(const std::string &name, const std::vector<float> &data, const std::array<unsigned int, 2> &size, const TextureFormat format, const TextureInterpolation interpolation, const TextureWrap wrap)
{
    this->textures.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(data, size, format, interpolation, wrap));
}

void Window::add_frame_buffer(const std::string &name, const std::array<unsigned int, 2> &size, const std::vector<TextureFormat> &color_attachment_texture_formats, const bool has_depth_attachment)
{
    if (name == this->window_frame_buffer_name)
        throw std::runtime_error("invalid frame buffer name");

    this->frame_buffers.emplace(std::piecewise_construct, std::forward_as_tuple(name), std::forward_as_tuple(size, color_attachment_texture_formats, has_depth_attachment));
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

void Window::clear(const bool color, const bool depth)
{
    if (!color && !depth)
        return;

    glClear((color ? GL_COLOR_BUFFER_BIT : 0) | (depth ? GL_DEPTH_BUFFER_BIT : 0));
}

void Window::draw()
{
    if (this->target_frame_buffer != this->frame_buffers.end())
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

    SDL_GL_SwapWindow(this->sdl_window);

    if (this->target_frame_buffer != this->frame_buffers.end())
    {
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, this->target_frame_buffer->second.get_id());
        glDrawBuffers(this->target_frame_buffer_color_attachments.size(), this->target_frame_buffer_color_attachments.data());
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR)
        throw std::runtime_error("opengl error");
}

std::tuple<bool, std::string> Window::validate() const
{
    glValidateProgram(this->shader->second.get_id());

    int validate_status;
    glGetProgramiv(this->shader->second.get_id(), GL_VALIDATE_STATUS, &validate_status);

    return std::make_tuple(validate_status == GL_TRUE, program_info_log(this->shader->second.get_id()));
}
