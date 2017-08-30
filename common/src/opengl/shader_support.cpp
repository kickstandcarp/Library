#include <stdexcept>
#include <utility>
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

unsigned int attach_program(const std::vector<unsigned int> &shader_ids)
{
	unsigned int id = glCreateProgram();

    for (auto const &shader_id : shader_ids)
        glAttachShader(id, shader_id);

    return id;
}

void assign_transform_feedback_varyings(const unsigned int program_id, const std::vector<std::string> &varying_names)
{
	std::vector<const char*> varyings;
	for (auto const &name : varying_names)
		varyings.push_back(name.c_str());

	glTransformFeedbackVaryings(program_id, varyings.size(), varyings.data(), GL_SEPARATE_ATTRIBS);
}

void link_program(const unsigned int program_id)
{
	glLinkProgram(program_id);

	int result = GL_FALSE;
	glGetProgramiv(program_id, GL_LINK_STATUS, &result);
	if (result == GL_FALSE)
	{
		std::stringstream what;
		what << "shader link error:" << std::endl << program_info_log(program_id);
		throw std::runtime_error(what.str());
	}
}

std::string shader_info_log(const unsigned int id)
{
	std::string info_log;

    int size;
    glGetShaderiv(id, GL_INFO_LOG_LENGTH, &size);
	if (size > 0)
	{
		char *info_log_c_str = new char[size];
		glGetShaderInfoLog(id, size, NULL, info_log_c_str);
		info_log = info_log_c_str;
		delete [] info_log_c_str;
	}

    return info_log;
}

std::string program_info_log(const unsigned int id)
{
	std::string info_log;

    int size;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &size);

	if (size > 0)
	{
		char *info_log_c_str = new char[size];
		glGetProgramInfoLog(id, size, NULL, info_log_c_str);
		info_log = info_log_c_str;
		delete [] info_log_c_str;
	}

    return info_log;
}

std::map<std::string, AttributeInformation> attribute_information(const unsigned int id)
{
	std::map<std::string, AttributeInformation> information;

	int num_attributes;
	glGetProgramiv(id, GL_ACTIVE_ATTRIBUTES, &num_attributes);

	int size;
	GLenum type;
	char name_c_str[1024];
	for (int i = 0; i < num_attributes; i++)
	{
		glGetActiveAttrib(id, i, 1024, NULL, &size, &type, name_c_str);
		std::string name = std::string(name_c_str);

		int location = glGetAttribLocation(id, name.c_str());
		if (location >= 0)
			information.insert(std::make_pair(name, AttributeInformation{static_cast<unsigned int>(location), static_cast<unsigned int>(size), type}));
	}

	return information;
}

std::map<std::string, AttributeInformation> transform_feedback_varying_information(const unsigned int id)
{
	std::map<std::string, AttributeInformation> information;

	int num_varyings;
	glGetProgramiv(id, GL_TRANSFORM_FEEDBACK_VARYINGS, &num_varyings);

	int size;
	GLenum type;
	char name_c_str[1024];
	for (int i = 0; i < num_varyings; i++)
	{
		glGetTransformFeedbackVarying(id, i, 1024, NULL, &size, &type, name_c_str);
		std::string name = std::string(name_c_str);
		information.insert(std::make_pair(name, AttributeInformation{static_cast<unsigned int>(i), static_cast<unsigned int>(size), type}));
	}

	return information;
}
