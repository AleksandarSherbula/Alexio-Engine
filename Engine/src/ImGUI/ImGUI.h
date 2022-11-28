#ifndef IMGUI_H
#define IMGUI_H

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

#endif // !IMGUI_H