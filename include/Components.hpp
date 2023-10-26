#pragma once

namespace box
{
    struct component
    {
        virtual ~component() = default;
        virtual void on_edit(entity& ent) {};

        template <typename T>
        T* as()
        {
            return static_cast<T*>(this);
        }

        template <typename T>
        const T* as() const
        {
            return static_cast<const T*>(this);
        }
    };

    struct component_type_info
    {
        std::string_view id;
        std::string_view name;
    };

} // namespace box