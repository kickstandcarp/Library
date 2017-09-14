#ifndef OPENGL_VERTEX_ARRAY_SUPPORT_HPP
#define OPENGL_VERTEX_ARRAY_SUPPORT_HPP

enum class DrawMode : GLenum { points = GL_POINTS, line_strip = GL_LINE_STRIP, line_loop = GL_LINE_LOOP, lines = GL_LINES, triangle_strip = GL_TRIANGLE_STRIP, triangle_fan = GL_TRIANGLE_FAN, triangles = GL_TRIANGLES }; 

struct VertexArrayBufferInformation
{
	std::shared_ptr<Buffer> buffer;

	int                     attribute_index;
	int                     transform_feedback_varying_index;
};

#endif
