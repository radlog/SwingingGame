#include "Geometry.h"

const float scale = 1.0f;

Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}

POS_TEX_VERTEX* Geometry::create_skybox(UINT *numverts)
{
	*numverts = cube_num_verts;
	const auto skybox = new POS_TEX_VERTEX[*numverts]
	{
		// back face 
		{XMFLOAT3(-scale, -scale, scale),   XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(-scale, scale, scale), 	XMFLOAT2(0.0f,1.0f)},
		{XMFLOAT3(scale, scale, scale),		XMFLOAT2(1.0f,0.0f)},
		{XMFLOAT3(-scale, -scale, scale),	XMFLOAT2(1.0f,0.0f)},
		{XMFLOAT3(scale, scale, scale),  	XMFLOAT2(0.0f,1.0f)},
		{XMFLOAT3(scale, -scale, scale), 	XMFLOAT2(1.0f,1.0f)},
		// front face
		{XMFLOAT3(-scale, scale, -scale), 	XMFLOAT2(0.0f,1.0f)},
		{XMFLOAT3(-scale, -scale, -scale),	XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(scale, scale, -scale),  	XMFLOAT2(1.0f,0.0f)},
		{XMFLOAT3(scale, scale, -scale),  	XMFLOAT2(0.0f,1.0f)},
		{XMFLOAT3(-scale, -scale, -scale),	XMFLOAT2(1.0f,0.0f)},
		{XMFLOAT3(scale, -scale, -scale), 	XMFLOAT2(1.0f,1.0f)},
		// left face
		{XMFLOAT3(-scale, -scale, scale), 	XMFLOAT2(0.0f,1.0f)},
		{XMFLOAT3(-scale, -scale, -scale),	XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(-scale, scale, -scale), 	XMFLOAT2(1.0f,1.0f)},
		{XMFLOAT3(-scale, scale, scale), 	XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(-scale, -scale, scale), 	XMFLOAT2(1.0f,0.0f)},
		{XMFLOAT3(-scale, scale, -scale), 	XMFLOAT2(1.0f,1.0f)},
		// right face
		{XMFLOAT3(scale, -scale, -scale), 	XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(scale, -scale, scale),  	XMFLOAT2(0.0f,1.0f)},
		{XMFLOAT3(scale, scale, -scale),  	XMFLOAT2(1.0f,1.0f)},
		{XMFLOAT3(scale, -scale, scale),  	XMFLOAT2(1.0f,0.0f)},
		{XMFLOAT3(scale, scale, scale), 	XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(scale, scale, -scale),  	XMFLOAT2(1.0f,1.0f)},
		// bottom face
		{XMFLOAT3(scale, -scale, scale), 	XMFLOAT2(1.0f,1.0f)},
		{XMFLOAT3(scale, -scale, -scale), 	XMFLOAT2(1.0f,0.0f)},
		{XMFLOAT3(-scale, -scale, -scale),	XMFLOAT2(0.0f,1.0f)},
		{XMFLOAT3(-scale, -scale, scale), 	XMFLOAT2(1.0f,0.0f)},
		{XMFLOAT3(scale, -scale, scale), 	XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(-scale, -scale, -scale),	XMFLOAT2(0.0f,1.0f)},
		// top face
		{XMFLOAT3(scale, scale, -scale),	XMFLOAT2(1.0f,0.0f)},
		{XMFLOAT3(scale, scale, scale), 	XMFLOAT2(1.0f,1.0f)},
		{XMFLOAT3(-scale, scale, -scale), 	XMFLOAT2(0.0f,1.0f)},
		{XMFLOAT3(scale, scale, scale), 	XMFLOAT2(0.0f,0.0f)},
		{XMFLOAT3(-scale, scale, scale), 	XMFLOAT2(1.0f,0.0f)},
		{XMFLOAT3(-scale, scale, -scale), 	XMFLOAT2(0.0f,1.0f)}
	};



	return skybox;
}

