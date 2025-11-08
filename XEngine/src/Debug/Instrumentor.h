#pragma once

#include "src/Core/LogSystem.h"

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <string>
#include <thread>
#include <mutex>
#include <sstream>

namespace XEngine {

	using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

	struct ProfileResult 
	{
		std::string Name;
		FloatingPointMicroseconds Start;
		std::chrono::microseconds ElapsedTime;
		std::thread::id ThreadID;
	};

	struct InstrumentationSession 
	{
		std::string Name;
	};

	class Instrumentor 
	{
	public:
		Instrumentor(const Instrumentor&) = delete;
		Instrumentor(Instrumentor&&) = delete;

		void BeginSession(const std::string& name, const std::string& filepath = "results.json") 
		{
			m_Filepath = filepath;

			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession) 
			{
				XEngine_CRITICAL(
					fmt::runtime("Instrumentor::BeginSession('{0}') when session '{1}' already open."), name, m_CurrentSession->Name);
				InternalEndSession();
			}
			m_OutputStream.open(filepath);
			if (m_OutputStream.is_open()) 
			{
				m_CurrentSession = new InstrumentationSession({ name });
				WriteHeader();
			}
			else 
			{
				XEngine_CRITICAL(fmt::runtime("Instrumentor could not open results file '{0}'."), filepath);
			}
		}

		void EndSession() 
		{
			std::lock_guard lock(m_Mutex);
			InternalEndSession();
		}

		void WriteProfile(const ProfileResult& result) {
			std::stringstream json;
			json << std::setprecision(5) << std::fixed;
			json << ",\n  {";
			json << "\n    \"cat\": \"function\",";
			json << "\n    \"dur\": " << (result.ElapsedTime.count() / 1'000'000.0) << ",";
			json << "\n    \"name\": \"" << result.Name << "\",";
			json << "\n    \"ph\": \"X\",";
			json << "\n    \"pid\": 0,";
			json << "\n    \"tid\": " << result.ThreadID << ",";
			json << "\n    \"ts\": " << result.Start.count();
			json << "\n  }";

			if (m_OutputStream.tellp() > 1'000'000) // 1 mb
			{
				m_OutputStream.close();
				m_OutputStream.open(m_Filepath, std::ios::trunc);
				WriteHeader();
			}

			std::lock_guard lock(m_Mutex);
			if (m_CurrentSession) {
				m_OutputStream << json.str();
				m_OutputStream.flush();
			}
		}

		static Instrumentor& Get() {
			static Instrumentor instance;
			return instance;
		}

	private:
		Instrumentor() : m_CurrentSession(nullptr) {}
		~Instrumentor() { EndSession(); }

		void WriteHeader() 
		{
			m_OutputStream << "{\n  \"otherData\": {},\n  \"traceEvents\": [\n    {}";
			m_OutputStream.flush();
		}

		void WriteFooter() 
		{
			m_OutputStream << "\n  ]\n}\n";
			m_OutputStream.flush();
		}

		void InternalEndSession() 
		{
			if (m_CurrentSession) 
			{
				WriteFooter();
				m_OutputStream.close();
				delete m_CurrentSession;
				m_CurrentSession = nullptr;
			}
		}

	private:
		std::mutex m_Mutex;
		InstrumentationSession* m_CurrentSession;
		std::ofstream m_OutputStream;
		std::string m_Filepath;
	};

	class InstrumentationTimer 
	{
	public:
		InstrumentationTimer(const char* name)
			: m_Name(name), m_Stopped(false) 
		{
			m_StartTimepoint = std::chrono::steady_clock::now();
		}

		~InstrumentationTimer() 
		{
			if (!m_Stopped) Stop();
		}

		void Stop() 
		{
			auto endTimepoint = std::chrono::steady_clock::now();
			auto highResStart = FloatingPointMicroseconds{ m_StartTimepoint.time_since_epoch() };
			auto elapsedTime = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch()
				- std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch();

			Instrumentor::Get().WriteProfile({ m_Name, highResStart, elapsedTime, std::this_thread::get_id() });
			m_Stopped = true;
		}

	private:
		const char* m_Name;
		std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
		bool m_Stopped;
	};

}

#if defined(XEngine_DEBUG_BUILD)
	#define XE_PROFILE 1

	#if XE_PROFILE
	#if defined(__GNUC__) || defined(__clang__)
		#define XE_FUNC_SIG __PRETTY_FUNCTION__
	#elif defined(_MSC_VER)
		#define XE_FUNC_SIG __FUNCSIG__
	#else
		#define XE_FUNC_SIG __func__
	#endif
		#define XE_PROFILE_BEGIN_SESSION(name, filepath) ::XEngine::Instrumentor::Get().BeginSession(name, filepath)
		#define XE_PROFILE_END_SESSION() ::XEngine::Instrumentor::Get().EndSession()
		#define XE_PROFILE_SCOPE(name) ::XEngine::InstrumentationTimer timer##__LINE__(name)
		#define XE_PROFILE_FUNCTION() XE_PROFILE_SCOPE(XE_FUNC_SIG)
	#else
		#define XE_PROFILE_BEGIN_SESSION(name, filepath)
		#define XE_PROFILE_END_SESSION()
		#define XE_PROFILE_SCOPE(name)
		#define XE_PROFILE_FUNCTION()
	#endif
#endif