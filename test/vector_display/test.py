from sys import path
from os.path import pardir, split, join, realpath
from time import sleep

path.append(join(split(realpath(__file__))[0], pardir, pardir, 'python'))
from glm import vec2, vec3
from geometry import PathVertexBool, SegmentedPathBool, PathVertexVec2, SegmentedPathVec2
from physics import OscillatorKinetics
from event import EventHandler, DeviceType
from opengl import Window, Camera, ShaderType, DrawMode
from image import GaussianFilter
from vector_display import VectorDisplay, VectorDisplayPath, TrochoidPath


size = 500, 500
beam_width = 0.0025
beam_velocity = 3.0

window = Window('Vector Display', size, True)
window.event_handler.add_direction_path('absolute', DeviceType.mouse)
window.event_handler.add_button_path('left', DeviceType.mouse)
window.event_handler.path_duration = 0.1

# camera = Camera(2.0, 2.0, 0.0, 1.0, True)

vector_display = VectorDisplay(window, size, 'vector_display')
vector_display.glow_filter.amplitude = 3.0

# path = TrochoidPath(1.0, 0.43, 0.19, 10.0*beam_width)
# vector_display.add_path('path_1', VectorDisplayPath(path, None, 0.0, beam_width, beam_velocity, 10.0*vec3(1.0)))

# path = SegmentedPathVec2([(vec2(-1.0, 0.0), 0.0), (vec2(1.0, 0.0), 1.0), (vec2(0.0, 1.0), 1.5), (vec2(0.0, -1.0), 2.5)])
# drawns = SegmentedPathBool([(True, 0.0), (False, 1.0), (True, 1.5), (False, 2.5)])
# vector_display.add_path('path_2', VectorDisplayPath(path, drawns, 0.0, beam_width, beam_velocity, 10.0*vec3(1.0)))

# vector_display.add_path('path_3', VectorDisplayPath(window.event_handler.get_direction_path('absolute', DeviceType.mouse), window.event_handler.get_button_path('left', DeviceType.mouse), 0.0, beam_width, 1.0, 10.0*vec3(1.0)))

time = 0.0
frame = 0
while not window.event_handler.quit:
    elapsed_time = window.refresh_time
    if time >= 100.0:
        window.event_handler.quit = True

    window.event_handler.update(time)

    # print([(path_vertex.vertex, path_vertex.t) for path_vertex in window.event_handler.get_direction_path('absolute', DeviceType.mouse).path_vertices])
    # print([(path_vertex.vertex, path_vertex.t) for path_vertex in window.event_handler.get_direction_path('absolute', DeviceType.mouse).vertices(time - elapsed_time, time)])

    vector_display.step(elapsed_time, window)
    vector_display.draw(window)

    '''
    if frame >= 0:
        from matplotlib.pylab import show, figure, imshow, colorbar

        figure()
        image = window.frame_buffers('vector_display').get_color_texture(1).get_data()[:, :, 0]
        imshow(image)
        colorbar()

        show()
    '''

    window.draw()

    time = time + elapsed_time
    frame = frame + 1
