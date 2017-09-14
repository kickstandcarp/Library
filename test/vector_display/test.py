from sys import path
from os.path import pardir, split, join, realpath
from time import sleep

path.append(join(split(realpath(__file__))[0], pardir, pardir, 'python'))
from glm import vec2, vec3
from geometry import PathVertex, Path
from physics import OscillatorKinetics
from event import EventHandler, DeviceType
from opengl import Window, Camera, ShaderType, DrawMode
from vector_display import VectorDisplay, VectorDisplayPath, TrochoidPath



size = 500, 500
beam_width = 0.0025
beam_velocity = 50.0

window = Window('Vector Display', size, True)
camera = Camera(2.0, 2.0, 0.0, 1.0, True)

vector_display = VectorDisplay(window, size)
vector_display.add_path('path_1', VectorDisplayPath(TrochoidPath(1.0, 0.5, 0.33, 10.0*beam_width), 0.0, beam_width, beam_velocity, vec3(1.0)))

window.event_handler.add_button_path('left', DeviceType.mouse)
window.event_handler.add_direction_path('absolute', DeviceType.mouse)
window.event_handler.path_duration = 1.0

time = 0.0
while not window.event_handler.quit:
    elapsed_time = window.refresh_time
    if time >= 2.0:
        window.event_handler.quit = True

    window.event_handler.update(time)

    vector_display.update(0.1)
    vector_display.draw(window)


    from matplotlib.pylab import show, figure, imshow, colorbar

    figure()
    image = window.frame_buffers('vector_display').get_color_texture(1).get_data()[:, :, 0]
    imshow(image)
    colorbar()

    figure()
    image = window.frame_buffers('vector_display').get_color_texture(1).get_data()[:, :, 1]
    imshow(image)
    colorbar()

    show()


    time = time + elapsed_time

# print(window.event_handler.get_button_path('left', DeviceType.mouse))
print(len(window.event_handler.get_direction_path('absolute', DeviceType.mouse).path_vertices), [path_vertex for path_vertex in window.event_handler.get_direction_path('absolute', DeviceType.mouse).path_vertices])
