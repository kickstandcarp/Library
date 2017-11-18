from sys import path
from os.path import pardir, split, join, realpath
path.append(join(split(realpath(__file__))[0], pardir, pardir, 'python', 'lib'))

from math import tan, atan, degrees, radians, pi

from glm import vec3, vec4, quat, mat4x4
from clock import Clock
from geometry import CoordinateTransform
from parametric import CurveInterpolation
from event import EventHandler, DeviceType
from opengl import Camera, BlendFactor
from paper_planes_support import initialize_window, Player, Square



clock = Clock()
window = initialize_window((1000, 500))

camera = Camera(70.0, 0.5*70.0, 0.1, 100.0, True)
# camera = Camera(radians(90.0), 2.0*atan((window.size[1] / window.size[0])*tan(radians(0.5*90.0))), 0.1, 100.0, False)
camera.coordinate_transform.translation = vec3(30.0, 0.0, -30.0)
camera.coordinate_transform.rotation = quat(vec3(0.0, 1.0, 0.0), 0.5*pi)

player = Player((1.0, 1.0), vec4(1.0, 1.0, 1.0, 0.5), vec3(0.0, 0.0, 0.0))
squares = []

time = 0.0
while not window.event_handler.quit:
    clock.step(window.refresh_time)
    window.event_handler.step(clock)

    square = player.update(window.event_handler, clock)
    if square is not None:
        squares.append(square)

    for square in squares:
        square.update(clock)

    window.set_target_frame_buffer('draw', [0,])
    window.clear()

    for square in squares:
        square.draw(window, camera)
    player.draw(window, camera)

    window.set_target_frame_buffer('window', [0,])
    window.get_frame_buffer('draw').copy(0)
    window.draw()
