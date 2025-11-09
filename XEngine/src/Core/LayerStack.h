#pragma once

#include "src/Core/Layer.h"
#include <vector>


namespace XEngine {

	class LayerStack
	{
	public:
		LayerStack();
		virtual ~LayerStack();

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* overlay);
		void RemoveLayer(Layer* layer);
		void RemoveOverlay(Layer* overlay);

		std::vector<Layer*>::iterator begin()			 { return m_Layers.begin();  }
		std::vector<Layer*>::iterator end()				 { return m_Layers.end();    }
		std::vector<Layer*>::reverse_iterator rbegin()	 { return m_Layers.rbegin(); }
		std::vector<Layer*>::reverse_iterator rend()	 { return m_Layers.rend();   }

		std::vector<Layer*>::const_iterator cbegin()			const { return m_Layers.cbegin();	}
		std::vector<Layer*>::const_iterator cend()				const { return m_Layers.cend();		}
		std::vector<Layer*>::const_reverse_iterator crbegin()	const { return m_Layers.crbegin();	}
		std::vector<Layer*>::const_reverse_iterator crend()		const { return m_Layers.crend();	}

	private:
		std::vector<Layer*> m_Layers;
		unsigned int m_InsertIndex;
	};

}