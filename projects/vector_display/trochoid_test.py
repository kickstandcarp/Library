from sys import path
from os.path import pardir, split, join, realpath
from random import uniform

path.append(join(split(realpath(__file__))[0], pardir, pardir, 'python', 'lib'))
from glm import vec2, vec3
from clock import Clock
from geometry import CoordinateTransform
from parametric import CurveVec2
from event import DeviceType, EventHandler
from opengl import Window
from image import GaussianFilter
from vector_display import VectorDisplay, VectorDisplayBeam, Trochoid



size = 500, 500
beam_width = 0.0025
beam_velocity = 10.0

clock = Clock()
window = Window('Vector Display', size, True)
vector_display = VectorDisplay(window, size)

curve = Trochoid(1.0, 0.19, 0.11, 10.0*beam_width)
vector_display.add_beam('test_1', VectorDisplayBeam(curve, vec3(0.625, 1.0, 0.8), beam_width, beam_velocity), window)
vector_display.beams('test_1').excitation = 2.0
vector_display.beams('test_1').decay_time_constant = 10.0

curve = Trochoid(0.25, -0.28, 0.13, 10.0*beam_width)
vector_display.add_beam('test_2', VectorDisplayBeam(curve, vec3(0.5, 0.3, 0.15), beam_width, beam_velocity), window)
vector_display.beams('test_2').excitation = 3.0
vector_display.beams('test_2').decay_time_constant = 10.0

curve = Trochoid(0.4, 0.13, 0.07, 10.0*beam_width)
vector_display.add_beam('test_3', VectorDisplayBeam(curve, vec3(1.0, 1.0, 1.0), beam_width, beam_velocity), window)
vector_display.beams('test_3').excitation = 1.0
vector_display.beams('test_3').decay_time_constant = 10.0

while not window.event_handler.quit:
    clock.step(window.refresh_time)
    window.event_handler.step(clock)

    window.set_target_frame_buffer('window')
    window.clear()

    vector_display.step(clock, window)
    vector_display.draw(window)

    window.draw()
