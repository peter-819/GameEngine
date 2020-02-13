#ifndef ENGINE_MATH_MATRIX2D_H
#define ENGINE_MATH_MATRIX2D_H

#include "Matrix2D.h"
using Math::Vector2D;

namespace Math {

	struct Matrix2D {
		float mat[4];
		explicit Matrix2D(
			const float& a = 1.0f,
			const float& b = 0.0f,
			const float& c = 0.0f,
			const float& d = 1.0f);
		Matrix2D(const Matrix2D& a);
	};
	inline Vector2D operator * (const Vector2D& a, const Matrix2D& b);

	#include "Matrix2D.inl"

}

#endif