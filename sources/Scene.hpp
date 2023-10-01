#pragma once

#include "Include.hpp"

namespace box
{
	class Scene : public IScene
	{
	public:
		Scene();
		~Scene() override;

	private:
		entt::registry _registry;
	};
}