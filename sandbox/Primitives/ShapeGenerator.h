#ifndef SHAPE_GENERATOR_H
#define SHAPE_GENERATOR_H

#include <Primitives/ShapeData.h>

class ShapeGenerator
{
public:
	static ShapeData makeTriangle();
	static ShapeData makeCube();
	ShapeGenerator();
	~ShapeGenerator();
};

#endif