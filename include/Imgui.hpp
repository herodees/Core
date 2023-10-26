#pragma once

namespace box
{
	class imgui
    {
    public:
        virtual ~imgui() = default;

        virtual bool is_active() const = 0;
        virtual void on_frame_begin() = 0;
        virtual void on_frame_end() = 0;
    };
}