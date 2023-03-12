#include "Alexio.h"

class ExampleLayer : public Alexio::Layer
{
public:
	std::shared_ptr<Alexio::VertexResources> vd;
	std::shared_ptr<Alexio::VertexBuffer> vb;
	std::shared_ptr<Alexio::IndexBuffer> ib;
	std::shared_ptr<Alexio::Shader> shader;
	
	std::shared_ptr<Alexio::VertexResources> blueSquareVD;
	std::shared_ptr<Alexio::VertexBuffer> blueSquareVB;
	std::shared_ptr<Alexio::IndexBuffer> blueSquareIB;
	std::shared_ptr<Alexio::Shader> blueSquareShader;

	glm::vec4 clearColor;
	glm::vec2 cameraPosition = { 0.0f, 0.0f };
	float moveSpeed = 120.0f;
	float scrollSpeed = 80.0f;

	ExampleLayer()
		: Layer("Example")
	{
		clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };

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
		
		vd = Alexio::VertexResources::Create();
		vb = Alexio::VertexBuffer::Create(vertices, sizeof(vertices));
		ib = Alexio::IndexBuffer::Create(indices, 3);

		float blueSquareVertices[] =
		{
		   -0.5f, -0.5f,  0.0f, 0.8f, 1.0f, 1.0f,
		    0.5f, -0.5f,  0.0f, 0.8f, 1.0f, 1.0f,
		    0.5f,  0.5f,  0.0f, 0.8f, 1.0f, 1.0f,
		   -0.5f,  0.5f,  0.0f, 0.8f, 1.0f, 1.0f,
		};

		uint32_t blueSquareIndices[] =
		{
			0, 1, 2,
			2, 3, 0
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

		blueSquareShader = Alexio::Shader::Create("basic");
		blueSquareShader->SetVertexResources(blueSquareVD);
		blueSquareShader->Compile();
	}

	void OnUpdate(float deltaTime) override
	{
		AIO_LOG_TRACE("{0}s", deltaTime);

		Alexio::Renderer::ClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		
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

	bool Game::OnUpdate(float deltaTime) override
	{
		return !Alexio::Input::KeyPressed(ESCAPE);
	}
};

int main(int argc, char** agrv)
{
	Game* game = new Game();
	game->Run();
	delete game;
	return 0;
}