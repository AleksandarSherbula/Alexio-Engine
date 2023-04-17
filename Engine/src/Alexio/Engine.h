#pragma once

#include "Window/Window.h"
#include "ImGui/ImGuiLayer.h"

namespace Alexio
{
	class Engine
	{	
	public:
		Engine();
		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window* GetWindow() const { return mWindow.get(); }

		void SetGraphicsAPI(GraphicsAPI api);

		inline uint32_t ScreenWidth()  const { return mWindow->GetWidth(); }
		inline uint32_t ScreenHeight() const { return mWindow->GetHeight(); }

		inline void SetFullScreen(bool fullscreen) { mWindow->SetFullScreen(fullscreen); }
		inline bool IsFullScreen() const { return mWindow->IsFullScreen(); }

		inline void Close() { if (mRunning) mRunning = false; }
		ImGuiLayer* GetImGuiLayer() { return mImGuiLayer; }

		inline static Engine* Get() { return sInstance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		Ref<Window> mWindow;
		bool mRunning;
		
		LayerStack mLayerStack;
		ImGuiLayer* mImGuiLayer;
	private:
		static Engine* sInstance;
	};
}