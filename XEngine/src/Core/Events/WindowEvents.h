#pragma once

#include "src/Core/Events/Event.h"

namespace XEngine {

	class WindowCloseEvent : public Event {
	public:
		WindowCloseEvent() = default;
		virtual ~WindowCloseEvent() = default;

		SET_STATIC_PART(WindowClose);
		std::string toString()      override;
		EventCategory GetCategory() override;
		EventType GetType()         override;
	};

	class WindowResizeEvent : public Event {
	public:
		WindowResizeEvent(int width, int height);
		virtual ~WindowResizeEvent() = default;

		SET_STATIC_PART(WindowResize);
		std::string toString()      override;
		EventCategory GetCategory() override;
		EventType GetType()         override;

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }

	private:
		int m_Width, m_Height;
	};

	class WindowMovedEvent : public Event {
	public:
		WindowMovedEvent(int x, int y);
		virtual ~WindowMovedEvent() = default;

		SET_STATIC_PART(WindowMoved);
		std::string toString()      override;
		EventCategory GetCategory() override;
		EventType GetType()         override;

		int GetX() const { return m_X; }
		int GetY() const { return m_Y; }

	private:
		int m_X, m_Y;
	};

}