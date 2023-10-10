#pragma once

namespace box
{
	using EntityId = uint32_t;

	enum class ComponentType
	{
		TRANSFORM = 0,
		SPRITE,
	};

	struct IComponent
	{

	};

	class IScene
	{
	public:
		virtual ~IScene() = default;

		virtual EntityId create() = 0;
		virtual void release(EntityId id) = 0;
		virtual bool is_valid(EntityId id) const = 0;
		virtual IComponent* emplace(EntityId id, std::string_view component) = 0;
		virtual void remove(EntityId id, std::string_view component) = 0;
		virtual bool contains(EntityId id, std::string_view component) const = 0;
		virtual void patch(EntityId id, std::string_view component) = 0;
	};
}