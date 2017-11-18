#ifndef OPENGL_VERTEX_ARRAY_SUPPORT_HPP
#define OPENGL_VERTEX_ARRAY_SUPPORT_HPP

enum class DrawMode : GLenum { points = GL_POINTS, line_strip = GL_LINE_STRIP, line_loop = GL_LINE_LOOP, lines = GL_LINES, line_strip_adjacency = GL_LINE_STRIP_ADJACENCY, lines_adjacency = GL_LINES_ADJACENCY, triangle_strip = GL_TRIANGLE_STRIP, triangle_fan = GL_TRIANGLE_FAN, triangles = GL_TRIANGLES, triangle_strip_adjacency = GL_TRIANGLE_STRIP_ADJACENCY, triangles_adjacency = GL_TRIANGLES_ADJACENCY }; 

struct VertexArrayBufferInformation
{
    std::shared_ptr<Buffer> buffer;

    int                     attribute_index;
    int                     transform_feedback_varying_index;
};

#endif
