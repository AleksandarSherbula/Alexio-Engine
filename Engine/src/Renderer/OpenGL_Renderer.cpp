#include "aio_pch.h"
#include "OpenGL_Renderer.h"
#include "Alexio/Engine.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Alexio
{
	Renderer_OpenGL* Renderer_OpenGL::sInstance = nullptr;

	Renderer_OpenGL::Renderer_OpenGL()
	{
		mWindow = nullptr;
		AIO_ASSERT(!sInstance, "OpenGL API object was already been made");
		sInstance = this;
	}

	void Renderer_OpenGL::Initialize()
	{
		glfwMakeContextCurrent((GLFWwindow*)mWindow->GetHandle());

		AIO_LOG_INFO("OpenGL Initialized");
	}

	void Renderer_OpenGL::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
	}

	void Renderer_OpenGL::Draw()
	{
	}

	void Renderer_OpenGL::ClearColor(float r, float g, float b, float a)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(r, g, b, a);
	}

	void Renderer_OpenGL::SwapBuffer()
	{
		glfwSwapInterval((int)mVSync);
		glfwSwapBuffers((GLFWwindow*)mWindow->GetHandle());
	}

	void Renderer_OpenGL::ImGuiBackendInit()
	{
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Engine::GetInstance()->GetWindow()->GetHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 410 core");
	}

	void Renderer_OpenGL::ImGuiBackendBegin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
	}

	void Renderer_OpenGL::ImGuiBackendDrawData()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}

	void Renderer_OpenGL::ImGuiBackendShutDown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}
}


