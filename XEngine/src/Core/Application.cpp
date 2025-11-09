#include "src/Core/Application.h"
#include "src/Core/Events/Event.h"
#include "src/Core/Events/WindowEvents.h"
#include "src/Core/LogSystem.h"
#include "src/Debug/Instrumentor.h"


namespace XEngine {

	Application::Application(ApplicationSpecification& spec)
	{
		XE_PROFILE_BEGIN_SESSION("XEngine session", "profile_results.json");
		XE_PROFILE_FUNCTION();

		WindowSpecification win_spec;
		win_spec.width			= spec.window_width;
		win_spec.height			= spec.window_heigt;
		win_spec.wantVsync		= spec.enableVsync;
		win_spec.title			= spec.window_name;
		win_spec.platform		= spec.platform;
		win_spec.eventCallback	= [this](Event& event) -> void
			{
				this->OnEvent(event);
			};

		m_Window = Window::CreateWindow(win_spec);
	}

	Application::~Application()
	{
		XE_PROFILE_END_SESSION();
	}

	void Application::Run()
	{
		XE_PROFILE_FUNCTION();

		while (m_Running)
		{
			m_Window->OnUpdate();
			for (auto layer : m_LayerStack)
			{
				layer->OnUpdate();
			}
		}
	}

	void Application::OnEvent(Event& event)
	{
		XE_PROFILE_FUNCTION();

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& event) -> void
			{
				this->m_Running = false;
			});

		for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); it++)
		{
			(*it)->OnEvent(event);
			if (event.IsHandled())
			{
				break;
			}
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		layer->OnAttach();
		m_LayerStack.PushLayer(layer);
	}

	void Application::PushOverlay(Layer* overlay)
	{
		overlay->OnAttach();
		m_LayerStack.PushOverlay(overlay);
	}

}