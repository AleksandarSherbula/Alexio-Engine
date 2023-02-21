#include "aio_pch.h"
#include "Engine.h"

namespace Alexio
{
	Engine* Engine::sInstance = nullptr;

	Engine::Engine()
	{
		AIO_ASSERT(!sInstance, "An instance of Engine has already been made");
		sInstance = this;
		
		mRunning = true;
	}

	void Engine::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(std::bind(&Engine::OnWindowClose, this, std::placeholders::_1));
		dispatcher.Dispatch<WindowResizeEvent>(std::bind(&Engine::OnWindowResize, this, std::placeholders::_1));

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
	}

	void Engine::Run()
	{
		Renderer::SetGraphicsAPI(GraphicsAPI::OpenGL);

		std::string apiName = (Renderer::GetGraphicsAPI() == GraphicsAPI::OpenGL) ? "OpenGL" : "DirectX11";

		mWindow = Window::Create("Alexio Engine (" + apiName + ")", 1280, 720);
		mWindow->SetEventCallback(std::bind(&Engine::OnEvent, this, std::placeholders::_1));
		Input::SetKeyCodes();

		Renderer::Begin(mWindow.get());

		/// Test code ///

		float vertices[] =
		{
		   -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
			0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		uint32_t indices[] =
		{
			0, 1, 2
		};

		vd = VertexData::Create();

		vb = VertexBuffer::Create(vertices, sizeof(vertices));
		vb->Bind();

		ib = IndexBuffer::Create(indices, 3);
		ib->Bind();

		{
			BufferLayout layout =
			{
				{ShaderDataType::Float2, "aPosition" },
				{ShaderDataType::Float4, "aColor" }
			};
			vb->SetLayout(layout);
		}

		vd->AddVertexBuffer(vb);
		vd->SetIndexBuffer(ib);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPosition;
			layout(location = 1) in vec4 aColor;

			out vec3 vPosition;
			out vec4 vColor;

			void main()
			{
				vPosition = aPosition;
				vColor = aColor;
				gl_Position = vec4(aPosition, 1.0);	
			}
			)";

		std::string pixelSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 vPosition;
			in vec4 vColor;

			void main()
			{
				color = vColor;
			}
			)";

		shader = Shader::Create(vertexSrc, pixelSrc);

		float blueSquareVertices[] =
		{
		   -0.5f, -0.5f,
			0.5f, -0.5f,
			0.5f,  0.5f,
		   -0.5f,  0.5f
		};

		uint32_t blueSquareIndices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		blueSquareVD = VertexData::Create();

		blueSquareVB = VertexBuffer::Create(blueSquareVertices, sizeof(blueSquareVertices));
		blueSquareVB->Bind();

		blueSquareIB = IndexBuffer::Create(blueSquareIndices, 6);
		blueSquareIB->Bind();

		{
			BufferLayout layout =
			{
				{ShaderDataType::Float2, "aPosition" }
			};
			blueSquareVB->SetLayout(layout);
		}

		blueSquareVD->AddVertexBuffer(blueSquareVB);
		blueSquareVD->SetIndexBuffer(blueSquareIB);

		std::string blueSquareVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 aPosition;

			void main()
			{				
				gl_Position = vec4(aPosition, 1.0);	
			}
			)";

		std::string blueSquareFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			void main()
			{
				color = vec4(0.0, 0.8, 1.0, 1.0);
			}
			)";

		blueSquareShader = Shader::Create(blueSquareVertexSrc, blueSquareFragmentSrc);

		AIO_ASSERT(OnStart(),"Initialization failed");

		while (mRunning)
		{
			mWindow->PollEvents();

			Input::Scan();

			for (Layer* layer : mLayerStack)
				layer->OnUpdate();
			
			if (!OnUpdate() ||
				// Manual code for closing on alt + F4 for Win32 API, since the system keys are not being checked
				(Window::GetAPI() == WindowAPI::Win32 && Input::KeyHeld(Alexio::L_ALT) && Input::KeyPressed(Alexio::F4)))
				mRunning = false;
			
			Renderer::ClearColor(0.0f, 0.0f, 0.0f, 1.0f);

			Renderer::Draw(blueSquareShader, blueSquareVD);
			Renderer::Draw(shader, vd);

			Renderer::imgui->Begin();
			Renderer::imgui->OnUpdate();
			Renderer::imgui->End();

			Renderer::SwapBuffer();
		}

		Renderer::End();
	}	

	bool Engine::OnWindowClose(WindowCloseEvent& e)
	{
		mRunning = false;
		return true;
	}

	bool Engine::OnWindowResize(WindowResizeEvent& e)
	{
		if (Renderer::GetAPI() != nullptr)
			Renderer::GetAPI()->SetViewport(0, 0, e.GetWidth(), e.GetHeight());
		return true;
	}
}


int main(int argc, char** agrv)
{
	Alexio::Log::Initialize();

	Alexio::Engine* engine = Create();
	engine->Run();
	delete engine;
	return 0;
}


