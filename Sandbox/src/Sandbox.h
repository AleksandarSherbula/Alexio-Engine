#pragma once

#include "Alexio.h"

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
public:
	MainLayer();

	void OnStart() override;
	void OnUpdate() override;
	void OnImGuiRender() override;
	void OnEvent(Alexio::Event& event) override;
};