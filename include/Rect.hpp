#pragma once

#include "Vec.hpp"

namespace box
{
	template <typename T>
	class Rect
	{
	public:
		Rect() = default;
		Rect(const Rect<T>&) = default;
		Rect(const T& x1, const T& y1, const T& x2, const T& y2) : min(x1, y1), max(x2, y2) {};
		Rect(const Vec2<T>& a, const Vec2<T>& b) : min(a), max(b) {};
		Rect& operator=(const Rect<T>&) = default;

		void expand(const T& value);
		void expand(const Vec2<T>& value);
		bool contains(const T& x, const T& y) const;
		bool contains(const Vec2<T>& v) const;
		bool operator==(const Rect<T>& o) const;
		bool operator!=(const Rect<T>& o) const;
		Vec2<T> size() const;
		T width() const;
		T height() const;

		Vec2<T> min;
		Vec2<T> max;
	};

	using Rectf = Rect<float>;
	using Recti = Rect<int32_t>;
	using Rectu = Rect<uint32_t>;

	template<typename T>
	inline void Rect<T>::expand(const T& value)
	{
		min.x -= value;
		min.y -= value;
		max.x += value;
		max.y += value;
	}

	template<typename T>
	inline void Rect<T>::expand(const Vec2<T>& value)
	{
		min -= value;
		max += value;
	}

	template<typename T>
	inline bool Rect<T>::contains(const T& x, const T& y) const
	{
		return contains(Vec2<T>(x,y));
	}

	template<typename T>
	inline bool Rect<T>::contains(const Vec2<T>& v) const
	{
		return (v.x >= min.x) && (v.x < max.x) && (v.y >= min.y) && (v.y < max.y);
	}

	template<typename T>
	inline bool Rect<T>::operator==(const Rect<T>& o) const
	{
		return o.min.x == min.x && o.min.y == min.y && o.max.x == max.x && o.max.y == max.y;
	}

	template<typename T>
	inline bool Rect<T>::operator!=(const Rect<T>& o) const
	{
		return !operator==(o);
	}

	template<typename T>
	inline Vec2<T> Rect<T>::size() const
	{
		return max - min;
	}

	template<typename T>
	inline T Rect<T>::width() const
	{
		return max.x - min.x;
	}

	template<typename T>
	inline T Rect<T>::height() const
	{
		return max.y - min.y;
	}
}