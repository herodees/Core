#pragma once

#define CP_USE_DOUBLES 0
#define CP_USE_CGTYPES 0

#include <Core.hpp>
#include <cstdint>
#include <algorithm>
#include <execution>

#include <imgui.h>
#include <entt/entt.hpp>

namespace ray
{
#include <raylib.h>
#include "rlgl.h"
#include "raymath.h"
}

namespace std
{
	struct string_hash
	{
		using is_transparent = void;
		[[nodiscard]] size_t operator()(const char* txt) const {
			return std::hash<std::string_view>{}(txt);
		}
		[[nodiscard]] size_t operator()(std::string_view txt) const {
			return std::hash<std::string_view>{}(txt);
		}
		[[nodiscard]] size_t operator()(const std::string& txt) const {
			return std::hash<std::string>{}(txt);
		}
	};
}
