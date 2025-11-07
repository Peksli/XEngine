#include <XEngine.h>

class SandBox : public XEngine::Application
{
public:
	SandBox(XEngine::ApplicationSpecification& spec)
		: XEngine::Application(spec)
	{

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