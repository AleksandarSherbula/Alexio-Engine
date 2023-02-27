#include "aio_pch.h"
#include "Alexio/Engine.h"

#include "OpenGL_Renderer.h"
#include "OpenGL_Buffer.h"

#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Alexio
{
	OpenGL_Renderer* OpenGL_Renderer::sInstance = nullptr;

	void OpenGLMessageCallback(
		unsigned source,
		unsigned type,
		unsigned id,
		unsigned severity,
		int length,
		const char* message,
		const void* userParam)
	{
		switch (severity)
		{
		case GL_DEBUG_SEVERITY_HIGH:         AIO_LOG_CRITICAL(message); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       AIO_LOG_ERROR(message); return;
		case GL_DEBUG_SEVERITY_LOW:          AIO_LOG_WARN(message); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: AIO_LOG_TRACE(message); return;
		}

		AIO_ASSERT(false, "Unknown severity level!");
	}


	OpenGL_Renderer::OpenGL_Renderer()
	{
		mWindow = nullptr;
		AIO_ASSERT(!sInstance, "OpenGL API object was already been made");
		sInstance = this;
	}

	OpenGL_Renderer::~OpenGL_Renderer()
	{
	}

	void OpenGL_Renderer::Initialize()
	{
		int gladLoad = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		AIO_ASSERT(gladLoad, "Failed to initialize GLAD");

#ifdef AIO_DEBUG
		glEnable(GL_DEBUG_OUTPUT);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		glDebugMessageCallback(OpenGLMessageCallback, nullptr);

		glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
#endif
		AIO_LOG_INFO("OpenGL Initialized");		
	}

	void OpenGL_Renderer::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);
	}

	void OpenGL_Renderer::Draw(const Ref<Shader>& shader, const Ref<VertexResources>& vertexResources)
	{
		vertexResources->Bind();
		shader->Bind();
		glDrawElements(GL_TRIANGLES, vertexResources->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
		shader->Unbind();
		vertexResources->Unbind();
	}

	void OpenGL_Renderer::ClearColor(float r, float g, float b, float a)
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(r, g, b, a);
	}

	void OpenGL_Renderer::SwapBuffer()
	{
		glfwSwapInterval((int)mVSync);
		glfwSwapBuffers((GLFWwindow*)mWindow->GetHandle());
	}

	void OpenGL_Renderer::ImGuiBackendInit()
	{
		ImGui_ImplGlfw_InitForOpenGL((GLFWwindow*)Engine::GetInstance()->GetWindow()->GetHandle(), true);
		ImGui_ImplOpenGL3_Init("#version 410 core");
	}

	void OpenGL_Renderer::ImGuiBackendBegin()
	{
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
	}

	void OpenGL_Renderer::ImGuiBackendUpdate()
	{
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void OpenGL_Renderer::ImGuiBackendShutDown()
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
	}
}


