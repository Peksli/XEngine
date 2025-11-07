#pragma once

#include "src/Core/Events/Event.h"


namespace XEngine {

	class MouseCursorMovedEvent : public Event {
	public:
		MouseCursorMovedEvent(int new_x, int new_y);
		virtual ~MouseCursorMovedEvent() = default;

		SET_STATIC_PART(MouseCursorMoved);
		void OnEvent()				override;
		std::string toString()		override;
		EventCategory GetCategory() override;
		EventType GetType()			override;

		int GetX() const { return m_X; }
		int GetY() const { return m_Y; }

	private:
		int m_X;
		int m_Y;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(float new_xoffset, float new_yoffset);
		virtual ~MouseScrolledEvent() = default;

		SET_STATIC_PART(MouseScrolled);
		void OnEvent()				override;
		std::string toString()		override;
		EventCategory GetCategory() override;
		EventType GetType()			override;

		float GetXoffset() const { return m_Xoffset; }
		float GetYoffset() const { return m_Yoffset; }

	private:
		float m_Xoffset;
		float m_Yoffset;
	};

	class MouseButtonPressedEvent : public Event {
	public:
		MouseButtonPressedEvent(int button);
		virtual ~MouseButtonPressedEvent() = default;

		SET_STATIC_PART(MouseButtonPressed);
		void OnEvent()              override;
		std::string toString()      override;
		EventCategory GetCategory() override;
		EventType GetType()         override;

		int GetButton() const { return m_Button; }

	private:
		int m_Button;
	};

	class MouseButtonReleasedEvent : public Event {
	public:
		MouseButtonReleasedEvent(int button);
		virtual ~MouseButtonReleasedEvent() = default;

		SET_STATIC_PART(MouseButtonReleased);
		void OnEvent()              override;
		std::string toString()      override;
		EventCategory GetCategory() override;
		EventType GetType()         override;

		int GetButton() const { return m_Button; }

	private:
		int m_Button;
	};

}