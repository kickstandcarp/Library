from glm import vec2
from opengl import Window, ShaderType, DrawMode
from event import DeviceType
from coordinate import Camera

from vector_display_support import VectorDisplay



window = Window('Vector Display', (500, 500), True)
window.add_vertex_array('window', DrawMode.triangle_strip)
window.vertex_arrays('window').add_buffer('vertex_positions', [(-1.0, -1.0), (1.0, -1.0), (-1.0, 1.0), (1.0, 1.0)])

camera = Camera(2.0, 2.0, 0.0, 1.0, True)

vector_display = VectorDisplay(window)

vertices = [vec2(-0.5, -0.5), vec2(0.5, -0.5), vec2(0.5, -0.5), vec2(0.5, 0.5), vec2(0.5, 0.5), vec2(-0.5, 0.5), vec2(-0.5, 0.5), vec2(-0.5, -0.5)]
vector_display.add_line(window, 'square', vertices)

max_excitation = vector_display.max_excitation(1.0 / 60.0)
glow_line_excitation_ratio = vector_display.glow_line_excitation_ratio()
print(max_excitation, max_excitation*glow_line_excitation_ratio)

while not window.event_handler.quit:
    window.event_handler.update()

    window.set_target_frame_buffer('window', [0])
    window.clear()

    elapsed_time = window.refresh_time
    vector_display.update(elapsed_time)
    vector_display.draw(window, 'window', 0, camera, elapsed_time)

    # window.copy_frame_buffer('vector_display', vector_display.current_value_color_attachment, 'window', 0)
    window.draw()
