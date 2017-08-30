from sys import path
from os.path import pardir, split, join, realpath

path.append(join(split(realpath(__file__))[0], pardir, 'python'))
from glm import ivec2, ivec3, ivec4, vec2, vec3, vec4, quat, mat4x4
from opengl import Window, BlendFactor, ShaderType, BufferUsageFrequency, BufferUsageAccess, DrawMode, TextureInterpolation, TextureWrap



with open(join(split(realpath(__file__))[0], 'test.vert')) as file:
     vertex_shader_text = file.read()
with open(join(split(realpath(__file__))[0], 'test.frag')) as file:
     fragment_shader_text = file.read()

window = Window('OpenGL Square Test', (500, 500), True)
window.discard_rasterizer = True

window.add_shader('test', [(vertex_shader_text, ShaderType.vertex), (fragment_shader_text, ShaderType.fragment)], ['transformed_data_1', 'transformed_data_2'])
window.shaders('test').initialize_uniform_buffer('uniform_buffer', 1)

window.add_vertex_array('test', DrawMode.points)
window.vertex_arrays('test').add_element_buffer([0, 1])
window.vertex_arrays('test').add_buffer('data', [vec3(1.0), vec3(1.0)], BufferUsageFrequency.dynamic, BufferUsageAccess.copy)
window.vertex_arrays('test').add_buffer('transformed_data', [vec3(2.0), vec3(2.0)], BufferUsageFrequency.dynamic, BufferUsageAccess.copy)

window.shaders('test').set_attribute('data', window.vertex_arrays('test'), 'data')

window.shaders('test').set_transform_feedback_varying('transformed_data_1', window.vertex_arrays('test'), 'data')
window.shaders('test').set_transform_feedback_varying('transformed_data_2', window.vertex_arrays('test'), 'transformed_data')


print(window.shaders('test').attribute_names)
print(window.shaders('test').transform_feedback_varying_names)
print(window.shaders('test').uniform_buffer_names)
print(window.shaders('test').uniform_names)

window.vertex_arrays('test').set_buffer('data', vec3(-1.0), 1)
window.vertex_arrays('test').set_buffer('transformed_data', vec3(-2.0), 0)
window.shaders('test').set_uniform('uniform_vec3', vec3(3.0))
window.shaders('test').set_uniform('uniform_ivec2', [ivec2(4), ivec2(4), ivec2(4)])
window.shaders('test').set_uniform('uniform_ivec2', [ivec2(-4), ivec2(-4)], 1)
window.shaders('test').set_uniform('uniform_buffer_float_1', 5.0)
window.shaders('test').set_uniform('uniform_buffer_float_2', 6.0)
window.shaders('test').set_uniform('uniform_buffer_vec3', [vec3(7.0), vec3(-7.0)])

print(window.vertex_arrays('test').get_buffer('data'), window.vertex_arrays('test').get_buffer('data', 1, 1))
print(window.vertex_arrays('test').get_buffer('transformed_data'))
print(window.shaders('test').get_uniform('uniform_vec3'))
print(window.shaders('test').get_uniform('uniform_ivec2'), window.shaders('test').get_uniform('uniform_ivec2', 0, 2))
print(window.shaders('test').get_uniform('uniform_buffer_float_1'))
print(window.shaders('test').get_uniform('uniform_buffer_float_2'))
print(window.shaders('test').get_uniform('uniform_buffer_vec3'))


print(window.validate())
for i in range(3):
     window.vertex_arrays('test').draw(transform_feedback=True)

     print(i)
     print(window.vertex_arrays('test').get_buffer('data'))
     print(window.vertex_arrays('test').get_buffer('transformed_data'))
