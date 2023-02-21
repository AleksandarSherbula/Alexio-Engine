#pragma once

#include "Window.h"
#include "Layer.h"
#include "ImGuiLayer.h"
#include "Renderer.h"
#include "Log.h"
#include "Input.h"
#include "Math.h"
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

		inline Window* GetWindow() const { return mWindow.get(); }
	private:
		bool OnWindowClose(WindowCloseEvent& e);
		bool OnWindowResize(WindowResizeEvent& e);
	private:
		std::unique_ptr<Window> mWindow;
		bool mRunning;
		
		LayerStack mLayerStack;
	public:
		std::shared_ptr<VertexData> vd;
		std::shared_ptr<VertexBuffer> vb;
		std::shared_ptr<IndexBuffer> ib;
		std::shared_ptr<Shader> shader;

		std::shared_ptr<VertexData> blueSquareVD;
		std::shared_ptr<VertexBuffer> blueSquareVB;
		std::shared_ptr<IndexBuffer> blueSquareIB;
		std::shared_ptr<Shader> blueSquareShader;
	private:
		static Engine* sInstance;
	};
}

Alexio::Engine* Create();