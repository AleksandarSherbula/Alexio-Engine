#ifndef LOG_H
#define LOG_H

#include "spdlog/spdlog.h"

namespace Alexio
{
	class Log
	{
	public:
		static void Initialize();

		static std::shared_ptr<spdlog::logger> sLogger;

	};
}

#define ALEXIO_LOG_TRACE(...)       Alexio::Log::sLogger->trace(__VA_ARGS__)
#define ALEXIO_LOG_INFO(...)        Alexio::Log::sLogger->info(__VA_ARGS__)
#define ALEXIO_LOG_WARN(...)        Alexio::Log::sLogger->warn(__VA_ARGS__)
#define ALEXIO_LOG_ERROR(...)       Alexio::Log::sLogger->error(__VA_ARGS__)
#define ALEXIO_LOG_CRITICAL(...)    Alexio::Log::sLogger->critical(__VA_ARGS__)


#endif // !LOG_H