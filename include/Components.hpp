#pragma once

namespace box
{
    struct component
    {
        virtual ~component() = default;
    };

    struct component_type_info
    {
        std::string_view id;
        std::string_view name;
    };

} // namespace box