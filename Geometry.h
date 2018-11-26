#pragma once
#include <d3d11.h>
#include <D3DX11.h>
#include <xnamath.h>
#include "Transform.h"



struct POS_COL_VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT4 Col;
};

struct POS_COL_TEX_VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT4 Col;
	XMFLOAT2 Texture0;
};

struct POS_COL_TEX_NORM_VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT4 Col;
	XMFLOAT2 Texture0;
	XMFLOAT3 Normal;
};

struct Plane
{
	POS_COL_TEX_NORM_VERTEX vertices[];
};

struct Polygon {

};

struct TexturedTriangle {
	POS_COL_TEX_NORM_VERTEX vertices[3];
};

struct TexturedQuad {
	POS_COL_TEX_NORM_VERTEX vertices[6];
};

struct TexturedCube {
	TexturedQuad quads[6];
};

static class Geometry
{
public:
	Geometry();
	~Geometry();



	
	void create_tri(POS_COL_TEX_NORM_VERTEX one[], POS_COL_TEX_NORM_VERTEX two[], POS_COL_TEX_NORM_VERTEX three[]);
	TexturedQuad create_quad(POS_COL_TEX_NORM_VERTEX one[], POS_COL_TEX_NORM_VERTEX two[], POS_COL_TEX_NORM_VERTEX three[], POS_COL_TEX_NORM_VERTEX four[]);

	void generate_plane(Plane plane);

	void pyramid();
	static void create_cube(float scale, TexturedCube* cube);
	void cone();
	void cylinder();
	void sphere();
};

