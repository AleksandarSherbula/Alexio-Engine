#include "aio_pch.h"
#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace Alexio
{
	std::shared_ptr<spdlog::logger> Log::sLogger;

	void Log::Initialize()
	{
		spdlog::set_pattern("%^[%T] %l: %v%$");
		sLogger = spdlog::stdout_color_mt("Alexio");
		sLogger->set_level(spdlog::level::trace);
	}
}

