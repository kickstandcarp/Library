from math import sin, cos, pi

from glm import vec2
from opengl import Window, ShaderType, DrawMode
from event import DeviceType
from coordinate import Camera

from vector_display_support import Path, SegmentPath, SpiroPath, VectorDisplay



window = Window('Vector Display', (500, 500), True)
window.add_vertex_array('window', DrawMode.triangle_strip)
window.vertex_arrays('window').add_buffer('vertex_positions', [(-1.0, -1.0), (1.0, -1.0), (-1.0, 1.0), (1.0, 1.0)])

camera = Camera(2.0, 2.0, 0.0, 1.0, True)

vector_display = VectorDisplay(window)

vector_display.paths.append(SegmentPath([[vec2(0.0, 0.5), vec2(0.0, -0.5)], [vec2(-0.5, 0.0), vec2(0.5, 0.0)]]))
# vector_display.paths.append(SpiroPath(1.0, 0.61, 0.61))

while not window.event_handler.quit:
    window.event_handler.update()

    window.set_target_frame_buffer('window', [0])
    window.clear()

    elapsed_time = window.refresh_time
    vector_display.update(elapsed_time)
    vector_display.draw(window, 'window', 0, camera, elapsed_time)

    # window.copy_frame_buffer('vector_display', vector_display.current_value_color_attachment, 'window', 0)
    window.draw()
