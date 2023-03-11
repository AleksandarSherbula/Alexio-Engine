#include "Alexio.h"
#include "imgui/imgui.h"

#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"

class ExampleLayer : public Alexio::Layer
{
public:
	Alexio::Ref<Alexio::VertexResources> vd;
	Alexio::Ref<Alexio::VertexBuffer> vb;
	Alexio::Ref<Alexio::IndexBuffer> ib;
	Alexio::Ref<Alexio::Shader> shader;
	
	Alexio::Ref<Alexio::VertexResources> blueSquareVD;
	Alexio::Ref<Alexio::VertexBuffer> blueSquareVB;
	Alexio::Ref<Alexio::IndexBuffer> blueSquareIB;
	Alexio::Ref<Alexio::Shader> blueSquareShader;

	glm::vec4 clearColor;

	ExampleLayer()
		: Layer("Example")
	{
		clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

		/// Test code ///
		float vertices[] =
		{
			0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
			1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f
		};

		//float vertices[] =
		//{
		//   -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f,
		//    0.0f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f,
		//    0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 1.0f
		//};

		uint32_t indices[] =
		{
			0, 1, 2
		};
		
		vd = Alexio::VertexResources::Create();
		vb = Alexio::VertexBuffer::Create(vertices, sizeof(vertices));
		ib = Alexio::IndexBuffer::Create(indices, 3);

		float blueSquareVertices[] =
		{
			0.0f, 0.0f,  0.0f, 0.8f, 1.0f, 1.0f,   //top-left
			1.0f, 0.0f,  0.0f, 0.8f, 1.0f, 1.0f,   //top-right
			0.0f, 1.0f,  0.0f, 0.8f, 1.0f, 1.0f,   //bottom-left
			1.0f, 1.0f,  0.0f, 0.8f, 1.0f, 1.0f,   //bottom-right
		};

		//float blueSquareVertices[] =
		//{
		//   -0.5f, -0.5f, 0.0f, 0.8f, 1.0f, 1.0f,
		//    0.5f, -0.5f, 0.0f, 0.8f, 1.0f, 1.0f,
		//    0.5f,  0.5f, 0.0f, 0.8f, 1.0f, 1.0f,
		//   -0.5f,  0.5f, 0.0f, 0.8f, 1.0f, 1.0f
		//};

		uint32_t blueSquareIndices[] =
		{
			0, 1, 2,
			2, 3, 1
		};

		blueSquareVD = Alexio::VertexResources::Create();
		blueSquareVB = Alexio::VertexBuffer::Create(blueSquareVertices, sizeof(blueSquareVertices));
		blueSquareIB = Alexio::IndexBuffer::Create(blueSquareIndices, 6);

		{
			Alexio::BufferLayout layout =
			{
				{Alexio::ShaderDataType::Float2, "aPosition" },
				{Alexio::ShaderDataType::Float4, "aColor" }
			};
			vb->SetLayout(layout);
			blueSquareVB->SetLayout(layout);
		}

		vd->AddVertexBuffer(vb);
		vd->SetIndexBuffer(ib);

		blueSquareVD->AddVertexBuffer(blueSquareVB);
		blueSquareVD->SetIndexBuffer(blueSquareIB);
		
		shader = Alexio::Shader::Create("basic");
		shader->SetVertexResources(vd);
		shader->Compile();
		//shader->Bind();

		blueSquareShader = Alexio::Shader::Create("basic");
		blueSquareShader->SetVertexResources(blueSquareVD);
		blueSquareShader->Compile();
		//blueSquareShader->Bind();
	}

	float blue = 0.0f;
	float increment = 0.05f;
	void OnUpdate() override
	{
		Alexio::Renderer::ClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);

		// Test code currently commented
		 
		//if (blue < 0.0f)
		//	increment = 0.05f;
		//else if (blue > 1.0f)
		//	increment = -0.05f;
		//
		//blue += increment;
		//
		//float blueSquareVertices[] =
		//{
		//	0.0f, 0.0f,    0.0f, 0.8f, blue, 1.0f,   //top-left
		//	1.0f, 0.0f,    0.0f, 0.8f, blue, 1.0f,   //top-right
		//	1.0f, 1.0f,    0.0f, 0.8f, blue, 1.0f,   //bottom-right
		//	0.0f, 1.0f,    0.0f, 0.8f, blue, 1.0f    //bottom-left
		//};
		//
		//blueSquareVB->SetData(blueSquareVertices, sizeof(blueSquareVertices));
		
		Alexio::Renderer::Draw(blueSquareShader, blueSquareVD);
		Alexio::Renderer::Draw(shader, vd);
	}

	void OnEvent(Alexio::Event& event) override
	{
		
	}

	void OnImGuiRender() override
	{
		
	}
};

class Game : public Alexio::Engine
{
public:
	Game::Game()
	{
		// Set Graphics API here
		// Example: Alexio::Renderer::SetGraphicsAPI(GraphicsAPI::OpenGL);
	}

	bool Game::OnStart() override
	{
		PushLayer(new ExampleLayer());

		return true;
	}

	bool Game::OnUpdate() override
	{
		if (Alexio::Input::MouseButtonPressed(Alexio::L_BUTTON))
			AIO_LOG_TRACE("L_BUTTON was pressed");
		if (Alexio::Input::MouseButtonReleased(Alexio::L_BUTTON))
			AIO_LOG_TRACE("L_BUTTON is being released");
		
		if (Alexio::Input::MouseButtonPressed(Alexio::R_BUTTON))
			AIO_LOG_TRACE("R_BUTTON was pressed");
		if (Alexio::Input::MouseButtonReleased(Alexio::R_BUTTON))
			AIO_LOG_TRACE("R_BUTTON is being released");

		if (Alexio::Input::MouseButtonPressed(Alexio::M_BUTTON))
			AIO_LOG_TRACE("M_BUTTON was pressed");
		if (Alexio::Input::MouseButtonReleased(Alexio::M_BUTTON))
			AIO_LOG_TRACE("M_BUTTON is being released");

		return !Alexio::Input::KeyPressed(Alexio::ESCAPE);
	}
};

int main(int argc, char** agrv)
{
	Game* game = new Game();
	game->Run();
	delete game;
	return 0;
}