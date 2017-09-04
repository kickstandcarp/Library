from math import ceil, floor, sqrt, exp

from matplotlib.cm import get_cmap

from glm import bvec2, vec2, vec3, quat, clamp, dot, normalize, distance, mix, rotate
from opengl import BlendFactor, ShaderType, DrawMode
from coordinate import CoordinateTransform



class Path(object):
    def __init__(self, vertices):
        super(Path, self).__init__()

        lengths = [distance(vertex, next_vertex) for vertex, next_vertex in zip(vertices, vertices[1:] + vertices[:1])]
        distances = [sum(lengths[:i]) if i != 0 else 0.0 for i in range(len(lengths))]

        self._vertices = [vec3(vertex.x, vertex.y, distance) for vertex, distance in zip(vertices, distances)]
        self._vertices.sort(key=lambda item: item.z)

        self.angle = 0.0
        self.angular_velocity = 2.0

    @property
    def length(self):
        return self._vertices[-1].z

    # def transform_vertex(self, vertex):
    #     pass

    def vertices(self, begin_distance, end_distance, velocity):
        vertices = []

        if begin_distance < self._vertices[0].z and end_distance >= self._vertices[0].z:
            vertices.append(self._vertices[0])

        for vertex, next_vertex in zip(self._vertices[:-1], self._vertices[1:]):
            if begin_distance >= vertex.z and begin_distance < next_vertex.z:
                vertices.append(mix(vertex, next_vertex, vec3((begin_distance - vertex.z) / (next_vertex.z - vertex.z))))
            if vertex.z > begin_distance and vertex.z < end_distance:
                vertices.append(vertex)
            if end_distance >= vertex.z and end_distance < next_vertex.z:
                vertices.append(mix(vertex, next_vertex, vec3((end_distance - vertex.z) / (next_vertex.z - vertex.z))))

        if end_distance >= self._vertices[-1].z and begin_distance < self._vertices[-1].z:
            vertices.append(self._vertices[-1])

        transformed_vertices = [vec3(rotate(vec2(vertex.x, vertex.y), self.angle + self.angular_velocity*((vertex.z - begin_distance) / velocity)), vertex.z) for vertex in vertices]

        for vertex, next_vertex, transformed_vertex, next_transformed_vertex in zip(vertices[:-1], vertices[1:], transformed_vertices[:-1], transformed_vertices[1:]):
            subdivisions = floor(distance(transformed_vertex, next_transformed_vertex) / (next_vertex.z - vertex.z)) if next_vertex.z - vertex.z > 0.0 else 0
            for i in range(subdivisions):
                new_vertex = mix(vertex, next_vertex, vec3((i+1) / (subdivisions+1)))
                new_transformed_vertex = vec3(rotate(vec2(new_vertex.x, new_vertex.y), self.angle + self.angular_velocity*((new_vertex.z - begin_distance) / velocity)), new_vertex.z)
                transformed_vertices.append(new_transformed_vertex)
        transformed_vertices.sort(key=lambda item: item.z)

        self.angle += self.angular_velocity*((end_distance - begin_distance) / velocity)

        return transformed_vertices

