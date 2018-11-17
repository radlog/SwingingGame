#pragma once
#include <d3d11.h>
#include <D3DX11.h>
#include <xnamath.h>
#include "Transform.h"

struct Vertex {
	XMFLOAT3 pos;
	XMFLOAT4 col;
};

struct Polygon {

};

struct Triangle {
	Vertex vertices[3];
};

struct Quad {
	Vertex vertices[6];
};

struct Cube {
	Quad quads[6];
};

static class Geometry
{
public:
	Geometry();
	~Geometry();



	
	void create_tri(Vertex one[], Vertex two[], Vertex three[]);	
	Quad create_quad(Vertex one[], Vertex two[], Vertex three[], Vertex four[]);

	void pyramid();
	void create_cube(float scale, Cube* cube);
	void cone();
	void cylinder();
	void sphere();
};

