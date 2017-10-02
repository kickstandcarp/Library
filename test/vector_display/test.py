from sys import path
from os.path import pardir, split, join, realpath
from time import sleep

path.append(join(split(realpath(__file__))[0], pardir, pardir, 'python', 'lib'))
from glm import vec2, vec3, mix, smoothstep
from clock import Clock
from parametric import SegmentCurveBool, SegmentCurveFloat, SegmentCurveVec2
from physics import OscillatorKinetics
from event import EventHandler, DeviceType
from opengl import Window, Camera, ShaderType, DrawMode
from image import GaussianFilter
from vector_display import VectorDisplay, VectorDisplayCurve, Trochoid



size = 500, 500
beam_width = 0.0025
beam_velocity = 10.0

clock = Clock()

window = Window('Vector Display', size, True)
window.event_handler.add_direction_history('absolute', DeviceType.mouse)
window.event_handler.add_button_history('left', DeviceType.mouse)
window.event_handler.history_duration = 1.0
window.event_handler.get_direction_history('absolute', DeviceType.mouse).min_vertex_distance = 10.0*beam_width

# camera = Camera(2.0, 2.0, 0.0, 1.0, True)

vector_display = VectorDisplay(window, size, 'vector_display')
vector_display.excitation_time_constant = 0.001
vector_display.decay_time_constant_1 = 0.03
vector_display.decay_time_constant_2 = 3.0
vector_display.beam_kinetics.frequency = vec2(1.0)
vector_display.beam_kinetics.damping_ratio = vec2(0.1)

# curve = Trochoid(1.0, 0.41, 0.17, 10.0*beam_width)
# vector_display.add_curve('curve_1', VectorDisplayCurve(curve, None, beam_velocity, 0.0, beam_width, 1.0*vec3(1.0, 1.0, 0.5)), window)

# segments = 35
# from numpy import linspace
# curve = SegmentCurveVec2([(vec2(x, 0.0), t) for x, t in zip(linspace(-1.0, 1.0, segments+1), linspace(0.0, 1.0, segments+1))] + [(vec2(0.0, y), t) for y, t in zip(linspace(1.0, -1.0, segments+1), linspace(2.0, 3.0, segments+1))])
# drawns = SegmentCurveBool([(i % 2 == 0, t) for i, t in enumerate(linspace(0.0, 1.0, segments+1))] + [(i % 2 == 0, t) for i, t in enumerate(linspace(2.0, 3.0, segments+1))])
# vector_display.add_curve('curve_2', VectorDisplayCurve(curve, drawns, 0.5*beam_velocity, 0.0, beam_width, 1.0*vec3(1.0)), window)

vector_display.add_curve('curve_3', VectorDisplayCurve(window.event_handler.get_direction_history('absolute', DeviceType.mouse), window.event_handler.get_button_history('left', DeviceType.mouse), 1.0, 0.0, beam_width, 3.0*vec3(1.0)), window)

# curve = Trochoid(1.0, 0.52, 0.14, 10.0*beam_width)
# vector_display.add_curve('curve_4', VectorDisplayCurve(curve, None, beam_velocity, 0.333, beam_width, 1.0*vec3(0.7, 1.0, 1.0)), window)

# curve = Trochoid(1.0, 0.26, 0.11, 10.0*beam_width)
# vector_display.add_curve('curve_5', VectorDisplayCurve(curve, None, beam_velocity, 0.666, beam_width, 1.0*vec3(0.7, 1.0, 0.7)), window)

# curve = Trochoid(1.0, 0.33, 0.1, 10.0*beam_width)
# vector_display.add_curve('curve_6', VectorDisplayCurve(curve, None, beam_velocity, 1.0, beam_width, 10.0*vec3(1.0, 1.0, 1.0)), window)

while not window.event_handler.quit:
    clock.step(window.refresh_time)
    window.event_handler.step(clock)

    window.set_target_frame_buffer('window')
    window.clear()

    # vector_display.beam_kinetics.external_acceleration = vec2(-500.0, 100.0) if clock.step_count == 1 else vec2(0.0)
    vector_display.curves('curve_3').offset_time = -clock.elapsed_time
    vector_display.step(clock, window)
    vector_display.draw(window)

    window.draw()

    '''
    if clock.step_count >= 100:
    # if any([value for value, t in window.event_handler.get_button_history('left', DeviceType.mouse).vertices(clock.time, clock.time- clock.elapsed_time)]):
        from numpy import min, max, clip
        from matplotlib.pylab import show, figure, subplot, imshow, colorbar

        figure()

        image = window.frame_buffers('vector_display').get_color_texture(0).get_data()

        subplot(2, 2, 1)
        imshow(image[:, :, 0])
        colorbar()
        print(min(image[:, :, 0]), max(image[:, :, 0]))

        image = window.frame_buffers('vector_display').get_color_texture(1).get_data()

        subplot(2, 2, 2)
        imshow(image[:, :, 0])
        colorbar()
        print(min(image[:, :, 0]), max(image[:, :, 0]))

        image = window.frame_buffers('vector_display').get_color_texture(2).get_data()

        subplot(2, 2, 3)
        imshow(image[:, :, 0])
        colorbar()
        print(min(image[:, :, 0]), max(image[:, :, 0]))


        figure()

        image = window.textures('curve_3').get_data()

        imshow(image[:, :, 0])
        colorbar()
        print(min(image[:, :, 0]), max(image[:, :, 0]))

        show()
    '''