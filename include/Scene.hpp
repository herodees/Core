#pragma once

namespace box
{
	using EntityId = uint32_t;

	struct IComponent
	{

	};

	class IScene
	{
	public:
		virtual ~IScene() = default;

		virtual EntityId create() = 0;
		virtual void release(EntityId id) = 0;
		virtual IComponent* add(EntityId id, std::string_view component) = 0;
		virtual void remove(EntityId id, std::string_view component) = 0;
		virtual bool contains(EntityId id, std::string_view component) const = 0;
	};
}