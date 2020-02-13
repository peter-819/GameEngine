#define r0c0 mat[0]
#define r0c1 mat[1]
#define r1c0 mat[2]
#define r1c1 mat[3]

Matrix2D::Matrix2D(
	const float& a,const float& b,
	const float& c,const float& d
):r0c0(a),r0c1(b),r1c0(c),r1c1(d){}

Vector2D operator * (const Vector2D& a, const Matrix2D& b) {
	Vector2D res;
	res.x = a.x * r0c0 + a.y * r1c0;
	res.y = a.x * r0c1 + a.y * r1c1;
	return res;
}