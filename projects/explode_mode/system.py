from math import atan, radians, degrees
from uuid import uuid4

from glm import vec2, vec3, quat
from clock import Clock
from geometry import CoordinateTransform
from parametric import CurveInterpolation
from physics import PaperKinetics
from event import EventHandler
from opengl import TextureFormat, Window, Camera
from image import initialize_window_gaussian_filter

from explode_mode import initialize_window_vector_display_curve, initialize_window_background, initialize_window_square, Background, Square
from menu_initialization import initialize_vector_display_glow_filter, initialize_menu_selector, initialize_start_menu, initialize_options_menu



class System(object):
    def __init__(self, size):
        super(System, self).__init__()

        self.clock = Clock()

        self.window = Window('Explode Mode', size, True)
        self.window.add_frame_buffer('draw', size, [TextureFormat.rgba, TextureFormat.rgba], False)
        initialize_window_gaussian_filter(self.window)
        initialize_window_vector_display_curve(self.window)
        initialize_window_background(self.window)
        initialize_window_square(self.window)

        self.camera = Camera(90.0, 90.0, 0.1, 100.0, False)
        self.camera.coordinate_transform.translation = vec3(0.0, 0.0, 10.0)
        self.camera.coordinate_transform.rotation = quat(vec3(1.0, 0.0, 0.0), radians(-15.0))

        self.glow_filter = initialize_vector_display_glow_filter(self)

        self.menus = {'start':initialize_start_menu(self), 'options':initialize_options_menu(self)}
        self.menu_selector = initialize_menu_selector(self)

        self.radius = 10.0
        self.background = Background(self.radius, 10.0, 5000, self.window, str(uuid4()))

        self.squares = [Square(vec3(0.0, 10.0, -10.0), quat(vec3(0.0, 1.0, 0.0), radians(90.0))*quat(vec3(1.0, 0.0, 0.0), -radians(45.0)), vec3(0.0, 0.0, 0.0), vec3(0.0, 0.0, 0.0))]
        self.squares[-1].kinetics.external_acceleration = vec3(0.0, -9.8, 0.0)
        self.squares[-1].kinetics.perpendicular_friction = 5.0
        self.squares[-1].kinetics.parallel_friction = 0.5
        self.squares[-1].kinetics.fluid_density = 0.1
        self.squares[-1].kinetics.paper_density = 1.0
        self.squares[-1].kinetics.size = vec3(1.0, 0.0, 1.0)
        self.squares[-1].coordinate_transform.scaling = vec3(3.0)

    def switch_menu(self, start_menu_name, stop_menu_name, hurry):
        if stop_menu_name is not None:
            self.menus[stop_menu_name].stop(self.clock)

        if start_menu_name is not None:
            self.menus[start_menu_name].start(self.clock, self.window, self.menu_selector)
            if hurry:
                self.menus[start_menu_name].hurry(self.clock)

    def invert_y(self, invert):
        pass

    def aim_sensitivity(self, sensitivity):
        pass

    def update(self):
        for menu in self.menus.values():
            menu.update(self.clock, self.window.event_handler)

    def step(self):
        for menu in self.menus.values():
            menu.step(self.clock, self.window)
        self.background.step(self.clock, self.window)
        for square in self.squares:
            square.step(self.clock)

    def draw(self):
        self.window.set_target_frame_buffer('draw', [0])
        self.window.clear()

        # for menu in self.menus.values():
        #     menu.draw(self.clock, self.window)

        self.window.set_target_frame_buffer('window', [0])
        self.window.clear()

        # self.background.draw(self.camera, self.window)
        for square in self.squares:
            square.draw(self.camera, self.window)

        self.glow_filter.apply(self.window)

        self.window.draw()
