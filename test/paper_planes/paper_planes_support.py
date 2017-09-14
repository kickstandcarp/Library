from math import copysign, pi

from matplotlib.cm import get_cmap

from glm import vec2, vec3, vec4, quat, mat4x4, rotate, rotation, length
from opengl import Window, BlendFactor, ShaderType, DrawMode, TextureInterpolation, TextureWrap
from event import DeviceType
from geometry import CoordinateTransform, translation, rotation, scaling
from physics import OscillatorKinetics, PaperKinetics



def initialize_window(size):
    window = Window('OpenGL Square Test', size, True)

    window.use_depth_test = True
    window.use_blending = True
    window.blend_factors = BlendFactor.source_alpha, BlendFactor.one_minus_source_alpha

    window.add_shader('flat', [(read_text('flat.vert'), ShaderType.vertex), (read_text('flat.frag'), ShaderType.fragment)])

    window.add_vertex_array('arrow', DrawMode.lines)
    window.vertex_arrays('arrow').add_element_buffer(sum([(0, 1), (1, 2), (1, 3)], ()))
    window.vertex_arrays('arrow').add_buffer('vertex_positions', [(0.0, 0.0, 0.0), (0.0, 1.0, 0.0), (0.15, 0.75, 0.0), (-0.15, 0.75, 0.0)])

    square_aspect_ratio = 0.1
    window.add_vertex_array('square', DrawMode.triangles)
    window.vertex_arrays('square').add_element_buffer(sum([(0, 1, 3), (1, 2, 3), (4, 5, 7), (5, 6, 7), (4, 5, 0), (5, 1, 0), (5, 6, 2), (6, 2, 1), (6, 7, 3), (7, 3, 2), (7, 4, 0), (4, 0, 3)], ()))
    window.vertex_arrays('square').add_buffer('vertex_positions', [(-1.0, -square_aspect_ratio, -1.0), (1.0, -square_aspect_ratio, -1.0), (1.0, -square_aspect_ratio, 1.0), (-1.0, -square_aspect_ratio, 1.0), (-1.0, square_aspect_ratio, -1.0), (1.0, square_aspect_ratio, -1.0), (1.0, square_aspect_ratio, 1.0), (-1.0, square_aspect_ratio, 1.0)])
    # window.vertex_arrays('square').add_buffer('vertex_texture_positions', [(0.0, 0.0), (1.0, 0.0), (1.0, 1.0), (0.0, 1.0)])

    # window.add_texture('plaid', image, TextureInterpolation.linear, TextureWrap.repeat)

    window.add_frame_buffer('draw', size, 1, True)

    return window

