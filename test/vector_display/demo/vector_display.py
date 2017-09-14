from glm import vec2
from event import EventHandler, DeviceType
from opengl import Window, Camera, ShaderType, DrawMode
from graphics import VectorDisplay, VectorDisplayPath

# from vector_display_support import PointerPath, SegmentPath, SpiroPath, VectorDisplay



window = Window('Vector Display', (500, 500), True)
window.add_vertex_array('window', DrawMode.triangle_strip)
window.vertex_arrays('window').add_buffer('vertex_positions', [(-1.0, -1.0), (1.0, -1.0), (-1.0, 1.0), (1.0, 1.0)])

camera = Camera(2.0, 2.0, 0.0, 1.0, True)

vector_display = VectorDisplay(window)

vector_display.paths.append(SegmentPath([[vec2(0.0, 0.5), vec2(0.0, -0.5)], [vec2(-0.5, 0.0), vec2(0.5, 0.0)]]))
# vector_display.paths.append(SpiroPath(1.0, 0.77, 0.71))

while not window.event_handler.quit:
    window.event_handler.update()

    hit = window.event_handler.get_button_moved_down('return', DeviceType.keyboard)
    vector_display.x_kinetics.acceleration = 500.0 if hit else 0.0 # Total energy transferred?
    vector_display.y_kinetics.acceleration = 500.0 if hit else 0.0

    window.set_target_frame_buffer('window', [0])
    window.clear()

    elapsed_time = window.refresh_time
    vector_display.update(elapsed_time)
    vector_display.draw(window, 'window', 0, camera, elapsed_time)

    # window.copy_frame_buffer('vector_display', vector_display.current_value_color_attachment, 'window', 0)
    window.draw()
