#include "src/Core/LayerStack.h"
#include "src/Core/LogSystem.h"


namespace XEngine {

	LayerStack::LayerStack()
		: m_InsertIndex(0)
	{

	}

	LayerStack::~LayerStack()
	{
		for (auto layer : m_Layers)
		{
			layer->OnDetach();
			delete layer;
		}
	}

	void LayerStack::PushLayer(Layer* layer)
	{
		m_Layers.emplace(m_Layers.begin() + m_InsertIndex, layer);
		m_InsertIndex++;
	}

	void LayerStack::PushOverlay(Layer* overlay)
	{
		m_Layers.emplace_back(overlay);
	}

	void LayerStack::RemoveLayer(Layer* layer)
	{
		std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), layer);
		if (it != m_Layers.end())
		{
			layer->OnDetach();
			m_Layers.erase(it);
			delete layer;

			m_InsertIndex--;

			XEngine_INFO(fmt::runtime("Layer {0} was deleted"), layer->GetName());
		}
		else
		{
			XEngine_CRITICAL(fmt::runtime("Didn't find a layer: {0}"), layer->GetName());
		}
	}

	void LayerStack::RemoveOverlay(Layer* overlay)
	{
		std::vector<Layer*>::iterator it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
		if (it != m_Layers.end())
		{
			overlay->OnDetach();
			m_Layers.erase(it);
			delete overlay;

			m_InsertIndex--;

			XEngine_INFO(fmt::runtime("Overlay {0} was deleted"), overlay->GetName());
		}
		else
		{
			XEngine_CRITICAL(fmt::runtime("Didn't find an overlay: {0}"), overlay->GetName());
		}
	}

}