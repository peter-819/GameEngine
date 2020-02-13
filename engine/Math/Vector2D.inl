Vector2D operator + (const Vector2D& a, const Vector2D& b) {
	return Vector2D(a.x + b.x, a.y + b.y);
}
Vector2D operator * (const Vector2D & a, const float& b) {
	return Vector2D(a.x * b, a.y * b);
}
Vector2D operator *(const float& a, const Vector2D & b) {
	return Vector2D(a * b.x, a * b.y);
}