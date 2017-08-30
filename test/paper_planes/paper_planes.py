from sys import path
from os.path import pardir, split, join, realpath
from math import tan, atan, degrees, radians, pi

# from matplotlib.pylab import show, figure, plot, grid

path.append(join(split(realpath(__file__))[0], pardir, 'python'))
from glm import vec3, vec4, quat, mat4x4
from opengl import BlendFactor
from event import DeviceType
from coordinate import Camera
from paper_airplanes_support import initialize_window, Player, Square



shaders_file_names = [(join(split(realpath(__file__))[0], 'flat.vert'), join(split(realpath(__file__))[0], 'flat.frag'))]
window = initialize_window((1000, 500), shaders_file_names)

camera = Camera(70.0, 0.5*70.0, 0.1, 100.0, True)
# camera = Camera(radians(90.0), 2.0*atan((window.size[1] / window.size[0])*tan(radians(0.5*90.0))), 0.1, 100.0, False)
camera.coordinate_transform.translation = vec3(30.0, 0.0, -30.0)
camera.coordinate_transform.rotation = quat(vec3(0.0, 1.0, 0.0), 0.5*pi)

player = Player((1.0, 1.0), vec4(1.0, 1.0, 1.0, 0.5), vec3(0.0, 0.0, 0.0))
squares = []

while not window.event_handler.quit:
    window.event_handler.update()

    square = player.update(window.event_handler, window.refresh_time)
    if square is not None:
        squares.append(square)

    for square in squares:
        square.update(window.refresh_time)

    window.set_target_frame_buffer('draw', [0,])
    window.clear()

    for square in squares:
        square.draw(window, camera)
    player.draw(window, camera)

    window.copy_frame_buffer('draw', 0, 'window', 0)
    window.draw()
