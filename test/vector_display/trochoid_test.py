from sys import path
from os.path import pardir, split, join, realpath
from random import uniform

path.append(join(split(realpath(__file__))[0], pardir, pardir, 'python', 'lib'))
from glm import vec2, vec3
from clock import Clock
from geometry import CoordinateTransform
from parametric import CurveInterpolation, SegmentCurveBool, SegmentCurveVec2
from physics import OscillatorKinetics
from event import DeviceType, EventHandler
from opengl import Window
from image import GaussianFilter
from vector_display import VectorDisplay, VectorDisplayBeam, Trochoid



size = 500, 500
beam_width = 0.002
beam_velocity = 3.0

clock = Clock()

window = Window('Vector Display', size, True)

vector_display = VectorDisplay(window, size)
vector_display.kinetics.frequency = vec2(5.0)
vector_display.kinetics.damping_ratio = vec2(1.0)
vector_display.beam_kinetics.frequency = vec2(3.33)
vector_display.beam_kinetics.damping_ratio = vec2(0.05)
vector_display.beam_kinetics.history_duration = 0.1

external_acceleration_magnitude = 500.0

curve = Trochoid(1.0, 0.19, 0.11, 10.0*beam_width)
vector_display.add_beam('test_1', VectorDisplayBeam(curve, None, 3.0, vec3(0.625, 1.0, 0.8), beam_width, beam_velocity), window)
vector_display.beams('test_1').decay_time_constant = 10.0

curve = Trochoid(0.25, -0.28, 0.13, 10.0*beam_width)
vector_display.add_beam('test_2', VectorDisplayBeam(curve, None, 5.0, vec3(0.5, 0.3, 0.15), beam_width, beam_velocity), window)
vector_display.beams('test_2').decay_time_constant = 10.0

curve = Trochoid(0.4, 0.13, 0.07, 10.0*beam_width)
vector_display.add_beam('test_3', VectorDisplayBeam(curve, None, 1.5, vec3(1.0, 1.0, 1.0), beam_width, beam_velocity), window)
vector_display.beams('test_3').decay_time_constant = 10.0

while not window.event_handler.quit:
    clock.step(window.refresh_time)
    window.event_handler.step(clock)

    window.set_target_frame_buffer('window')
    window.clear()

    external_acceleration = external_acceleration_magnitude*vec2(uniform(-1.0, 1.0), uniform(-1.0, 1.0)) if window.event_handler.get_button_moved_down('space', DeviceType.keyboard) else vec2(0.0)
    vector_display.kinetics.external_acceleration = external_acceleration
    vector_display.beam_kinetics.external_acceleration = external_acceleration

    vector_display.step(clock, window)
    vector_display.draw(window)

    window.draw()
