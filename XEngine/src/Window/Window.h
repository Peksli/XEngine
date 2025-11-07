#pragma once

#include <memory>
#include <string>


namespace XEngine {

	enum class Platform {
		None,
		Windows,
		Linux,
		Mac
	};

	struct WindowSpecification {
		Platform platform;
		int width;
		int height;
		bool wantVsync;
		std::string title;
	};

	class Window {
	public:
		Window(const WindowSpecification& spec);
		virtual ~Window() = default;

		virtual void OnUpdate() = 0;

		int GetWidth() const { return m_Width; }
		int GetHeight() const { return m_Height; }
		bool IsVsync() const { return m_IsVsync; }
		std::string GetTitle() const { return m_Title; }

		static std::unique_ptr<Window> CreateWindow(const WindowSpecification& spec);

	protected:
		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;

	protected:
		int m_Width;
		int m_Height;
		bool m_IsVsync;
		std::string m_Title;
	};

}