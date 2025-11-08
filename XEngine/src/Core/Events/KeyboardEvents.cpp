#include "src/Core/Events/KeyboardEvents.h"
#include "src/Core/LogSystem.h"

namespace XEngine {

#pragma region KeyPressedEvent
	KeyPressedEvent::KeyPressedEvent(int keyCode, bool repeat)
		: m_KeyCode(keyCode), m_Repeat(repeat) 
	{

	}

	std::string KeyPressedEvent::toString() 
	{
		std::stringstream ss;
		ss << "Key pressed: " << m_KeyCode << (m_Repeat ? " (repeat)" : "");
		return ss.str();
	}

	EventCategory KeyPressedEvent::GetCategory() 
	{
		return EventCategory::Keyboard;
	}

	EventType KeyPressedEvent::GetType() 
	{
		return EventType::KeyPressed;
	}
#pragma endregion

#pragma region KeyReleasedEvent
	KeyReleasedEvent::KeyReleasedEvent(int keyCode)
		: m_KeyCode(keyCode)
	{

	}

	std::string KeyReleasedEvent::toString() 
	{
		std::stringstream ss;
		ss << "Key released: " << m_KeyCode;
		return ss.str();
	}

	EventCategory KeyReleasedEvent::GetCategory()
	{
		return EventCategory::Keyboard;
	}

	EventType KeyReleasedEvent::GetType() 
	{
		return EventType::KeyReleased;
	}
#pragma endregion

}