class Player(object):
    def __init__(self, size, color, position):
        super(Player, self).__init__()

        self.arrow_shader_name = 'flat'
        self.arrow_vertex_array_name = 'arrow'

        self.coordinate_transform = CoordinateTransform(position)

        self.velocity_arrow_color = vec4(1.0, 0.0, 0.0, 1.0)
        self.velocity_arrow_coordinate_transform = CoordinateTransform(vec3(0.0), quat(vec3(1.0, 0.0, 0.0), -0.5*pi), vec3(2.0))
        self.velocity_arrow_coordinate_transform.parent = self.coordinate_transform

        self.square = Square(size, color, vec3(0.0), quat(1.0, 0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0))
        self.square.coordinate_transform.parent = self.coordinate_transform

        self.tilt_kinetics = OscillatorKinetics(vec2(0.0), vec2(0.0), vec2(0.0), vec2(2.0), vec2(0.2))

        self.normal_arrow_color = vec4(0.0, 1.0, 0.0, 1.0)
        self.normal_arrow_coordinate_transform = CoordinateTransform(vec3(0.0), quat(1.0, 0.0, 0.0, 0.0), 2.0 / self.square.coordinate_transform.scaling)
        self.normal_arrow_coordinate_transform.parent = self.square.coordinate_transform

        self.color_map = get_cmap('jet')
        self.color_map_index = 0.0
        self.color_map_interval = 0.05

    def update(self, event_handler, elapsed_time, time):
        if event_handler.num_controllers > 0:
            velocity_direction = event_handler.get_direction('left_stick', DeviceType.controller, 0)
            square_direction = event_handler.get_direction('right_stick', DeviceType.controller, 0)
            fire = event_handler.get_button_moved_down('right_trigger', DeviceType.controller, 0)
        else:
            velocity_direction = event_handler.get_direction('w,a,s,d', DeviceType.keyboard)
            square_direction = event_handler.get_direction('up,left,down,right', DeviceType.keyboard)
            fire = event_handler.get_button_moved_down('return', DeviceType.keyboard)

        self.coordinate_transform.rotate(quat(vec3(1.0, 0.0, 0.0), elapsed_time*velocity_direction.y))

        self.tilt_kinetics.frequency = vec2(2.0 if square_direction.x == 0.0 else 0.5, 2.0 if square_direction.y == 0.0 else 0.5)
        self.tilt_kinetics.damping_ratio = vec2(0.2 if square_direction.x == 0.0 else 1.0, 0.2 if square_direction.y == 0.0 else 1.0)
        self.tilt_kinetics.external_acceleration = self.tilt_kinetics.steady_state_value_to_acceleration(vec2(0.25*pi))*square_direction
        self.tilt_kinetics.step(elapsed_time, time)

        self.square.coordinate_transform.rotation = quat(vec3(1.0, 0.0, 0.0), self.tilt_kinetics.value.y)*quat(vec3(0.0, 0.0, 1.0), self.tilt_kinetics.value.x)

        if fire:
            size = scaling(self.square.coordinate_transform, None)
            color = vec4(self.color_map(abs(1.0 - (self.color_map_index % 2.0))))
            position = translation(self.square.coordinate_transform, None)
            orientation = rotation(self.square.coordinate_transform, None)
            velocity = 25.0*self.velocity_arrow_coordinate_transform.transform_direction(vec3(0.0, 1.0, 0.0), True)
            square = Square((size.x, size.z), color, position, orientation, velocity, vec3(0.0))

            self.color_map_index += self.color_map_interval

            return square

    def draw(self, window, camera):
        window.shaders(self.arrow_shader_name).set_attribute('vertex_position', window.vertex_arrays(self.arrow_vertex_array_name), 'vertex_positions')

        window.shaders(self.arrow_shader_name).set_uniform('transform_matrix', camera.transform_matrix*self.normal_arrow_coordinate_transform.global_matrix)
        window.shaders(self.arrow_shader_name).set_uniform('color', self.normal_arrow_color)
        window.vertex_arrays(self.arrow_vertex_array_name).draw()

        window.shaders(self.arrow_shader_name).set_uniform('transform_matrix', camera.transform_matrix*self.velocity_arrow_coordinate_transform.global_matrix)
        window.shaders(self.arrow_shader_name).set_uniform('color', self.velocity_arrow_color)
        window.vertex_arrays(self.arrow_vertex_array_name).draw()

        self.square.draw(window, camera)

class Square(object):
    def __init__(self, size, color, position, orientation, velocity, angular_velocity):
        super(Square, self).__init__()

        self.shader_name = 'flat'
        self.vertex_array_name = 'square'

        self.color = color
        self.coordinate_transform = CoordinateTransform(position, orientation, vec3(size[0], 1.0, size[1]))
        self.kinetics = PaperKinetics(position, orientation, velocity, angular_velocity, external_acceleration=vec3(0.0, -9.8, 0.0), external_angular_acceleration=vec3(0.0), perpendicular_friction=10.0, parallel_friction=0.1, fluid_density=0.1, paper_density=1.0, size=vec3(size[0], 0.0, size[1]))

    def update(self, elapsed_time, time):
        intervals = 10
        for i in range(intervals):
            self.kinetics.step(elapsed_time / intervals, time)
            time = time + elapsed_time / intervals

        self.coordinate_transform.translation = self.kinetics.position
        self.coordinate_transform.rotation = self.kinetics.orientation

    def draw(self, window, camera):
        window.shaders(self.shader_name).set_attribute('vertex_position', window.vertex_arrays(self.vertex_array_name), 'vertex_positions')
        # window.shaders(self.shader_name).set_attribute('vertex_texture_position', window.vertex_arrays(self.vertex_array_name), 'vertex_texture_positions')

        window.shaders(self.shader_name).set_uniform('transform_matrix', camera.transform_matrix*self.coordinate_transform.global_matrix)
        window.shaders(self.shader_name).set_uniform('color', self.color)
        # window.shaders(self.shader_name).set_uniform('texture_sampler', 0)
        # window.textures('').set_texture_unit(0)

        window.vertex_arrays(self.vertex_array_name).draw()

def read_text(file_name):
    with open(file_name) as file:
        return file.read()
