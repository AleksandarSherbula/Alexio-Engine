#include "aio_pch.h"
#include "Engine.h"


namespace Alexio
{
	Engine* Engine::sInstance = nullptr;

	static GLenum ShaderDataTypeSizeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:    return GL_FLOAT;
		case ShaderDataType::Float2:   return GL_FLOAT;
		case ShaderDataType::Float3:   return GL_FLOAT;
		case ShaderDataType::Float4:   return GL_FLOAT;
		case ShaderDataType::Mat3:     return GL_FLOAT;
		case ShaderDataType::Mat4:     return GL_FLOAT;
		case ShaderDataType::Int:      return GL_INT;
		case ShaderDataType::Int2:     return GL_INT;
		case ShaderDataType::Int3:     return GL_INT;
		case ShaderDataType::Int4:     return GL_INT;
		case ShaderDataType::Bool:     return GL_BOOL;
		}

		AIO_ASSERT(false, "Unknown ShaderDataType!");
		return 0;
	}

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
		Renderer::SetAPIflag(GraphicsAPIflag::DirectX11);

		std::string apiName = (Renderer::GetAPIflag() == GraphicsAPIflag::DirectX11) ? "DirectX 11" : "OpenGL";

		mWindow = Window::Create("Alexio Engine (" + apiName + ")", 1280, 720, Renderer::GetAPIflag());
		mWindow->SetEventCallback(std::bind(&Engine::OnEvent, this, std::placeholders::_1));
		Input::SetKeyCodes();

		Renderer::Begin(mWindow.get());

		/// TEMPORARY CODE!!! ///////
		if (Renderer::GetAPIflag() == GraphicsAPIflag::OpenGL)
		{
			float vertices[] =
			{
			   -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
				0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
				0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f
			};

			uint32_t indices[3] =
			{
				0, 1, 2
			};

			glGenVertexArrays(1, &va);
			glBindVertexArray(va);

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

			uint32_t index = 0;
			auto& layout = vb->GetLayout();
			for (auto& element : layout)
			{
				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index,
					element.GetComponentCount(),
					ShaderDataTypeSizeToOpenGLBaseType(element.type),
					element.normalized ? GL_TRUE : GL_FALSE,
					layout.GetStride(),
					(const void*)element.offset);
				index++;
			}

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

			std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 vPosition;
			in vec4 vColor;

			void main()
			{
				color = vColor;
			}
			)";

			shader = std::make_unique<Shader>(vertexSrc, fragmentSrc);
			shader->Bind();
		}
		

		AIO_ASSERT(OnStart(),"Initialization failed");

		while (mRunning)
		{
			mWindow->PollEvents();

			Input::Scan();

			for (Layer* layer : mLayerStack)
				layer->OnUpdate();
			
			if (!OnUpdate() || 
			// Because Win32 API needs to make my life hard
				(Window::GetAPI() == WindowAPI::Win32 && Input::KeyHeld(Alexio::L_ALT) && Input::KeyPressed(Alexio::F4)))
				mRunning = false;
			
			Renderer::DrawFrame();
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


