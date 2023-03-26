#include "aio_pch.h"
#include "Engine.h"

namespace Alexio
{
	Engine* Engine::sInstance = nullptr;
	Ref<Camera> Engine::sMainCamera = nullptr;

	Engine::Engine()
	{
		Alexio::Log::Initialize();

		AIO_ASSERT(!sInstance, "An instance of Engine has already been made");
		sInstance = this;

		Renderer::SetGraphicsAPI(DirectX11);
		imgui = new ImGUI();
		mRunning = true;
	}

	void Engine::Run()
	{
		static std::string apiName = "";
		#if defined(AIO_API_OPENGL) && defined(AIO_API_DX11)
				apiName = (Renderer::GetGraphicsAPI() == OpenGL) ? "OpenGL" : "DirectX11";
		#elif defined(AIO_API_OPENGL)
				apiName = "OpenGL";
		#elif defined(AIO_API_DX11)
				apiName = "DirectX11";
		#endif

		mWindow = Window::Create("Alexio (" + apiName + ")", 1280, 720);
		mWindow->SetEventCallback(std::bind(&Engine::OnEvent, this, std::placeholders::_1));
		Input::SetKeyCodes();

		sMainCamera = CreateRef<Camera>((float)mWindow->GetWidth() / (float)mWindow->GetHeight());
		Renderer::Init();

		AIO_ASSERT(OnStart(), "Failed to initialize application");

		PushOverlay(imgui);

		Timer::StartApp();
		while (mRunning)
		{
			Timer::Update();

			mWindow->PollEvents();
			Input::Scan();
			
			if (!OnUpdate() ||
				// Manual code for closing on alt + F4 for Win32 API, since the system keys are not being checked
				(Renderer::GetGraphicsAPI() == DirectX11 && (Input::KeyHeld(L_ALT) && Input::KeyPressed(F4))))
				mRunning = false;
			
			sMainCamera->OnUpdate(Timer::DetlaTime());
			
			imgui->Begin();
			for (Layer* layer : mLayerStack)
				layer->OnImGuiRender();
			
			for (Layer* layer : mLayerStack)
				layer->OnUpdate(Timer::DetlaTime());
			
			imgui->End();

			Renderer::GetBackend()->SwapBuffer();
		}
	}

	void Engine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Engine::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Engine::OnWindowResize, this, std::placeholders::_1));

		if (sMainCamera != nullptr)
			sMainCamera->OnEvent(e);

		for (auto it = mLayerStack.end(); it != mLayerStack.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled)
				break;
		}
	}

	void Engine::PushLayer(Layer* layer)
	{
		mLayerStack.PushLayer(layer);
	}

	void Engine::PushOverlay(Layer* layer)
	{
		mLayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	bool Engine::OnWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}

	bool Engine::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() != 0 && e.GetHeight() != 0)
			Renderer::SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}
}



