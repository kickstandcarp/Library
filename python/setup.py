from os import environ
from os.path import pardir, join, split, abspath, realpath
from distutils.core import setup, Extension
import platform

from pybind11 import get_include

# python setup.py build_ext --build-lib "lib" --build-temp "build"

path = split(realpath(__file__))[0]

include_dirs = []
include_dirs.append(get_include(False))
include_dirs.append(abspath(join(path, 'include')))
include_dirs.append(abspath(join(path, pardir, 'common', 'include')))

extra_compile_args = ['-std=c++11', '-O0'] if platform.system() != 'Windows' else ['/Zi', '/Od']
extra_link_args = [] if platform.system() != 'Windows' else ['/DEBUG']

glm_src_files = []
glm_src_files.append(abspath(join(path, 'src', 'glm.cpp')))
glm_src_files.append(abspath(join(path, 'src', 'repr.cpp')))

clock_src_files = []
clock_src_files.append(abspath(join(path, 'src', 'clock.cpp')))
clock_src_files.append(abspath(join(path, pardir, 'common', 'src', 'clock.cpp')))

geometry_src_files = []
geometry_src_files.append(abspath(join(path, 'src', 'geometry.cpp')))
geometry_src_files.append(abspath(join(path, pardir, 'common', 'src', 'geometry', 'coordinate_transform.cpp')))
geometry_src_files.append(abspath(join(path, pardir, 'common', 'src', 'geometry', 'coordinate_transform_support.cpp')))

parametric_src_files = []
parametric_src_files.append(abspath(join(path, 'src', 'parametric.cpp')))
parametric_src_files.append(abspath(join(path, pardir, 'common', 'src', 'math', 'interpolation.cpp')))
parametric_src_files.append(abspath(join(path, pardir, 'common', 'src', 'parametric', 'curve_support.cpp')))
parametric_src_files.append(abspath(join(path, pardir, 'common', 'src', 'parametric', 'segment_curve.cpp')))

physics_src_files = []
physics_src_files.append(abspath(join(path, 'src', 'physics.cpp')))
physics_src_files.append(abspath(join(path, pardir, 'common', 'src', 'math', 'arithmatic.cpp')))
physics_src_files.append(abspath(join(path, pardir, 'common', 'src', 'math', 'integration.cpp')))
physics_src_files.append(abspath(join(path, pardir, 'common', 'src', 'physics', 'paper_kinetics.cpp')))

event_src_files = []
event_src_files.append(abspath(join(path, 'src', 'event.cpp')))
event_src_files.append(abspath(join(path, pardir, 'common', 'src', 'event', 'event_handler.cpp')))
event_libraries = ['SDL2']

opengl_src_files = []
opengl_src_files.append(abspath(join(path, 'src', 'opengl.cpp')))
opengl_src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'window.cpp')))
opengl_src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'buffer.cpp')))
opengl_src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'buffer_support.cpp')))
opengl_src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'shader.cpp')))
opengl_src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'shader_support.cpp')))
opengl_src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'uniform_support.cpp')))
opengl_src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'vertex_array.cpp')))
opengl_src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'texture.cpp')))
opengl_src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'frame_buffer.cpp')))
opengl_src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'camera.cpp')))
opengl_libraries = ['opengl32', 'glew32'] if platform.system() == 'Windows' else ['GLEW']

image_src_files = []
image_src_files.append(abspath(join(path, 'src', 'image.cpp')))
image_src_files.append(abspath(join(path, pardir, 'common', 'src', 'image', 'gaussian_filter.cpp')))

vector_display_include_dirs = [abspath(join(path, pardir, 'test', 'vector_display', 'common', 'include'))]
vector_display_src_files = []
vector_display_src_files.append(abspath(join(path, pardir, 'test', 'vector_display', 'python', 'src', 'vector_display.cpp')))
vector_display_src_files.append(abspath(join(path, pardir, 'test', 'vector_display', 'common', 'src', 'vector_display.cpp')))
vector_display_src_files.append(abspath(join(path, pardir, 'test', 'vector_display', 'common', 'src', 'vector_display_curve.cpp')))
vector_display_src_files.append(abspath(join(path, pardir, 'test', 'vector_display', 'common', 'src', 'trochoid.cpp')))

'''
glm_extension = Extension('glm', glm_src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
clock_extension = Extension('clock', clock_src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
geometry_extension = Extension('geometry', geometry_src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
parametric_extension = Extension('parametric', parametric_src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
physics_extension = Extension('physics', physics_src_files + parametric_src_files + clock_src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
event_extension = Extension('event', event_src_files + parametric_src_files + clock_src_files, language='c++', include_dirs=include_dirs, libraries=event_libraries, extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
opengl_extension = Extension('opengl', opengl_src_files + event_src_files + parametric_src_files + geometry_src_files + clock_src_files, language='c++', include_dirs=include_dirs, libraries=opengl_libraries + event_libraries, extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
image_extension = Extension('image', image_src_files + opengl_src_files + event_src_files + parametric_src_files + geometry_src_files + clock_src_files, language='c++', include_dirs=include_dirs, libraries=opengl_libraries + event_libraries, extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
vector_display_extension = Extension('vector_display', vector_display_src_files + image_src_files + opengl_src_files + event_src_files + parametric_src_files + geometry_src_files + clock_src_files, language='c++', include_dirs=include_dirs + vector_display_include_dirs, libraries=opengl_libraries + event_libraries, extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
'''

glm_extension = Extension('glm', glm_src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
clock_extension = Extension('clock', clock_src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
geometry_extension = Extension('geometry', geometry_src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
parametric_extension = Extension('parametric', parametric_src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
physics_extension = Extension('physics', physics_src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
event_extension = Extension('event', event_src_files, language='c++', include_dirs=include_dirs, libraries=event_libraries, extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
opengl_extension = Extension('opengl', opengl_src_files, language='c++', include_dirs=include_dirs, libraries=opengl_libraries + event_libraries, extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
image_extension = Extension('image', image_src_files, language='c++', include_dirs=include_dirs, libraries=opengl_libraries + event_libraries, extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
vector_display_extension = Extension('vector_display', vector_display_src_files, language='c++', include_dirs=include_dirs + vector_display_include_dirs, libraries=opengl_libraries + event_libraries, extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)

ext_modules = [glm_extension, clock_extension, geometry_extension, parametric_extension, physics_extension, event_extension, opengl_extension, image_extension, vector_display_extension]
setup(ext_modules=ext_modules)
