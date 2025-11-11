#pragma once


struct GLFWwindow;

namespace XEngine {

	class Context
	{
	public:
		Context(GLFWwindow* window);
		virtual ~Context();

		virtual void Initialize() = 0;
		virtual void SwapBuffers() = 0;

	private:
		GLFWwindow* m_Window = nullptr;
	};

}