class VectorDisplay(object):
    def __init__(self, window):
        super(VectorDisplay, self).__init__()

        self.excitation_shader_name = 'vector_display_excite'
        self.decay_shader_name = 'vector_display_decay'
        self.glow_1_shader_name = 'vector_display_glow_1'
        self.glow_2_shader_name = 'vector_display_glow_2'

        self.vertex_array_name = 'window'

        self.frame_buffer_name = 'vector_display'
        self.current_value_color_attachment = 0
        self.next_value_color_attachment = 1

        self.beam_width = 0.0025
        self.beam_velocity = 2.0
        self.beam_intensity = 30.0
        self.beam_color = vec3(1.0, 1.0, 1.0)

        self.excitation_time_constant = 0.01
        self.decay_time_constant_1 = 1.0
        self.decay_threshold_1 = 1.0
        self.decay_time_constant_2 = 100.0
        self.decay_threshold_2 = 0.5 / 255.0

        self.glow_size = 100
        self.glow_width = 15.0
        self.glow_intensity = 3.0

        self.scan_distance = 0.0

        self.paths = []

        window.add_shader(self.excitation_shader_name, [(read_text('vector_display_window.vert'), ShaderType.vertex), (read_text('vector_display_excite.frag'), ShaderType.fragment)])
        window.add_shader(self.decay_shader_name, [(read_text('vector_display_window.vert'), ShaderType.vertex), (read_text('vector_display_decay.frag'), ShaderType.fragment)])
        window.add_shader(self.glow_1_shader_name, [(read_text('vector_display_window.vert'), ShaderType.vertex), (read_text('vector_display_glow_1.frag'), ShaderType.fragment)])
        window.add_shader(self.glow_2_shader_name, [(read_text('vector_display_window.vert'), ShaderType.vertex), (read_text('vector_display_glow_2.frag'), ShaderType.fragment)])

        window.add_frame_buffer(self.frame_buffer_name, window.size, 3, False) # 2
        window.set_target_frame_buffer(self.frame_buffer_name, list(range(window.frame_buffers(self.frame_buffer_name).num_color_attachments)))
        window.clear()

        self.color_map_index = 0.0

        self.i = 0

    def update(self, elapsed_time):
        pass

    def draw(self, window, final_frame_buffer_name, final_frame_buffer_color_attachment, camera, elapsed_time):
        self.beam_color = vec3(get_cmap('spectral')(1.0 - abs (self.color_map_index - 1.0))[:3])
        self.color_map_index = (self.color_map_index + 0.3*elapsed_time) % 2.0

        begin_distance = self.scan_distance
        end_distance = self.scan_distance + elapsed_time*self.beam_velocity
        if end_distance > self.paths[0].length:
            vertices = self.paths[0].vertices(begin_distance, self.paths[0].length, self.beam_velocity)
            vertices += [vec3(vertex.x, vertex.y, vertex.z + self.paths[0].length) for vertex in self.paths[0].vertices(0.0, end_distance % self.paths[0].length, self.beam_velocity)[1:]]
            end_distance = end_distance % self.paths[0].length
        else:
            vertices = self.paths[0].vertices(begin_distance, end_distance, self.beam_velocity)

        length = sum([distance(vertex, next_vertex) for vertex, next_vertex in zip(vertices[:-1], vertices[1:])])
        iterations = int(ceil(length / self.beam_width))

        window.use_blending = False
        window.copy_frame_buffer('vector_display', self.current_value_color_attachment, 'vector_display', self.next_value_color_attachment)
        window.set_target_frame_buffer(self.frame_buffer_name, [self.next_value_color_attachment,])
        window.shaders(self.excitation_shader_name).set_attribute('vertex_position', window.vertex_arrays(self.vertex_array_name), 'vertex_positions')
        window.shaders(self.excitation_shader_name).set_uniform('num_vertices', len(vertices))
        window.shaders(self.excitation_shader_name).set_uniform('vertices', vertices)
        window.shaders(self.excitation_shader_name).set_uniform('iterations', iterations)
        window.shaders(self.excitation_shader_name).set_uniform('scan_distance', self.scan_distance)
        window.shaders(self.excitation_shader_name).set_uniform('elapsed_time', elapsed_time)
        window.frame_buffers(self.frame_buffer_name).get_color_texture(self.current_value_color_attachment).set_texture_unit(0)
        window.shaders(self.excitation_shader_name).set_uniform('value_sampler', 0)
        window.shaders(self.excitation_shader_name).set_uniform('beam_width', self.beam_width)
        window.shaders(self.excitation_shader_name).set_uniform('beam_velocity', self.beam_velocity)
        window.shaders(self.excitation_shader_name).set_uniform('beam_intensity', self.beam_intensity)
        window.shaders(self.excitation_shader_name).set_uniform('beam_color', self.beam_color)
        window.shaders(self.excitation_shader_name).set_uniform('time_constant', self.excitation_time_constant)
        window.vertex_arrays(self.vertex_array_name).draw()

        window.set_target_frame_buffer(self.frame_buffer_name, [self.current_value_color_attachment])
        window.shaders(self.decay_shader_name).set_attribute('vertex_position', window.vertex_arrays(self.vertex_array_name), 'vertex_positions')
        window.shaders(self.decay_shader_name).set_uniform('elapsed_time', elapsed_time)
        window.frame_buffers(self.frame_buffer_name).get_color_texture(self.next_value_color_attachment).set_texture_unit(0)
        window.shaders(self.decay_shader_name).set_uniform('value_sampler', 0)
        window.shaders(self.decay_shader_name).set_uniform('time_constant_1', self.decay_time_constant_1)
        window.shaders(self.decay_shader_name).set_uniform('threshold_1', self.decay_threshold_1)
        window.shaders(self.decay_shader_name).set_uniform('time_constant_2', self.decay_time_constant_2)
        window.shaders(self.decay_shader_name).set_uniform('threshold_2', self.decay_threshold_2)
        window.vertex_arrays(self.vertex_array_name).draw()

        window.set_target_frame_buffer(self.frame_buffer_name, [self.next_value_color_attachment])
        window.clear()
        window.shaders(self.glow_1_shader_name).set_attribute('vertex_position', window.vertex_arrays(self.vertex_array_name), 'vertex_positions')
        window.shaders(self.glow_1_shader_name).set_uniform('size', self.glow_size)
        window.shaders(self.glow_1_shader_name).set_uniform('width', self.glow_width)
        window.shaders(self.glow_1_shader_name).set_uniform('intensity', self.glow_intensity)
        window.frame_buffers(self.frame_buffer_name).get_color_texture(self.current_value_color_attachment).set_texture_unit(0)
        window.shaders(self.glow_1_shader_name).set_uniform('color_sampler', 0)
        window.vertex_arrays(self.vertex_array_name).draw()

        window.use_blending = True
        window.blend_factors = BlendFactor.one, BlendFactor.one
        window.set_target_frame_buffer(final_frame_buffer_name, [final_frame_buffer_color_attachment])
        # window.set_target_frame_buffer(self.frame_buffer_name, [2])
        # window.clear()
        window.shaders(self.glow_2_shader_name).set_attribute('vertex_position', window.vertex_arrays(self.vertex_array_name), 'vertex_positions')
        window.shaders(self.glow_2_shader_name).set_uniform('size', self.glow_size)
        window.shaders(self.glow_2_shader_name).set_uniform('width', self.glow_width)
        window.frame_buffers(self.frame_buffer_name).get_color_texture(self.next_value_color_attachment).set_texture_unit(0)
        window.shaders(self.glow_2_shader_name).set_uniform('intermediate_color_sampler', 0)
        window.frame_buffers(self.frame_buffer_name).get_color_texture(self.current_value_color_attachment).set_texture_unit(1)
        window.shaders(self.glow_2_shader_name).set_uniform('source_color_sampler', 1)
        window.vertex_arrays(self.vertex_array_name).draw()

        self.scan_distance = end_distance

        '''
        if self.i >= 10:
            from numpy import array, clip, max
            from matplotlib.pylab import show, figure, imshow, colorbar

            figure()
            image = window.frame_buffers(self.frame_buffer_name).get_color_texture(0).get_data()[:, :, 0]
            imshow(image)
            colorbar()
            print(min(image.flat), max(image.flat))

            figure()
            image = window.frame_buffers(self.frame_buffer_name).get_color_texture(1).get_data()[:, :, 0]
            imshow(image)
            colorbar()
            print(min(image.flat), max(image.flat))

            figure()
            image = window.frame_buffers(self.frame_buffer_name).get_color_texture(2).get_data()[:, :, 0]
            imshow(image)
            colorbar()
            print(min(image.flat), max(image.flat))

            show()
        '''

        self.i += 1

def read_text(file_name):
    with open(file_name) as file:
        return file.read()

def runge_kutta(dv_dt, dt, v, arguments=()):
    k1 = dv_dt(0.0, v, *arguments)
    k2 = dv_dt(0.5*dt, v + 0.5*dt*k1, *arguments)
    k3 = dv_dt(0.5*dt, v + 0.5*dt*k2, *arguments)
    k4 = dv_dt(dt, v + dt*k3, *arguments)
    return v + (dt / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4)
