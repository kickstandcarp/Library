#include <stdexcept>
#include <utility>
#include <list>

#include <SDL2/SDL_keyboard.h>
#include <SDL2/SDL_keycode.h>
#include <SDL2/SDL_scancode.h>
#include <SDL2/SDL_mouse.h>
#include <SDL2/SDL_joystick.h>
#include <SDL2/SDL_gamecontroller.h>

#include "event/event_handler.hpp"


#include <iostream>


EventHandler::EventHandler(const std::array<unsigned int, 2> &window_size)
:	quit(false),
	button_direction_delimiter(','),
	stick_threshold(0.25f),
	trigger_threshold(0.5f),
	path_duration(0.0f),
	update_count(1),
    window_size(window_size),
    mouse_button_states({{SDL_BUTTON_LEFT, ButtonState{false, 0, 0}}, {SDL_BUTTON_MIDDLE, ButtonState{false, 0, 0}}, {SDL_BUTTON_RIGHT, ButtonState{false, 0, 0}}, {SDL_BUTTON_X1, ButtonState{false, 0, 0}} ,{SDL_BUTTON_X2, ButtonState{false, 0, 0}}}),
	mouse_button_paths({{SDL_BUTTON_LEFT, std::shared_ptr<SegmentedPath<bool> >()}, {SDL_BUTTON_MIDDLE, std::shared_ptr<SegmentedPath<bool> >()}, {SDL_BUTTON_RIGHT, std::shared_ptr<SegmentedPath<bool> >()}, {SDL_BUTTON_X1, std::shared_ptr<SegmentedPath<bool> >()} ,{SDL_BUTTON_X2, std::shared_ptr<SegmentedPath<bool> >()}}),
	mouse_motion_count(0),
	mouse_wheel_count(0),
	mouse_motion(0),
	mouse_position_path(std::shared_ptr<SegmentedPath<glm::vec2> >()),
	mouse_wheel_motion(0)
{
	if (SDL_WasInit(SDL_INIT_EVENTS) != SDL_TRUE)
        SDL_Init(SDL_INIT_EVENTS);

	if (SDL_WasInit(SDL_INIT_GAMECONTROLLER) != SDL_TRUE)
    {
        SDL_Init(SDL_INIT_GAMECONTROLLER);
        SDL_GameControllerAddMapping(xbox360_controller_mapping.c_str());
    }

	int num_keys;
	SDL_GetKeyboardState(&num_keys);
	
	this->key_states = std::vector<ButtonState>(num_keys);
	for (auto &key_state : this->key_states)
		key_state = ButtonState{false, 0, 0};

	int mouse_position_x, mouse_position_y;
	SDL_GetMouseState(&mouse_position_x, &mouse_position_y);
	this->mouse_position = glm::vec2(static_cast<float>(mouse_position_x) / static_cast<float>(this->window_size[0]), static_cast<float>(window_size[1] - mouse_position_y) / static_cast<float>(this->window_size[1]));

    int num_controllers = SDL_NumJoysticks();
    for (int index = 0; index < num_controllers; index++)
        this->add_controller(index);
}

EventHandler::~EventHandler()
{
    for (auto const &button_states : this->controller_button_states)
        SDL_GameControllerClose(SDL_GameControllerFromInstanceID(button_states.first));
}

unsigned int EventHandler::get_num_controllers() const
{
    return this->controller_ids.size();
}

bool EventHandler::get_button(const std::string &name, const DeviceType type, const unsigned int index) const
{
	return this->button_state(name, type, index).is_down;
}

bool EventHandler::get_button_moved_down(const std::string &name, const DeviceType type, const unsigned int index) const
{
	return this->button_state(name, type, index).moved_down_count == this->update_count;
}

bool EventHandler::get_button_moved_up(const std::string &name, const DeviceType type, const unsigned int index) const
{
	return this->button_state(name, type, index).moved_up_count == this->update_count;
}

float EventHandler::get_value(const std::string &name, const DeviceType type, const unsigned int index) const
{
	switch (type)
	{
		case DeviceType::keyboard:
			throw std::runtime_error("unknown keyboard value name");
			break;
		case DeviceType::mouse:
			if (name == "wheel")
				return this->mouse_wheel_count == this->update_count ? static_cast<float>(this->mouse_wheel_motion) : 0.0f;
			else
				throw std::runtime_error("unknown mouse value name");
			break;
		case DeviceType::controller:
			if (index >= this->controller_ids.size())
				return 0.0f;
			else
			{
				if (name == "left_trigger")
					return this->controller_axis_positions.at(this->controller_ids[index]).at(SDL_CONTROLLER_AXIS_TRIGGERLEFT);
				else if (name == "right_trigger")
					return this->controller_axis_positions.at(this->controller_ids[index]).at(SDL_CONTROLLER_AXIS_TRIGGERRIGHT);
				else
					throw std::runtime_error("unknown controller direction name");
			}
			break;
		default:
			throw std::runtime_error("unknown device type");
	}
}

