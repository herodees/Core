#pragma once

#include "Include.hpp"
#include <chipmunk/chipmunk_structs.h>
#include <chipmunk/chipmunk_private.h>
#include "Scene.hpp"

namespace box
{
    struct rigid_body;

	class physics_impl final : public physics
	{
    public:
        physics_impl();
        ~physics_impl() override;

        static physics_impl& active();
        void     activate();

        void     set_gravity(Vec2f gravity) override;
        Vec2f    get_gravity() const override;
        void     set_damping(float damping) override;
        float    get_damping() const override;
        uint32_t get_iterations() const override;
        void     set_iterations(uint32_t iterations) override;
        float    get_idle_speed_treshold() const override;
        void     set_idle_speed_treshold(float idleSpeedThreshold) override;
        float    get_sleep_time_treshold() const override;
        void     set_sleep_time_treshold(float sleepTimeThreshold) override;
        float    get_collision_slop() const override;
        void     set_collision_slop(float collisionSlop) override;
        float    get_collision_bias() const override;
        void     set_collision_bias(float collisionBias) override;
        uint32_t get_collision_persistance() const override;
        void     set_collision_persistance(uint32_t collisionPersistence) override;
        void     add_body(rigid_body_component* body) override;
        void     add_collider(collider_component* collider) override;

        void     init(scene& scn) override;
        void     update(scene& scn, float delta) override;
        void     reset();
        cpSpace& space();

        void debug_draw(renderer& rnd);

    private:
        void space_step(cpSpace* space, cpFloat dt);

        cpSpace _space{};
        float   _curent_time{};
    };

    struct transform : transform_component
    {
        static const component_definition* definition;

        ~transform() override = default;

        Vec2f get_position() const override;
        void  set_position(Vec2f v) override;
        float get_rotation() const override;
        void  set_rotation(float v) override;
        Vec2f get_scale() const override;
        void  set_scale(Vec2f v) override;
        rigid_body* get_body() const;
        void        set_body(rigid_body* b);

        void  on_edit(entity& ent) override;

        Vec2f _position;
        Vec2f _scale{1.f,1.f};
        Vec2f _rotation;
        float _angle{};
        rigid_body* _body{};
    };

    struct rigid_body : rigid_body_component
    {
        static const component_definition* definition;
        static constexpr auto              in_place_delete = true;

        rigid_body();
        ~rigid_body() override;

        void      on_edit(entity& ent) override;

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
        transform* get_transform() const;
        void       set_transform(transform* tr);

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

        cpBody _body{};
    };

    template <typename T, typename SHAPE>
    struct base_collider : T
    {
        static const component_definition* definition;
        static constexpr auto              in_place_delete = true;

        base_collider();
        ~base_collider() override;
        void            set_body(rigid_body_component* body) override;
        float           get_mass() override;
        void            set_mass(float mass) override;
        float           get_density() override;
        void            set_density(float density) override;
        float           get_moment() override;
        float           get_area() override;
        Vec2f           get_center_of_gravity() override;
        Rectf           get_bb() const override;
        bool            get_sensor() const override;
        void            set_sensor(bool sensor) override;
        float           get_elasticity() const override;
        void            set_elasticity(float elasticity) override;
        float           get_friction() const override;
        void            set_friction(float friction) override;
        Vec2f           get_surface_velocity() const override;
        void            set_surface_velocity(Vec2f surfaceVelocity) override;
        void*           get_user_data() const override;
        void            set_user_data(void* userData) override;
        uintptr_t       get_collision_type() const override;
        void            set_collision_type(uintptr_t collisionType) override;
        collider_filter get_filter() const override;
        void            set_filter(collider_filter filter) override;
        void            on_edit(entity& ent) override;

        SHAPE _shape{};
    };

    struct circle_collider : base_collider<circle_collider_component, cpCircleShape>
    {
        void setup(float radius, Vec2f offset) override;
        void on_edit(entity& ent) override;
    };

    struct segment_collider : base_collider<segment_collider_component, cpSegmentShape>
    {
        void setup(Vec2f a, Vec2f b, float r) override;
        void on_edit(entity& ent) override;
    };

    struct polygon_collider : base_collider<polygon_collider_component, cpPolyShape>
    {
        void setup(const Vec2f* verts, size_t count, float radius) override;
        void on_edit(entity& ent) override;
    };



    template <typename T, typename SHAPE>
    const component_definition* base_collider<T, SHAPE>::definition = nullptr;

    template <>
    inline base_collider<circle_collider_component, cpCircleShape>::base_collider()
    {
        cpCircleShapeInit(&_shape, nullptr, 0, {0, 0});
    }

    template <>
    inline base_collider<segment_collider_component, cpSegmentShape>::base_collider()
    {
        cpSegmentShapeInit(&_shape, nullptr, {}, {}, 0);
    }

    template <>
    inline base_collider<polygon_collider_component, cpPolyShape>::base_collider()
    {
        cpPolyShapeInit(&_shape, nullptr, 0, nullptr, {}, 0);
        cpShapeSetBody(&_shape.shape, physics_impl::active().space().staticBody);
    }

    template <typename T, typename SHAPE>
    inline base_collider<T, SHAPE>::~base_collider()
    {
        cpShapeDestroy(&_shape.shape);
    }

    template <typename T, typename SHAPE>
    inline void base_collider<T, SHAPE>::set_body(rigid_body_component* body)
    {
        cpShapeSetBody(&_shape.shape, &static_cast<rigid_body*>(body)->_body);
    }

