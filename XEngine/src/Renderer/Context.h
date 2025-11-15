#pragma once


namespace XEngine {

	class Context
	{
	public:
		Context();
		virtual ~Context();

		virtual void Initialize() = 0;
		virtual void SwapBuffers() = 0;
		virtual void Shutdown() = 0;
	};

}