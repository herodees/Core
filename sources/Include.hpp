#pragma once

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

namespace chip
{
#include "chipmunk/chipmunk.h"
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
