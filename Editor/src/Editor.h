#pragma once

#include "Alexio/Engine.h"

#include <vector>

namespace Alexio
{
	class Editor : public Engine
	{
	public:
		Editor();
		~Editor();
	};

	class EditorLayer : public Alexio::Layer
	{
	public:
		std::shared_ptr<Alexio::Texture> texture;
		std::shared_ptr<Alexio::Texture> texture2;
		std::shared_ptr<Alexio::Texture> tileMap;

		Alexio::FramebufferSpecification fbSpec;

		std::shared_ptr<Alexio::Framebuffer> framebuffer;

		Vector2 mViewportSize = { 0.0f, 0.0f };

		bool mViewportFocused = false, mViewportHovered = false;

		Ref<Scene> mCurrentScene;
		Object mSquare;

		static Ref<Camera> sCamera;
	public:
		EditorLayer();

		void OnStart() override;
		void OnUpdate(float deltaTime) override;
		void OnImGuiRender() override;
		void OnEvent(Alexio::Event& event) override;
	};
}

