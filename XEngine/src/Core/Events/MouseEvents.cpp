#include "src/Core/Events/MouseEvents.h"
#include "src/Core/LogSystem.h"


namespace XEngine {

#pragma region MouseCursorMovedEvent

	MouseCursorMovedEvent::MouseCursorMovedEvent(int new_x, int new_y)
		:m_X(new_x), m_Y(new_y) {

	}

	void MouseCursorMovedEvent::OnEvent() {

	}

	std::string MouseCursorMovedEvent::toString() {
		std::stringstream sstream;
		sstream << "Mouse cursor moved to: [" << m_X << ", " << m_Y << "]\n";
		return sstream.str();
	}

	EventCategory MouseCursorMovedEvent::GetCategory() {
		return EventCategory::Mouse;
	}

	EventType MouseCursorMovedEvent::GetType() {
		return EventType::MouseCursorMoved;
	}
#pragma endregion

#pragma region MouseScrolledEvent
	MouseScrolledEvent::MouseScrolledEvent(float new_xoffset, float new_yoffset)
		: m_Xoffset(new_xoffset), m_Yoffset(new_yoffset) {

	}

	void MouseScrolledEvent::OnEvent() {

	}

	std::string MouseScrolledEvent::toString() {
		std::stringstream sstream;
		sstream << "Mouse scrolled to: [" << m_Xoffset << ", " << m_Yoffset << "]\n";
		return sstream.str();
	}

	EventCategory MouseScrolledEvent::GetCategory() {
		return EventCategory::Mouse;
	}

	EventType MouseScrolledEvent::GetType() {
		return EventType::MouseScrolled;
	}
#pragma endregion

#pragma region MouseButtonPressedEvent
	MouseButtonPressedEvent::MouseButtonPressedEvent(int button)
		: m_Button(button) {
	}

	void MouseButtonPressedEvent::OnEvent() {

	}

	std::string MouseButtonPressedEvent::toString() {
		std::stringstream sstream;
		sstream << "Mouse button pressed: [" << m_Button << "]\n";
		return sstream.str();
	}

	EventCategory MouseButtonPressedEvent::GetCategory() {
		return EventCategory::Mouse;
	}

	EventType MouseButtonPressedEvent::GetType() {
		return EventType::MouseButtonPressed;
	}
#pragma endregion

#pragma region MouseButtonReleasedEvent

	MouseButtonReleasedEvent::MouseButtonReleasedEvent(int button)
		: m_Button(button) {
	}

	void MouseButtonReleasedEvent::OnEvent() {

	}

	std::string MouseButtonReleasedEvent::toString() {
		std::stringstream sstream;
		sstream << "Mouse button released: [" << m_Button << "]\n";
		return sstream.str();
	}

	EventCategory MouseButtonReleasedEvent::GetCategory() {
		return EventCategory::Mouse;
	}

	EventType MouseButtonReleasedEvent::GetType() {
		return EventType::MouseButtonReleased;
	}

#pragma endregion

}