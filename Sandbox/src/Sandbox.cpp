#include "Sandbox.h"

#include <iostream>

using namespace Alexio;

Sandbox::Sandbox()
{
	SetGraphicsAPI(OpenGL);

	PushLayer(new MainLayer());
}

Sandbox::~Sandbox()
{
}

MainLayer::MainLayer()
	: Layer("Main")
{
	Index = 0;
	particles.resize(1000);
}

void MainLayer::OnUpdate(float deltaTime)
{
	if (Index >= 1000)
		Index = 0;

	if (Input::MouseButtonHeld(L_BUTTON))
	{		
		Particle& particle = particles[Index];
		glm::vec2 mousePos = Input::GetMousePosition();

		float width = Engine::GetInstance()->GetWindow()->GetWidth();
		float height = Engine::GetInstance()->GetWindow()->GetHeight();

		float aspectRatio = width / height;

		glm::vec2 windowPos =
		{
			mousePos.x / width * (2.0f * aspectRatio) - aspectRatio,
			mousePos.y / height * 2.0f - 1.0f
		};

		particle.position = windowPos;
		particle.velocity.x = Random::Range(-1.0, 1.0);
		particle.velocity.y = -1.0f;
		particle.color = { 0.0f, 0.0f, 1.0f, 1.0f };
		particle.isActive = true;
		Index++;
	}

	for (Particle& particle : particles)
	{
		if (particle.isActive)
		{
			particle.position += particle.velocity * deltaTime;
			particle.size = { particle.lifeTime * 0.5f, particle.lifeTime * 0.5f };
			particle.color = { particle.lifeTime * 0.5f, particle.lifeTime * 0.5f, particle.lifeTime * 0.5f, 1.0f };
		}
	}

	Renderer::Clear(0.0f, 0.8f, 1.0f, 1.0f);

	for (Particle& particle : particles)
	{
		if (particle.isActive)
		{			
			Renderer::DrawCircle(particle.position, particle.color, particle.lifeTime * 0.5f, 1.0f, 0.8f);
			particle.lifeTime -= deltaTime * 0.3f;

			if (particle.lifeTime <= 0.0f)
			{
				particle.isActive = false;
				particle.lifeTime = 1.0f;
			}
		}
	}
}

void MainLayer::OnImGuiRender()
{
}

void MainLayer::OnEvent(Alexio::Event& event)
{
}