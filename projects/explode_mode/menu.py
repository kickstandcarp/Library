from glm import vec2, vec3, distance
from event import DeviceType



menu_text_scale = 0.1
menu_text_delay = 0.2
menu_text_velocity, menu_selector_velocity = 1.0, 1.5
menu_text_velocity_hurry_mulitplier = 100.0
menu_text_width = 0.007
menu_text_color = vec3(0.7, 0.7, 1.0)
menu_text_excitation = 10.0
menu_text_excitation_decay_time_constant, menu_text_decay_decay_time_constant = 0.05, 0.2
menu_text_min_vertex_distance = 0.01

class Menu(object):
    def __init__(self, elements):
        super(Menu, self).__init__()

        self.start_time, self.stop_time = None, 0.0
        self.hurry_ready_delay = 0.25

        self.elements = elements

        self.selector = None
        self.selection_index = 0
        self.selectable_elements = [element for element in self.elements if isinstance(element, MenuSelection)]

    def start(self, clock, window, selector):
        self.start_time = clock.time
        self.stop_time = None
        self.selector = selector

        delay = 0.0
        for element in self.elements:
            element.start(clock, delay)
            delay += element.duration() + menu_text_delay

        self.selection_index = 0
        self.selector.start(clock, delay, self.selectable_elements[self.selection_index].selector_position)

    def hurry(self, clock):
        for element in self.elements:
            element.hurry(clock)
        self.selector.hurry(clock, False)

    def stop(self, clock):
        self.start_time = None
        self.stop_time = clock.time

        self.selector.stop()
        self.selector = None

        for element in self.elements:
            element.stop()

    def update(self, clock, event_handler):
        if self.stop_time is None or clock.time <= self.stop_time + 5.0*menu_text_decay_decay_time_constant:
            if self.selector is not None and event_handler.get_button_moved_down('return', DeviceType.keyboard):
                if self.selector.ready(clock):
                    self.selectable_elements[self.selection_index].select(clock)
                elif clock.time > self.start_time + self.hurry_ready_delay:
                    self.hurry(clock)

            if self.selector is not None:
                if self.selector.ready(clock) and ((self.selector.switch_selection_ready(clock) and event_handler.get_button('down', DeviceType.keyboard)) or event_handler.get_button_moved_down('down', DeviceType.keyboard)):
                    self.selection_index = (self.selection_index + 1) % len(self.selectable_elements)
                    self.selector.switch_selection(clock, 0.0, self.selectable_elements[self.selection_index].selector_position)
                elif self.selector.ready(clock) and ((self.selector.switch_selection_ready(clock) and event_handler.get_button('up', DeviceType.keyboard)) or event_handler.get_button_moved_down('up', DeviceType.keyboard)):
                    self.selection_index = (self.selection_index - 1) % len(self.selectable_elements)
                    self.selector.switch_selection(clock, 0.0, self.selectable_elements[self.selection_index].selector_position)

                if self.selector.ready(clock) and event_handler.get_button('right', DeviceType.keyboard):
                    self.selectable_elements[self.selection_index].forward(clock, not event_handler.get_button_moved_down('right', DeviceType.keyboard))
                elif self.selector.ready(clock) and event_handler.get_button('left', DeviceType.keyboard):
                    self.selectable_elements[self.selection_index].back(clock, not event_handler.get_button_moved_down('left', DeviceType.keyboard))

    def step(self, clock, window):
        if self.stop_time is None or clock.time <= self.stop_time + 5.0*menu_text_decay_decay_time_constant:
            for element in self.elements:
                element.step(clock, window)

            if self.selector is not None:
                self.selector.step(clock, window)

    def draw(self, clock, window):
        if self.stop_time is None or clock.time <= self.stop_time + 5.0*menu_text_decay_decay_time_constant:
            window.set_target_frame_buffer('draw', [0])
            for element in self.elements:
                element.draw(window)

            if self.selector is not None:
                self.selector.draw(window)

