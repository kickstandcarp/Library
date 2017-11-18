from sys import path
from os.path import pardir, split, join, realpath
path.append(join(split(realpath(__file__))[0], pardir, 'python', 'lib'))

from math import radians

from matplotlib.pylab import show, figure, subplot, plot, grid

from glm import vec2, vec3, quat, exp
from clock import Clock
from physics import OscillatorKineticsFloat, OscillatorKineticsVec2, PaperKinetics



dt = 0.016667

clock = Clock(0.0, dt)

# kinetics = PaperKinetics(position=vec3(0.0), orientation=quat(vec3(1.0, 0.0, 0.0), radians(45.0)), velocity=vec3(0.0), angular_velocity=vec3(100.0, 0.0, 0.0), external_acceleration=vec3(0.0, -9.8, 0.0), external_angular_acceleration=vec3(0.0), perpendicular_friction=10.0, parallel_friction=0.1, fluid_density=0.1, paper_density=1.0, size=vec3(1.0, 0.0, 1.0))
# kinetics.min_step = 0.001
# kinetics.x_step_tolerance = 0.001, 0.001
# kinetics.dx_dt_step_tolerance = 0.1, 0.1

kinetics = OscillatorKineticsVec2(value=vec2(0.0), velocity=vec2(0.0), external_acceleration=vec2(0.0), frequency=vec2(2.0, 10.0), damping_ratio=vec2(0.5, 0.1))
kinetics.min_step = 0.001
kinetics.x_step_tolerance = 0.001
kinetics.dx_dt_step_tolerance = 0.1

times, values, velocities = [clock.time], [kinetics.value], [kinetics.velocity]
while clock.time < 1.0:
    kinetics.external_acceleration = vec2(100.0) if clock.time == 0.0 else vec2(0.0)
    kinetics.step(clock)

    times.append(clock.time)
    values.append(kinetics.value)
    velocities.append(kinetics.velocity)

    clock.step(dt)

figure()

subplot(1, 2, 1)
plot(times, list(zip(*values))[0])
plot(times, list(zip(*values))[1])
grid(True)
subplot(1, 2, 2)
plot(times, list(zip(*velocities))[0])
plot(times, list(zip(*velocities))[1])
grid(True)

show()
