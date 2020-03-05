#ifndef SHAPE_DATA_H
#define SHAPE_DATA_H

#include <GL/glew.h>
#include <Primitives/vertex.h>

struct ShapeData {
	Vertex* Vertices;
	GLuint NumVertices;
	GLushort* Indices;
	GLuint NumIndices;
	
	GLsizeiptr vertexBufferSize() const {
		return NumVertices * sizeof(Vertex);
	}

	GLsizeiptr indexBufferSize() const {
		return NumIndices * sizeof(GLushort);
	}
	ShapeData():
		Vertices(0),
		NumVertices(0),
		Indices(0),
		NumIndices(0){}

	void cleanup() {
		delete[] Vertices;
		delete[] Indices;
		NumVertices = 0;
		NumIndices = 0;
	}
};
#endif