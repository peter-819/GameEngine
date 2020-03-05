#include "ShapeGenerator.h"
#include <glm.hpp>
#include <Primitives/vertex.h>

#define NUM_ELEMENTS_OF(x) sizeof(x)/sizeof(*x)

struct Vertex;
struct ShapeData;
using glm::vec3;
ShapeData ShapeGenerator::makeTriangle() {
	Vertex verts[] = {
		glm::vec3(-1.0f, -1.0f, +0.0f),
		glm::vec3(+1.0f, +0.0f, +0.0f),

		glm::vec3(+0.0f, +1.0f, +0.0f),
		glm::vec3(+0.0f, +1.0f, +0.0f),

		glm::vec3(+1.0f, -1.0f, +0.0f),
		glm::vec3(+0.0f, +0.0f, +1.0f),
	};
	GLushort ind[] = { 0,1,2 };
	
	ShapeData Tri;
	
	Tri.NumVertices = NUM_ELEMENTS_OF(verts);
	Tri.Vertices = new Vertex[Tri.NumVertices];
	memcpy(Tri.Vertices, verts, sizeof(verts));

	Tri.NumIndices = NUM_ELEMENTS_OF(ind);
	Tri.Indices = new GLushort[Tri.NumIndices];
	memcpy(Tri.Indices, ind, sizeof(ind));
	
	return Tri;
}

ShapeData ShapeGenerator::makeCube() {
	Vertex verts[] = {
		vec3(-1.0f, +1.0f, +1.0f), //1
		vec3(+1.0f, +0.0f, +0.0f), //
		vec3(+1.0f, +1.0f, +1.0f), //2
		vec3(+0.0f, +1.0f, +0.0f), //
		vec3(+1.0f, +1.0f, -1.0f), //3
		vec3(+0.0f, +0.0f, +1.0f), //
		vec3(-1.0f, +1.0f, -1.0f), //4
		vec3(+1.0f, +1.0f, +1.0f), //
								   //
		vec3(-1.0f, +1.0f, -1.0f), //5
		vec3(+1.0f, +0.0f, +1.0f), //
		vec3(+1.0f, +1.0f, -1.0f), //6
		vec3(+0.0f, +0.5f, +0.2f), //
		vec3(+1.0f, -1.0f, -1.0f), //7
		vec3(+0.8f, +0.6f, +0.4f), //
		vec3(-1.0f, -1.0f, -1.0f), //8
		vec3(+0.3f, +1.0f, +0.5f), //
								   //
		vec3(+1.0f, +1.0f, -1.0f), //9
		vec3(+0.2f, +0.5f, +0.2f), //
		vec3(+1.0f, +1.0f, +1.0f), //10
		vec3(+0.9f, +0.3f, +0.7f), //
		vec3(+1.0f, -1.0f, +1.0f), //11
		vec3(+0.3f, +0.7f, +0.5f), //
		vec3(+1.0f, -1.0f, -1.0f), //12
		vec3(+0.5f, +0.7f, +0.5f), //
								   //
		vec3(-1.0f, +1.0f, +1.0f), //13
		vec3(+0.7f, +0.8f, +0.2f), //
		vec3(-1.0f, +1.0f, -1.0f), //14
		vec3(+0.5f, +0.7f, +0.3f), //
		vec3(-1.0f, -1.0f, -1.0f), //15
		vec3(+0.4f, +0.7f, +0.7f), //
		vec3(-1.0f, -1.0f, +1.0f), //16
		vec3(+0.2f, +0.5f, +1.0f), //
								   //
		vec3(+1.0f, +1.0f, +1.0f), //17
		vec3(+0.6f, +1.0f, +0.7f), //
		vec3(-1.0f, +1.0f, +1.0f), //18
		vec3(+0.6f, +0.4f, +0.8f), //
		vec3(-1.0f, -1.0f, +1.0f), //19
		vec3(+0.2f, +0.8f, +0.7f), //
		vec3(+1.0f, -1.0f, +1.0f), //20
		vec3(+0.2f, +0.7f, +1.0f), //
								   //
		vec3(+1.0f, -1.0f, -1.0f), //21
		vec3(+0.8f, +0.3f, +0.7f), //
		vec3(-1.0f, -1.0f, -1.0f), //22
		vec3(+0.8f, +0.9f, +0.5f), //
		vec3(-1.0f, -1.0f, +1.0f), //23
		vec3(+0.5f, +0.8f, +0.5f), //
		vec3(+1.0f, -1.0f, +1.0f), //24
		vec3(+0.9f, +1.0f, +0.2f), //
	};
	GLushort ind[] = {
		0  ,1  ,2,  0,  2,  3,
		4  ,5  ,6,  4,  6,  7,
		8  ,9  ,10, 8,  10, 11,
		12 ,13 ,14, 12, 14, 15,
		16 ,17 ,18, 16, 18, 19,
		20 ,22 ,21, 20, 23, 22,
	};
	ShapeData Shape;

	Shape.NumVertices = NUM_ELEMENTS_OF(verts);
	Shape.Vertices = new Vertex[Shape.NumVertices];
	memcpy(Shape.Vertices, verts, sizeof(verts));

	Shape.NumIndices = NUM_ELEMENTS_OF(ind);
	Shape.Indices = new GLushort[Shape.NumIndices];
	memcpy(Shape.Indices, ind, sizeof(ind));

	return Shape;
}

ShapeGenerator::ShapeGenerator(){

}


ShapeGenerator::~ShapeGenerator(){

}
