#ifndef EVENT_EVENT_HANDLER_HPP
#define EVENT_EVENT_HANDLER_HPP

#include <memory>
#include <string>
#include <map>
#include <vector>
#include <array>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "clock.hpp"
#include "event/event_handler_support.hpp"
#include "parametric/curve_support.hpp"
#include "parametric/segment_curve.hpp"

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

		const std::shared_ptr<const SegmentCurve<bool> >        get_button_history(const std::string &name, const DeviceType type, const unsigned int index=0) const;
		const std::shared_ptr<const SegmentCurve<float> >       get_value_history(const std::string &name, const DeviceType type, const unsigned int index=0) const;
		const std::shared_ptr<const SegmentCurve<glm::vec2> >   get_direction_history(const std::string &name, const DeviceType type, const unsigned int index=0) const;

		void													add_button_history(const std::string &name, const DeviceType type, const unsigned int index=0, const float time=0.0f);
		void													add_value_history(const std::string &name, const DeviceType type, const unsigned int index=0, const float time=0.0f);
		void													add_direction_history(const std::string &name, const DeviceType type, const unsigned int index=0, const float time=0.0f);

		void													remove_button_history(const std::string &name, const DeviceType type, const unsigned int index=0);
		void													remove_value_history(const std::string &name, const DeviceType type, const unsigned int index=0);
		void													remove_direction_history(const std::string &name, const DeviceType type, const unsigned int index=0);

		void                                                    step(const Clock &clock);

		bool                                                    quit;

		char                                                    button_direction_delimiter;

        float                                                   stick_threshold;
		float													trigger_threshold;

		float													history_duration;

	private:
        glm::vec2                                               pixel_to_gl_position(const glm::ivec2 &pixel_position) const;

		ButtonState                                             button_state(const std::string &name, const DeviceType type, const unsigned int index=0) const;
		glm::vec2                                               button_direction(const std::string &name, const DeviceType type, const unsigned int index=0) const;
		
		std::shared_ptr<SegmentCurve<bool> >&					button_history(const std::string &name, const DeviceType type, const unsigned int index=0);
		std::shared_ptr<SegmentCurve<float> >&					value_history(const std::string &name, const DeviceType type, const unsigned int index=0);
		std::shared_ptr<SegmentCurve<glm::vec2> >&              direction_history(const std::string &name, const DeviceType type, const unsigned int index=0);

        void                                                    add_controller(const int sdl_index);
        void                                                    remove_controller(const SDL_JoystickID id);

		unsigned int                                            step_count;

        std::array<unsigned int, 2>                             window_size;

		std::vector<ButtonState>                                key_states;

		std::map<int, ButtonState>                              mouse_button_states;
		std::map<int, std::shared_ptr<SegmentCurve<bool> > >	mouse_button_histories;
		unsigned int                                            mouse_motion_count, mouse_wheel_count;
		glm::vec2                                               mouse_position, mouse_motion;
		std::shared_ptr<SegmentCurve<glm::vec2> >				mouse_position_history;
		float		                                            mouse_wheel_motion;

		std::vector<SDL_JoystickID>                             controller_ids;
		std::map<SDL_JoystickID, std::map<int, ButtonState> >   controller_button_states;
		std::map<SDL_JoystickID, std::map<int, ButtonState> >   controller_trigger_states;
		std::map<SDL_JoystickID, std::map<int, float> >         controller_axis_positions;
};

#endif
