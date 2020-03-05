#ifndef SHAPE_GENERATOR_H
#define SHAPE_GENERATOR_H

#include <Primitives/ShapeData.h>

class ShapeGenerator
{
private:
	static void getPlaneVertices(int dimension, GLuint& Num, Vertex*& ver);
	static void getPlaneIndices(int dimension, GLuint& Num, GLushort*& ind);
public:
	static ShapeData makeTriangle();
	static ShapeData makeCube();
	static ShapeData makePlane(int dimension);
	ShapeGenerator();
	~ShapeGenerator();
};

#endif