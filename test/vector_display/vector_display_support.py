from math import ceil, sin, cos

from matplotlib.cm import get_cmap

from glm import bvec2, vec2, vec3, quat, clamp, dot, normalize, distance, mix, rotate
from opengl import BlendFactor, ShaderType, DrawMode
from coordinate import CoordinateTransform
from physics import OscillatorKinetics



class Path(object):
    def __init__(self):
        super(Path, self).__init__()

        self.scan_distance = 0.0

        self.kinetics = OscillatorKinetics(0.0, 10.0, 5.0, 0.1, 0.0) # value, velocity, frequency, damping_ratio, acceleration
        self.kinetic_path = [vec3(0.0), vec3(0.0)]

    def update(self, elapsed_time):
        self.kinetic_path = [vec3(self.kinetics.value, 0.0, -elapsed_time)]
        for i in range(4):
            self.kinetics.step(elapsed_time / 4)
            self.kinetic_path.append(vec3(self.kinetics.value, 0.0, self.kinetic_path[-1].z + (elapsed_time / 4)))

    def scan_vertex(self, scan_distance):
        pass

    def scan_vertices(self, scan_length, scan_velocity):
        pass

    def transform_vertex(self, vertex, scan_length, scan_velocity):
        elapsed_time = ((vertex.z - self.scan_distance) - scan_length) / scan_velocity

        if elapsed_time < self.kinetic_path[0].z:
            path_position = mix(self.kinetic_path[0], self.kinetic_path[1], vec3((elapsed_time - self.kinetic_path[0].z) / (self.kinetic_path[1].z - self.kinetic_path[0].z)))

        for path_vertex, next_path_vertex in zip(self.kinetic_path[:-1], self.kinetic_path[1:]):
            if elapsed_time >= path_vertex.z and elapsed_time <= next_path_vertex.z:
                path_position = mix(path_vertex, next_path_vertex, vec3((elapsed_time - path_vertex.z) / (next_path_vertex.z - path_vertex.z)))

        if elapsed_time > self.kinetic_path[-1].z:
            path_position = mix(self.kinetic_path[-1], self.kinetic_path[-2], vec3((elapsed_time - self.kinetic_path[-1].z) / (self.kinetic_path[-2].z - self.kinetic_path[-1].z)))

        return vec3(vertex.x + path_position.x, vertex.y + path_position.y, vertex.z)

    def transform_vertices(self, segment_vertices, scan_length, scan_velocity):
        transformed_segment_vertices = []
        for segment in segment_vertices:
            vertices = []
            for vertex, next_vertex in zip(segment[:-1], segment[1:]):
                for i in range(len(self.kinetic_path)):
                    vertices.append(self.transform_vertex(mix(vertex, next_vertex, vec3(i / (len(self.kinetic_path)-1))), scan_length, scan_velocity))

            transformed_segment_vertices.append(vertices)

        return transformed_segment_vertices

class SegmentPath(Path):
    def __init__(self, segments):
        super(SegmentPath, self).__init__()

        lengths = [0.0] + [distance(vertex, next_vertex) for vertex, next_vertex in zip(sum(segments, [])[:-1], sum(segments, [])[1:])]
        self.scan_length = sum(lengths)

        index_iterator = iter(range(1, len(lengths)+1))
        self.segments = [[vec3(vertex.x, vertex.y, sum(lengths[:next(index_iterator)])) for vertex in segment] for segment in segments]

    def scan_vertex(self, scan_distance):
        for index, segment in enumerate(self.segments):
            for vertex, next_vertex in zip(segment[:-1], segment[1:]):
                if index == 0 and scan_distance < vertex.z:
                    return mix(vertex, next_vertex, vec3((scan_distance - vertex.z) / (next_vertex.z - vertex.z)))
                if scan_distance >= vertex.z and scan_distance <= next_vertex.z:
                    return mix(vertex, next_vertex, vec3((scan_distance - vertex.z) / (next_vertex.z - vertex.z)))
                if index == len(self.segments)-1 and scan_distance > next_vertex.z:
                    return mix(vertex, next_vertex, vec3((scan_distance - vertex.z) / (next_vertex.z - vertex.z)))

    def scan_vertices(self, scan_length, scan_velocity):
        segment_vertices = []
        for index, segment in enumerate(self.segments):
            vertices = [vertex for vertex in segment if vertex.z >= self.scan_distance and vertex.z <= self.scan_distance + scan_length]

            if self.scan_distance > segment[0].z and self.scan_distance < segment[-1].z:
                vertices.insert(0, self.scan_vertex(self.scan_distance))

            if self.scan_distance + scan_length > segment[0].z and self.scan_distance + scan_length < segment[-1].z:
                vertices.append(self.scan_vertex(self.scan_distance + scan_length))

            if len(vertices) > 0:
                segment_vertices.append(vertices)

        segment_vertices = super(SegmentPath, self).transform_vertices(segment_vertices, scan_length, scan_velocity)

        self.scan_distance = (self.scan_distance + scan_length) % self.scan_length

        return segment_vertices

