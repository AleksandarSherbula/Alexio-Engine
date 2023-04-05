#pragma once

#include "Alexio/Engine.h"

#include <vector>

struct Particle
{
	glm::vec2 position;
	glm::vec2 velocity;
	glm::vec4 color;
	float rotation;
	glm::vec2 size = { 0.5f, 0.5f };

	float lifeTime = 1.0f;
	bool isActive = false;
};


class Sandbox : public Alexio::Engine
{
public:
	Sandbox();
	~Sandbox();
};

class MainLayer : public Alexio::Layer
{
public:
	std::vector<Particle> particles;
	uint32_t Index;
public:
	MainLayer();

	void OnUpdate(float deltaTime) override;
	void OnImGuiRender() override;
	void OnEvent(Alexio::Event& event) override;
};