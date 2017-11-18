from math import ceil
from collections import Iterable
from itertools import accumulate
from json import loads

from glm import vec2, vec3, vec4, distance, mix



character_size = vec2(2.0 / 3.0, 1.0)
character_spacing = vec2(2.0 / 12.0, 2.0 / 12.0)

font_vertex_data_file_name = 'font_vertex_data.json'
with open(font_vertex_data_file_name) as file:
    font_vertex_data = loads(file.read())

selector_size = vec2(0.5, 0.5)
selector_offset = vec2(0.5*(character_size.x - selector_size.x), 0.5*(character_size.y - selector_size.y))
font_vertex_data['selector'] = [[selector_size.x*vertex[0] + selector_offset.x, selector_size.y*vertex[1] + selector_offset.y] for vertex in [[0.0, 1.0], [0.0, 0.0], [0.0, 1.0], [1.0, 1.0], [0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [1.0, 0.0]]]

check_box_size = vec2(min(character_size), min(character_size))
check_box_offset = vec2(0.0, 0.5*(max(character_size) - min(character_size)))
font_vertex_data['check_box'] = [[check_box_size.x*vertex[0] + check_box_offset.x, check_box_size.y*vertex[1] + check_box_offset.y] for vertex in [[0.0, 1.0], [0.0, 0.0], [0.0, 1.0], [1.0, 1.0], [0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [1.0, 0.0]]]
font_vertex_data['check_mark'] = [[check_box_size.x*vertex[0] + check_box_offset.x, check_box_size.y*vertex[1] + check_box_offset.y] for vertex in [[0.0, 1.0], [1.0, 0.0], [0.0, 0.0], [1.0, 1.0]]]

slide_size = vec2(0.375, 0.833333333333333)
slide_offset = vec2(0.5*(character_size.x - slide_size.x), 0.5*(character_size.y - slide_size.y))
font_vertex_data['slider'] = [[0.0, 0.7083333333333334], [0.0, 0.29166666666666663], [0.0, 0.5], [1.0, 0.5], [1.0, 0.7083333333333334], [1.0, 0.29166666666666663]]
font_vertex_data['slide'] = [[slide_size.x*vertex[0] + slide_offset.x, slide_size.y*vertex[1] + slide_offset.y] for vertex in [[0.0, 1.0], [0.0, 0.0], [0.0, 1.0], [1.0, 1.0], [0.0, 0.0], [1.0, 0.0], [1.0, 1.0], [1.0, 0.0]]]

def text_size(text):
    size, line_size = vec2(0.0), vec2(0.0)
    for character in text:
        if character == '\n':
            size.x = max(size.x, line_size.x)
            size.y = size.y + character_size.y + character_spacing.y
            line_size = vec2(0.0)
        else:
            line_size.x = line_size.x + character_size.x + character_spacing.x

    size.x = max(size.x, line_size.x) - (character_spacing.x if len(text) > 0.0 else 0.0)
    size.y = size.y + character_size.y

    return size

def vector_display_character_vertices(character, translation, scale, velocity, min_vertex_distance):
    scale = vec2(scale, scale) if not isinstance(scale, Iterable) else vec2(scale[0], scale[1])

    vertices = []
    scaled_translated_vertices = [scale*vec2(vertex) + translation for vertex in font_vertex_data[character]]
    for vertex, next_vertex in zip(scaled_translated_vertices[:-1:2], scaled_translated_vertices[1::2]):
        num_vertices = int(ceil(distance(vertex, next_vertex) / min_vertex_distance))
        vertices += [vec3(mix(vertex, next_vertex, vec2(i / (num_vertices-1))), 1.0) for i in range(num_vertices)]

        vertices.insert(len(vertices) - num_vertices, vec3(vertices[len(vertices) - num_vertices].x, vertices[len(vertices) - num_vertices].y, 0.0))
        vertices.insert(len(vertices), vec3(vertices[len(vertices)-1].x, vertices[len(vertices)-1].y, 0.0))

    ts = list(accumulate([0.0] + [distance(vec2(vertex.x, vertex.y), vec2(next_vertex.x, next_vertex.y)) for vertex, next_vertex in zip(vertices[:-1], vertices[1:])]))
    return [vec4(vertex.x, vertex.y, t / velocity, vertex.z) for vertex, t in zip(vertices, ts)]

def concatenate_vector_display_text_vertices(vertices, concatenated_vertices, velocity):
    t = vertices[-1].z + distance(vec2(vertices[-1].x, vertices[-1].y), vec2(concatenated_vertices[0].x,  concatenated_vertices[0].y)) / velocity if len(vertices) > 0 else 0.0
    vertices += [vec4(vertex.x, vertex.y, vertex.z + t, vertex.w) for vertex in concatenated_vertices]
    return vertices

def vector_display_text_vertices(text, translation, scale, velocity, min_vertex_distance):
    vertices = []
    offset = vec2(0.0)
    for character in text:
        if character == '\n':
            offset.x = 0.0
            offset.y -= scale*(character_spacing.y + character_size.y)
        elif character == ' ':
            offset.x += scale*(character_spacing.x + character_size.x)
        else:
            vertices = concatenate_vector_display_text_vertices(vertices, vector_display_character_vertices(character, translation + offset, scale, velocity, min_vertex_distance), velocity)
            offset.x += scale*(character_spacing.x + character_size.x)

    return vertices