POS_TEX_NORM_VERTEX* Geometry::cube_ptn(UINT *numverts)
{
	*numverts = cube_num_verts;
	const auto cube = new POS_TEX_NORM_VERTEX[*numverts]
	{
		// back face 
		{XMFLOAT3(-scale, scale, scale),   XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,0.0f,1.0f)},
		{XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,1.0f)},
		{XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,1.0f)},
		{XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,1.0f)},
		{XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,1.0f)},
		{XMFLOAT3(scale, -scale, scale),   XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,0.0f,1.0f)},
		// front face																		
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,-1.0f)},
		{XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,0.0f,-1.0f)},
		{XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,-1.0f)},
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,-1.0f)},
		{XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,-1.0f)},
		{XMFLOAT3(scale, -scale, -scale),  XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,0.0f,-1.0f)},
		// left face														,				
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(-1.0f,0.0f,0.0f)},
		{XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,0.0f), XMFLOAT3(-1.0f,0.0f,0.0f)},
		{XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(1.0f,1.0f), XMFLOAT3(-1.0f,0.0f,0.0f)},
		{XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,0.0f), XMFLOAT3(-1.0f,0.0f,0.0f)},
		{XMFLOAT3(-scale, scale, scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(-1.0f,0.0f,0.0f)},
		{XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(1.0f,1.0f), XMFLOAT3(-1.0f,0.0f,0.0f)},
		// right face														,				
		{XMFLOAT3(scale, -scale, scale),   XMFLOAT2(0.0f,0.0f), XMFLOAT3(1.0f,0.0f,0.0f)},
		{XMFLOAT3(scale, -scale, -scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(1.0f,0.0f,0.0f)},
		{XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,1.0f), XMFLOAT3(1.0f,0.0f,0.0f)},
		{XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(1.0f,0.0f,0.0f)},
		{XMFLOAT3(scale, -scale, scale),   XMFLOAT2(0.0f,0.0f), XMFLOAT3(1.0f,0.0f,0.0f)},
		{XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,1.0f), XMFLOAT3(1.0f,0.0f,0.0f)},
		// bottom face														,				
		{XMFLOAT3(scale, -scale, -scale),  XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,-1.0f,0.0f)},
		{XMFLOAT3(scale, -scale, scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,-1.0f,0.0f)},
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,-1.0f,0.0f)},
		{XMFLOAT3(scale, -scale, scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,-1.0f,0.0f)},
		{XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,-1.0f,0.0f)},
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,-1.0f,0.0f)},
		// top face										 					,				
		{XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,1.0f,0.0f) },
		{XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,1.0f,0.0f) },
		{XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,1.0f,0.0f) },
		{XMFLOAT3(-scale, scale, scale),   XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,1.0f,0.0f) },
		{XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,1.0f,0.0f) },
		{XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,1.0f,0.0f) }
	};


	return cube;
}

