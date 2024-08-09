#pragma once

#include <memory>

#include "Core.h"
#include "spdlog/spdlog.h"

namespace Keela {
	class KEELA_API Log
	{
	public:
		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger;  }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	};

}

// Core log macros
#define KEE_CORE_TRACE(...)    ::Keela::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define KEE_CORE_INFO(...)     ::Keela::Log::GetCoreLogger()->info(__VA_ARGS__)
#define KEE_CORE_WARN(...)     ::Keela::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define KEE_CORE_ERROR(...)    ::Keela::Log::GetCoreLogger()->error(__VA_ARGS__)
#define KEE_CORE_FATAL(...)    ::Keela::Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Client log macros
#define KEE_TRACE(...)         ::Keela::Log::GetClientLogger()->trace(__VA_ARGS__)
#define KEE_INFO(...)          ::Keela::Log::GetClientLogger()->info(__VA_ARGS__)
#define KEE_WARN(...)          ::Keela::Log::GetClientLogger()->warn(__VA_ARGS__)
#define KEE_ERROR(...)         ::Keela::Log::GetClientLogger()->error(__VA_ARGS__)
#define KEE_FATAL(...)         ::Keela::Log::GetClientLogger()->fatal(__VA_ARGS__)