class SpiroPath(Path):
    def __init__(self, stator_radius, rotor_radius, rotor_offset):
        super(SpiroPath, self).__init__()

        self.min_scan_distance = 0.05

        self.stator_radius = stator_radius
        self.rotor_radius = rotor_radius
        self.rotor_offset = rotor_offset

    def scan_vertex(self, scan_distance):
        x = (self.stator_radius - self.rotor_radius)*cos(scan_distance) + self.rotor_offset*cos((self.stator_radius - self.rotor_radius)*scan_distance / self.rotor_radius)
        y = (self.stator_radius - self.rotor_radius)*sin(scan_distance) - self.rotor_offset*sin((self.stator_radius - self.rotor_radius)*scan_distance / self.rotor_radius)
        return vec3(x, y, scan_distance)

    def scan_vertices(self, scan_length, scan_velocity):
        vertices = [self.scan_vertex(self.scan_distance), self.scan_vertex(self.scan_distance + scan_length)]

        index = 0
        while index < len(vertices)-1:
            if distance(vec2(vertices[index].x, vertices[index].y), vec2(vertices[index+1].x, vertices[index+1].y)) > self.min_scan_length:
                vertices.insert(index+1, self.scan_vertex(0.5*(vertices[index].z + vertices[index+1].z)))
            else:
                index += 1

        segment_vertices = [vertices]
        segment_vertices = super(SpiroPath, self).transform_vertices(segment_vertices, scan_length, scan_velocity)

        self.scan_distance += scan_length

        return segment_vertices

