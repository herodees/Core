#include "Physics.hpp"
#include <chipmunk/chipmunk.h>

namespace box
{
    const component_definition* rigid_body::definition = nullptr;

    physics_impl::physics_impl()
    {
        reset();
    }

    physics_impl::~physics_impl()
    {
        if (_space.dynamicBodies)
        {
            cpSpaceDestroy(&_space);
        }
    }

    void physics_impl::update(game& game, float delta)
    {
        constexpr float step_time = 0.0166666666f;
        _curent_time += delta;
        while (_curent_time >= step_time)
        {
            cpSpaceStep(&_space, delta);
            _curent_time -= step_time;
        }
    }

    void physics_impl::reset()
    {
        if (_space.dynamicBodies)
        {
            cpSpaceDestroy(&_space);
        }
        cpSpaceInit(&_space);
    }

    rigid_body::rigid_body()
    {
        cpBodyInit(&_body, 0.f, 0.f);
    }

    rigid_body::~rigid_body()
    {
        cpBodyDestroy(&_body);
    }

    void rigid_body::activate(bool act)
    {
        if (act)
            cpBodyActivate(&_body);
        else
            cpBodySleep(&_body);
    }

    bool rigid_body::is_active() const
    {
        return !cpBodyIsSleeping(&_body);
    }

    body_type rigid_body::get_type() const
    {
        return static_cast<body_type>(cpBodyGetType(const_cast<cpBody*>(&_body)));
    }

    void rigid_body::set_type(body_type type)
    {
        cpBodySetType(&_body, static_cast<cpBodyType>(type));
    }

    void rigid_body::set_position(Vec2f position)
    {
        cpBodySetPosition(&_body, reinterpret_cast<cpVect&>(position));
    }

    Vec2f rigid_body::get_position() const
    {
        return Vec2f(cpBodyGetPosition(&_body));
    }

    void rigid_body::set_mass(float mass)
    {
        cpBodySetMass(&_body, mass);
    }

    float rigid_body::get_mass() const
    {
        return _body.m;
    }

    void rigid_body::set_moment(float moment)
    {
        cpBodySetMoment(&_body, moment);
    }

    float rigid_body::get_moment() const
    {
        return _body.i;
    }

    void rigid_body::set_angle(float angle)
    {
        cpBodySetAngle(&_body, angle);
    }

    float rigid_body::get_angle() const
    {
        return _body.a;
    }

    void rigid_body::set_angular_velocity(float velocity)
    {
        cpBodySetAngularVelocity(&_body, velocity);
    }

    float rigid_body::get_angular_velocity() const
    {
        return _body.w;
    }

    void rigid_body::set_velocity(Vec2f velocity)
    {
        cpBodySetVelocity(&_body, reinterpret_cast<cpVect&>(velocity));
    }

    Vec2f rigid_body::get_velocity() const
    {
        return Vec2f(_body.v);
    }

    void rigid_body::set_force(Vec2f force)
    {
        cpBodySetForce(&_body, reinterpret_cast<cpVect&>(force));
    }

    Vec2f rigid_body::get_force() const
    {
        return Vec2f(_body.f);
    }

    void rigid_body::set_torque(float torque)
    {
        cpBodySetTorque(&_body, torque);
    }

    float rigid_body::get_torque() const
    {
        return _body.t;
    }

    void rigid_body::set_data(void* data)
    {
        _body.userData = data;
    }

    void* rigid_body::get_data() const
    {
        return _body.userData;
    }

    void rigid_body::update_velocity(Vec2f gravity, float damping, float dt)
    {
        cpBodyUpdateVelocity(&_body, reinterpret_cast<cpVect&>(gravity), damping, dt);
    }

    void rigid_body::update_position(float dt)
    {
        cpBodyUpdatePosition(&_body, dt);
    }

    Vec2f rigid_body::local_to_world(Vec2f pos) const
    {
        return Vec2f(cpBodyLocalToWorld(&_body, reinterpret_cast<cpVect&>(pos)));
    }

    Vec2f rigid_body::world_to_local(Vec2f pos) const
    {
        return Vec2f(cpBodyWorldToLocal(&_body, reinterpret_cast<cpVect&>(pos)));
    }

    Vec2f rigid_body::get_velocity_at_world(Vec2f pos) const
    {
        return Vec2f(cpBodyGetVelocityAtWorldPoint(&_body, reinterpret_cast<cpVect&>(pos)));
    }

    Vec2f rigid_body::get_velocity_at_local(Vec2f pos) const
    {
        return Vec2f(cpBodyGetVelocityAtLocalPoint(&_body, reinterpret_cast<cpVect&>(pos)));
    }

    float rigid_body::kinetic_energy() const
    {
        return cpBodyKineticEnergy(&_body);
    }

    void rigid_body::apply_force_world(Vec2f f, Vec2f r)
    {
        cpBodyApplyForceAtWorldPoint(&_body, reinterpret_cast<cpVect&>(f), reinterpret_cast<cpVect&>(r));
    }

    void rigid_body::apply_impulse_world(Vec2f j, Vec2f r)
    {
        cpBodyApplyImpulseAtWorldPoint(&_body, reinterpret_cast<cpVect&>(j), reinterpret_cast<cpVect&>(r));
    }

    void rigid_body::apply_force_local(Vec2f f, Vec2f r)
    {
        cpBodyApplyForceAtLocalPoint(&_body, reinterpret_cast<cpVect&>(f), reinterpret_cast<cpVect&>(r));
    }

    void rigid_body::apply_impulse_local(Vec2f j, Vec2f r)
    {
        cpBodyApplyImpulseAtLocalPoint(&_body, reinterpret_cast<cpVect&>(j), reinterpret_cast<cpVect&>(r));
    }



} // namespace box