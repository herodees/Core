#include "Physics.hpp"
#include <chipmunk/chipmunk.h>

namespace box
{
    const component_definition* rigid_body::definition = nullptr;
    const component_definition* transform::definition = nullptr;

    static physics_impl* s_physics = nullptr;

    physics_impl::physics_impl()
    {
        activate();
        reset();
    }

    physics_impl::~physics_impl()
    {
        if (_space.dynamicBodies)
        {
            cpSpaceDestroy(&_space);
        }
    }

    physics_impl& physics_impl::active()
    {
        return *s_physics;
    }

    void physics_impl::init(scene& scn)
    {
        scene_impl& scene = static_cast<scene_impl&>(scn);
        scene.register_component<transform>();
        scene.register_component<rigid_body>();
        scene.register_component<circle_collider>();
        scene.register_component<segment_collider>();
        scene.register_component<polygon_collider>();
    }

    void physics_impl::space_step(cpSpace* space, cpFloat dt)
    {
        cpSpaceStep(space, dt);
    }

    void physics_impl::activate()
    {
        s_physics = this;
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

    uint32_t physics_impl::get_iterations() const
    {
        return _space.iterations;
    }

    void physics_impl::set_iterations(uint32_t iterations)
    {
        _space.iterations = iterations;
    }

    float physics_impl::get_idle_speed_treshold() const
    {
        return _space.idleSpeedThreshold;
    }

    void physics_impl::set_idle_speed_treshold(float idleSpeedThreshold)
    {
        _space.idleSpeedThreshold = idleSpeedThreshold;
    }

    float physics_impl::get_sleep_time_treshold() const
    {
        return _space.sleepTimeThreshold;
    }

    void physics_impl::set_sleep_time_treshold(float sleepTimeThreshold)
    {
        _space.sleepTimeThreshold = sleepTimeThreshold;
    }

    float physics_impl::get_collision_slop() const
    {
        return _space.collisionSlop;
    }

    void physics_impl::set_collision_slop(float collisionSlop)
    {
        _space.collisionSlop = collisionSlop;
    }

    float physics_impl::get_collision_bias() const
    {
        return _space.collisionBias;
    }

    void physics_impl::set_collision_bias(float collisionBias)
    {
        _space.collisionBias = collisionBias;
    }

    uint32_t physics_impl::get_collision_persistance() const
    {
        return _space.collisionPersistence;
    }

    void physics_impl::set_collision_persistance(uint32_t collisionPersistence)
    {
        _space.collisionPersistence = collisionPersistence;
    }

    void physics_impl::add_body(rigid_body_component* body)
    {
        cpSpaceAddBody(&_space, &static_cast<rigid_body*>(body)->_body);
    }

    void physics_impl::add_collider(collider_component* collider)
    {
        cpSpaceAddShape(&_space, &static_cast<circle_collider*>(collider)->_shape.shape);
    }

    void physics_impl::update(scene& scn, float delta)
    {
        constexpr float step_time = 0.0166666666f;
        _curent_time += delta;
        while (_curent_time >= step_time)
        {
            space_step(&_space, step_time);
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

    cpSpace& physics_impl::space()
    {
        return _space;
    }

    void physics_impl::debug_draw(renderer& rnd)
    {
        auto draw_circle_proc =
            [](cpVect            pos,
               cpFloat           angle,
               cpFloat           radius,
               cpSpaceDebugColor outlineColor,
               cpSpaceDebugColor fillColor,
               cpDataPointer     data)
        { 
            renderer* rnd = (renderer*)(data);
            rnd->draw_circle_segment({pos}, radius, (const color&)outlineColor, 0);
        };
        auto debug_seg_proc = [](cpVect a, cpVect b, cpSpaceDebugColor color, cpDataPointer data) {};
       auto debug_color_proc = [](cpShape* shape, cpDataPointer data) -> cpSpaceDebugColor
       {
            return {25, 25, 25, 255};
       };

        cpSpaceDebugDrawOptions options{};
        options.data = &rnd;
        options.flags = cpSpaceDebugDrawFlags(
            CP_SPACE_DEBUG_DRAW_SHAPES | CP_SPACE_DEBUG_DRAW_CONSTRAINTS | CP_SPACE_DEBUG_DRAW_COLLISION_POINTS);
        options.drawCircle = draw_circle_proc;
        options.colorForShape = debug_color_proc;
        options.drawSegment       = debug_seg_proc;
        options.shapeOutlineColor = {255, 25, 25, 255};


        cpSpaceDebugDraw(&_space, &options);
    }

    rigid_body::rigid_body()
    {
        cpBodyInit(&_body, 0.f, 0.f);

        transform* tr = static_cast<transform*>(
            transform::definition->storage->get(static_cast<scene_impl*>(s_physics->_scene)->current_entity()));
        _body.userData      = tr;
        tr->_body           = this;
        _body.position_func = [](cpBody* body, cpFloat dt)
        {
            transform* tr    = static_cast<transform*>(body->userData);
            body->p          = cpvadd({tr->_position.x, tr->_position.y}, cpvmult(cpvadd(body->v, body->v_bias), dt));
            tr->_position.x  = body->p.x;
            tr->_position.y  = body->p.y;
            auto rot         = cpvforangle(tr->_angle);
            tr->_rotation.x  = rot.x;
            tr->_rotation.y  = rot.y;
            body->a          = body->a + (body->w + body->w_bias) * dt;
            tr->_angle       = body->a; 
            body->transform  = cpTransformNewTranspose(rot.x, -rot.y,
                                                      body->p.x - (body->cog.x * rot.x - body->cog.y * rot.y),
                                                      rot.y, rot.x,
                                                      body->p.y - (body->cog.x * rot.y + body->cog.y * rot.x));
            body->v_bias     = cpvzero;
            body->w_bias     = 0.0f;
        };

        if (!_body.space)
            cpSpaceAddBody(&s_physics->space(), &_body);
    }

    rigid_body::~rigid_body()
    {
        get_transform()->_body = nullptr;
        auto space = _body.space;
        if (space)
        {
            if (get_type() == body_type::STATIC)
            {
                cpSpaceRemoveBody(space, &_body);
                cpSpaceReindexStatic(space);
            }
            else
            {
                cpSpaceRemoveBody(space, &_body);
            }
        }
    }



    void rigid_body::on_edit(entity& ent)
    {
        const char* type_names[] = {"Dynamic", "Kinematic", "Static"};

        if (ImGui::BeginCombo("Body type", type_names[(int)get_type()]))
        {
            for (auto& label : type_names)
                if (ImGui::Selectable(label))
                {
                    cpSpaceRemoveBody(_body.space, &_body);
                    set_type((body_type)std::distance(type_names, &label));
                    cpSpaceAddBody(&s_physics->space(), &_body);
                }
            ImGui::EndCombo();
        }
        

        ImGui::InputFloat2("Velocity", &_body.v.x);
        ImGui::InputFloat2("Force", &_body.f.x);


        if (ImGui::InputFloat("Mass", &_body.m))
        {
            cpBodySetMass(&_body, _body.m);
        }

        ImGui::InputFloat("Inertia", &_body.i);
        ImGui::InputFloat2("Center of gravity", &_body.cog.x);

        ImGui::InputFloat2("Angular velocity", &_body.w);
        ImGui::InputFloat2("Torque", &_body.t);
    }

    void rigid_body::activate(bool act)
    {
        if (get_type() != body_type::DYNAMIC)
            return;

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
        transform* tr = static_cast<transform*>(_body.userData);
        tr->_position = _body.p;
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

    transform* rigid_body::get_transform() const
    {
        return static_cast<transform*>(_body.userData);
    }

    void rigid_body::set_transform(transform* tr)
    {
        _body.userData = tr;
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

        if (!_shape.shape.space)
        {
            cpSpaceAddShape(&s_physics->space(), &_shape.shape);
        }
    }

    void circle_collider::on_edit(entity& ent)
    {
        if (ImGui::InputFloat2("Center", &_shape.c.x))
        {
            _shape.shape.massInfo.cog = _shape.c;
        }

        if (ImGui::DragFloat("Radius", &_shape.r))
        {
            if (_shape.r < 1.f)
                _shape.r = 1.0f;
            _shape.shape.massInfo.i = cpMomentForCircle(1.0f, 0.0f, _shape.r, cpvzero);
            _shape.shape.massInfo.area = cpAreaForCircle(0.0f, _shape.r);
        }

        base_collider<circle_collider_component, cpCircleShape>::on_edit(ent);

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

        if (!_shape.shape.space)
        {
            cpSpaceAddShape(&s_physics->space(), &_shape.shape);
        }
    }

    void segment_collider::on_edit(entity& ent)
    {
        base_collider<segment_collider_component, cpSegmentShape>::on_edit(ent);
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

        if (!_shape.shape.space)
        {
            cpSpaceAddShape(&s_physics->space(), &_shape.shape);
        }
    }

    void polygon_collider::on_edit(entity& ent)
    {
        base_collider<polygon_collider_component, cpPolyShape>::on_edit(ent);
    }

    Vec2f transform::get_position() const
    {
        return _position;
    }

    void transform::set_position(Vec2f v)
    {
        _position = v;
        if (_body)
        {
            cpBodySetPosition(&_body->_body, reinterpret_cast<cpVect&>(v));
        }
    }

    float transform::get_rotation() const
    {
        return _angle;
    }

    void transform::set_rotation(float v)
    {
        _rotation.x = std::cos(v);
        _rotation.y = std::sin(v);
        _angle      = v;
        if (_body)
        {
            cpBodySetAngle(&_body->_body, v);
        }
    }

    Vec2f transform::get_scale() const
    {
        return _scale;
    }

    void transform::set_scale(Vec2f v)
    {
        _scale = v;
    }

    rigid_body* transform::get_body() const
    {
        return _body;
    }

    void transform::set_body(rigid_body* b)
    {
        _body = b;
    }

    void transform::on_edit(entity& ent)
    {
        if (ImGui::InputFloat2("Position", &_position.x))
        {
            set_position(_position);
        }
        if (ImGui::InputFloat2("Scale", &_scale.x))
        {
        }
        if (ImGui::InputFloat("Rotation", &_angle))
        {
            set_rotation(_angle);
        }
    }

} // namespace box