glm::vec2 EventHandler::get_direction(const std::string &name, const DeviceType type, const unsigned int index) const
{
	switch (type)
	{
		case DeviceType::keyboard:
			return this->button_direction(name, type, index);
			break;
		case DeviceType::mouse:
			if (name == "relative")
				return this->mouse_motion_count == this->update_count ? static_cast<glm::vec2>(this->mouse_motion) : glm::vec2(0.0f);
			else if (name == "absolute")
				return 2.0f*(static_cast<glm::vec2>(this->mouse_position) / glm::vec2(static_cast<float>(this->window_size[0]), static_cast<float>(this->window_size[1]))) - glm::vec2(1.0f);
			else
				throw std::runtime_error("unknown mouse direction name");
			break;
		case DeviceType::controller:
            if (index >= this->controller_ids.size())
                return glm::vec2(0.0f);
            else
            {
                if (name == "left_stick")
                    return glm::vec2(this->controller_axis_positions.at(this->controller_ids[index]).at(SDL_CONTROLLER_AXIS_LEFTX), -this->controller_axis_positions.at(this->controller_ids[index]).at(SDL_CONTROLLER_AXIS_LEFTY));
                else if (name == "right_stick")
                    return glm::vec2(this->controller_axis_positions.at(this->controller_ids[index]).at(SDL_CONTROLLER_AXIS_RIGHTX), -this->controller_axis_positions.at(this->controller_ids[index]).at(SDL_CONTROLLER_AXIS_RIGHTY));
                else if (name == "pad")
                    return this->button_direction("up" + std::string(&this->button_direction_delimiter) + "left" + std::string(&this->button_direction_delimiter) + "down" + std::string(&this->button_direction_delimiter) + "right", type, index);
                else
                    throw std::runtime_error("unknown controller direction name");
            }
			break;
		default:
			throw std::runtime_error("unknown device type");
	}
}

const std::shared_ptr<SegmentedPath<bool> > EventHandler::get_button_path(const std::string &name, const DeviceType type, const unsigned int index) const
{
	return const_cast<EventHandler*>(this)->button_path(name, type, index);
}

const std::shared_ptr<SegmentedPath<float> > EventHandler::get_value_path(const std::string &name, const DeviceType type, const unsigned int index) const
{
	return const_cast<EventHandler*>(this)->value_path(name, type, index);
}

const std::shared_ptr<SegmentedPath<glm::vec2> > EventHandler::get_direction_path(const std::string &name, const DeviceType type, const unsigned int index) const
{
	return const_cast<EventHandler*>(this)->direction_path(name, type, index);
}

void EventHandler::add_button_path(const std::string &name, const DeviceType type, const unsigned int index, const float time)
{
	this->button_path(name, type, index) = std::make_shared<SegmentedPath<bool> >(std::list<PathVertex<bool> >(1, PathVertex<bool>(this->get_button(name, type, index), time)));
}

void EventHandler::add_value_path(const std::string &name, const DeviceType type, const unsigned int index, const float time)
{
	this->value_path(name, type, index) = std::make_shared<SegmentedPath<float> >(std::list<PathVertex<float> >(1, PathVertex<float>(this->get_value(name, type, index), time)));
}

void EventHandler::add_direction_path(const std::string &name, const DeviceType type, const unsigned int index, const float time)
{
	this->direction_path(name, type, index) = std::make_shared<SegmentedPath<glm::vec2> >(std::list<PathVertex<glm::vec2> >(1, PathVertex<glm::vec2>(this->get_direction(name, type, index), time)));
}

void EventHandler::remove_button_path(const std::string &name, const DeviceType type, const unsigned int index)
{
	this->button_path(name, type, index).reset();
}

void EventHandler::remove_value_path(const std::string &name, const DeviceType type, const unsigned int index)
{
	this->value_path(name, type, index).reset();
}

void EventHandler::remove_direction_path(const std::string &name, const DeviceType type, const unsigned int index)
{
	this->direction_path(name, type, index).reset();
}

