#pragma once

#include "Include.hpp"
#include <chipmunk/chipmunk_structs.h>
#include "Scene.hpp"

namespace box
{
	class physics_impl final : public physics
	{
	public:
        physics_impl();
        ~physics_impl() override;

        void update(game& game, float delta) override;

        void reset();

    private:
        cpSpace _space{};
        float   _curent_time{};
    };

    struct rigid_body : rigid_body_component
    {
        static const component_definition* definition;

        rigid_body();
        ~rigid_body() override;

        void      activate(bool act) override;
        bool      is_active() const override;
        body_type get_type() const override;
        void      set_type(body_type type) override;

        void  set_position(Vec2f position) override;
        Vec2f get_position() const override;
        void  set_mass(float mass) override;
        float get_mass() const override;
        void  set_moment(float moment) override;
        float get_moment() const override;
        void  set_angle(float angle) override;
        float get_angle() const override;
        void  set_angular_velocity(float velocity) override;
        float get_angular_velocity() const override;
        void  set_velocity(Vec2f velocity) override;
        Vec2f get_velocity() const override;
        void  set_force(Vec2f force) override;
        Vec2f get_force() const override;
        void  set_torque(float torque) override;
        float get_torque() const override;
        void  set_data(void* data) override;
        void* get_data() const override;

        void  update_velocity(Vec2f gravity, float damping, float dt) override;
        void  update_position(float dt) override;
        Vec2f local_to_world(Vec2f pos) const override;
        Vec2f world_to_local(Vec2f pos) const override;
        Vec2f get_velocity_at_world(Vec2f pos) const override;
        Vec2f get_velocity_at_local(Vec2f pos) const override;
        float kinetic_energy() const override;

        void apply_force_world(Vec2f f, Vec2f r) override;
        void apply_impulse_world(Vec2f j, Vec2f r) override;
        void apply_force_local(Vec2f f, Vec2f r) override;
        void apply_impulse_local(Vec2f j, Vec2f r) override;

        cpBody _body;
    };

} // namespace box