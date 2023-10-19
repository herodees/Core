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

    void physics_impl::init(scene& scn)
    {
        scene_impl& scene = static_cast<scene_impl&>(scn);

        scene.register_component<rigid_body>("Rigidbody", "rigidbody");
        scene.register_component<circle_collider>("Circle collider", "circle_collider");
        scene.get_registry().on_construct<circle_collider>().connect<&entt::registry::emplace_or_replace<rigid_body>>();
        scene.register_component<segment_collider>("Line segment collider", "segment_collider");
        scene.get_registry().on_construct<segment_collider>().connect<&entt::registry::emplace_or_replace<rigid_body>>();
        scene.register_component<polygon_collider>("Polygon collider", "polygon_collider");
        scene.get_registry().on_construct<polygon_collider>().connect<&entt::registry::emplace_or_replace<rigid_body>>();
    }

    void physics_impl::set_gravity(Vec2f gravity)
    {
        _space.gravity.x = gravity.x;
        _space.gravity.y = gravity.y;
    }

    Vec2f physics_impl::get_gravity() const
    {
        return Vec2f(_space.gravity);
    }

    void physics_impl::set_damping(float damping)
    {
        _space.damping = damping;
    }

    float physics_impl::get_damping() const
    {
        return _space.damping;
    }

    void physics_impl::update(scene& scn, float delta)
    {
        constexpr float step_time = 0.0166666666f;
        _curent_time += delta;
        while (_curent_time >= step_time)
        {
            cpSpaceStep(&_space, delta);
            _curent_time -= step_time;
        }

        _space.gravity;
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



    void circle_collider::setup(float radius, Vec2f offset)
    {
        _shape.c.x                 = offset.x;
        _shape.c.y                 = offset.y;
        _shape.r                   = radius;
        _shape.shape.massInfo.m    = 0;
        _shape.shape.massInfo.i    = cpMomentForCircle(1.0f, 0.0f, radius, cpvzero);
        _shape.shape.massInfo.cog  = _shape.c;
        _shape.shape.massInfo.area = cpAreaForCircle(0.0f, radius);
    }

    void segment_collider::setup(Vec2f a, Vec2f b, float r)
    {
        _shape.a.x = a.x;
        _shape.a.y = a.y;
        _shape.b.x = b.x;
        _shape.b.y = b.y;
        _shape.n   = cpvrperp(cpvnormalize(cpvsub(_shape.b, _shape.a)));
        _shape.r = r;
        _shape.a_tangent = cpvzero;
        _shape.b_tangent = cpvzero;
        _shape.shape.massInfo.m = 0;
        _shape.shape.massInfo.i    = cpMomentForBox(1.0f, cpvdist(_shape.a, _shape.b) + 2.0f * r, 2.0f * r);
        _shape.shape.massInfo.cog  = cpvlerp(_shape.a, _shape.b, 0.5f);
        _shape.shape.massInfo.area = cpAreaForSegment(_shape.a, _shape.b, r);
    }

    void polygon_collider::setup(const Vec2f* verts, size_t count, float radius)
    {
        cpVect* hullVerts = (cpVect*)alloca(count * sizeof(cpVect));
        for (size_t i = 0; i < count; ++i)
        {
            hullVerts[i].x = verts[i].x;
            hullVerts[i].y = verts[i].y;
        }
        uint32_t hullCount = cpConvexHull(count, hullVerts, hullVerts, nullptr, 0.0f);
        
        cpVect centroid = cpCentroidForPoly(count, hullVerts);
        _shape.shape.massInfo.m     = 0;
        _shape.shape.massInfo.i     = cpMomentForPoly(1.0f, count, hullVerts, cpvneg(centroid), radius);
        _shape.shape.massInfo.cog   = centroid;
        _shape.shape.massInfo.area  = cpAreaForPoly(count, hullVerts, radius);
        _shape.r                    = radius;
        _shape.count                = count;

        if (count <= CP_POLY_SHAPE_INLINE_ALLOC)
        {
            _shape.planes = _shape._planes;
        }
        else
        {
            _shape.planes = (struct cpSplittingPlane*)cpcalloc(2 * count, sizeof(struct cpSplittingPlane));
        }

        for (int i = 0; i < count; i++)
        {
            cpVect a = hullVerts[(i - 1 + count) % count];
            cpVect b = hullVerts[i];
            cpVect n = cpvnormalize(cpvrperp(cpvsub(b, a)));
            _shape.planes[i + count].v0 = b;
            _shape.planes[i + count].n  = n;
        }
    }

} // namespace box