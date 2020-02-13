#include <gtest/gtest.h>
#include <Math/Vector2D.h>

using Math::Vector2D;

TEST(MathTest, Vector2DAdditionTest) {
	Vector2D a(1, 1), b(2, 2);
	Vector2D c = a + b, d = a * 3.0f, e = 2.0f * a;
	EXPECT_EQ(c.x, 3.0f);
	EXPECT_EQ(c.y, 3.0f);
}

TEST(MathTest, Vector2DMutiplicationScale) {
	Vector2D a(1, 1), b(2, 2);
	Vector2D c = a + b, d = a * 3.0f, e = 2.0f * a;
	EXPECT_EQ(d.x, 3.0f);
	EXPECT_EQ(d.y, 3.0f);
}

TEST(MathTest, Vector2DScaleMutiplication) {
	Vector2D a(1, 1), b(2, 2);
	Vector2D c = a + b, d = a * 3.0f, e = 2.0f * a;
	EXPECT_EQ(e.x, 2.0f);
	EXPECT_EQ(e.y, 2.0f);
}