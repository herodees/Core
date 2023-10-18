#pragma once

namespace box
{
	class physics : public system
    {
    public:
        physics() = default;
        virtual ~physics() = default;

    };

    enum class body_type
    {
        DYNAMIC,
        KINEMATIC,
        STATIC
    };

    struct rigid_body_component : component
    {
        virtual ~rigid_body_component() = default;

        virtual void      activate(bool act)                                      = 0;
        virtual bool      is_active() const                                       = 0;
        virtual body_type get_type() const                                        = 0;
        virtual void      set_type(body_type type)                                = 0;
        virtual void      set_position(Vec2f position)                            = 0;
        virtual Vec2f     get_position() const                                    = 0;
        virtual void      set_mass(float mass)                                    = 0;
        virtual float     get_mass() const                                        = 0;
        virtual void      set_moment(float moment)                                = 0;
        virtual float     get_moment() const                                      = 0;
        virtual void      set_angle(float angle)                                  = 0;
        virtual float     get_angle() const                                       = 0;
        virtual void      set_angular_velocity(float velocity)                    = 0;
        virtual float     get_angular_velocity() const                            = 0;
        virtual void      set_velocity(Vec2f vlocity)                             = 0;
        virtual Vec2f     get_velocity() const                                    = 0;
        virtual void      set_force(Vec2f force)                                  = 0;
        virtual Vec2f     get_force() const                                       = 0;
        virtual void      set_torque(float torque)                                = 0;
        virtual float     get_torque() const                                      = 0;
        virtual void      set_data(void* data)                                    = 0;
        virtual void*     get_data() const                                        = 0;
        virtual void      update_velocity(Vec2f gravity, float damping, float dt) = 0;
        virtual void      update_position(float dt)                               = 0;
        virtual Vec2f     local_to_world(Vec2f pos) const                         = 0;
        virtual Vec2f     world_to_local(Vec2f pos) const                         = 0;
        virtual Vec2f     get_velocity_at_world(Vec2f pos) const                  = 0;
        virtual Vec2f     get_velocity_at_local(Vec2f pos) const                  = 0;
        virtual float     kinetic_energy() const                                  = 0;
        virtual void      apply_force_world(Vec2f f, Vec2f r)                     = 0;
        virtual void      apply_impulse_world(Vec2f j, Vec2f r)                   = 0;
        virtual void      apply_force_local(Vec2f f, Vec2f r)                     = 0;
        virtual void      apply_impulse_local(Vec2f j, Vec2f r)                   = 0;
    };

} // namespace box