class MenuSelector(object):
    def __init__(self, vector_display_curves):
        super(MenuSelector, self).__init__()

        self.start_time, self.start_drawing_time, self.switch_selection_time = None, None, None
        self.switch_selection_ready_delay = 0.25

        self.index = 0
        self.curves = vector_display_curves

        self.rotational_velocity = 0.0
        self.rotation_update_delay = 0.75

    def ready(self, clock):
        return self.start_drawing_time is not None and clock.time > self.start_drawing_time

    def switch_selection_ready(self, clock):
        return self.switch_selection_time is not None and clock.time > self.switch_selection_time + self.switch_selection_ready_delay

    def next_curve(self, clock, delay):
        translation, rotation = self.curves[self.index].translation, self.curves[self.index].rotation
        self.curves[self.index].excitation = 0.0
        self.curves[self.index].decay_time_constant = menu_text_decay_decay_time_constant

        self.index = (self.index + 1) % len(self.curves)

        self.curves[self.index].start_time = clock.time + delay
        self.curves[self.index].velocity = 1.0
        self.curves[self.index].excitation = menu_text_excitation
        self.curves[self.index].decay_time_constant = menu_text_excitation_decay_time_constant
        self.curves[self.index].translation = translation
        self.curves[self.index].rotation = rotation

    def switch_selection(self, clock, delay, position):
        self.switch_selection_time = clock.time

        self.next_curve(clock, delay)
        self.curves[self.index].translation = position
        self.curves[self.index].rotation = self.rotational_velocity*max(0.0, clock.time - self.start_drawing_time)

    def start(self, clock, delay, position):
        self.start_time, self.start_drawing_time = clock.time, clock.time + delay

        self.switch_selection(clock, delay, position)
        self.switch_selection_time = clock.time + delay

    def hurry(self, clock, hurry_velocity):
        self.start_drawing_time = clock.time + ((self.start_drawing_time - clock.time) / menu_text_velocity_hurry_mulitplier)
        self.switch_selection_time = self.start_drawing_time

        self.curves[self.index].start_time = self.start_drawing_time
        if hurry_velocity:
            self.curves[self.index].velocity = menu_text_velocity_hurry_mulitplier

    def stop(self):
        self.start_time, self.start_drawing_time, self.switch_selection_time = None, None, None

        for curve in self.curves:
            curve.excitation = 0.0
            curve.decay_time_constant = menu_text_decay_decay_time_constant

    def step(self, clock, window):
        if abs(self.rotational_velocity) > 0.0 and self.start_drawing_time is not None and clock.time > self.start_drawing_time and self.switch_selection_time is not None and (clock.time - self.switch_selection_time) // self.rotation_update_delay != (clock.time + clock.elapsed_time - self.switch_selection_time) // self.rotation_update_delay:
            self.curves[self.index].rotation = self.rotational_velocity*(clock.time - self.start_drawing_time)
            self.next_curve(clock, 0.0)

        for curve in self.curves:
            curve.step(clock, window)

    def draw(self, window):
        for curve in self.curves:
            curve.draw(window)

class MenuElement(object):
    def __init__(self, vector_display_curve):
        super(MenuElement, self).__init__()

        self.start_time, self.start_drawing_time = None, None

        self.curve = vector_display_curve

    @staticmethod
    def start_curve(curve, start_time):
        curve.start_time = start_time
        curve.velocity = 1.0 # menu_text_velocity
        curve.excitation = menu_text_excitation
        curve.decay_time_constant = menu_text_excitation_decay_time_constant

    @staticmethod
    def hurry_curve(curve, start_time):
        curve.start_time = start_time
        curve.velocity = menu_text_velocity_hurry_mulitplier

    @staticmethod
    def stop_curve(curve):
        curve.excitation = 0.0
        curve.decay_time_constant = menu_text_decay_decay_time_constant

    def duration(self):
        return self.curve.last_vertex.z

    def start(self, clock, delay):
        self.start_time, self.start_drawing_time = clock.time, clock.time + delay
        MenuElement.start_curve(self.curve, self.start_drawing_time)

    def hurry(self, clock):
        self.start_drawing_time = clock.time + ((self.start_drawing_time - clock.time) / menu_text_velocity_hurry_mulitplier)
        MenuElement.hurry_curve(self.curve, self.start_drawing_time)

    def stop(self):
        self.start_time, self.start_drawing_time = None, None
        MenuElement.stop_curve(self.curve)

    def step(self, clock, window):
        self.curve.step(clock, window)

    def draw(self, window):
        self.curve.draw(window)

class MenuSelection(MenuElement):
    def __init__(self, vector_display_curve, selector_position, select_callback=None, select_callback_arguments=()):
        super(MenuSelection, self).__init__(vector_display_curve)

        self.selector_position = selector_position

        self.forward_back_time = None
        self.forward_back_ready_delay = 0.25

        self.select_callback = select_callback
        self.select_callback_arguments = select_callback_arguments

    def forward_back_ready(self, clock):
        return self.forward_back_time is not None and clock.time - self.forward_back_time > self.forward_back_ready_delay

    def forward(self, clock, use_ready_delay):
        pass

    def back(self, clock, use_ready_delay):
        pass

    def select(self, clock):
        if self.select_callback is not None:
            self.select_callback(*self.select_callback_arguments)

