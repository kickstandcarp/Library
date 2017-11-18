from uuid import uuid4

from glm import vec2, vec3
from image import GaussianFilter
from explode_mode import VectorDisplayCurve

from text import character_size, character_spacing, slide_size, font_vertex_data, text_size, vector_display_character_vertices, concatenate_vector_display_text_vertices, vector_display_text_vertices
from menu import menu_text_scale, menu_text_delay, menu_text_velocity, menu_selector_velocity, menu_text_width, menu_text_color, menu_text_excitation, menu_text_min_vertex_distance, Menu, MenuElement, MenuSelector, MenuSelection, MenuSetting, MenuCheckBox, MenuSlider



def initialize_vector_display_glow_filter(system):
    glow_filter = GaussianFilter(system.window)

    glow_filter.size = 0.1
    glow_filter.width = 0.01
    glow_filter.amplitude = 3.0

    glow_filter.source_frame_buffer_name = 'draw'
    glow_filter.source_color_attachment_index = 0

    glow_filter.intermediate_frame_buffer_name = 'draw'
    glow_filter.intermediate_color_attachment_index = 1

    glow_filter.destination_frame_buffer_name = 'window'
    glow_filter.destination_color_attachment_index = 0

    return glow_filter

def initialize_menu_selector(system):
    vertices = vector_display_character_vertices('selector', -0.5*menu_text_scale*character_size, menu_text_scale, menu_selector_velocity, menu_text_min_vertex_distance)

    vector_display_curves = []
    for i in range(5):
        vector_display_curves.append(VectorDisplayCurve(vertices, system.window, str(uuid4())))
        vector_display_curves[-1].color, vector_display_curves[-1].excitation, vector_display_curves[-1].width, vector_display_curves[-1].cap_height = menu_text_color, menu_text_excitation, menu_text_width, 0.5*menu_text_width

    selector = MenuSelector(vector_display_curves)
    selector.rotational_velocity = -0.5

    return selector

def initialize_start_menu(system):
    title_text_scale = 0.2
    x_translation = 1.0 - title_text_scale*(text_size('EXPLODE\nMODE').x + 2.0*character_spacing.x)

    title_y = 0.2
    title_vector_display_curve = VectorDisplayCurve(vector_display_text_vertices('EXPLODE\nMODE', vec2(x_translation, title_y), title_text_scale, 1.5*menu_text_velocity, menu_text_min_vertex_distance), system.window, str(uuid4()))
    title_vector_display_curve.color, title_vector_display_curve.excitation, title_vector_display_curve.width, title_vector_display_curve.cap_height = menu_text_color, menu_text_excitation, 0.01, 0.5*0.01
    title = MenuElement(title_vector_display_curve)

    y_translation = menu_text_scale*(character_size.y + 2.0*character_spacing.y)

    start_y = -0.5
    start_vector_display_curve = VectorDisplayCurve(vector_display_text_vertices('START', vec2(x_translation, start_y), menu_text_scale, menu_text_velocity, menu_text_min_vertex_distance), system.window, str(uuid4()))
    start_vector_display_curve.color, start_vector_display_curve.excitation, start_vector_display_curve.width, start_vector_display_curve.cap_height = menu_text_color, menu_text_excitation, menu_text_width, 0.5*menu_text_width
    start_selector_position = vec2(x_translation - menu_text_scale*(0.5*character_size.x + 2.0*character_spacing.x), start_y + 0.5*menu_text_scale*character_size.y)
    start = MenuSelection(start_vector_display_curve, start_selector_position)

    options_y = start_y - y_translation
    options_vector_display_curve = VectorDisplayCurve(vector_display_text_vertices('OPTIONS', vec2(x_translation, options_y), menu_text_scale, menu_text_velocity, menu_text_min_vertex_distance), system.window, str(uuid4()))
    options_vector_display_curve.color, options_vector_display_curve.excitation, options_vector_display_curve.width, options_vector_display_curve.cap_height = menu_text_color, menu_text_excitation, menu_text_width, 0.5*menu_text_width
    options_selector_position = vec2(x_translation - menu_text_scale*(0.5*character_size.x + 2.0*character_spacing.x), options_y + 0.5*menu_text_scale*character_size.y)
    options = MenuSelection(options_vector_display_curve, options_selector_position, system.switch_menu, ('options', 'start', True))

    instructions_y = start_y - 2.0*y_translation
    instructions_vector_display_curve = VectorDisplayCurve(vector_display_text_vertices('INSTRUCTIONS', vec2(x_translation, instructions_y), menu_text_scale, menu_text_velocity, menu_text_min_vertex_distance), system.window, str(uuid4()))
    instructions_vector_display_curve.color, instructions_vector_display_curve.excitation, instructions_vector_display_curve.width, instructions_vector_display_curve.cap_height = menu_text_color, menu_text_excitation, menu_text_width, 0.5*menu_text_width
    instructions_selector_position = vec2(x_translation - menu_text_scale*(0.5*character_size.x + 2.0*character_spacing.x), instructions_y + 0.5*menu_text_scale*character_size.y)
    instructions = MenuSelection(instructions_vector_display_curve, instructions_selector_position)

    return Menu([title, start, options, instructions])

