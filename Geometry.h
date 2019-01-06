#pragma once
#include "Transform.h"
// vertex format with position, texcoord
struct POS_TEX_VERTEX
{
	XMFLOAT3 pos;	
	XMFLOAT2 texcoord;
};
// vertex format with position, color
struct POS_COL_VERTEX
{
	XMFLOAT3 pos;
	XMFLOAT4 col;
};
// vertex format with position, texcoord, normal
struct POS_TEX_NORM_VERTEX
{
	XMFLOAT3 pos;
	XMFLOAT2 texcoord;
	XMFLOAT3 normal;
};
// vertex format with position, color, texcoord
struct POS_COL_TEX_VERTEX
{
	XMFLOAT3 pos;
	XMFLOAT4 col;
	XMFLOAT2 texcoord;
};
// vertex format with position, texcoord, normal, color
struct POS_TEX_NORM_COL_VERTEX
{
	XMFLOAT3 pos;	
	XMFLOAT2 texcoord;
	XMFLOAT3 normal;
	XMFLOAT4 col;
};



// structure representing a plane used for collision detection
struct Plane
{
	XMVECTOR normal; // normal of a plane
	float offset; // offset of the normal
};

class Geometry
{
public:
	Geometry();
	~Geometry();

	static POS_TEX_VERTEX* create_skybox(float scale = 1.0f); // create a textured skybox with a scale and return vertex structure pointer
	static POS_COL_VERTEX* cube_pc(float scale = 1.0f); // create a cube with position, color, scale and return vertex structure pointer
	static POS_TEX_NORM_VERTEX* cube_ptn(float scale = 1.0f); // create a cube with position, texture, normal, scale and return vertex structure pointer	
	static POS_COL_TEX_VERTEX* cube_ptc(float scale = 1.0f); // create a cube with position, texture, color, scale and return vertex structure pointer
	static POS_TEX_NORM_COL_VERTEX* cube_ptnc(float scale = 1.0f); // create a cube with position, texture, normal, color, scale and return vertex structure pointer
	static void plane_ittn(POS_TEX_NORM_COL_VERTEX **plane, unsigned int **indices, UINT tiles, float scale = 1.0f); // create an indexed, tiled, texture, normal, scaled plane

};

