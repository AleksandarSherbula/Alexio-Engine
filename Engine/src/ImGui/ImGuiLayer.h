#pragma once

#include "Alexio/Layer.h"

#include <imgui.h>

namespace Alexio
{
	class ImGuiLayer : public Layer
	{
	public:
		void OnStart() override;
		void OnDetach() override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;
		void BlockEvents(bool block) { mBlockEvents = block; }
		void Begin();
		void End();
	private:
		bool mBlockEvents = true;
	};
}