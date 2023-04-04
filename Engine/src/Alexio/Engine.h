#pragma once

#include "Layer.h"
#include "Log.h"
#include "Timer.h"
#include "Utilities.h"
#include "Random.h"

#include "Math/Math.h"
#include "Input/Input.h"
#include "Events/Events.h"
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
		virtual bool OnUpdate() = 0;

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window* GetWindow() const { return mWindow.get(); }
		inline static Camera* GetCamera() { return sMainCamera.get(); }

		inline void SetFullScreen(bool fullscreen) { mWindow->SetFullScreen(fullscreen); }
		inline bool IsFullScreen() const { return mWindow->IsFullScreen(); }
		
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
		static Ref<Camera> sMainCamera;
	};
}