from math import ceil, exp
from itertools import accumulate

from glm import ivec2, vec2, vec3, vec4, quat, distance
from opengl import BlendFactor, ShaderType, DrawMode
from coordinate import CoordinateTransform



class VectorDisplay(object):
    def __init__(self, window):
        super(VectorDisplay, self).__init__()

        self.excite_shader_name = 'vector_display_excite'
        self.decay_shader_name = 'vector_display_decay'
        self.glow_1_shader_name = 'vector_display_glow_1'
        self.glow_2_shader_name = 'vector_display_glow_2'

        self.vertex_array_name = 'window'

        self.frame_buffer_name = 'vector_display'
        self.current_value_color_attachment = 0
        self.next_value_color_attachment = 1

        self.beam_width = 0.002
        self.beam_velocity = 30.0
        self.beam_intensity = 3.0

        self.excitation_time_constant = 0.001
        self.decay_time_constant = 1.0

        self.glow_size = 31
        self.glow_standard_deviation = 5.0
        self.glow_amplitude = 2.0

        self.scan_distance = 0.0
        self.total_scan_distance = 0.0

        self.lines = {}

        window.add_shader(self.excite_shader_name, [(read_text('vector_display_excite.vert'), ShaderType.vertex), (read_text('vector_display_excite.frag'), ShaderType.fragment)])
        window.add_shader(self.decay_shader_name, [(read_text('vector_display_window.vert'), ShaderType.vertex), (read_text('vector_display_decay.frag'), ShaderType.fragment)])
        window.add_shader(self.glow_1_shader_name, [(read_text('vector_display_window.vert'), ShaderType.vertex), (read_text('vector_display_glow_1.frag'), ShaderType.fragment)])
        window.add_shader(self.glow_2_shader_name, [(read_text('vector_display_window.vert'), ShaderType.vertex), (read_text('vector_display_glow_2.frag'), ShaderType.fragment)])

        window.add_frame_buffer(self.frame_buffer_name, window.size, 3, False) # 2
        window.set_target_frame_buffer(self.frame_buffer_name, list(range(window.frame_buffers(self.frame_buffer_name).num_color_attachments)))
        window.clear()

        self.i = 0

    def add_line(self, window, name, vertices):
        lengths = [distance(vertices[i], vertices[(i+1) % len(vertices)]) for i in range(len(vertices))]
        distances = [sum(lengths[:i]) if i != 0 else 0.0 for i in range(len(lengths))]

        window.add_vertex_array(name, DrawMode.lines)
        window.vertex_arrays(name).add_buffer('vertex_positions', [vec3(vertex.x, vertex.y, distance) for vertex, distance in zip(vertices, distances)])

        self.lines[name] = (CoordinateTransform(),)
        self.total_scan_distance = max(distances)

    def excitation_iterations(self, elapsed_time):
        return int(ceil((elapsed_time*self.beam_velocity) / (0.5*self.beam_width)))

    def max_excitation(self, elapsed_time):
        if self.beam_velocity == 0.0:
            return self.beam_intensity

        excitation_function = lambda dt, v, distance: max(0.0, (self.beam_intensity*exp(-((distance - dt*self.beam_velocity)**2) / (2.0*self.beam_width*self.beam_width)) - v) / self.excitation_time_constant)
        decay_function = lambda dt, v: -v / self.decay_time_constant

        value, max_value = 0.0, 0.0
        distance = -5.0*self.beam_width
        iterations = self.excitation_iterations(elapsed_time)
        while distance < 5.0*self.beam_width:
            for i in range(iterations):
                value = runge_kutta(excitation_function, elapsed_time / float(iterations), value, (distance,))
                distance += (elapsed_time / float(iterations))*self.beam_velocity
            value = runge_kutta(decay_function, elapsed_time, value)
            max_value = max(max_value, value)

        return max_value

    def glow_point_excitation_ratio(self):
        return 1.0 / sum([self.glow_amplitude*exp(-float((x - (self.glow_size // 2))*(x - (self.glow_size // 2)) + (y - (self.glow_size // 2))* (y - (self.glow_size // 2))) / (2.0*self.glow_standard_deviation*self.glow_standard_deviation)) for x in range(self.glow_size) for y in range(self.glow_size)])

    def glow_line_excitation_ratio(self):
        return 1.0 / sum([self.glow_amplitude*exp(-float((x - (self.glow_size // 2))*(x - (self.glow_size // 2))) / (2.0*self.glow_standard_deviation*self.glow_standard_deviation)) for x in range(self.glow_size)])

    def update(self, elapsed_time):
        pass

    def draw(self, window, final_frame_buffer_name, final_frame_buffer_color_attachment, camera, elapsed_time):
        window.use_blending = False
        window.copy_frame_buffer('vector_display', self.current_value_color_attachment, 'vector_display', self.next_value_color_attachment)
        window.set_target_frame_buffer(self.frame_buffer_name, [self.next_value_color_attachment,])
        window.shaders(self.excite_shader_name).set_uniform('iterations', self.excitation_iterations(elapsed_time))
        window.shaders(self.excite_shader_name).set_uniform('elapsed_time', elapsed_time)
        window.shaders(self.excite_shader_name).set_uniform('scan_distance', self.scan_distance)
        window.shaders(self.excite_shader_name).set_uniform('beam_width', self.beam_width)
        window.shaders(self.excite_shader_name).set_uniform('beam_velocity', self.beam_velocity)
        window.shaders(self.excite_shader_name).set_uniform('beam_intensity', self.beam_intensity)
        window.shaders(self.excite_shader_name).set_uniform('time_constant', self.excitation_time_constant)
        window.frame_buffers(self.frame_buffer_name).get_color_texture(self.current_value_color_attachment).set_texture_unit(0)
        window.shaders(self.excite_shader_name).set_uniform('current_value_sampler', 0)
        for vertex_array_name, (coordinate_transform,) in self.lines.items():
            window.shaders(self.excite_shader_name).set_attribute('vertex_position', window.vertex_arrays(vertex_array_name), 'vertex_positions')
            window.shaders(self.excite_shader_name).set_uniform('transform_matrix', camera.transform_matrix*coordinate_transform.global_matrix)
            window.vertex_arrays(vertex_array_name).draw()

        self.scan_distance = (self.scan_distance + (elapsed_time*self.beam_velocity)) % self.total_scan_distance

        window.set_target_frame_buffer(self.frame_buffer_name, [self.current_value_color_attachment])
        window.shaders(self.decay_shader_name).set_attribute('vertex_position', window.vertex_arrays(self.vertex_array_name), 'vertex_positions')
        window.shaders(self.decay_shader_name).set_uniform('elapsed_time', elapsed_time)
        window.shaders(self.decay_shader_name).set_uniform('time_constant', self.decay_time_constant)
        window.frame_buffers(self.frame_buffer_name).get_color_texture(self.next_value_color_attachment).set_texture_unit(0)
        window.shaders(self.decay_shader_name).set_uniform('current_value_sampler', 0)
        window.vertex_arrays(self.vertex_array_name).draw()

        window.set_target_frame_buffer(self.frame_buffer_name, [self.next_value_color_attachment])
        window.clear()

        window.shaders(self.glow_1_shader_name).set_uniform('size', self.glow_size)
        window.shaders(self.glow_1_shader_name).set_uniform('standard_deviation', self.glow_standard_deviation)
        window.shaders(self.glow_1_shader_name).set_uniform('amplitude', self.glow_amplitude)
        window.frame_buffers(self.frame_buffer_name).get_color_texture(self.current_value_color_attachment).set_texture_unit(0)
        window.shaders(self.glow_1_shader_name).set_uniform('color_sampler', 0)
        window.vertex_arrays(self.vertex_array_name).draw()

        window.set_target_frame_buffer(final_frame_buffer_name, [final_frame_buffer_color_attachment])
        # window.set_target_frame_buffer(self.frame_buffer_name, [2])
        # window.clear()

        window.use_blending = True
        window.blend_factors = BlendFactor.one, BlendFactor.one

        window.shaders(self.glow_2_shader_name).set_uniform('size', self.glow_size)
        window.shaders(self.glow_2_shader_name).set_uniform('standard_deviation', self.glow_standard_deviation)
        window.frame_buffers(self.frame_buffer_name).get_color_texture(self.next_value_color_attachment).set_texture_unit(0)
        window.shaders(self.glow_2_shader_name).set_uniform('intermediate_color_sampler', 0)
        window.frame_buffers(self.frame_buffer_name).get_color_texture(self.current_value_color_attachment).set_texture_unit(1)
        window.shaders(self.glow_2_shader_name).set_uniform('source_color_sampler', 1)
        window.vertex_arrays(self.vertex_array_name).draw()

        '''
        if self.i >= 20:
            from numpy import array, clip, max
            from matplotlib.pylab import show, figure, imshow, colorbar

            figure()
            image = window.frame_buffers(self.frame_buffer_name).get_color_texture(0).get_data()[:, :, 0]
            imshow(image)
            colorbar()
            print(max(image))


            figure()
            image = window.frame_buffers(self.frame_buffer_name).get_color_texture(1).get_data()[:, :, 0]
            imshow(image)
            colorbar()
            print(max(image))

            figure()
            image = window.frame_buffers(self.frame_buffer_name).get_color_texture(2).get_data()[:, :, 0]
            imshow(image)
            colorbar()
            print(max(image))

            show()

        self.i += 1
        '''

def read_text(file_name):
    with open(file_name) as file:
        return file.read()

def runge_kutta(dv_dt, dt, v, arguments=()):
    k1 = dv_dt(0.0, v, *arguments)
    k2 = dv_dt(0.5*dt, v + 0.5*dt*k1, *arguments)
    k3 = dv_dt(0.5*dt, v + 0.5*dt*k2, *arguments)
    k4 = dv_dt(dt, v + dt*k3, *arguments)
    return v + (dt / 6.0)*(k1 + 2.0*k2 + 2.0*k3 + k4)
