#pragma once
#include "Transform.h"

struct POS_TEX_VERTEX
{
	XMFLOAT3 Pos;	
	XMFLOAT2 Texture0;
};

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

