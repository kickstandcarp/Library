from sys import path
from os.path import pardir, split, join, realpath
from time import sleep

path.append(join(split(realpath(__file__))[0], pardir, pardir, 'python'))
from glm import vec2, vec3, mix, smoothstep
from clock import Clock
from geometry import PathVertexBool, SegmentedPathBool, PathVertexVec2, SegmentedPathVec2
from physics import OscillatorKinetics
from event import EventHandler, DeviceType
from opengl import Window, Camera, ShaderType, DrawMode
from image import GaussianFilter
from vector_display import VectorDisplay, VectorDisplayPath, TrochoidPath



size = 500, 500
beam_width = 0.0025
beam_velocity = 3.0

clock = Clock()

window = Window('Vector Display', size, True)
window.event_handler.add_direction_path('absolute', DeviceType.mouse)
window.event_handler.add_button_path('left', DeviceType.mouse)
window.event_handler.path_duration = 0.1

# camera = Camera(2.0, 2.0, 0.0, 1.0, True)

vector_display = VectorDisplay(window, size, 'vector_display')
vector_display.decay_time_constant_2 = 3.0
vector_display.beam_kinetics.frequency = vec2(1.0)
vector_display.beam_kinetics.damping_ratio = vec2(0.1)

path = TrochoidPath(1.0, 0.43, 0.19, 10.0*beam_width)
vector_display.add_path('path_1', VectorDisplayPath(path, None, 0.0, beam_width, beam_velocity, 100.0*vec3(1.0)))

# segments = 35
# from numpy import linspace
# path = SegmentedPathVec2([(vec2(x, 0.0), t) for x, t in zip(linspace(-1.0, 1.0, segments+1), linspace(0.0, 1.0, segments+1))] + [(vec2(0.0, y), t) for y, t in zip(linspace(1.0, -1.0, segments+1), linspace(2.0, 3.0, segments+1))])
# drawns = SegmentedPathBool([(i % 2 == 0, t) for i, t in enumerate(linspace(0.0, 1.0, segments+1))] + [(i % 2 == 0, t) for i, t in enumerate(linspace(2.0, 3.0, segments+1))])
# vector_display.add_path('path_2', VectorDisplayPath(path, drawns, 0.0, beam_width, beam_velocity, 10.0*vec3(1.0)))

# vector_display.add_path('path_3', VectorDisplayPath(window.event_handler.get_direction_path('absolute', DeviceType.mouse), window.event_handler.get_button_path('left', DeviceType.mouse), 0.0, beam_width, 1.0, 30.0*vec3(1.0)))

while not window.event_handler.quit:
    clock.update(window.refresh_time)
    if clock.time >= 10.0:
        window.event_handler.quit = True

    window.event_handler.update(clock)

    vector_display.beam_kinetics.external_acceleration = vec2(-1000.0, 0.0) if clock.count == 1 else vec2(0.0)
    vector_display.step(clock, window)
    vector_display.draw(window)

    window.draw()

    '''
    if clock.count >= 1:
        from matplotlib.pylab import show, figure, subplot, imshow, colorbar

        figure()

        image = window.frame_buffers('vector_display').get_color_texture(0).get_data()[:, :, 0]
        subplot(1, 2, 1)
        imshow(image)
        colorbar()

        image = window.frame_buffers('vector_display').get_color_texture(1).get_data()[:, :, 0]
        subplot(1, 2, 2)
        imshow(image)
        colorbar()

        show()
    '''