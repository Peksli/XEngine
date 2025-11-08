#include "src/Core/Events/WindowEvents.h"
#include "src/Core/LogSystem.h"

namespace XEngine {

#pragma region WindowCloseEvent
	std::string WindowCloseEvent::toString() 
	{
		return "Window close event";
	}

	EventCategory WindowCloseEvent::GetCategory() 
	{
		return EventCategory::Window; 
	}

	EventType WindowCloseEvent::GetType() 
	{
		return EventType::WindowClose;
	}
#pragma endregion

#pragma region WindowResizeEvent
	WindowResizeEvent::WindowResizeEvent(int width, int height)
		: m_Width(width), m_Height(height) 
	{

	}

	std::string WindowResizeEvent::toString() 
	{
		std::stringstream ss;
		ss << "Window resized to: [" << m_Width << ", " << m_Height << "]";
		return ss.str();
	}

	EventCategory WindowResizeEvent::GetCategory()
	{
		return EventCategory::None;
	}

	EventType WindowResizeEvent::GetType() 
	{
		return EventType::WindowResize;
	}
#pragma endregion

#pragma region WindowMovedEvent
	WindowMovedEvent::WindowMovedEvent(int x, int y)
		: m_X(x), m_Y(y) 
	{

	}

	std::string WindowMovedEvent::toString() 
	{
		std::stringstream ss;
		ss << "Window moved to: [" << m_X << ", " << m_Y << "]";
		return ss.str();
	}

	EventCategory WindowMovedEvent::GetCategory() 
	{
		return EventCategory::None;
	}

	EventType WindowMovedEvent::GetType() 
	{
		return EventType::WindowMoved;
	}
#pragma endregion 

}