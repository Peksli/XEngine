#include "src/Core/Events/Event.h"
#include "src/Core/LogSystem.h"


namespace XEngine {

	EventDispatcher::EventDispatcher(Event& event)
		: m_Event(event)
	{

	}

	template<class EventT>
	void XEngine::EventDispatcher::Dispatch(std::function<bool(EventT&)> callback)
	{
		if (m_Event.GetType() == EventT::GetStaticType())
		{
			if (!callback(m_Event))
			{
				XEngine_CRITICAL(fmt::runtime("Didn't dispatch event: {0}", m_Event.toString()));
			}
		}
	}

}