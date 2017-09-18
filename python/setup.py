from os import environ
from os.path import pardir, join, split, abspath, realpath
from distutils.core import setup, Extension
import platform

from pybind11 import get_include



path = split(realpath(__file__))[0]

include_dirs = []
include_dirs.append(get_include(False))
include_dirs.append(abspath(join(path, 'include')))
include_dirs.append(abspath(join(path, pardir, 'common', 'include')))

extra_compile_args = ['-std=c++11', '-O0'] if platform.system() != 'Windows' else ['/Zi', '/Od']
extra_link_args = [] if platform.system() != 'Windows' else ['/DEBUG']

src_files = []
src_files.append(abspath(join(path, 'src', 'glm.cpp')))
src_files.append(abspath(join(path, 'src', 'repr.cpp')))

libraries = []
glm_extension = Extension('glm', src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)

src_files = []
src_files.append(abspath(join(path, 'src', 'geometry.cpp')))
src_files.append(abspath(join(path, 'src', 'repr.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'geometry', 'coordinate_transform.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'geometry', 'coordinate_transform_support.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'geometry', 'path_vertex.cpp')))

libraries = []
geometry_extension = Extension('geometry', src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)

src_files = []
src_files.append(abspath(join(path, 'src', 'physics.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'math', 'arithmatic.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'math', 'integration.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'physics', 'paper_kinetics.cpp')))

libraries = []
physics_extension = Extension('physics', src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)

src_files = []
src_files.append(abspath(join(path, 'src', 'event.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'event', 'event_handler.cpp')))

libraries = ['SDL2']
event_extension = Extension('event', src_files, language='c++', include_dirs=include_dirs, libraries=libraries, extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)

src_files = []
src_files.append(abspath(join(path, 'src', 'opengl.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'window.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'buffer.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'buffer_support.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'shader.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'shader_support.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'uniform_support.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'vertex_array.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'texture.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'frame_buffer.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'camera.cpp')))

libraries = ['opengl32', 'glew32', 'SDL2'] if platform.system() == 'Windows' else ['GLEW', 'SDL2'] 
opengl_extension = Extension('opengl', src_files, language='c++', include_dirs=include_dirs, libraries=libraries, extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)

src_files = []
src_files.append(abspath(join(path, 'src', 'image.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'image', 'gaussian_filter.cpp')))

libraries = []
image_extension = Extension('image', src_files, language='c++', include_dirs=include_dirs, libraries=libraries, extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)


include_dirs.append(abspath(join(path, pardir, 'test', 'vector_display', 'common', 'include')))

src_files = []
src_files.append(abspath(join(path, pardir, 'test', 'vector_display', 'python', 'src', 'vector_display.cpp')))
src_files.append(abspath(join(path, pardir, 'test', 'vector_display', 'common', 'src', 'vector_display.cpp')))
src_files.append(abspath(join(path, pardir, 'test', 'vector_display', 'common', 'src', 'vector_display_path.cpp')))
src_files.append(abspath(join(path, pardir, 'test', 'vector_display', 'common', 'src', 'trochoid_path.cpp')))

libraries = []
vector_display_extension = Extension('vector_display', src_files, language='c++', include_dirs=include_dirs, libraries=libraries, extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)


ext_modules = [glm_extension, geometry_extension, physics_extension, event_extension, opengl_extension, image_extension, vector_display_extension]
setup(ext_modules=ext_modules)
