#pragma once

#include "src/Core/Events/Event.h"

namespace XEngine {

	class KeyPressedEvent : public Event {
	public:
		KeyPressedEvent(int keyCode, bool repeat);
		virtual ~KeyPressedEvent() = default;

		SET_STATIC_PART(KeyPressed);
		void OnEvent()              override;
		std::string toString()      override;
		EventCategory GetCategory() override;
		EventType GetType()         override;

		int GetKeyCode() const { return m_KeyCode; }
		bool IsRepeat() const { return m_Repeat; }

	private:
		int m_KeyCode;
		bool m_Repeat;
	};

	class KeyReleasedEvent : public Event {
	public:
		KeyReleasedEvent(int keyCode);
		virtual ~KeyReleasedEvent() = default;

		SET_STATIC_PART(KeyReleased);
		void OnEvent()              override;
		std::string toString()      override;
		EventCategory GetCategory() override;
		EventType GetType()         override;

		int GetKeyCode() const { return m_KeyCode; }

	private:
		int m_KeyCode;
	};

}