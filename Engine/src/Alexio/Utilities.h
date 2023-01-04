#pragma once

#include <string>

#define BIT(x) (1 << x)

enum class GraphicsAPI
{
	None,
	DirectX11,
	OpenGL
};

enum class WindowAPI
{
	None,
	GLFW,
	Win32
};

inline std::wstring StringToWide(const std::string& str)
{
	std::wstring wide_string(str.begin(), str.end());
	return wide_string;
}

inline std::string WideToString(const std::wstring& wstr)
{
	std::string str(wstr.begin(), wstr.end());
	return str;
}

#ifndef AIO_PLATFORM_WINDOWS
#error Alexio Engine only supports Windows
#endif