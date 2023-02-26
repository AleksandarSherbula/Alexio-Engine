#include "aio_pch.h"
#include "OpenGL_Shader.h"

namespace Alexio
{
	static GLenum ShaderTypeFromString(const std::string& type)
	{
		if (type == "vertex")
			return GL_VERTEX_SHADER;
		if (type == "fragment" || type == "pixel")
			return GL_FRAGMENT_SHADER;

		AIO_ASSERT(false, "Unknown shader type!");
		return 0;
	}


	OpenGL_Shader::OpenGL_Shader(const std::string& name)
	{
		mName = name;
		std::string filepath = "res/shaders/OpenGL/" + name + ".glsl";

		std::string source = ReadFile(filepath);
		mShaderSource = PreProcess(source);
	}

	OpenGL_Shader::OpenGL_Shader(const std::string& name, const std::string& filepath)
	{
		mName = name;
		std::string source = ReadFile(filepath);
		mShaderSource = PreProcess(source);
	}

	OpenGL_Shader::OpenGL_Shader(const std::string& name, const std::string& vertexSrc, const std::string& pixelSrc)
	{
		mShaderSource[GL_VERTEX_SHADER] = vertexSrc;
		mShaderSource[GL_FRAGMENT_SHADER] = pixelSrc;
	}

	OpenGL_Shader::~OpenGL_Shader()
	{		
		glDeleteProgram(mID);
	}

	void OpenGL_Shader::Compile()
	{
		GLuint program = glCreateProgram();

		std::array<GLenum, 2> glShaderIDs;
		uint8_t glShaderIDIndex = 0;
		for (auto& kv : mShaderSource)
		{
			GLenum type = kv.first;
			const std::string& source = kv.second;
			// Create an empty vertex shader handle
			GLuint shader = glCreateShader(type);

			// Send the vertex shader source code to GL
			// Note that std::string's .c_str is NULL character terminated.
			const GLchar* sourceC = source.c_str();
			glShaderSource(shader, 1, &sourceC, 0);

			// Compile the vertex shader
			glCompileShader(shader);

			GLint isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);
			if (isCompiled == GL_FALSE)
			{
				GLint maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

				// The maxLength includes the NULL character
				std::vector<GLchar> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);

				// We don't need the shader anymore.
				glDeleteShader(shader);

				AIO_LOG_ERROR("{0}", infoLog.data());
				AIO_ASSERT(false, "Vertex shader compilation failure!");
			}
			glAttachShader(program, shader);
			glShaderIDs[glShaderIDIndex++] = shader;
		}

		mID = program;

		glLinkProgram(program);
		
		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			
			glDeleteProgram(program);

			for (auto id : glShaderIDs)
				glDeleteShader(id);

			AIO_LOG_ERROR("{0}", infoLog.data());
			AIO_ASSERT(false, "Shader link failure!");
			return;
		}

		for (auto id : glShaderIDs)
			glDetachShader(program, id);

	}

	void OpenGL_Shader::Bind() const
	{
		glUseProgram(mID);
	}

	void OpenGL_Shader::Unbind() const
	{
		glUseProgram(0);
	}

	std::string OpenGL_Shader::ReadFile(const std::string& filepath)
	{
		std::string result;
		std::ifstream in(filepath, std::ios::in | std::ios::binary);
		if (in)
		{
			in.seekg(0, std::ios::end);
			result.resize(in.tellg());
			in.seekg(0, std::ios::beg);
			in.read(&result[0], result.size());
			in.close();
		}
		else
		{
			AIO_LOG_ERROR("Could not open file '{0}'", filepath);
		}

		return result;
	}

	std::unordered_map<GLenum, std::string> OpenGL_Shader::PreProcess(const std::string& source)
	{
		std::unordered_map<GLenum, std::string> shaderSources;

		const char* typeToken = "#type";
		size_t typeTokenLength = strlen(typeToken);
		size_t pos = source.find(typeToken, 0);
		while (pos != std::string::npos)
		{
			size_t eol = source.find_first_of("\r\n", pos);
			AIO_ASSERT(eol != std::string::npos, "Syntax error");
			size_t begin = pos + typeTokenLength + 1;
			std::string type = source.substr(begin, eol - begin);
			AIO_ASSERT(ShaderTypeFromString(type), "Invalid shader type specified");

			size_t nextLinePos = source.find_first_not_of("\r\n", eol);
			pos = source.find(typeToken, nextLinePos);
			shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
		}

		return shaderSources;
	}
}

