#ifndef ENGINE_MATH_VECTOR2D_H
#define ENGINE_MATH_VECTOR2D_H

namespace Math {
	struct Vector2D {
		float x;
		float y;
		explicit Vector2D(const float& x = 0.0f, const float& y = 0.0f) :x(x), y(y) {}
	};
	inline Vector2D operator + (const Vector2D& a, const Vector2D& b);
	inline Vector2D operator * (const Vector2D& a, const float& b);
	inline Vector2D operator * (const float& a, const Vector2D& b);

	#include "Vector2D.inl"
}

#endif