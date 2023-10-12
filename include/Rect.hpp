#pragma once

#include "Vec.hpp"

namespace box
{
    template <typename T>
    class rect
    {
    public:
        rect()               = default;
        rect(const rect<T>&) = default;
        rect(const T& x1, const T& y1, const T& x2, const T& y2) : min(x1, y1), max(x2, y2){};
        rect(const vec2<T>& a, const vec2<T>& b) : min(a), max(b){};
        rect&   operator=(const rect<T>&) = default;
        void    expand(const T& value);
        void    expand(const vec2<T>& value);
        bool    contains(const T& x, const T& y) const;
        bool    contains(const vec2<T>& v) const;
        bool    operator==(const rect<T>& o) const;
        bool    operator!=(const rect<T>& o) const;
        vec2<T> size() const;
        T       width() const;
        T       height() const;

        vec2<T> min;
        vec2<T> max;
    };

    using Rectf = rect<float>;
    using Recti = rect<int32_t>;
    using Rectu = rect<uint32_t>;

    template <typename T>
    inline void rect<T>::expand(const T& value)
    {
        min.x -= value;
        min.y -= value;
        max.x += value;
        max.y += value;
    }

    template <typename T>
    inline void rect<T>::expand(const vec2<T>& value)
    {
        min -= value;
        max += value;
    }

    template <typename T>
    inline bool rect<T>::contains(const T& x, const T& y) const
    {
        return contains(vec2<T>(x, y));
    }

    template <typename T>
    inline bool rect<T>::contains(const vec2<T>& v) const
    {
        return (v.x >= min.x) && (v.x < max.x) && (v.y >= min.y) && (v.y < max.y);
    }

    template <typename T>
    inline bool rect<T>::operator==(const rect<T>& o) const
    {
        return o.min.x == min.x && o.min.y == min.y && o.max.x == max.x && o.max.y == max.y;
    }

    template <typename T>
    inline bool rect<T>::operator!=(const rect<T>& o) const
    {
        return !operator==(o);
    }

    template <typename T>
    inline vec2<T> rect<T>::size() const
    {
        return max - min;
    }

    template <typename T>
    inline T rect<T>::width() const
    {
        return max.x - min.x;
    }

    template <typename T>
    inline T rect<T>::height() const
    {
        return max.y - min.y;
    }
} // namespace box