POS_COL_TEX_VERTEX* Geometry::cube_ptc(UINT *numverts)
{
	*numverts = cube_num_verts;
	const auto cube = new POS_COL_TEX_VERTEX[*numverts]
	{
		// back face 
		{ XMFLOAT3(-scale, scale, scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(-scale, -scale, scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(scale, scale, scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(scale, scale, scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(-scale, -scale, scale),XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(scale, -scale, scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,1.0f) },
		// front face
		{ XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(-scale, scale, -scale), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(scale, scale, -scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(-scale, -scale, -scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(scale, scale, -scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(scale, -scale, -scale),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,1.0f) },
		// left face
		{ XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(-scale, -scale, scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,1.0f) },
		{ XMFLOAT3(-scale, -scale, scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(-scale, scale, scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,1.0f) },
		// right face
		{ XMFLOAT3(scale, -scale, scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(scale, -scale, -scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(scale, scale, -scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,1.0f) },
		{ XMFLOAT3(scale, scale, scale), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(scale, -scale, scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(scale, scale, -scale),  XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f,1.0f) },
		// bottom face
		{ XMFLOAT3(scale, -scale, -scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,1.0f) },
		{ XMFLOAT3(scale, -scale, scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(scale, -scale, scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(-scale, -scale, scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),XMFLOAT2(0.0f,1.0f) },
		// top face
		{ XMFLOAT3(scale, scale, scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(scale, scale, -scale),XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,1.0f) },
		{ XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f) },
		{ XMFLOAT3(-scale, scale, scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,0.0f) },
		{ XMFLOAT3(scale, scale, scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(1.0f,0.0f) },
		{ XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f), XMFLOAT2(0.0f,1.0f) }
	};


	return cube;
}

POS_TEX_NORM_COL_VERTEX* Geometry::cube_ptnc(UINT *numverts)
{
	*numverts = cube_num_verts;
	const auto cube = new POS_TEX_NORM_COL_VERTEX[*numverts]
	{
		// back face 
		{ XMFLOAT3(-scale, scale, scale),   XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,0.0f,1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(scale, -scale, scale),   XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,0.0f,1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	// front face																				   
	{ XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(scale, -scale, -scale),  XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f) },
	// left face														,					   
	{ XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(-1.0f,0.0f,0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,0.0f), XMFLOAT3(-1.0f,0.0f,0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(1.0f,1.0f), XMFLOAT3(-1.0f,0.0f,0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,0.0f), XMFLOAT3(-1.0f,0.0f,0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(-scale, scale, scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(-1.0f,0.0f,0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(1.0f,1.0f), XMFLOAT3(-1.0f,0.0f,0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	// right face														,					   		  
	{ XMFLOAT3(scale, -scale, scale),   XMFLOAT2(0.0f,0.0f), XMFLOAT3(1.0f,0.0f,0.0f) , XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(scale, -scale, -scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(1.0f,0.0f,0.0f) , XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,1.0f), XMFLOAT3(1.0f,0.0f,0.0f) , XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(1.0f,0.0f,0.0f) , XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(scale, -scale, scale),   XMFLOAT2(0.0f,0.0f), XMFLOAT3(1.0f,0.0f,0.0f) , XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	{ XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,1.0f), XMFLOAT3(1.0f,0.0f,0.0f) , XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f) },
	// bottom face														,					   
	{ XMFLOAT3(scale, -scale, -scale),  XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,-1.0f,0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(scale, -scale, scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,-1.0f,0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,-1.0f,0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(scale, -scale, scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,-1.0f,0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,-1.0f,0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,-1.0f,0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	// top face										 					,					   		  
	{ XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,1.0f,0.0f) , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,1.0f,0.0f) , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,1.0f,0.0f) , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-scale, scale, scale),   XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,1.0f,0.0f) , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,1.0f,0.0f) , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) },
	{ XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,1.0f,0.0f) , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f) }
	};


	return cube;
}

void Geometry::plane_ittn(POS_TEX_NORM_COL_VERTEX** plane, unsigned int **indices, const UINT tiles)
{
	const auto plane_vertices = new POS_TEX_NORM_COL_VERTEX[(tiles + 1)*(tiles + 1)];
	const auto plane_indices = new unsigned int[tiles* tiles * 6];

	for (size_t z = 0; z < (tiles + 1); z++)
	{
		for (size_t x = 0; x < (tiles + 1); x++)
		{
			plane_vertices[z*(tiles + 1) + x].pos = XMFLOAT3(x * scale, 0.0f, z * scale);
			plane_vertices[z*(tiles + 1) + x].texcoord = XMFLOAT2(float(x), float(z));
			plane_vertices[z*(tiles + 1) + x].normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			plane_vertices[z*(tiles + 1) + x].col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	for (unsigned int z = 0; z < tiles; z++)
	{
		for (unsigned int x = 0; x < tiles; x++)
		{
			plane_indices[z*tiles * 6 + x * 6 + 0] = (z + 0) * (tiles + 1) + x + 0;
			plane_indices[z*tiles * 6 + x * 6 + 1] = (z + 1) * (tiles + 1) + x + 0;
			plane_indices[z*tiles * 6 + x * 6 + 2] = (z + 1) * (tiles + 1) + x + 1;
			plane_indices[z*tiles * 6 + x * 6 + 3] = (z + 0) * (tiles + 1) + x + 0;
			plane_indices[z*tiles * 6 + x * 6 + 4] = (z + 1) * (tiles + 1) + x + 1;
			plane_indices[z*tiles * 6 + x * 6 + 5] = (z + 0) * (tiles + 1) + x + 1;
		}
	}

	*plane = plane_vertices;
	*indices = plane_indices;
}

// NOTE::this method is not used in this solution. I still want to keep it for testing
//POS_COL_VERTEX* Geometry::cube_pc(const float scale)
//{
//	POS_COL_VERTEX color_cube[] =
//	{
//		// back face 
//		{XMFLOAT3(-scale, scale, scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
//		{XMFLOAT3(-scale, -scale, scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
//		{XMFLOAT3(scale, scale, scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
//		{XMFLOAT3(scale, scale, scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
//		{XMFLOAT3(-scale, -scale, scale),XMFLOAT4(1, 0.0f, 0.0f, 1)},
//		{XMFLOAT3(scale, -scale, scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
//		// front face
//		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
//		{XMFLOAT3(-scale, scale, -scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
//		{XMFLOAT3(scale, scale, -scale),  XMFLOAT4(1, 0.0f, 0.0f, 1)},
//		{XMFLOAT3(-scale, -scale, -scale),  XMFLOAT4(1, 0.0f, 0.0f, 1)},
//		{XMFLOAT3(scale, scale, -scale),  XMFLOAT4(1, 0.0f, 0.0f, 1)},
//		{XMFLOAT3(scale, -scale, -scale),  XMFLOAT4(1, 0.0f, 0.0f, 1)},
//		// left face
//		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
//		{XMFLOAT3(-scale, -scale, scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
//		{XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
//		{XMFLOAT3(-scale, -scale, scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
//		{XMFLOAT3(-scale, scale, scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
//		{XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
//		// right face
//		{XMFLOAT3(scale, -scale, scale),  XMFLOAT4(0.0f, 0.0f, 1, 1)},
//		{XMFLOAT3(scale, -scale, -scale),  XMFLOAT4(0.0f, 0.0f, 1, 1)},
//		{XMFLOAT3(scale, scale, -scale),  XMFLOAT4(0.0f, 0.0f, 1, 1)},
//		{XMFLOAT3(scale, scale, scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
//		{XMFLOAT3(scale, -scale, scale),  XMFLOAT4(0.0f, 0.0f, 1, 1)},
//		{XMFLOAT3(scale, scale, -scale),  XMFLOAT4(0.0f, 0.0f, 1, 1)},
//		// bottom face
//		{XMFLOAT3(scale, -scale, -scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
//		{XMFLOAT3(scale, -scale, scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
//		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
//		{XMFLOAT3(scale, -scale, scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
//		{XMFLOAT3(-scale, -scale, scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
//		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
//		// top face
//		{XMFLOAT3(scale, scale, scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
//		{XMFLOAT3(scale, scale, -scale),XMFLOAT4(0.0f, 1, 0.0f, 1)},
//		{XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
//		{XMFLOAT3(-scale, scale, scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
//		{XMFLOAT3(scale, scale, scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
//		{XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f, 1, 0.0f, 1)}
//	};
//
//
//	return color_cube;
//}