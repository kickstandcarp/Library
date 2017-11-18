#ifndef OPENGL_VERTEX_ARRAY_HPP
#define OPENGL_VERTEX_ARRAY_HPP

#include <memory>
#include <utility>
#include <algorithm>
#include <string>
#include <map>
#include <vector>
#include <GL/glew.h>
#include "opengl/buffer.hpp"
#include "opengl/vertex_array_support.hpp"

class VertexArray
{
    public:
        VertexArray(const DrawMode draw_mode);
        ~VertexArray();

        unsigned int                                        get_id() const;
        unsigned int                                        get_num_vertices() const;
        std::vector<std::string>                            get_buffer_names() const;
        unsigned int                                        get_buffer_size(const std::string &name) const;
        unsigned int                                        get_element_buffer_size() const;
        GLenum                                              get_buffer_type(const std::string &name) const;
        unsigned int                                        get_buffer_dimension(const std::string &name) const;
        int                                                 get_buffer_attribute_index(const std::string &name) const;
        int                                                 get_buffer_transform_feedback_varying_index(const std::string &name) const;
        template <class T> T                                get_buffer(const std::string &name, const unsigned int offset) const;
        template <class T> std::vector<T>                   get_buffer(const std::string &name, const unsigned int offset, const unsigned int size) const;
        unsigned int                                        get_element_buffer(const unsigned int offset) const;
        std::vector<unsigned int>                           get_element_buffer(const unsigned int offset, const unsigned int size) const;

        void                                                set_buffer_attribute_index(const std::string &name, const int index);
        void                                                set_buffer_transform_feedback_varying_index(const std::string &name, const int index);
        template <class T> void                             set_buffer(const std::string &name, const std::vector<T> &data, const unsigned int offset);
        template <class T> void                             set_buffer(const std::string &name, const T &data, const unsigned int offset);
        void                                                set_element_buffer(const unsigned int data, const unsigned int offset);
        void                                                set_element_buffer(const std::vector<unsigned int> &data, const unsigned int offset);

        template <class T> void                             add_buffer(const std::string &name, const std::vector<T> &data, const BufferUsageFrequency frequency=BufferUsageFrequency::statical, const BufferUsageAccess access=BufferUsageAccess::draw);
        void                                                add_element_buffer(const std::vector<unsigned int> &data, const BufferUsageFrequency frequency=BufferUsageFrequency::statical, const BufferUsageAccess access=BufferUsageAccess::draw);

        void                                                remove_buffer(const std::string &name);
        void                                                remove_element_buffer();

        void                                                draw(const unsigned int count=1, const bool transform_feedback=false) const;

        DrawMode                                            draw_mode;

    private:
        GLenum                                              transform_feedback_primitive_mode() const;

        unsigned int                                        id;

        unsigned int                                        num_vertices;

        std::shared_ptr<Buffer>                             element_buffer;
        std::map<std::string, VertexArrayBufferInformation> buffers;
};



template <class T>
T VertexArray::get_buffer(const std::string &name, const unsigned int offset) const
{
    return this->buffers.at(name).buffer->get<T>(offset, false);
}

template <class T>
std::vector<T> VertexArray::get_buffer(const std::string &name, const unsigned int offset, const unsigned int size) const
{
    return this->buffers.at(name).buffer->get<T>(offset, size, false);
}

template <class T>
void VertexArray::set_buffer(const std::string &name, const T &data, const unsigned int offset)
{
    this->buffers.at(name).buffer->set(data, offset, false);
}

template <class T>
void VertexArray::set_buffer(const std::string &name, const std::vector<T> &data, const unsigned int offset)
{
    this->buffers.at(name).buffer->set(data, offset, false);
}

template <class T>
void VertexArray::add_buffer(const std::string &name, const std::vector<T> &data, const BufferUsageFrequency frequency, const BufferUsageAccess access)
{
    this->buffers.insert(std::make_pair(name, VertexArrayBufferInformation{std::make_shared<Buffer>(), -1, -1}));
    this->buffers.at(name).buffer->initialize(data, frequency, access);

    this->num_vertices = this->buffers.size() == 1 ? this->buffers.at(name).buffer->get_size() : std::min(this->num_vertices, this->buffers.at(name).buffer->get_size());
}

#endif
