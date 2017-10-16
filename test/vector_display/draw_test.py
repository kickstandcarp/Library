from sys import path
from os.path import pardir, split, join, realpath

path.append(join(split(realpath(__file__))[0], pardir, pardir, 'python', 'lib'))
from glm import vec2, vec3
from clock import Clock
from geometry import CoordinateTransform
from parametric import CurveInterpolation, SegmentCurveBool, SegmentCurveVec2
from physics import OscillatorKinetics
from event import DeviceType, EventHandler
from opengl import Window
from image import GaussianFilter
from vector_display import VectorDisplay, VectorDisplayBeam



size = 500, 500
beam_width = 0.002
beam_velocity = 1.0

clock = Clock()

window = Window('Vector Display', size, True)
window.event_handler.add_direction_history('absolute', DeviceType.mouse)
window.event_handler.add_button_history('left', DeviceType.mouse)
window.event_handler.history_duration = 0.1
window.event_handler.get_direction_history('absolute', DeviceType.mouse).min_vertex_distance = 10.0*beam_width

vector_display = VectorDisplay(window, size)

vector_display.add_beam('test', VectorDisplayBeam(window.event_handler.get_direction_history('absolute', DeviceType.mouse), window.event_handler.get_button_history('left', DeviceType.mouse), 5.0, vec3(1.0), beam_width, beam_velocity, -window.refresh_time), window)
vector_display.beams('test').decay_time_constant = 3.0

while not window.event_handler.quit:
    clock.step(window.refresh_time)
    window.event_handler.step(clock)

    window.set_target_frame_buffer('window')
    window.clear()

    vector_display.step(clock, window)
    vector_display.draw(window)

    window.draw()
