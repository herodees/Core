#pragma once

namespace box
{
	struct CTransform : IComponent
	{
		Vec2f position{};
		Vec2f scale{1.f, 1.f};
		float rotation{ 0 };
	};

	struct CRigidbody : IComponent
	{
		float mass{};
		float drag{};
		float angular_drag{};
		bool gravity{};
	};

	struct CConstantForce: IComponent
	{
		Vec2f force;
		float torque{};
	};

	struct CCamera : IComponent
	{
		uint32_t clear;
		Color bg_color{0,0,0,255};
		bool msaa{};
		Rectf viewport{ 0, 0, 1.f, 1.f };
		IRenderTexture* target{};
	};

	struct CCameraFolower : IComponent
	{
		Vec2f speed;
	};

	struct CColider : IComponent
	{
		bool is_trigger{};
		Rectf region;
		Vec2f center;
	};

	struct CSpriteRenderer : IComponent
	{
		uint32_t sprite{};
		Color color{ 255,255,255,255 };
		IMaterial* material{};
		bool flip_x{};
		bool flip_y{};
		uint32_t sorting_layer{};
		uint32_t order{};
	};

	struct CAnimator : IComponent
	{
		uint32_t animation{};
		uint32_t animations{};
		bool play;
	};
}