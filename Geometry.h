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

struct POS_TEX_NORM_COL_VERTEX
{
	XMFLOAT3 Pos;	
	XMFLOAT2 Texture0;
	XMFLOAT3 Normal;
	XMFLOAT4 Col;
};

// Define model vertex structure
struct MODEL_POS_TEX_NORM_VERTEX
{
	XMFLOAT3 Pos;
	XMFLOAT2 TexCoord;
	XMFLOAT3 Normal;
};

struct Plane
{
	POS_TEX_NORM_COL_VERTEX vertices[];
};

struct Polygon {

};

struct TexturedTriangle {
	POS_TEX_NORM_COL_VERTEX vertices[3];
};

struct TexturedQuad {
	POS_TEX_NORM_COL_VERTEX vertices[6];
};

struct TexturedCube {
	TexturedQuad quads[6];
};

static class Geometry
{
public:
	Geometry();
	~Geometry();



	
	void create_tri(POS_TEX_NORM_COL_VERTEX one[], POS_TEX_NORM_COL_VERTEX two[], POS_TEX_NORM_COL_VERTEX three[]);
	TexturedQuad create_quad(POS_TEX_NORM_COL_VERTEX one[], POS_TEX_NORM_COL_VERTEX two[], POS_TEX_NORM_COL_VERTEX three[], POS_TEX_NORM_COL_VERTEX four[]);

	void generate_plane(Plane plane);

	void pyramid();
	static void create_cube(float scale, TexturedCube* cube);
	void cone();
	void cylinder();
	void sphere();
};

