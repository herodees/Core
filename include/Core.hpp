#pragma once

namespace box
{
	struct vertex;
	class asset_provider;
	class texture;
	class material;
	class prototype;
	class sound;
	class plugin;
	class renderer;
	class IScene;
	class Entity;
	enum class blend_mode;
	enum class uniform_type;
}

#include <cstdint>
#include <cmath>
#include <string_view>
#include <atomic>
#include <charconv>
#include "Export.hpp"
#include "Var.hpp"
#include "Vec.hpp"
#include "Rect.hpp"
#include "Scene.hpp"
#include "Graphics.hpp"
#include "Components.hpp"
#include "Asset.hpp"
#include "Game.hpp"
