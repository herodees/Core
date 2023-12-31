#pragma once

namespace box
{
    class physics : public system
    {
    public:
        static constexpr component_type_info type_info = {"physics", "Physics"};

        physics()          = default;
        virtual ~physics() = default;

        /// Gravity to pass to rigid bodies when integrating velocity.
        virtual void  set_gravity(Vec2f gravity) = 0;
        virtual Vec2f get_gravity() const        = 0;

        /// Damping rate expressed as the fraction of velocity bodies retain each second.
        /// A value of 0.9 would mean that each body's velocity will drop 10% per second.
        /// The default value is 1.0, meaning no damping is applied.
        virtual void  set_damping(float damping) = 0;
        virtual float get_damping() const        = 0;

        /// Number of iterations to use in the impulse solver to solve contacts and other constraints.
        virtual uint32_t get_iterations() const              = 0;
        virtual void     set_iterations(uint32_t iterations) = 0;

        /// Speed threshold for a body to be considered idle.
        /// The default value of 0 means to let the space guess a good threshold based on gravity.
        virtual float get_idle_speed_treshold() const                   = 0;
        virtual void  set_idle_speed_treshold(float idleSpeedThreshold) = 0;

        /// Time a group of bodies must remain idle in order to fall asleep.
        /// Enabling sleeping also implicitly enables the the contact graph.
        /// The default value of INFINITY disables the sleeping algorithm.
        virtual float get_sleep_time_treshold() const                   = 0;
        virtual void  set_sleep_time_treshold(float sleepTimeThreshold) = 0;

        /// Amount of encouraged penetration between colliding shapes.
        /// Used to reduce oscillating contacts and keep the collision cache warm.
        /// Defaults to 0.1. If you have poor simulation quality,
        /// increase this number as much as possible without allowing visible amounts of overlap.
        virtual float get_collision_slop() const              = 0;
        virtual void  set_collision_slop(float collisionSlop) = 0;

        /// Determines how fast overlapping shapes are pushed apart.
        /// Expressed as a fraction of the error remaining after each second.
        /// Defaults to pow(1.0 - 0.1, 60.0) meaning that Chipmunk fixes 10% of overlap each frame at 60Hz.
        virtual float get_collision_bias() const              = 0;
        virtual void  set_collision_bias(float collisionBias) = 0;

        /// Number of frames that contact information should persist.
        /// Defaults to 3. There is probably never a reason to change this value.
        virtual uint32_t get_collision_persistance() const                        = 0;
        virtual void     set_collision_persistance(uint32_t collisionPersistence) = 0;

        virtual void add_body(rigid_body_component* body) = 0;
        virtual void add_collider(collider_component* collider) = 0;
    };

    enum class body_type
    {
        DYNAMIC,
        KINEMATIC,
        STATIC
    };

    struct transform_component : component
    {
        static constexpr component_type_info type_info = {"transform", "Transform"};

        virtual ~transform_component() = default;

        virtual Vec2f get_position() const  = 0;
        virtual void  set_position(Vec2f v) = 0;
        virtual float get_rotation() const  = 0;
        virtual void  set_rotation(float v) = 0;
        virtual Vec2f get_scale() const     = 0;
        virtual void  set_scale(Vec2f v)    = 0;
    };


    struct rigid_body_component : component
    {
        static constexpr component_type_info type_info = {"rigidbody", "Rigidbody"};

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

    struct collider_filter
    {
        /// Two objects with the same non-zero group value do not collide.
        /// This is generally used to group objects in a composite object together to disable self collisions.
        uintptr_t group;
        /// A bitmask of user definable categories that this object belongs to.
        /// The category/mask combinations of both objects in a collision must agree for a collision to occur.
        uint32_t categories;
        /// A bitmask of user definable category types that this object object collides with.
        /// The category/mask combinations of both objects in a collision must agree for a collision to occur.
        uint32_t mask;
    };

    struct collider_component : component
    {
        virtual ~collider_component() = default;
        /// Set the cpBody this shape is connected to.
        /// Can only be used if the shape is not currently added to a space.
        virtual void set_body(rigid_body_component* body) = 0;

        /// Get the mass of the shape if you are having Chipmunk calculate mass properties for you.
        virtual float get_mass() = 0;
        /// Set the mass of this shape to have Chipmunk calculate mass properties for you.
        virtual void set_mass(float mass) = 0;

        /// Get the density of the shape if you are having Chipmunk calculate mass properties for you.
        virtual float get_density() = 0;
        /// Set the density  of this shape to have Chipmunk calculate mass properties for you.
        virtual void set_density(float density) = 0;

        /// Get the calculated moment of inertia for this shape.
        virtual float get_moment() = 0;
        /// Get the calculated area of this shape.
        virtual float get_area() = 0;
        /// Get the centroid of this shape.
        virtual Vec2f get_center_of_gravity() = 0;

        /// Get the bounding box that contains the shape given it's current position and angle.
        virtual Rectf get_bb() const = 0;

        /// Get if the shape is set to be a sensor or not.
        virtual bool get_sensor() const = 0;
        /// Set if the shape is a sensor or not.
        virtual void set_sensor(bool sensor) = 0;

        /// Get the elasticity of this shape.
        virtual float get_elasticity() const = 0;
        /// Set the elasticity of this shape.
        virtual void set_elasticity(float elasticity) = 0;

        /// Get the friction of this shape.
        virtual float get_friction() const = 0;
        /// Set the friction of this shape.
        virtual void set_friction(float friction) = 0;

        /// Get the surface velocity of this shape.
        virtual Vec2f get_surface_velocity() const = 0;
        /// Set the surface velocity of this shape.
        virtual void set_surface_velocity(Vec2f surfaceVelocity) = 0;

        /// Set the collision type of this shape.
        virtual uintptr_t get_collision_type() const = 0;
        /// Get the collision type of this shape.
        virtual void set_collision_type(uintptr_t collisionType) = 0;

        /// Get the collision filtering parameters of this shape.
        virtual collider_filter get_filter() const = 0;
        /// Set the collision filtering parameters of this shape.
        virtual void set_filter(collider_filter filter) = 0;
    };

    struct circle_collider_component : collider_component
    {
        static constexpr component_type_info type_info = {"circle_collider", "Circle collider"};

        virtual void setup(float radius, Vec2f offset) = 0;
    };

    struct segment_collider_component : collider_component
    {
        static constexpr component_type_info type_info = {"segment_collider", "Line segment collider"};

        virtual void setup(Vec2f a, Vec2f b, float r) = 0;
    };

    struct polygon_collider_component : collider_component
    {
        static constexpr component_type_info type_info = {"polygon_collider", "Polygon collider"};

        virtual void setup(const Vec2f* verts, size_t count, float radius) = 0;
    };
} // namespace box