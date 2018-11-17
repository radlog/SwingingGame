#include "Geometry.h"



Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}

void Geometry::create_cube(float scale, Cube* cube)
{
	// back face 
	cube->quads[0].vertices[0] = { XMFLOAT3(-scale, scale, scale), XMFLOAT4(1.0f,0.0f,0.0f,1.0f) };
	cube->quads[0].vertices[1] = { XMFLOAT3(-scale, -scale, scale), XMFLOAT4(1.0f,0.0f,0.0f,1.0f) };
	cube->quads[0].vertices[2] = { XMFLOAT3(scale, scale, scale), XMFLOAT4(1.0f,0.0f,0.0f,1.0f) };
	cube->quads[0].vertices[3] = { XMFLOAT3(scale, scale, scale), XMFLOAT4(1.0f,0.0f,0.0f,1.0f) };
	cube->quads[0].vertices[4] = { XMFLOAT3(-scale, -scale, scale), XMFLOAT4(1.0f,0.0f,0.0f,1.0f) };
	cube->quads[0].vertices[5] = { XMFLOAT3(scale, -scale, scale), XMFLOAT4(1.0f,0.0f,0.0f,1.0f) };

	// front face
	cube->quads[1].vertices[0] = { XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(1.0f,0.0f,0.0f,1.0f) };
	cube->quads[1].vertices[1] = { XMFLOAT3(-scale, scale, -scale) , XMFLOAT4(1.0f,0.0f,0.0f,1.0f) };
	cube->quads[1].vertices[2] = { XMFLOAT3(scale, scale, -scale), XMFLOAT4(1.0f,0.0f,0.0f,1.0f) };
	cube->quads[1].vertices[3] = { XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(1.0f,0.0f,0.0f,1.0f) };
	cube->quads[1].vertices[4] = { XMFLOAT3(scale, scale, -scale), XMFLOAT4(1.0f,0.0f,0.0f,1.0f) };
	cube->quads[1].vertices[5] = { XMFLOAT3(scale, -scale, -scale), XMFLOAT4(1.0f,0.0f,0.0f,1.0f) };

	// left face
	cube->quads[2].vertices[0] = { XMFLOAT3(-scale, -scale, -scale) , XMFLOAT4(0.0f,1.0f,0.0f,1.0f) };
	cube->quads[2].vertices[1] = { XMFLOAT3(-scale, -scale, scale), XMFLOAT4(0.0f,1.0f,0.0f,1.0f) };
	cube->quads[2].vertices[2] = { XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f,1.0f,0.0f,1.0f) };
	cube->quads[2].vertices[3] = { XMFLOAT3(-scale, -scale, scale), XMFLOAT4(0.0f,1.0f,0.0f,1.0f) };
	cube->quads[2].vertices[4] = { XMFLOAT3(-scale, scale, scale), XMFLOAT4(0.0f,1.0f,0.0f,1.0f) };
	cube->quads[2].vertices[5] = { XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f,1.0f,0.0f,1.0f) };

	// right face			
	cube->quads[3].vertices[0] = { XMFLOAT3(scale, -scale, scale), XMFLOAT4(0.0f,1.0f,0.0f,1.0f) };
	cube->quads[3].vertices[1] = { XMFLOAT3(scale, -scale, -scale) , XMFLOAT4(0.0f,1.0f,0.0f,1.0f) };
	cube->quads[3].vertices[2] = { XMFLOAT3(scale, scale, -scale), XMFLOAT4(0.0f,1.0f,0.0f,1.0f) };
	cube->quads[3].vertices[3] = { XMFLOAT3(scale, scale, scale), XMFLOAT4(0.0f,1.0f,0.0f,1.0f) };
	cube->quads[3].vertices[4] = { XMFLOAT3(scale, -scale, scale), XMFLOAT4(0.0f,1.0f,0.0f,1.0f) };
	cube->quads[3].vertices[5] = { XMFLOAT3(scale, scale, -scale), XMFLOAT4(0.0f,1.0f,0.0f,1.0f) };

	// bottom face			
	cube->quads[4].vertices[0] = { XMFLOAT3(scale, -scale, -scale), XMFLOAT4(0.0f,0.0f,1.0f,1.0f) };
	cube->quads[4].vertices[1] = { XMFLOAT3(scale, -scale, scale) , XMFLOAT4(0.0f,0.0f,1.0f,1.0f) };
	cube->quads[4].vertices[2] = { XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(0.0f,0.0f,1.0f,1.0f) };
	cube->quads[4].vertices[3] = { XMFLOAT3(scale, -scale, scale), XMFLOAT4(0.0f,0.0f,1.0f,1.0f) };
	cube->quads[4].vertices[4] = { XMFLOAT3(-scale, -scale, scale), XMFLOAT4(0.0f,0.0f,1.0f,1.0f) };
	cube->quads[4].vertices[5] = { XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(0.0f,0.0f,1.0f,1.0f) };

	// top face			
	cube->quads[5].vertices[0] = { XMFLOAT3(scale, scale, scale), XMFLOAT4(0.0f,0.0f,1.0f,1.0f) };
	cube->quads[5].vertices[1] = { XMFLOAT3(scale, scale, -scale), XMFLOAT4(0.0f,0.0f,1.0f,1.0f) };
	cube->quads[5].vertices[2] = { XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f,0.0f,1.0f,1.0f) };
	cube->quads[5].vertices[3] = { XMFLOAT3(-scale, scale, scale), XMFLOAT4(0.0f,0.0f,1.0f,1.0f) };
	cube->quads[5].vertices[4] = { XMFLOAT3(scale, scale, scale), XMFLOAT4(0.0f,0.0f,1.0f,1.0f) };
	cube->quads[5].vertices[5] = { XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f,0.0f,1.0f,1.0f) };

}
