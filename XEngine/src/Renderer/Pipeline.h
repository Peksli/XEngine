#pragma once

#include <cstdint>


namespace XEngine {

	class Pipeline
	{
	public:	
		Pipeline();
		virtual ~Pipeline();

		virtual void Initialize() = 0;
		virtual void Shutdown() = 0;
		virtual void Recreate() = 0;

		uint32_t GetId() const { return m_Id; }

	protected:
		uint32_t m_Id;
	};

}