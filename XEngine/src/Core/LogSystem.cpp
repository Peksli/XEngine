#include "src/Core/LogSystem.h"


namespace XEngine{

	std::shared_ptr<sink> LogSystem::xengineSink;
	std::shared_ptr<sink> LogSystem::clientSink;

	std::shared_ptr<logger> LogSystem::xengineLogger;
	std::shared_ptr<logger> LogSystem::clientLogger;

	void LogSystem::Initialize()
	{
		xengineSink = std::make_shared<sink>();
		clientSink = std::make_shared<sink>();

		xengineLogger = std::make_shared<logger>("xengineLogger", xengineSink);
		clientLogger = std::make_shared<logger>("clientLogger", clientSink);

		xengineLogger->set_level(spdlog::level::trace);
		clientLogger->set_level(spdlog::level::trace);
	}

}