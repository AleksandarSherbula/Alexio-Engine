#pragma once

#include "Window.h"
#include "Layer.h"
#include "ImGuiLayer.h"
#include "Events/AppEvent.h"

namespace Alexio
{
	class Engine
	{
	public:
		inline static Engine* GetInstance() { return sInstance; }
	public:
		Engine();
		void Run();

		void OnEvent(Event& e);

		virtual bool OnStart() = 0;
		virtual bool OnUpdate() = 0;

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window* GetWindow() { return mWindow.get(); }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> mWindow;
		bool mRunning;
		LayerStack mLayerStack;
	private:
		static Engine* sInstance;
	};
}

Alexio::Engine* Create();