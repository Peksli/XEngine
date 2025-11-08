#pragma once

#include "src/Core/Events/Event.h"


namespace XEngine {

	class MouseCursorMovedEvent : public Event {
	public:
		MouseCursorMovedEvent(double new_x, double new_y);
		virtual ~MouseCursorMovedEvent() = default;

		SET_STATIC_PART(MouseCursorMoved);
		std::string toString()		override;
		EventCategory GetCategory() override;
		EventType GetType()			override;

		double GetX() const { return m_X; }
		double GetY() const { return m_Y; }

	private:
		double m_X;
		double m_Y;
	};

	class MouseScrolledEvent : public Event {
	public:
		MouseScrolledEvent(double new_xoffset, double new_yoffset);
		virtual ~MouseScrolledEvent() = default;

		SET_STATIC_PART(MouseScrolled);
		std::string toString()		override;
		EventCategory GetCategory() override;
		EventType GetType()			override;

		double GetXoffset() const { return m_Xoffset; }
		double GetYoffset() const { return m_Yoffset; }

	private:
		double m_Xoffset;
		double m_Yoffset;
	};

	class MouseButtonPressedEvent : public Event {
	public:
		MouseButtonPressedEvent(int button);
		virtual ~MouseButtonPressedEvent() = default;

		SET_STATIC_PART(MouseButtonPressed);
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
		std::string toString()      override;
		EventCategory GetCategory() override;
		EventType GetType()         override;

		int GetButton() const { return m_Button; }

	private:
		int m_Button;
	};

}