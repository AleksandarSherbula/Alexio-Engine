#pragma once

#include "Alexio/Layer.h"

#include <imgui.h>

namespace Alexio
{
	class ImGUI : public Layer
	{
	public:
		void OnStart() override;
		void OnDetach() override;
		void OnImGuiRender() override;
		void Begin();
		void End();
	};
}