class VectorDisplay(object):
    def __init__(self, window):
        super(VectorDisplay, self).__init__()

        self.max_vertices = 32

        self.excitation_shader_name = 'vector_display_excite'
        self.decay_shader_name = 'vector_display_decay'
        self.glow_1_shader_name = 'vector_display_glow_1'
        self.glow_2_shader_name = 'vector_display_glow_2'

        self.vertex_array_name = 'window'

        self.frame_buffer_name = 'vector_display'
        self.current_value_color_attachment = 0
        self.next_value_color_attachment = 1

        self.beam_width = 0.0025
        self.beam_velocity = 10.0
        self.beam_intensity = 20.0
        self.beam_color = vec3(1.0, 1.0, 1.0)

        self.excitation_time_constant = 0.01
        self.decay_time_constant_1 = 0.1
        self.decay_threshold_1 = 1.0
        self.decay_time_constant_2 = 10.0
        self.decay_threshold_2 = 0.5 / 255.0

        self.glow_size = 100
        self.glow_width = 15.0
        self.glow_intensity = 3.0

        self.paths = []

        window.add_shader(self.excitation_shader_name, [(read_text('vector_display_window.vert'), ShaderType.vertex), (read_text('vector_display_excite.frag'), ShaderType.fragment)])
        window.add_shader(self.decay_shader_name, [(read_text('vector_display_window.vert'), ShaderType.vertex), (read_text('vector_display_decay.frag'), ShaderType.fragment)])
        window.add_shader(self.glow_1_shader_name, [(read_text('vector_display_window.vert'), ShaderType.vertex), (read_text('vector_display_glow_1.frag'), ShaderType.fragment)])
        window.add_shader(self.glow_2_shader_name, [(read_text('vector_display_window.vert'), ShaderType.vertex), (read_text('vector_display_glow_2.frag'), ShaderType.fragment)])

        window.add_frame_buffer(self.frame_buffer_name, window.size, 3, False) # 2
        window.set_target_frame_buffer(self.frame_buffer_name, list(range(window.frame_buffers(self.frame_buffer_name).num_color_attachments)))
        window.clear()

        self.i = 0

    def update(self, elapsed_time):
        pass

    def draw(self, window, final_frame_buffer_name, final_frame_buffer_color_attachment, camera, elapsed_time):
        for path in self.paths:
            path.update(elapsed_time)

            scan_distance = path.scan_distance
            segment_vertices = path.scan_vertices(elapsed_time*self.beam_velocity, self.beam_velocity)

            window.use_blending = False
            window.copy_frame_buffer('vector_display', self.current_value_color_attachment, 'vector_display', self.next_value_color_attachment)
            window.set_target_frame_buffer(self.frame_buffer_name, [self.next_value_color_attachment,])
            window.shaders(self.excitation_shader_name).set_attribute('vertex_position', window.vertex_arrays(self.vertex_array_name), 'vertex_positions')
            window.shaders(self.excitation_shader_name).set_uniform('beam_width', self.beam_width)
            window.shaders(self.excitation_shader_name).set_uniform('beam_velocity', self.beam_velocity)
            window.shaders(self.excitation_shader_name).set_uniform('beam_intensity', self.beam_intensity)
            window.shaders(self.excitation_shader_name).set_uniform('beam_color', self.beam_color)
            window.shaders(self.excitation_shader_name).set_uniform('time_constant', self.excitation_time_constant)

            for segment_index, vertices in enumerate(segment_vertices):
                lengths = [0.0] + [distance(vec2(vertex.x, vertex.y), vec2(next_vertex.x, next_vertex.y)) for vertex, next_vertex in zip(vertices[:-1], vertices[1:])]

                for begin_index, end_index in zip(list(range(0, len(vertices), self.max_vertices-1)), list(range(min(len(vertices), self.max_vertices), len(vertices), self.max_vertices-1)) + [len(vertices)]):
                    window.shaders(self.excitation_shader_name).set_uniform('num_vertices', end_index - begin_index)
                    window.shaders(self.excitation_shader_name).set_uniform('vertices', vertices[begin_index:end_index])
                    window.shaders(self.excitation_shader_name).set_uniform('iterations', int(ceil(sum(lengths[begin_index:end_index]) / self.beam_width)))
                    window.shaders(self.excitation_shader_name).set_uniform('scan_distance', vertices[begin_index].z)
                    window.shaders(self.excitation_shader_name).set_uniform('elapsed_time', (vertices[end_index-1].z - vertices[begin_index].z) / self.beam_velocity)
                    window.frame_buffers(self.frame_buffer_name).get_color_texture(self.current_value_color_attachment).set_texture_unit(0)
                    window.shaders(self.excitation_shader_name).set_uniform('value_sampler', 0)
                    window.vertex_arrays(self.vertex_array_name).draw()

                    if end_index != len(vertices):
                        window.copy_frame_buffer('vector_display', self.next_value_color_attachment, 'vector_display', self.current_value_color_attachment)

                    scan_distance += vertices[end_index-1].z - vertices[begin_index].z

                if segment_index != len(segment_vertices)-1:
                    window.copy_frame_buffer('vector_display', self.next_value_color_attachment, 'vector_display', self.current_value_color_attachment)

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

        '''
        if self.i >= 0:
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

            show()
        '''

        self.i += 1

def read_text(file_name):
    with open(file_name) as file:
        return file.read()
