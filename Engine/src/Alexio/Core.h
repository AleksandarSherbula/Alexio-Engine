#ifndef CORE_H
#define CORE_H

#ifndef AIO_PLATFORM_WINDOWS
	#error Alexio Engine only supports Windows
#endif

#define AIO_ASSERT(x, ...) { if(!(x)) { AIO_LOG_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }

#endif // !UTILITIES_H