class MenuSetting(MenuSelection):
    def __init__(self, vector_display_curve, setting_vector_display_curves, selector_position, setting_callback=None, setting_callback_arguments=(), select_callback=None, select_callback_arguments=()):
        super(MenuSetting, self).__init__(vector_display_curve, selector_position, select_callback, select_callback_arguments)

        self.setting_index = 0
        self.setting_curves = setting_vector_display_curves
        self.setting_curve_velocity = 25.0

        self.setting_callback = setting_callback
        self.setting_callback_arguments = setting_callback_arguments

    def duration(self):
        return self.curve.last_vertex.z + (distance(vec2(self.curve.last_vertex.x, self.curve.last_vertex.y), vec2(self.setting_curves[self.setting_index].first_vertex.x, self.setting_curves[self.setting_index].first_vertex.y)) / menu_text_velocity) + self.setting_curves[self.setting_index].last_vertex.z

    def forward(self, clock, use_ready_delay):
        if not use_ready_delay or self.forward_back_ready(clock):
            self.forward_back_time = clock.time

            prev_setting_index = self.setting_index
            self.setting_index = (self.setting_index + 1) % len(self.setting_curves)

            MenuElement.stop_curve(self.setting_curves[prev_setting_index])
            MenuElement.start_curve(self.setting_curves[self.setting_index], clock.time)
            self.setting_curves[self.setting_index].velocity = self.setting_curve_velocity

            if self.setting_callback is not None:
                self.setting_callback(self.setting_index, *self.slider_callback_arguments)

    def back(self, clock, use_ready_delay):
        if not use_ready_delay or self.forward_back_ready(clock):
            self.forward_back_time = clock.time

            prev_setting_index = self.setting_index
            self.setting_index = (self.setting_index - 1) % len(self.setting_curves)

            MenuElement.stop_curve(self.setting_curves[prev_setting_index])
            MenuElement.start_curve(self.setting_curves[self.setting_index], clock.time)
            self.setting_curves[self.setting_index].velocity = self.setting_curve_velocity

            if self.setting_callback is not None:
                self.setting_callback(self.setting_index, *self.slider_callback_arguments)

    def start(self, clock, delay):
        super(MenuSetting, self).start(clock, delay)
        MenuElement.start_curve(self.setting_curves[self.setting_index], clock.time + delay + self.curve.last_vertex.z + distance(vec2(self.curve.last_vertex.x, self.curve.last_vertex.y), vec2(self.setting_curves[self.setting_index].first_vertex.x, self.setting_curves[self.setting_index].first_vertex.y)))

    def hurry(self, clock):
        super(MenuSetting, self).hurry(clock)
        MenuElement.hurry_curve(self.setting_curves[self.setting_index], clock.time + ((self.setting_curves[self.setting_index].start_time - clock.time) / menu_text_velocity_hurry_mulitplier))

    def stop(self):
        super(MenuSetting, self).stop()
        MenuElement.stop_curve(self.setting_curves[self.setting_index])

    def step(self, clock, window):
        super(MenuSetting, self).step(clock, window)
        for curve in self.setting_curves:
            curve.step(clock, window)

    def draw(self, window):
        super(MenuSetting, self).draw(window)
        for curve in self.setting_curves:
            curve.draw(window)

