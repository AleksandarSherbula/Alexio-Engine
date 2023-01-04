#pragma once

#include "Alexio/Layer.h"

namespace Alexio
{
	class ImGUI : public Layer
	{
	public:
		void OnAttach() override;
		void OnDetach() override;
		void OnUpdate() override;
		void Begin();
	public:
		bool showWindow;
	};
}