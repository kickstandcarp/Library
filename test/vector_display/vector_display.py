from glm import vec2
from opengl import Window, ShaderType, DrawMode
from event import DeviceType
from coordinate import Camera

from vector_display import VectorDisplay, VectorDisplayLine



window = Window('Vector Display', (500, 500), True)
window.add_vertex_array('window', DrawMode.triangle_strip)
window.vertex_arrays('window').add_buffer('vertex_positions', [(-1.0, -1.0), (1.0, -1.0), (-1.0, 1.0), (1.0, 1.0)])

camera = Camera(2.0, 2.0, 0.0, 1.0, True)

vector_display = VectorDisplay(window)

lines = [[vec2(-0.5, -0.5), vec2(0.5, -0.5)], [vec2(0.5, -0.5), vec2(0.5, 0.5)], [vec2(0.5, 0.5), vec2(-0.5, 0.5)], [vec2(-0.5, 0.5), vec2(-0.5, -0.5)]]
vector_display.lines.append(VectorDisplayLine(window, lines))

while not window.event_handler.quit:
    window.event_handler.update()

    vector_display.draw(window, camera)

    window.copy_frame_buffer('vector_display', vector_display.current_value_color_attachment, 'window', 0)
    window.draw()
