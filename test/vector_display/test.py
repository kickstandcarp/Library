from sys import path
from os.path import pardir, split, join, realpath
from json import loads

path.append(join(split(realpath(__file__))[0], pardir, pardir, 'python', 'lib'))
from glm import vec2, vec3, step
from clock import Clock
from geometry import CoordinateTransform
from parametric import CurveInterpolation, SegmentCurveBool, SegmentCurveVec2
from event import EventHandler
from opengl import Window
from image import GaussianFilter
from vector_display import VectorDisplay, VectorDisplayBeam



# from math import exp, log
# from glm import mix, smoothstep
# v = 1.0
# exp(mix(log(0.001), log(0.05), smoothstep(2.0*1.0, 1.0, v)))


size = 500, 500
beam_width = 0.002

clock = Clock()

window = Window('Vector Display', size, True)

vector_display = VectorDisplay(window, size)

# t0 = 0.0
# vertex_curve_vertices = []
# drawn_curve_vertices = []
# with open('font_vertex_data.json') as file:
#     vertex_curves = loads(file.read())
# for vertex_curve in vertex_curves.values():
#     vertex_curve_vertices += [[0.5*(vec2(vertex) - vec2(0.333, 0.5)), t + t0] for vertex, t in vertex_curve]
#     drawn_curve_vertices += [[i % 2 == 0, t + t0] for i, (vertex, t) in enumerate(vertex_curve)]
#     t0 = vertex_curve_vertices[-1][1] + 5.0
# vector_display_beam = VectorDisplayBeam(SegmentCurveVec2(vertex_curve_vertices, CurveInterpolation.linear), SegmentCurveBool(drawn_curve_vertices, CurveInterpolation.nearest), 10.0, vec3(1.0), beam_width, 10.0)
# vector_display_beam.decay_time_constant = 0.5

with open('character_vertex_data.json') as file:
    data = loads(file.read())
vertex_curve_vertices = [(vec2(vertex), t) for vertex, t in data['vertex_curve']]
drawn_curve_vertices = [(vertex, t) for vertex, t in data['drawn_curve']]
vector_display_beam = VectorDisplayBeam(SegmentCurveVec2(vertex_curve_vertices, CurveInterpolation.linear), SegmentCurveBool(drawn_curve_vertices, CurveInterpolation.nearest), 10.0, vec3(1.0), beam_width, 3.0)
vector_display_beam.decay_time_constant = 30.0

vector_display.add_beam('test', vector_display_beam, window)

while not window.event_handler.quit:
    clock.step(window.refresh_time)
    window.event_handler.step(clock)

    window.set_target_frame_buffer('window')
    window.clear()

    vector_display.step(clock, window)
    vector_display.draw(window)

    window.draw()
