#ifndef EVENT_EVENT_HANDLER_HPP
#define EVENT_EVENT_HANDLER_HPP

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "event/event_handler_support.hpp"

enum class DeviceType : unsigned int { keyboard, mouse, controller };

class EventHandler
{
	public:
		EventHandler(const std::array<unsigned int, 2> &window_size);
		~EventHandler();

        unsigned int                                            get_num_controllers() const;

		bool                                                    get_button(const std::string &name, const DeviceType type, const unsigned int index=0) const;
		bool                                                    get_button_moved_down(const std::string &name, const DeviceType type, const unsigned int index=0) const;
		bool                                                    get_button_moved_up(const std::string &name, const DeviceType type, const unsigned int index=0) const;
		float                                                   get_value(const std::string &name, const DeviceType type, const unsigned int index=0) const;
		glm::vec2                                               get_direction(const std::string &name, const DeviceType type, const unsigned int index=0) const;

		void                                                    update();

		bool                                                    quit;

		char                                                    button_direction_delimiter;

        float                                                   stick_threshold;
		float													trigger_threshold;

	private:
		ButtonState                                             button_state(const std::string &name, const DeviceType type, const unsigned int index=0) const;
		glm::vec2                                               button_direction(const std::string &name, const DeviceType type, const unsigned int index=0) const;

        void                                                    add_controller(const int sdl_index);
        void                                                    remove_controller(const SDL_JoystickID id);

		unsigned int                                            update_count;

        std::array<unsigned int, 2>                             window_size;

		std::vector<ButtonState>                                key_states;

		std::map<int, ButtonState>                              mouse_button_states;
		unsigned int                                            mouse_motion_count, mouse_wheel_count;
		glm::ivec2                                              mouse_position, mouse_motion;
		int			                                            mouse_wheel_motion;

		std::vector<SDL_JoystickID>                             controller_ids;
		std::map<SDL_JoystickID, std::map<int, ButtonState> >   controller_button_states;
		std::map<SDL_JoystickID, std::map<int, ButtonState> >   controller_trigger_states;
		std::map<SDL_JoystickID, std::map<int, float> >         controller_axis_positions;
};

#endif
