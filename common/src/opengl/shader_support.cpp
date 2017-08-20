#include <stdexcept>
#include <sstream>

#include "opengl/shader_support.hpp"



unsigned int compile_shader(const std::string &text, const GLenum shader_type)
{
	unsigned int  id = glCreateShader(shader_type);

    const char* text_ptr = text.c_str();
    glShaderSource(id, 1, &text_ptr, NULL);
    glCompileShader(id);

    int result = GL_FALSE;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        std::string shader_type_string;
        switch (shader_type)
        {
			case GL_VERTEX_SHADER:
				shader_type_string = "vertex";
				break;
			case GL_GEOMETRY_SHADER:
                shader_type_string = "geometry";
                break;
            case GL_FRAGMENT_SHADER:
                shader_type_string = "fragment";
                break;
        }

		std::stringstream what;
		what << shader_type_string << " shader compile error:" << std::endl << shader_info_log(id);
        throw std::runtime_error(what.str());
    }

    return id;
}

unsigned int link_shader(const std::vector<unsigned int> &shader_ids)
{
	unsigned int id = glCreateProgram();

    for (auto const &shader_id : shader_ids)
        glAttachShader(id, shader_id);
    glLinkProgram(id);

    int result = GL_FALSE;
    glGetProgramiv(id, GL_LINK_STATUS, &result);
    if (result == GL_FALSE)
    {
		std::stringstream what;
		what << "shader link error:" << std::endl << program_info_log(id);
        throw std::runtime_error(what.str());
    }

    return id;
}

std::string shader_info_log(const unsigned int id)
{
    int size;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size);

    std::string info_log(size, '\0');
    glGetShaderInfoLog(id, size, NULL, &info_log[0]);

    return info_log;
}

std::string program_info_log(const unsigned int id)
{
    int size;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &size);

    std::string info_log(size, '\0');
    glGetProgramInfoLog(id, size, NULL, &info_log[0]);

    return info_log;
}
