#ifndef OPENGL_SHADER_BUILD_SUPPORT_HPP
#define OPENGL_SHADER_BUILD_SUPPORT_HPP

#include <string>
#include <vector>
#include <GL/glew.h>

unsigned int    compile_shader(const std::string &text, const GLenum shader_type);
unsigned int    link_shader(const std::vector<unsigned int> &shader_ids);

std::string     shader_info_log(const unsigned int id);
std::string     program_info_log(const unsigned int  id);

#endif
