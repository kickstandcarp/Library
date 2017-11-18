from sys import path
from os.path import pardir, split, join, realpath
path.append(join(split(realpath(__file__))[0], pardir, pardir, 'python', 'lib'))

from system import System



size = 500, 500
system = System(size)

system.switch_menu('start', None, False)

while not system.window.event_handler.quit:
    system.clock.step(system.window.refresh_time)
    system.window.event_handler.step(system.clock)

    system.update()
    system.step()
    system.draw()
