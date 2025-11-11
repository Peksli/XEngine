#pragma once

#include <memory>
#include <string>

#include "src/Core/Events/Event.h"
#include "src/Core/Events/WindowEvents.h"
#include "src/Core/Events/KeyboardEvents.h"
#include "src/Core/Events/MouseEvents.h"
#include "src/Renderer/Context.h"


namespace XEngine {

	enum class Platform 
	{
		None,
		Windows,
		Linux,
		Mac
	};

	struct WindowSpecification 
	{
		Platform platform;
		int width;
		int height;
		bool wantVsync;
		std::string title;
		std::function<void(Event&)> eventCallback;
	};

	class Window 
	{
	public:
		Window(const WindowSpecification& spec);
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		int GetWidth()			const	{ return m_Spec.width;		}
		int GetHeight()			const	{ return m_Spec.height;		}
		bool IsVsync()			const	{ return m_Spec.wantVsync;	}
		std::string GetTitle()	const	{ return m_Spec.title;		}

		static std::unique_ptr<Window> CreateWindow(const WindowSpecification& spec);

	protected:
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;

	protected:
		WindowSpecification m_Spec;
		std::unique_ptr<Context> m_Context;
	};

}