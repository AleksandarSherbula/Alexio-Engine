#include "Alexio.h"

class ExampleLayer : public Alexio::Layer
{
public:
	std::shared_ptr<Alexio::VertexResources> vd;
	std::shared_ptr<Alexio::VertexBuffer> vb;
	std::shared_ptr<Alexio::IndexBuffer> ib;
	std::shared_ptr<Alexio::Shader> shader;
	
	std::shared_ptr<Alexio::VertexResources> textureVD;
	std::shared_ptr<Alexio::VertexBuffer> textureVB;
	std::shared_ptr<Alexio::IndexBuffer> textureIB;
	std::shared_ptr<Alexio::Shader> textureShader;
	std::shared_ptr<Alexio::Texture> texture;

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

		float textureVertices[] =
		{
		   -0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 0.0f,
		    0.5f, -0.5f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 0.0f,
		    0.5f,  0.5f,   1.0f, 1.0f, 1.0f, 1.0f,   1.0f, 1.0f,
		   -0.5f,  0.5f,   1.0f, 1.0f, 1.0f, 1.0f,   0.0f, 1.0f
		};

		uint32_t textureIndices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		textureVD = Alexio::VertexResources::Create();
		textureVB = Alexio::VertexBuffer::Create(textureVertices, sizeof(textureVertices));
		textureIB = Alexio::IndexBuffer::Create(textureIndices, 6);
		texture = Alexio::Texture::Create("assets/img/awesomeface.png");

		{
			Alexio::BufferLayout layout =
			{
				{Alexio::ShaderDataType::Float2, "aPosition" },
				{Alexio::ShaderDataType::Float4, "aColor" }
			};
			vb->SetLayout(layout);
		}

		{
			Alexio::BufferLayout layout =
			{
				{Alexio::ShaderDataType::Float2, "aPosition" },
				{Alexio::ShaderDataType::Float4, "aColor" },
				{Alexio::ShaderDataType::Float2, "aTexCoord" },
			};
			textureVB->SetLayout(layout);
		}

		vd->AddVertexBuffer(vb);
		vd->SetIndexBuffer(ib);

		textureVD->AddVertexBuffer(textureVB);
		textureVD->SetIndexBuffer(textureIB);
		
		shader = Alexio::Shader::Create("flatColor");
		shader->SetVertexResources(vd);
		shader->Compile();

		textureShader = Alexio::Shader::Create("texture");
		textureShader->SetVertexResources(textureVD);
		textureShader->Compile();
		textureShader->SetInt("uTexture", 0);

		texture->Bind(0);
	}

	void OnUpdate(float deltaTime) override
	{
		AIO_LOG_TRACE("{0}s", deltaTime);

		Alexio::Renderer::ClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
		
		Alexio::Renderer::Draw(textureShader, textureVD);
		//Alexio::Renderer::Draw(shader, vd);
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