    template <typename T, typename SHAPE>
    inline float base_collider<T, SHAPE>::get_mass()
    {
        return _shape.shape.massInfo.m;
    }

    template <typename T, typename SHAPE>
    inline void base_collider<T, SHAPE>::set_mass(float mass)
    {
        cpShapeSetMass(&_shape.shape, mass > 0.f ? mass : 0.001f);
    }

    template <typename T, typename SHAPE>
    inline float base_collider<T, SHAPE>::get_density()
    {
        return _shape.shape.massInfo.m / _shape.shape.massInfo.area;
    }

    template <typename T, typename SHAPE>
    inline void base_collider<T, SHAPE>::set_density(float density)
    {
        cpShapeSetDensity(&_shape.shape, density > 0.f ? density : 0.001f);
    }

    template <typename T, typename SHAPE>
    inline float base_collider<T, SHAPE>::get_moment()
    {
        return _shape.shape.massInfo.m * _shape.shape.massInfo.i;
    }

    template <typename T, typename SHAPE>
    inline float base_collider<T, SHAPE>::get_area()
    {
        return _shape.shape.massInfo.area;
    }

    template <typename T, typename SHAPE>
    inline Vec2f base_collider<T, SHAPE>::get_center_of_gravity()
    {
        return Vec2f(_shape.shape.massInfo.cog);
    }

    template <typename T, typename SHAPE>
    inline Rectf base_collider<T, SHAPE>::get_bb() const
    {
        return Rectf(_shape.shape.bb.l, _shape.shape.bb.t, _shape.shape.bb.r, _shape.shape.bb.b);
    }

    template <typename T, typename SHAPE>
    inline bool base_collider<T, SHAPE>::get_sensor() const
    {
        return _shape.shape.sensor;
    }

    template <typename T, typename SHAPE>
    inline void base_collider<T, SHAPE>::set_sensor(bool sensor)
    {
        cpShapeSetSensor(&_shape.shape, sensor);
    }

    template <typename T, typename SHAPE>
    inline float base_collider<T, SHAPE>::get_elasticity() const
    {
        return _shape.shape.e;
    }

    template <typename T, typename SHAPE>
    inline void base_collider<T, SHAPE>::set_elasticity(float elasticity)
    {
        cpShapeSetElasticity(&_shape.shape, elasticity);
    }

    template <typename T, typename SHAPE>
    inline float base_collider<T, SHAPE>::get_friction() const
    {
        return _shape.shape.u;
    }

    template <typename T, typename SHAPE>
    inline void base_collider<T, SHAPE>::set_friction(float friction)
    {
        cpShapeSetFriction(&_shape.shape, friction);
    }

    template <typename T, typename SHAPE>
    inline Vec2f base_collider<T, SHAPE>::get_surface_velocity() const
    {
        return Vec2f(_shape.shape.surfaceV);
    }

    template <typename T, typename SHAPE>
    inline void base_collider<T, SHAPE>::set_surface_velocity(Vec2f surfaceVelocity)
    {
        cpShapeSetSurfaceVelocity(&_shape.shape, {surfaceVelocity.x, surfaceVelocity.y});
    }

    template <typename T, typename SHAPE>
    inline void* base_collider<T, SHAPE>::get_user_data() const
    {
        return _shape.shape.userData;
    }

    template <typename T, typename SHAPE>
    inline void base_collider<T, SHAPE>::set_user_data(void* userData)
    {
        cpShapeSetUserData(&_shape.shape, userData);
    }

    template <typename T, typename SHAPE>
    inline uintptr_t base_collider<T, SHAPE>::get_collision_type() const
    {
        return _shape.shape.type;
    }

    template <typename T, typename SHAPE>
    inline void base_collider<T, SHAPE>::set_collision_type(uintptr_t collisionType)
    {
        cpShapeSetCollisionType(&_shape.shape, collisionType);
    }

    template <typename T, typename SHAPE>
    inline collider_filter base_collider<T, SHAPE>::get_filter() const
    {
        return reinterpret_cast<const collider_filter&>(_shape.shape.filter);
    }

    template <typename T, typename SHAPE>
    inline void base_collider<T, SHAPE>::set_filter(collider_filter filter)
    {
        cpShapeSetFilter(&_shape.shape, reinterpret_cast<const cpShapeFilter&>(filter));
    }

    template <typename T, typename SHAPE>
    inline void base_collider<T, SHAPE>::on_edit(entity& ent)
    {
        cpShape& shape = _shape.shape;
       
        if (ImGui::InputFloat("Mass", &shape.massInfo.m, 0, 0, "%.5f"))
            set_mass(shape.massInfo.m);

        float density = get_density();
        if (ImGui::InputFloat("Density", &density, 0, 0, "%.5f"))
            set_density(density);

        if (ImGui::InputFloat("Elasticity", &_shape.shape.e, 0, 0, "%.5f"))
            set_elasticity(_shape.shape.e);

        if (ImGui::InputFloat("Friction", &shape.u, 0, 0, "%.5f"))
            set_friction(shape.u);

        if (ImGui::InputFloat2("Surface velocity", &_shape.shape.surfaceV.x, "%.5f"))
            cpShapeSetSurfaceVelocity(&_shape.shape, _shape.shape.surfaceV);
    }





} // namespace box