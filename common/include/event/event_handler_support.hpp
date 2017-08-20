#ifndef EVENT_EVENT_HANDLER_SUPPORT_HPP
#define EVENT_EVENT_HANDLER_SUPPORT_HPP

#include <string>
#include <map>
#include <SDL2/SDL.h>

struct ButtonState
{
	bool            is_down;
	unsigned int    moved_down_count;
	unsigned int	moved_up_count;
};

static const int                            MIN_AXIS_VALUE = -32768;
static const int                            MAX_AXIS_VALUE = 32767;
static const int                            MIN_TRIGGER_VALUE = 0;
static const int                            MAX_TRIGGER_VALUE = 32767;

static const std::map<std::string, int>     mouse_button_name_to_enum({{"left", SDL_BUTTON_LEFT}, {"middle", SDL_BUTTON_MIDDLE}, {"right", SDL_BUTTON_RIGHT}, {"x1", SDL_BUTTON_X1}, {"x2", SDL_BUTTON_X2}});
static const std::map<std::string, int>     controller_button_name_to_enum({{"a", SDL_CONTROLLER_BUTTON_A}, {"b", SDL_CONTROLLER_BUTTON_B}, {"x", SDL_CONTROLLER_BUTTON_X}, {"y", SDL_CONTROLLER_BUTTON_Y}, {"back", SDL_CONTROLLER_BUTTON_BACK}, {"guide", SDL_CONTROLLER_BUTTON_GUIDE}, {"start", SDL_CONTROLLER_BUTTON_START}, {"left_stick", SDL_CONTROLLER_BUTTON_LEFTSTICK}, {"right_stick", SDL_CONTROLLER_BUTTON_RIGHTSTICK}, {"left_shoulder", SDL_CONTROLLER_BUTTON_LEFTSHOULDER}, {"right_shoulder", SDL_CONTROLLER_BUTTON_RIGHTSHOULDER}, {"up", SDL_CONTROLLER_BUTTON_DPAD_UP}, {"down", SDL_CONTROLLER_BUTTON_DPAD_DOWN}, {"left", SDL_CONTROLLER_BUTTON_DPAD_LEFT}, {"right", SDL_CONTROLLER_BUTTON_DPAD_RIGHT}, {"left_trigger", SDL_CONTROLLER_AXIS_TRIGGERLEFT}, {"right_trigger", SDL_CONTROLLER_AXIS_TRIGGERRIGHT}});

static const std::string                    xbox360_controller_mapping{"5e040000000000008e02000000000000,X360 Controller,a:b0,b:b1,back:b9,dpdown:b12,dpleft:b13,dpright:b14,dpup:b11,guide:b10,leftshoulder:b4,leftstick:b6,lefttrigger:a2,leftx:a0,lefty:a1,rightshoulder:b5,rightstick:b7,righttrigger:a5,rightx:a3,righty:a4,start:b8,x:b2,y:b3,platform:Mac OS X"};

#endif
