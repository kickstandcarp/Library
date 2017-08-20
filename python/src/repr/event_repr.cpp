#include <sstream>

#include "repr/repr.hpp"
#include "repr/event_repr.hpp"



std::string	event_handler_repr(const EventHandler &event_handler)
{
	std::stringstream stream;
	stream << "EventHandler()";
	return stream.str();
}
