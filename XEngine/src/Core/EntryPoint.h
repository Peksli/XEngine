#pragma once

#include "src/Core/Application.h"
#include "src/Core/LogSystem.h"


int main(int argc, char** argv) 
{
	XEngine::LogSystem::Initialize();
	XEngine_INFO("Hello from XEngine");

	XEngine::Application* app = XEngine::CreateApplication();
	app->Run();
	delete app;

	return 0;
}