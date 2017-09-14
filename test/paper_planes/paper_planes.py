from math import tan, atan, degrees, radians, pi

# from matplotlib.pylab import show, figure, plot, grid

from glm import vec3, vec4, quat, mat4x4
from geometry import CoordinateTransform
from event import EventHandler, DeviceType
from opengl import Camera, BlendFactor
from paper_planes_support import initialize_window, Player, Square



window = initialize_window((1000, 500))

camera = Camera(70.0, 0.5*70.0, 0.1, 100.0, True)
# camera = Camera(radians(90.0), 2.0*atan((window.size[1] / window.size[0])*tan(radians(0.5*90.0))), 0.1, 100.0, False)
camera.coordinate_transform.translation = vec3(30.0, 0.0, -30.0)
camera.coordinate_transform.rotation = quat(vec3(0.0, 1.0, 0.0), 0.5*pi)

player = Player((1.0, 1.0), vec4(1.0, 1.0, 1.0, 0.5), vec3(0.0, 0.0, 0.0))
squares = []

time = 0.0
while not window.event_handler.quit:
    elapsed_time = window.refresh_time

    window.event_handler.update(time)

    square = player.update(window.event_handler, elapsed_time, time)
    if square is not None:
        squares.append(square)

    for square in squares:
        square.update(elapsed_time, time)

    window.set_target_frame_buffer('draw', [0,])
    window.clear()

    for square in squares:
        square.draw(window, camera)
    player.draw(window, camera)

    time = time + elapsed_time

    window.copy_frame_buffer('draw', 0, 'window', 0)
    window.draw()
