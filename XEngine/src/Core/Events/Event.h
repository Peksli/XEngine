#pragma once

#include <string>
#include <functional>

#define LEFT_SHIFT_BY(x) (1 << (x))

#define SET_STATIC_PART(event) static EventType GetStaticType() { return EventType::event; }


namespace XEngine {

	enum class EventCategory 
	{
		None			= 0,
		Keyboard		= LEFT_SHIFT_BY(0),
		Mouse			= LEFT_SHIFT_BY(1),
		Window			= LEFT_SHIFT_BY(2)
	};

	enum class EventType 
	{
		None = 0,
		WindowClose, WindowResize, WindowMoved,
		KeyPressed, KeyReleased, 
		MouseButtonPressed, MouseButtonReleased, MouseCursorMoved, MouseScrolled
	};

	class Event 
	{
	public:
		Event() = default;
		virtual ~Event() = default;

		virtual void OnEvent()				= 0;
		virtual std::string toString()		= 0;
		virtual EventCategory GetCategory() = 0;
		virtual EventType GetType()			= 0;
	};

	class EventDispatcher 
	{
	public:
		EventDispatcher(Event& event);
		virtual ~EventDispatcher() = default;

		template<class EventT>
		void Dispatch(std::function<void(EventT&)> callback)
		{
			if (m_Event.GetType() == EventT::GetStaticType())
			{
				callback(static_cast<EventT&>(m_Event));
			}
		}

	private:
		Event& m_Event;
	};

}