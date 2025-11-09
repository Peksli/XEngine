#include <XEngine.h>

class WorldLayer : public XEngine::Layer
{
public:
	WorldLayer(std::string name)
		: Layer(name)
	{

	}

	virtual ~WorldLayer()
	{

	}

	virtual void OnEvent(XEngine::Event& event) override	
	{
		XEngine_INFO(fmt::runtime("{0} layer: received an event: {1}"), GetName(), event.toString()); 
		event.SetHandled(true);
	}

};


class DebugLayer : public XEngine::Layer
{
public:
	DebugLayer(std::string name)
		: Layer(name)
	{

	}

	virtual ~DebugLayer()
	{

	}

	virtual void OnEvent(XEngine::Event& event) override
	{
		XEngine_INFO(fmt::runtime("{0} layer: received an event: {1}"), GetName(), event.toString());
		event.SetHandled(true);
	}

};


class SandBox : public XEngine::Application
{
public:
	SandBox(XEngine::ApplicationSpecification& spec)
		: XEngine::Application(spec)
	{
		PushLayer(new WorldLayer("World"));
		PushOverlay(new DebugLayer("Debug"));
	}

	virtual ~SandBox()
	{

	}
};


XEngine::Application* XEngine::CreateApplication()
{
	Client_INFO("Hello from sandbox");

	XEngine::ApplicationSpecification app_spec;
	app_spec.window_name = "XEngine";
	app_spec.enableVsync = true;
	app_spec.platform = XEngine::Platform::Windows;
	app_spec.window_width = 1280;
	app_spec.window_heigt = 720;

	return new SandBox(app_spec);
}