void EventHandler::update(const float time)
{
	this->update_count++;

	SDL_Event event;
	while (SDL_PollEvent(&event) != 0)
	{
		switch (event.type)
		{
			case SDL_QUIT:
				this->quit = true;
				break;
			case SDL_KEYDOWN:
				this->key_states[event.key.keysym.scancode].is_down = true;
				if (event.key.repeat == 0)
					this->key_states[event.key.keysym.scancode].moved_down_count = this->update_count;
				break;
			case SDL_KEYUP:
				this->key_states[event.key.keysym.scancode].is_down = false;
				if (event.key.repeat == 0)
					this->key_states[event.key.keysym.scancode].moved_up_count = this->update_count;
				break;
			case SDL_MOUSEBUTTONDOWN:
				this->mouse_button_states.at(event.button.button).is_down = true;
				this->mouse_button_states.at(event.button.button).moved_down_count = this->update_count;
				break;
			case SDL_MOUSEBUTTONUP:
				this->mouse_button_states.at(event.button.button).is_down = false;
				this->mouse_button_states.at(event.button.button).moved_up_count = this->update_count;
				break;
			case SDL_MOUSEMOTION:
				this->mouse_motion_count = this->update_count;
				this->mouse_position.x = static_cast<float>(event.motion.x) / static_cast<float>(this->window_size[0]);
				this->mouse_position.y = static_cast<float>(this->window_size[1] - event.motion.y) / static_cast<float>(this->window_size[1]);
				this->mouse_motion.x = static_cast<float>(event.motion.xrel) / static_cast<float>(this->window_size[0]);
				this->mouse_motion.y = static_cast<float>(-event.motion.yrel) / static_cast<float>(this->window_size[1]);
				break;
			case SDL_MOUSEWHEEL:
				this->mouse_wheel_count = this->update_count;
				this->mouse_wheel_motion = static_cast<float>(event.wheel.y);
				break;
            case SDL_CONTROLLERAXISMOTION:
                if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT || event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
				{
					float prev_value = this->controller_axis_positions.at(event.caxis.which).at(event.caxis.axis), next_value = static_cast<float>(event.caxis.value) / static_cast<float>(MAX_TRIGGER_VALUE);
					this->controller_axis_positions.at(event.caxis.which).at(event.caxis.axis) = next_value;
					this->controller_trigger_states.at(event.caxis.which).at(event.caxis.axis).is_down = next_value >= this->trigger_threshold;
					if (prev_value < this->trigger_threshold && next_value >= this->trigger_threshold)
						this->controller_trigger_states.at(event.caxis.which).at(event.caxis.axis).moved_down_count = this->update_count;
					if (prev_value >= this->trigger_threshold && next_value < this->trigger_threshold)
						this->controller_trigger_states.at(event.caxis.which).at(event.caxis.axis).moved_up_count = this->update_count;
				}
                else
                {
                    float value = event.caxis.value >= 0 ? static_cast<float>(event.caxis.value) / static_cast<float>(MAX_AXIS_VALUE) : static_cast<float>(event.caxis.value) / static_cast<float>(MIN_AXIS_VALUE);
                    this->controller_axis_positions.at(event.caxis.which).at(event.caxis.axis) = std::max(0.0f, (value - this->stick_threshold) / (1.0f - this->stick_threshold));
                }
                break;
            case SDL_CONTROLLERBUTTONDOWN:
				this->controller_button_states.at(event.cbutton.which).at(event.cbutton.button).is_down = true;
				this->controller_button_states.at(event.cbutton.which).at(event.cbutton.button).moved_down_count = this->update_count;
                break;
            case SDL_CONTROLLERBUTTONUP:
				this->controller_button_states.at(event.cbutton.which).at(event.cbutton.button).is_down = false;
				this->controller_button_states.at(event.cbutton.which).at(event.cbutton.button).moved_up_count = this->update_count;
                break;
            case SDL_CONTROLLERDEVICEADDED:
                this->add_controller(event.cdevice.which);
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                this->remove_controller(event.cdevice.which);
                break;
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED)
                    this->window_size = {{static_cast<unsigned int>(event.window.data1), static_cast<unsigned int>(event.window.data2)}};
                break;
		}
	}

    for (auto &path : this->mouse_button_paths)
    {
        if (path.second)
        {
            path.second->remove_path_vertices_prior(time - this->path_duration);
            path.second->add_path_vertex(PathVertex<bool>(this->mouse_button_states.at(path.first).is_down, time));
        }
    }

    if (this->mouse_position_path)
    {
        this->mouse_position_path->remove_path_vertices_prior(time - this->path_duration);
        this->mouse_position_path->add_path_vertex(PathVertex<glm::vec2>(this->mouse_position, time));
    }
}

ButtonState EventHandler::button_state(const std::string &name, const DeviceType type, const unsigned int index) const
{
	switch (type)
	{
		case DeviceType::keyboard:
			return this->key_states[SDL_GetScancodeFromName(name.c_str())];
			break;
		case DeviceType::mouse:
			return this->mouse_button_states.at(mouse_button_name_to_enum.at(name));
			break;
		case DeviceType::controller:
            if (index >= this->controller_ids.size())
                return ButtonState({false, 0, 0});
            else
			{
				if (name == "left_trigger" || name == "right_trigger")
					return this->controller_trigger_states.at(this->controller_ids[index]).at(controller_button_name_to_enum.at(name));
				else
					return this->controller_button_states.at(this->controller_ids[index]).at(controller_button_name_to_enum.at(name));
			}
			break;
		default:
			throw std::runtime_error("unknown device type");
	}
}

