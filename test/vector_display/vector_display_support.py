from os.path import join
from math import ceil
from itertools import accumulate

from numpy import linspace
from scipy.special import erfc

from glm import vec2, vec3, vec4, distance
from opengl import BlendFactor, ShaderType, DrawMode
from coordinate import CoordinateTransform



class VectorDisplay(object):
    def __init__(self, window):
        super(VectorDisplay, self).__init__()

        self.shader_name = 'vector_display_accumulate'
        self.vertex_array_name = 'window'
        self.frame_buffer_name = 'vector_display'

        window.add_shader(self.shader_name, [(read_text(join('glsl', 'vector_display_accumulate.vert')), ShaderType.vertex), (read_text(join('glsl', 'vector_display_accumulate.frag')), ShaderType.fragment)])

        window.add_frame_buffer(self.frame_buffer_name, window.size, 3, False)

        self.forcing_value_color_attachment = 0
        self.current_value_color_attachment = 1
        self.next_value_color_attachment = 2

        window.set_target_frame_buffer(self.frame_buffer_name, list(range(window.frame_buffers(self.frame_buffer_name).num_color_attachments)))
        window.clear()

        self.lines = []

        self.beam_width = 0.01
        self.beam_velocity = 10.0
        self.beam_intensity = 4000.0

        self.excitation_time_constant = 0.001
        self.decay_time_constant = 1.0

    def update(self):
        pass

    def draw(self, window, camera):
        elapsed_time = window.refresh_time # self.clock.elapsed_time

        window.use_blending = True
        window.blend_factors = BlendFactor.one, BlendFactor.one

        window.set_target_frame_buffer(self.frame_buffer_name, [self.forcing_value_color_attachment,])
        window.clear()

        for line in self.lines:
            line.draw(window, camera, elapsed_time, self.beam_width, self.beam_velocity, self.beam_intensity)

        window.use_blending = False

        window.set_target_frame_buffer(self.frame_buffer_name, [self.next_value_color_attachment])
        window.clear()

        window.shaders(self.shader_name).set_attribute('vertex_position', window.vertex_arrays(self.vertex_array_name), 'vertex_positions')

        window.frame_buffers(self.frame_buffer_name).get_color_texture(self.forcing_value_color_attachment).set_texture_unit(0)
        window.frame_buffers(self.frame_buffer_name).get_color_texture(self.current_value_color_attachment).set_texture_unit(1)

        window.shaders(self.shader_name).set_uniform('elapsed_time', elapsed_time)
        window.shaders(self.shader_name).set_uniform('excitation_time_constant', self.excitation_time_constant)
        window.shaders(self.shader_name).set_uniform('decay_time_constant', self.decay_time_constant)

        window.shaders(self.shader_name).set_uniform('forcing_value_texture', 0)
        window.shaders(self.shader_name).set_uniform('current_value_texture', 1)

        window.vertex_arrays(self.vertex_array_name).draw()

        self.current_value_color_attachment, self.next_value_color_attachment = self.next_value_color_attachment, self.current_value_color_attachment

        '''
        from numpy import array
        from matplotlib.pylab import show, figure, imshow, colorbar

        figure()
        imshow(window.frame_buffers(self.frame_buffer_name).get_color_texture(self.forcing_value_color_attachment).get_data()[:, :, 0])
        colorbar()

        show()
        '''

class VectorDisplayLine(object):
    def __init__(self, window, lines):
        super(VectorDisplayLine, self).__init__()

        self.shader_name = 'vector_display_scan'
        self.vertex_array_name = 'vector_square'

        window.add_shader(self.shader_name, [(read_text(join('glsl', 'vector_display_scan.vert')), ShaderType.vertex), (read_text(join('glsl', 'vector_display_scan.frag')), ShaderType.fragment)])

        vertices = list(sum(lines, []))
        vertex_distances = [distance(vertex, next_vertex) for vertex, next_vertex in zip(vertices, vertices[1:] + vertices[:1])]
        cumulative_vertex_distances = [sum(vertex_distances[:i]) if i != 0 else 0.0 for i in range(len(vertex_distances))]

        vertex_positions = [vec3(vertex.x, vertex.y, cumulative_vertex_distance) for vertex, cumulative_vertex_distance in zip(vertices, cumulative_vertex_distances)]

        window.add_vertex_array(self.vertex_array_name, DrawMode.lines)
        window.vertex_arrays(self.vertex_array_name).add_buffer('vertex_positions', vertex_positions)

        self.coordinate_transform = CoordinateTransform()

        self.total_distance = cumulative_vertex_distances[-1]
        self.elapsed_distance = 0.0

    def update(self):
        pass

    def draw(self, window, camera, elapsed_time, beam_width, beam_velocity, beam_intensity):
        # print(elapsed_time, self.elapsed_distance)

        window.shaders(self.shader_name).set_attribute('vertex_position', window.vertex_arrays(self.vertex_array_name), 'vertex_positions')

        window.shaders(self.shader_name).set_uniform('window_size', vec2(window.size))
        window.shaders(self.shader_name).set_uniform('transform_matrix', camera.transform_matrix*self.coordinate_transform.global_matrix)

        window.shaders(self.shader_name).set_uniform('elapsed_time', elapsed_time)
        window.shaders(self.shader_name).set_uniform('elapsed_distance', self.elapsed_distance)

        window.shaders(self.shader_name).set_uniform('beam_width', beam_width)
        window.shaders(self.shader_name).set_uniform('beam_velocity', beam_velocity)
        window.shaders(self.shader_name).set_uniform('beam_intensity', beam_intensity)

        window.vertex_arrays(self.vertex_array_name).draw()

        self.elapsed_distance += elapsed_time*beam_velocity

def read_text(file_name):
    with open(file_name) as file:
        return file.read()
