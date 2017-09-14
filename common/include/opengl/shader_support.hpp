#ifndef OPENGL_SHADER_BUILD_SUPPORT_HPP
#define OPENGL_SHADER_BUILD_SUPPORT_HPP

#include <string>
#include <map>
#include <vector>
#include <GL/glew.h>

enum class ShaderType : GLenum { vertex = GL_VERTEX_SHADER, geometry = GL_GEOMETRY_SHADER, fragment = GL_FRAGMENT_SHADER };

struct AttributeInformation
{
	unsigned int    index;
	unsigned int	size;
	GLenum          type;
};

unsigned int									compile_shader(const std::string &text, const GLenum shader_type);
unsigned int									attach_program(const std::vector<unsigned int> &shader_ids);
void											assign_transform_feedback_varyings(const unsigned int program_id, const std::vector<std::string> &varying_names);
void											link_program(const unsigned int program_id);

std::string										shader_info_log(const unsigned int id);
std::string										program_info_log(const unsigned int  id);

std::map<std::string, AttributeInformation>     attribute_information(const unsigned int id);
std::map<std::string, AttributeInformation>     transform_feedback_varying_information(const unsigned int id);

#endif