class MenuCheckBox(MenuSelection):
    def __init__(self, vector_display_curve, mark_vector_display_curve, selector_position, select_callback=None, select_callback_arguments=()):
        super(MenuCheckBox, self).__init__(vector_display_curve, selector_position, select_callback, select_callback_arguments)

        self.mark_curve = mark_vector_display_curve

        self.checked = False
        self.checked_curve_velocity = 1.0

    def duration(self):
        mark_duration = (distance(vec2(self.curve.last_vertex.x, self.curve.last_vertex.y), vec2(self.mark_curve.first_vertex.x, self.mark_curve.first_vertex.y)) / menu_text_velocity) + self.mark_curve.last_vertex.z
        return self.curve.last_vertex.z + (mark_duration if self.checked else 0.0)

    def select(self, clock):
        self.checked = not self.checked
        self.select_callback_arguments = (self.checked,) + self.select_callback_arguments

        super(MenuCheckBox, self).select(clock)
        if self.checked:
            MenuElement.start_curve(self.mark_curve, clock.time)
            self.mark_curve.velocity = self.checked_curve_velocity
        else:
            MenuElement.stop_curve(self.mark_curve)

        self.select_callback_arguments = self.select_callback_arguments[1:]

    def start(self, clock, delay):
        super(MenuCheckBox, self).start(clock, delay)
        if self.checked:
            MenuElement.start_curve(self.mark_curve, clock.time + delay + self.curve.last_vertex.z)
        else:
            MenuElement.stop_curve(self.mark_curve)

    def hurry(self, clock):
        super(MenuCheckBox, self).hurry(clock)
        MenuElement.hurry_curve(self.mark_curve, clock.time + ((self.mark_curve.start_time - clock.time) / menu_text_velocity_hurry_mulitplier))

    def stop(self):
        super(MenuCheckBox, self).stop()
        MenuElement.stop_curve(self.mark_curve)

    def step(self, clock, window):
        super(MenuCheckBox, self).step(clock, window)
        self.mark_curve.step(clock, window)

    def draw(self, window):
        super(MenuCheckBox, self).draw(window)
        self.mark_curve.draw(window)

class MenuSlider(MenuSelection):
    def __init__(self, vector_display_curve, slider_selector, slider_positions, selector_position, slider_callback=None, slider_callback_arguments=(), select_callback=None, select_callback_arguments=()):
        super(MenuSlider, self).__init__(vector_display_curve, selector_position, select_callback, select_callback_arguments)

        self.slider_selector = slider_selector
        self.slider_positions = slider_positions
        self.slider_position_index = 0
        self.slide_curve_velocity = 3.0

        self.slider_callback = slider_callback
        self.slider_callback_arguments = slider_callback_arguments

    def duration(self):
        return self.curve.last_vertex.z + (distance(vec2(self.curve.last_vertex.x, self.curve.last_vertex.y), vec2(self.slider_selector.curves[self.slider_selector.index].last_vertex.x, self.slider_selector.curves[self.slider_selector.index].last_vertex.y) + self.slider_selector.curves[self.slider_selector.index].translation) / menu_text_velocity) + self.slider_selector.curves[self.slider_selector.index].last_vertex.z

    def forward(self, clock, use_ready_delay):
        if not use_ready_delay or self.forward_back_ready(clock):
            self.forward_back_time = clock.time

            self.slider_position_index = min(len(self.slider_positions)-1, self.slider_position_index+1)
            self.slider_selector.switch_selection(clock, 0.0, self.slider_positions[self.slider_position_index])
            self.slider_selector.curves[self.slider_selector.index].velocity = self.slide_curve_velocity

            if self.slider_callback is not None:
                self.slider_callback(self.slider_position_index / (len(self.slider_positions)-1), *self.slider_callback_arguments)

    def back(self, clock, use_ready_delay):
        if not use_ready_delay or self.forward_back_ready(clock):
            self.forward_back_time = clock.time

            self.slider_position_index = max(0, self.slider_position_index-1)
            self.slider_selector.switch_selection(clock, 0.0, self.slider_positions[self.slider_position_index])
            self.slider_selector.curves[self.slider_selector.index].velocity = self.slide_curve_velocity

            if self.slider_callback is not None:
                self.slider_callback(self.slider_position_index / (len(self.slider_positions)-1), *self.slider_callback_arguments)

    def start(self, clock, delay):
        super(MenuSlider, self).start(clock, delay)
        self.forward_back_time = clock.time
        self.slider_selector.start(clock, delay + self.curve.last_vertex.z + (distance(vec2(self.curve.last_vertex.x, self.curve.last_vertex.y), vec2(self.slider_selector.curves[self.slider_selector.index].last_vertex.x, self.slider_selector.curves[self.slider_selector.index].last_vertex.y) + self.slider_positions[self.slider_position_index]) / menu_text_velocity), self.slider_positions[self.slider_position_index])

    def hurry(self, clock):
        super(MenuSlider, self).hurry(clock)
        self.slider_selector.hurry(clock, True)

    def stop(self):
        super(MenuSlider, self).stop()
        self.forward_back_time = None
        self.slider_selector.stop()

    def step(self, clock, window):
        super(MenuSlider, self).step(clock, window)
        self.slider_selector.step(clock, window)

    def draw(self, window):
        super(MenuSlider, self).draw(window)
        self.slider_selector.draw(window)
