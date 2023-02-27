#pragma once

#include <string>
#include <memory>

#define BIT(x) (1 << x)

enum class GraphicsAPI
{
	None,
	OpenGL,
	DirectX11
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

namespace Alexio
{
	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> CreateScope(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}

	template<typename T>
	using Ref = std::shared_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Ref<T> CreateRef(Args&& ... args)
	{
		return std::make_shared<T>(std::forward<Args>(args)...);
	}
}

#ifndef AIO_PLATFORM_WINDOWS
#error Alexio Engine only supports Windows
#endif