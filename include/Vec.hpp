#pragma once

namespace box
{
	template <typename T>
	class Vec2
	{
	public:
		Vec2() = default;
		~Vec2() = default;
		Vec2(const Vec2&) = default;
		Vec2(const T& X, const T& Y) : x(X), y(Y) {};
		Vec2& operator=(const Vec2&) = default;
		Vec2& operator+=(const T& value);
		Vec2& operator+=(const Vec2& other);
		Vec2& operator-=(const T& value);
		Vec2& operator-=(const Vec2& other);
		Vec2& operator/=(const T& value);
		Vec2& operator/=(const Vec2& other);
		Vec2& operator*=(const T& value);
		Vec2& operator*=(const Vec2& other);
		Vec2 operator-() const;
		Vec2 operator+(const T& value) const;
		Vec2 operator+(const Vec2& other) const;
		Vec2 operator-(const T& value) const;
		Vec2 operator-(const Vec2& other) const;
		Vec2 operator/(const T& value) const;
		Vec2 operator/(const Vec2& other) const;
		Vec2 operator*(const T& value) const;
		Vec2 operator*(const Vec2& other) const;
		float get_angle() const;
		float get_angle_deg() const;
		T& operator[](size_t n);
		const T& operator[](size_t n) const;

		T x{};
		T y{};
	};

	using Vec2f = Vec2<float>;
	using Vec2i = Vec2<int32_t>;
	using Vec2u = Vec2<uint32_t>;

	template <typename T>
	constexpr bool operator==(const Vec2<T>& a, const Vec2<T>& b)
	{
		return a.x == b.x && a.y == b.y;
	}

	template <typename T>
	constexpr bool operator!=(const Vec2<T>& a, const Vec2<T>& b)
	{
		return !(a == b);
	}

	template<typename T>
	inline Vec2<T>& Vec2<T>::operator/=(const T& value)
	{
		x /= value;
		y /= value;
		return *this;
	}

	template<typename T>
	inline Vec2<T>& Vec2<T>::operator/=(const Vec2<T>& other)
	{
		x /= other.x;
		y /= other.y;
		return *this;
	}

	template<typename T>
	inline Vec2<T>& Vec2<T>::operator*=(const T& value)
	{
		x *= value;
		y *= value;
		return *this;
	}

	template<typename T>
	inline Vec2<T>& Vec2<T>::operator*=(const Vec2<T>& other)
	{
		x *= other.x;
		y *= other.y;
		return *this;
	}

	template<typename T>
	inline Vec2<T> Vec2<T>::operator-() const
	{
		return Vec2(-x, -y);
	}

	template<typename T>
	inline Vec2<T> Vec2<T>::operator+(const Vec2& other) const
	{
		return Vec2(x + other.x, y + other.y);
	}

	template<typename T>
	inline Vec2<T> Vec2<T>::operator-(const Vec2& other) const
	{
		return Vec2(x - other.x, y - other.y);
	}

	template<typename T>
	inline Vec2<T> Vec2<T>::operator/(const T& value) const
	{
		return BasicPoint(x / value, y / value);
	}

	template<typename T>
	inline Vec2<T> Vec2<T>::operator*(const T& value) const
	{
		return BasicPoint(x * value, y * value);
	}

	template<typename T>
	inline Vec2<T> Vec2<T>::operator+(const T& value) const
	{
		return BasicPoint(x + value, y + value);
	}

	template<typename T>
	inline Vec2<T> Vec2<T>::operator-(const T& value) const
	{
		return BasicPoint(x - value, y - value);
	}

	template<typename T>
	inline Vec2<T> Vec2<T>::operator/(const Vec2& other) const
	{
		return Vec2(x / other.x, y / other.y);
	}

	template<typename T>
	inline Vec2<T> Vec2<T>::operator*(const Vec2& other) const
	{
		return Vec2(x * other.x, y * other.y);
	}

	template<typename T>
	inline float Vec2<T>::get_angle() const
	{
		return std::atan2(y, x);
	}

	template<typename T>
	inline float Vec2<T>::get_angle_deg() const
	{
		return get_angle() * 57.2957f;
	}

	template<typename T>
	inline Vec2<T>& Vec2<T>::operator+=(const T& value)
	{
		x += value;
		y += value;
		return *this;
	}

	template<typename T>
	inline Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& other)
	{
		x += other.x;
		y += other.y;
		return *this;
	}

	template<typename T>
	inline Vec2<T>& Vec2<T>::operator-=(const T& value)
	{
		x -= value;
		y -= value;
		return *this;
	}

	template<typename T>
	inline Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& other)
	{
		x -= other.x;
		y -= other.y;
		return *this;
	}

	template<typename T>
	inline T& Vec2<T>::operator[](size_t n)
	{
		return reinterpret_cast<T*>(this)[n];
	}

	template<typename T>
	inline const T& Vec2<T>::operator[](size_t n) const
	{
		return reinterpret_cast<const T*>(this)[n];
	}
}