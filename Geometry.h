#pragma once
#include "Transform.h"

struct POS_TEX_VERTEX
{
	XMFLOAT3 pos;	
	XMFLOAT2 texture0;
};

struct POS_COL_VERTEX
{
	XMFLOAT3 pos;
	XMFLOAT4 col;
};

struct POS_COL_TEX_VERTEX
{
	XMFLOAT3 pos;
	XMFLOAT4 col;
	XMFLOAT2 texture0;
};

struct POS_TEX_NORM_COL_VERTEX
{
	XMFLOAT3 pos;	
	XMFLOAT2 texture0;
	XMFLOAT3 normal;
	XMFLOAT4 col;
};

// Define model vertex structure
struct MODEL_POS_TEX_NORM_VERTEX
{
	XMFLOAT3 pos;
	XMFLOAT2 texcoord;
	XMFLOAT3 normal;
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

	const float scale = 100.0f;

	static void create_textured_normal_cube(POS_TEX_NORM_COL_VERTEX* cube[], float scale);
	static POS_TEX_VERTEX* create_textured_skybox(float scale);
	static POS_COL_TEX_VERTEX* create_pos_col_tex_cube(float scale);
	static POS_TEX_NORM_COL_VERTEX* pos_tex_norm_col_cube(float scale);
	static void create_indexed_tiled_textured_normal_plane(POS_TEX_NORM_COL_VERTEX **plane, unsigned int **indices, UINT tiles, float scale);
	static POS_COL_VERTEX* create_color_cube(float scale);
	

};

