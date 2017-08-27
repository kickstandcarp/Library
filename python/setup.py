from os import environ
from os.path import pardir, join, split, abspath, realpath
from distutils.core import setup, Extension

from pybind11 import get_include



path = split(realpath(__file__))[0]

include_dirs = []
include_dirs.append(get_include(False))
include_dirs.append(abspath(join(path, 'include')))
include_dirs.append(abspath(join(path, pardir, 'common', 'include')))

extra_compile_args = ['-std=c++11', '-O0']
# extra_compile_args = ['/Zi', '/Od']
extra_link_args = []
# extra_link_args = ['/DEBUG']

src_files = []
src_files.append(abspath(join(path, 'src', 'module', 'glm.cpp')))
src_files.append(abspath(join(path, 'src', 'repr', 'repr.cpp')))
src_files.append(abspath(join(path, 'src', 'repr', 'glm_repr.cpp')))

glm_extension = Extension('glm', src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)

src_files = []
src_files.append(abspath(join(path, 'src', 'module', 'opengl.cpp')))
src_files.append(abspath(join(path, 'src', 'repr', 'repr.cpp')))
src_files.append(abspath(join(path, 'src', 'repr', 'glm_repr.cpp')))
src_files.append(abspath(join(path, 'src', 'repr', 'opengl_repr.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'window.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'buffer.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'buffer_support.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'shader.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'shader_support.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'uniform_support.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'vertex_array.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'texture.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'opengl', 'frame_buffer.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'event', 'event_handler.cpp')))

opengl_extension = Extension('opengl', src_files, language='c++', include_dirs=include_dirs, libraries=['GLEW', 'SDL2'], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)
# opengl_extension = Extension('opengl', src_files, language='c++', include_dirs=include_dirs, libraries=['opengl32', 'glew32', 'SDL2'], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)

src_files = []
src_files.append(abspath(join(path, 'src', 'module', 'event.cpp')))
src_files.append(abspath(join(path, 'src', 'repr', 'repr.cpp')))
src_files.append(abspath(join(path, 'src', 'repr', 'event_repr.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'event', 'event_handler.cpp')))

event_extension = Extension('event', src_files, language='c++', include_dirs=include_dirs, libraries=['SDL2'], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)

src_files = []
src_files.append(abspath(join(path, 'src', 'module', 'coordinate.cpp')))
src_files.append(abspath(join(path, 'src', 'repr', 'repr.cpp')))
src_files.append(abspath(join(path, 'src', 'repr', 'coordinate_repr.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'coordinate', 'coordinate_transform.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'coordinate', 'coordinate_transform_support.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'coordinate', 'camera.cpp')))

coordinate_extension = Extension('coordinate', src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)

src_files = []
src_files.append(abspath(join(path, 'src', 'module', 'physics.cpp')))
src_files.append(abspath(join(path, 'src', 'repr', 'repr.cpp')))
src_files.append(abspath(join(path, 'src', 'repr', 'physics_repr.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'math', 'arithmatic.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'math', 'integration.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'physics', 'oscillator_kinetics.cpp')))
src_files.append(abspath(join(path, pardir, 'common', 'src', 'physics', 'paper_kinetics.cpp')))

physics_extension = Extension('physics', src_files, language='c++', include_dirs=include_dirs, libraries=[], extra_compile_args=extra_compile_args, extra_link_args=extra_link_args)

ext_modules = [glm_extension, opengl_extension, event_extension, coordinate_extension, physics_extension]
setup(ext_modules=ext_modules)