glm::vec2 EventHandler::button_direction(const std::string &name, const DeviceType type, const unsigned int index) const
{
	glm::vec2 state(0.0f);
    unsigned int button_index = 0;
	std::size_t begin = 0, end = 0;
	while (end != std::string::npos && button_index < 4)
	{
		end = name.find_first_of(this->button_direction_delimiter, begin);

		std::string button_name = name.substr(begin, end - begin);
		if (this->get_button(button_name, type, index))
			state[1 - (button_index % 2)] += ((button_index+1) / 2) % 2 == 0 ? 1.0f : -1.0f;
		button_index++;

		begin = end+1;
	}

	return state;
}

std::shared_ptr<SegmentedPath<bool> >& EventHandler::button_path(const std::string &name, const DeviceType type, const unsigned int index)
{
	switch (type)
	{
		case DeviceType::keyboard:
			throw std::runtime_error("keyboard button paths are unsupported");
			break;
		case DeviceType::mouse:
			return this->mouse_button_paths.at(mouse_button_name_to_enum.at(name));
			break;
		case DeviceType::controller:
			throw std::runtime_error("controller button paths are unsupported");
			break;
		default:
			throw std::runtime_error("unknown device type");
	}
}

std::shared_ptr<SegmentedPath<float> >& EventHandler::value_path(const std::string &name, const DeviceType type, const unsigned int index)
{
	throw std::runtime_error("value paths are unsupported");
}

std::shared_ptr<SegmentedPath<glm::vec2> >& EventHandler::direction_path(const std::string &name, const DeviceType type, const unsigned int index)
{
	switch (type)
	{
		case DeviceType::keyboard:
			throw std::runtime_error("keyboard direction paths are unsupported");
			break;
		case DeviceType::mouse:
			if (name == "relative")
				throw std::runtime_error("relative mouse direction paths are unsupported");
			else if (name == "absolute")
				return this->mouse_position_path;
			else
				throw std::runtime_error("unknown mouse direction name");
			break;
		case DeviceType::controller:
			throw std::runtime_error("controller direction paths are unsupported");
			break;
		default:
			throw std::runtime_error("unknown device type");
	}
}

void EventHandler::add_controller(const int sdl_index)
{
    SDL_GameController *controller = SDL_GameControllerOpen(sdl_index);
    SDL_JoystickID id = SDL_JoystickInstanceID(SDL_GameControllerGetJoystick(controller));

    if (std::find(this->controller_ids.begin(), this->controller_ids.end(), id) == this->controller_ids.end())
    {
        this->controller_ids.push_back(id);
        this->controller_button_states.insert({id, {{SDL_CONTROLLER_BUTTON_A, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_B, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_X, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_Y, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_BACK, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_GUIDE, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_START, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_LEFTSTICK, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_RIGHTSTICK, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_LEFTSHOULDER, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_RIGHTSHOULDER, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_DPAD_UP, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_DPAD_DOWN, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_DPAD_LEFT, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_BUTTON_DPAD_RIGHT, ButtonState{false, 0, 0}}}});
		this->controller_trigger_states.insert({id, {{SDL_CONTROLLER_AXIS_TRIGGERLEFT, ButtonState{false, 0, 0}}, {SDL_CONTROLLER_AXIS_TRIGGERRIGHT, ButtonState{false, 0, 0}}}});
		this->controller_axis_positions.insert({id, {{SDL_CONTROLLER_AXIS_LEFTX, 0.0f}, {SDL_CONTROLLER_AXIS_LEFTY, 0.0f}, {SDL_CONTROLLER_AXIS_RIGHTX, 0.0f}, {SDL_CONTROLLER_AXIS_RIGHTY, 0.0f}, {SDL_CONTROLLER_AXIS_TRIGGERLEFT, 0.0f}, {SDL_CONTROLLER_AXIS_TRIGGERRIGHT, 0.0f}}});
    }
}

void EventHandler::remove_controller(const SDL_JoystickID id)
{
    SDL_GameController* controller = SDL_GameControllerFromInstanceID(id);
    SDL_GameControllerClose(controller);

    if (std::find(this->controller_ids.begin(), this->controller_ids.end(), id) != this->controller_ids.end())
    {
        this->controller_ids.erase(std::find(this->controller_ids.begin(), this->controller_ids.end(), id));
        this->controller_button_states.erase(id);
		this->controller_trigger_states.erase(id);
        this->controller_axis_positions.erase(id);
    }
}
