#pragma once

#include "Alexio.h"

namespace Alexio
{
	class Editor : public Engine
	{
	public:
		Editor();
		~Editor();
	};

	class EditorLayer : public Layer
	{
	public:
		std::shared_ptr<Texture> texture;
		std::shared_ptr<Texture> texture2;
		std::shared_ptr<Texture> tileMap;

		FramebufferSpecification fbSpec;

		std::shared_ptr<Framebuffer> framebuffer;

		Vector2 mViewportSize = { 0.0f, 0.0f };

		bool mViewportFocused = false, mViewportHovered = false;

		Ref<Scene> mCurrentScene;
		Object mSquare;
	public:
		EditorLayer();

		void OnStart() override;
		void OnUpdate() override;
		void OnImGuiRender() override;
		void OnEvent(Event& e) override;
	};
}

