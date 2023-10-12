#pragma once

namespace box
{
    template <typename T>
    class vec2
    {
    public:
        vec2()            = default;
        ~vec2()           = default;
        vec2(const vec2&) = default;
        vec2(const T& X, const T& Y) : x(X), y(Y){};
        vec2&    operator=(const vec2&) = default;
        vec2&    operator+=(const T& value);
        vec2&    operator+=(const vec2& other);
        vec2&    operator-=(const T& value);
        vec2&    operator-=(const vec2& other);
        vec2&    operator/=(const T& value);
        vec2&    operator/=(const vec2& other);
        vec2&    operator*=(const T& value);
        vec2&    operator*=(const vec2& other);
        vec2     operator-() const;
        vec2     operator+(const T& value) const;
        vec2     operator+(const vec2& other) const;
        vec2     operator-(const T& value) const;
        vec2     operator-(const vec2& other) const;
        vec2     operator/(const T& value) const;
        vec2     operator/(const vec2& other) const;
        vec2     operator*(const T& value) const;
        vec2     operator*(const vec2& other) const;
        float    get_angle() const;
        float    get_angle_deg() const;
        T&       operator[](size_t n);
        const T& operator[](size_t n) const;

        T x{};
        T y{};
    };

    using Vec2f = vec2<float>;
    using Vec2i = vec2<int32_t>;
    using Vec2u = vec2<uint32_t>;

    template <typename T>
    constexpr bool operator==(const vec2<T>& a, const vec2<T>& b)
    {
        return a.x == b.x && a.y == b.y;
    }

    template <typename T>
    constexpr bool operator!=(const vec2<T>& a, const vec2<T>& b)
    {
        return !(a == b);
    }

    template <typename T>
    inline vec2<T>& vec2<T>::operator/=(const T& value)
    {
        x /= value;
        y /= value;
        return *this;
    }

    template <typename T>
    inline vec2<T>& vec2<T>::operator/=(const vec2<T>& other)
    {
        x /= other.x;
        y /= other.y;
        return *this;
    }

    template <typename T>
    inline vec2<T>& vec2<T>::operator*=(const T& value)
    {
        x *= value;
        y *= value;
        return *this;
    }

    template <typename T>
    inline vec2<T>& vec2<T>::operator*=(const vec2<T>& other)
    {
        x *= other.x;
        y *= other.y;
        return *this;
    }

    template <typename T>
    inline vec2<T> vec2<T>::operator-() const
    {
        return vec2(-x, -y);
    }

    template <typename T>
    inline vec2<T> vec2<T>::operator+(const vec2& other) const
    {
        return vec2(x + other.x, y + other.y);
    }

    template <typename T>
    inline vec2<T> vec2<T>::operator-(const vec2& other) const
    {
        return vec2(x - other.x, y - other.y);
    }

    template <typename T>
    inline vec2<T> vec2<T>::operator/(const T& value) const
    {
        return BasicPoint(x / value, y / value);
    }

    template <typename T>
    inline vec2<T> vec2<T>::operator*(const T& value) const
    {
        return BasicPoint(x * value, y * value);
    }

    template <typename T>
    inline vec2<T> vec2<T>::operator+(const T& value) const
    {
        return BasicPoint(x + value, y + value);
    }

    template <typename T>
    inline vec2<T> vec2<T>::operator-(const T& value) const
    {
        return BasicPoint(x - value, y - value);
    }

    template <typename T>
    inline vec2<T> vec2<T>::operator/(const vec2& other) const
    {
        return vec2(x / other.x, y / other.y);
    }

    template <typename T>
    inline vec2<T> vec2<T>::operator*(const vec2& other) const
    {
        return vec2(x * other.x, y * other.y);
    }

    template <typename T>
    inline float vec2<T>::get_angle() const
    {
        return std::atan2(y, x);
    }

    template <typename T>
    inline float vec2<T>::get_angle_deg() const
    {
        return get_angle() * 57.2957f;
    }

    template <typename T>
    inline vec2<T>& vec2<T>::operator+=(const T& value)
    {
        x += value;
        y += value;
        return *this;
    }

    template <typename T>
    inline vec2<T>& vec2<T>::operator+=(const vec2<T>& other)
    {
        x += other.x;
        y += other.y;
        return *this;
    }

    template <typename T>
    inline vec2<T>& vec2<T>::operator-=(const T& value)
    {
        x -= value;
        y -= value;
        return *this;
    }

    template <typename T>
    inline vec2<T>& vec2<T>::operator-=(const vec2<T>& other)
    {
        x -= other.x;
        y -= other.y;
        return *this;
    }

    template <typename T>
    inline T& vec2<T>::operator[](size_t n)
    {
        return reinterpret_cast<T*>(this)[n];
    }

    template <typename T>
    inline const T& vec2<T>::operator[](size_t n) const
    {
        return reinterpret_cast<const T*>(this)[n];
    }
} // namespace box