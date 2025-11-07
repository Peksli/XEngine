#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>


namespace XEngine {

	using sink = spdlog::sinks::stdout_color_sink_mt;
	using logger = spdlog::logger;

	class LogSystem
	{
	public:
		static std::shared_ptr<sink> xengineSink;
		static std::shared_ptr<sink> clientSink;

		static std::shared_ptr<logger> xengineLogger;
		static std::shared_ptr<logger> clientLogger;

		static void Initialize();
		static std::shared_ptr<logger> GetxengineLogger() { return xengineLogger; }
		static std::shared_ptr<logger> GetClientLogger() { return clientLogger; }

		template<typename... Args> static void XEngineTrace(Args&&... args) { xengineLogger->trace(std::forward<Args>(args)...); }
		template<typename... Args> static void XEngineDebug(Args&&... args) { xengineLogger->debug(std::forward<Args>(args)...); }
		template<typename... Args> static void XEngineInfo(Args&&... args) { xengineLogger->info(std::forward<Args>(args)...); }
		template<typename... Args> static void XEngineWarn(Args&&... args) { xengineLogger->warn(std::forward<Args>(args)...); }
		template<typename... Args> static void XEngineError(Args&&... args) { xengineLogger->error(std::forward<Args>(args)...); }
		template<typename... Args> static void XEngineCritical(Args&&... args) { xengineLogger->critical(std::forward<Args>(args)...); }

		template<typename... Args> static void ClientTrace(Args&&... args) { clientLogger->trace(std::forward<Args>(args)...); }
		template<typename... Args> static void ClientDebug(Args&&... args) { clientLogger->debug(std::forward<Args>(args)...); }
		template<typename... Args> static void ClientInfo(Args&&... args) { clientLogger->info(std::forward<Args>(args)...); }
		template<typename... Args> static void ClientWarn(Args&&... args) { clientLogger->warn(std::forward<Args>(args)...); }
		template<typename... Args> static void ClientError(Args&&... args) { clientLogger->error(std::forward<Args>(args)...); }
		template<typename... Args> static void ClientCritical(Args&&... args) { clientLogger->critical(std::forward<Args>(args)...); }
	};

}

#define XEngine_TRACE(...)		XEngine::LogSystem::XEngineTrace(__VA_ARGS__);
#define XEngine_DEBUG(...)		XEngine::LogSystem::XEngineDebug(__VA_ARGS__);
#define XEngine_INFO(...)		XEngine::LogSystem::XEngineInfo(__VA_ARGS__);
#define XEngine_WARN(...)		XEngine::LogSystem::XEngineWarn(__VA_ARGS__);
#define XEngine_ERROR(...)		XEngine::LogSystem::XEngineError(__VA_ARGS__);
#define XEngine_CRITICAL(...)	XEngine::LogSystem::XEngineCritical(__VA_ARGS__);


#define Client_TRACE(...)		XEngine::LogSystem::ClientTrace(__VA_ARGS__);
#define Client_DEBUG(...)		XEngine::LogSystem::ClientDebug(__VA_ARGS__);
#define Client_INFO(...)		XEngine::LogSystem::ClientInfo(__VA_ARGS__);
#define Client_WARN(...)		XEngine::LogSystem::ClientWarn(__VA_ARGS__);
#define Client_CRITICAL(...)	XEngine::LogSystem::ClientCritical(__VA_ARGS__);