def initialize_options_menu(system):
    x_translation = 0.85*menu_text_scale*(character_size.x + 2.0*character_spacing.x)
    y_translation = 0.85*menu_text_scale*(character_size.y + 2.0*character_spacing.y)

    mode_y = 0.8
    mode_vector_display_curve = VectorDisplayCurve(vector_display_text_vertices('MODE', vec2(-1.0 + x_translation + 0.85*menu_text_scale*character_size.x, mode_y), 0.85*menu_text_scale, menu_text_velocity, menu_text_min_vertex_distance), system.window, str(uuid4()))
    mode_vector_display_curve.color, mode_vector_display_curve.excitation, mode_vector_display_curve.width, mode_vector_display_curve.cap_height = menu_text_color, menu_text_excitation, menu_text_width, 0.5*menu_text_width
    mode_selector_position = vec2(-1.0 + x_translation - 0.85*menu_text_scale*(-0.5*character_size.x + 2.0*character_spacing.x), mode_y + 0.5*0.85*menu_text_scale*character_size.y)
    mode_setting_vector_display_curves = []
    for text in ['NORMAL', 'ENDLESS', 'OTHER']:
        mode_setting_vector_display_curves.append(VectorDisplayCurve(vector_display_text_vertices(text, vec2(1.0 - x_translation - 0.85*menu_text_scale*text_size(text).x, mode_y), 0.85*menu_text_scale, menu_text_velocity, menu_text_min_vertex_distance), system.window, str(uuid4())))
        mode_setting_vector_display_curves[-1].color, mode_setting_vector_display_curves[-1].excitation, mode_setting_vector_display_curves[-1].width, mode_setting_vector_display_curves[-1].cap_height = menu_text_color, menu_text_excitation, menu_text_width, 0.5*menu_text_width
    mode = MenuSetting(mode_vector_display_curve, mode_setting_vector_display_curves, mode_selector_position)

    invert_y_y = mode_y - 2.0*y_translation
    invert_y_vertices = concatenate_vector_display_text_vertices(vector_display_text_vertices('INVERT Y', vec2(-1.0 + x_translation + 0.85*menu_text_scale*character_size.x, invert_y_y), 0.85*menu_text_scale, menu_text_velocity, menu_text_min_vertex_distance), vector_display_character_vertices('check_box', vec2(1.0 - x_translation - 0.85*menu_text_scale*character_size.x, invert_y_y), 0.85*menu_text_scale, menu_text_velocity, menu_text_min_vertex_distance), menu_text_velocity)
    invert_y_vector_display_curve = VectorDisplayCurve(invert_y_vertices, system.window, str(uuid4()))
    invert_y_vector_display_curve.color, invert_y_vector_display_curve.excitation, invert_y_vector_display_curve.width, invert_y_vector_display_curve.cap_height = menu_text_color, menu_text_excitation, menu_text_width, 0.5*menu_text_width
    invert_y_mark_vertices = vector_display_character_vertices('check_mark', vec2(1.0 - x_translation - 0.85*menu_text_scale*character_size.x, invert_y_y), 0.85*menu_text_scale, menu_text_velocity, menu_text_min_vertex_distance)
    invert_y_mark_vector_display_curve = VectorDisplayCurve(invert_y_mark_vertices, system.window, str(uuid4()))
    invert_y_mark_vector_display_curve.color, invert_y_mark_vector_display_curve.excitation, invert_y_mark_vector_display_curve.width, invert_y_mark_vector_display_curve.cap_height = menu_text_color, menu_text_excitation, menu_text_width, 0.5*menu_text_width
    invert_y_selector_position = vec2(-1.0 + x_translation - 0.85*menu_text_scale*(-0.5*character_size.x + 2.0*character_spacing.x), invert_y_y + 0.5*0.85*menu_text_scale*character_size.y)
    invert_y = MenuCheckBox(invert_y_vector_display_curve, invert_y_mark_vector_display_curve, invert_y_selector_position, system.invert_y)

    slider_size = 0.85*menu_text_scale*(text_size('SENSITIVITY').x + 2.0*character_size.x)
    slider_offset = 1.0 - x_translation - slider_size

    sensitivity_y = invert_y_y - y_translation
    sensitivity_vertices = concatenate_vector_display_text_vertices(vector_display_text_vertices('SENSITIVITY', vec2(-1.0 + x_translation + 0.85*menu_text_scale*character_size.x, sensitivity_y), 0.85*menu_text_scale, menu_text_velocity, menu_text_min_vertex_distance), vector_display_character_vertices('slider', vec2(slider_offset, sensitivity_y), vec2(slider_size, 0.85*menu_text_scale), menu_text_velocity, menu_text_min_vertex_distance), menu_text_velocity)
    sensitivity_vector_display_curve = VectorDisplayCurve(sensitivity_vertices, system.window, str(uuid4()))
    sensitivity_vector_display_curve.color, sensitivity_vector_display_curve.excitation, sensitivity_vector_display_curve.width, sensitivity_vector_display_curve.cap_height = menu_text_color, menu_text_excitation, menu_text_width, 0.5*menu_text_width
    sensitivity_slider_vertices = vector_display_character_vertices('slide', -0.5*0.85*menu_text_scale*character_size, 0.85*menu_text_scale, menu_text_velocity, menu_text_min_vertex_distance)
    sensitivity_slider_vector_display_curves = []
    for i in range(5):
        sensitivity_slider_vector_display_curves.append(VectorDisplayCurve(sensitivity_slider_vertices, system.window, str(uuid4())))
        sensitivity_slider_vector_display_curves[-1].color, sensitivity_slider_vector_display_curves[-1].excitation, sensitivity_slider_vector_display_curves[-1].width, sensitivity_slider_vector_display_curves[-1].cap_height = menu_text_color, menu_text_excitation, menu_text_width, 0.5*menu_text_width
    sensitivity_slide = MenuSelector(sensitivity_slider_vector_display_curves)
    sensitivity_slider_positions = [vec2((i / 10.0)*(slider_size - 0.85*menu_text_scale*slide_size.x) + slider_offset + 0.5*0.85*menu_text_scale*slide_size.x, sensitivity_y + 0.5*0.85*menu_text_scale*slide_size.y) for i in range(11)]
    sensitivity_selector_position = vec2(-1.0 + x_translation - 0.85*menu_text_scale*(-0.5*character_size.x + 2.0*character_spacing.x), sensitivity_y + 0.5*0.85*menu_text_scale*character_size.y)
    sensitivity = MenuSlider(sensitivity_vector_display_curve, sensitivity_slide, sensitivity_slider_positions, sensitivity_selector_position, system.aim_sensitivity)

    exit_y = sensitivity_y - 2.0*y_translation
    exit_vector_display_curve = VectorDisplayCurve(vector_display_text_vertices('EXIT', vec2(-1.0 + x_translation + 0.85*menu_text_scale*character_size.x, exit_y), 0.85*menu_text_scale, menu_text_velocity, menu_text_min_vertex_distance), system.window, str(uuid4()))
    exit_vector_display_curve.color, exit_vector_display_curve.excitation, exit_vector_display_curve.width, exit_vector_display_curve.cap_height = menu_text_color, menu_text_excitation, menu_text_width, 0.5*menu_text_width
    exit_selector_position = vec2(-1.0 + x_translation - 0.85*menu_text_scale*(-0.5*character_size.x + 2.0*character_spacing.x), exit_y + 0.5*0.85*menu_text_scale*character_size.y)
    exit = MenuSelection(exit_vector_display_curve, exit_selector_position, system.switch_menu, ('start', 'options', True))

    return Menu([mode, invert_y, sensitivity, exit])
