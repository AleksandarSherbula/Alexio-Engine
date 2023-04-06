#pragma once

#include "Alexio/Engine.h"

#include <vector>

class Sandbox : public Alexio::Engine
{
public:
	Sandbox();
	~Sandbox();
};

class MainLayer : public Alexio::Layer
{
public:
	std::shared_ptr<Alexio::Texture> texture;
	std::shared_ptr<Alexio::Texture> texture2;
	std::shared_ptr<Alexio::Texture> tileMap;

	Alexio::FramebufferSpecification fbSpec;

	std::shared_ptr<Alexio::Framebuffer> framebuffer;
public:
	MainLayer();

	void OnStart() override;
	void OnUpdate(float deltaTime) override;
	void OnImGuiRender() override;
	void OnEvent(Alexio::Event& event) override;
};