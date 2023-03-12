#pragma once

#include "Layer.h"
#include "Log.h"
#include "Math.h"
#include "Time.h"
#include "Input/Input.h"
#include "Events/AppEvent.h"
#include "Window/Window.h"
#include "Renderer/Renderer.h"
#include "ImGui/ImGuiLayer.h"

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
		virtual bool OnUpdate(float dt) = 0;

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window* GetWindow() const { return mWindow.get(); }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Ref<Window> mWindow;
		bool mRunning;
		
		LayerStack mLayerStack;
		ImGUI* imgui;

		
	private:
		static Engine* sInstance;
	};
}