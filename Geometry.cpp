#include "Geometry.h"



Geometry::Geometry()
{
}


Geometry::~Geometry()
{
}

void Geometry::create_textured_normal_cube(POS_TEX_NORM_COL_VERTEX* cube[], float scale)
{
	POS_TEX_NORM_COL_VERTEX textured_normal_cube[] =
	{
		// back face 
		{XMFLOAT3(-scale, scale, scale),   XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,0.0f,1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(scale, -scale, scale),   XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,0.0f,1.0f),  XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		// front face																				   
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		{XMFLOAT3(scale, -scale, -scale),  XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,0.0f,-1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)},
		// left face														,					   
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(-1.0f,0.0f,0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,0.0f), XMFLOAT3(-1.0f,0.0f,0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(1.0f,1.0f), XMFLOAT3(-1.0f,0.0f,0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,0.0f), XMFLOAT3(-1.0f,0.0f,0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{XMFLOAT3(-scale, scale, scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(-1.0f,0.0f,0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(1.0f,1.0f), XMFLOAT3(-1.0f,0.0f,0.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		// right face														,					   		  
		{XMFLOAT3(scale, -scale, scale),   XMFLOAT2(0.0f,0.0f), XMFLOAT3(1.0f,0.0f,0.0f) , XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{XMFLOAT3(scale, -scale, -scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(1.0f,0.0f,0.0f) , XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,1.0f), XMFLOAT3(1.0f,0.0f,0.0f) , XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(1.0f,0.0f,0.0f) , XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{XMFLOAT3(scale, -scale, scale),   XMFLOAT2(0.0f,0.0f), XMFLOAT3(1.0f,0.0f,0.0f) , XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		{XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,1.0f), XMFLOAT3(1.0f,0.0f,0.0f) , XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)},
		// bottom face														,					   
		{XMFLOAT3(scale, -scale, -scale),  XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,-1.0f,0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(scale, -scale, scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,-1.0f,0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,-1.0f,0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(scale, -scale, scale),   XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,-1.0f,0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-scale, -scale, scale),  XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,-1.0f,0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,-1.0f,0.0f), XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		// top face										 					,					   		  
		{XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,1.0f,0.0f) , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(scale, scale, -scale),   XMFLOAT2(1.0f,1.0f), XMFLOAT3(0.0f,1.0f,0.0f) , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,1.0f,0.0f) , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-scale, scale, scale),   XMFLOAT2(0.0f,0.0f), XMFLOAT3(0.0f,1.0f,0.0f) , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(scale, scale, scale),    XMFLOAT2(1.0f,0.0f), XMFLOAT3(0.0f,1.0f,0.0f) , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)},
		{XMFLOAT3(-scale, scale, -scale),  XMFLOAT2(0.0f,1.0f), XMFLOAT3(0.0f,1.0f,0.0f) , XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)}
	};

	*cube = textured_normal_cube;
}

POS_TEX_VERTEX* Geometry::create_textured_skybox(float scale)
{
	POS_TEX_VERTEX* skybox = new POS_TEX_VERTEX[36]
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

	return skybox ;
}

void Geometry::create_tex_col_pos_norm_skybox(POS_TEX_NORM_COL_VERTEX* cube[], float scale)
{
	POS_TEX_NORM_COL_VERTEX skybox[] =
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

	*cube = skybox;
}

void Geometry::create_indexed_tiled_textured_normal_plane(POS_TEX_NORM_COL_VERTEX** plane, unsigned int **indices, UINT tiles, float scale)
{
	POS_TEX_NORM_COL_VERTEX *plane_vertices = new POS_TEX_NORM_COL_VERTEX[(tiles + 1)*(tiles + 1)];
	unsigned int *plane_indices = new unsigned int[tiles* tiles * 6];

	for (size_t z = 0; z < (tiles + 1); z++)
	{
		for (size_t x = 0; x < (tiles + 1); x++)
		{
			plane_vertices[z*(tiles + 1) + x].Pos = XMFLOAT3(x * scale, 0.0f, z * scale);
			plane_vertices[z*(tiles + 1) + x].Texture0 = XMFLOAT2((float)x, (float)z);
			plane_vertices[z*(tiles + 1) + x].Normal = XMFLOAT3(0.0f, 1.0f, 0.0f);
			plane_vertices[z*(tiles + 1) + x].Col = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
		}
	}

	for (size_t z = 0; z < tiles; z++)
	{
		for (size_t x = 0; x < tiles; x++)
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

POS_COL_VERTEX*  Geometry::create_color_cube(float scale)
{
	POS_COL_VERTEX color_cube[] =
	{
		// back face 
		{XMFLOAT3(-scale, scale, scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
		{XMFLOAT3(-scale, -scale, scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
		{XMFLOAT3(scale, scale, scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
		{XMFLOAT3(scale, scale, scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
		{XMFLOAT3(-scale, -scale, scale),XMFLOAT4(1, 0.0f, 0.0f, 1)},
		{XMFLOAT3(scale, -scale, scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
		// front face
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
		{XMFLOAT3(-scale, scale, -scale), XMFLOAT4(1, 0.0f, 0.0f, 1)},
		{XMFLOAT3(scale, scale, -scale),  XMFLOAT4(1, 0.0f, 0.0f, 1)},
		{XMFLOAT3(-scale, -scale, -scale),  XMFLOAT4(1, 0.0f, 0.0f, 1)},
		{XMFLOAT3(scale, scale, -scale),  XMFLOAT4(1, 0.0f, 0.0f, 1)},
		{XMFLOAT3(scale, -scale, -scale),  XMFLOAT4(1, 0.0f, 0.0f, 1)},
		// left face
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
		{XMFLOAT3(-scale, -scale, scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
		{XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
		{XMFLOAT3(-scale, -scale, scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
		{XMFLOAT3(-scale, scale, scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
		{XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
		// right face
		{XMFLOAT3(scale, -scale, scale),  XMFLOAT4(0.0f, 0.0f, 1, 1)},
		{XMFLOAT3(scale, -scale, -scale),  XMFLOAT4(0.0f, 0.0f, 1, 1)},
		{XMFLOAT3(scale, scale, -scale),  XMFLOAT4(0.0f, 0.0f, 1, 1)},
		{XMFLOAT3(scale, scale, scale), XMFLOAT4(0.0f, 0.0f, 1, 1)},
		{XMFLOAT3(scale, -scale, scale),  XMFLOAT4(0.0f, 0.0f, 1, 1)},
		{XMFLOAT3(scale, scale, -scale),  XMFLOAT4(0.0f, 0.0f, 1, 1)},
		// bottom face
		{XMFLOAT3(scale, -scale, -scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
		{XMFLOAT3(scale, -scale, scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
		{XMFLOAT3(scale, -scale, scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
		{XMFLOAT3(-scale, -scale, scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
		{XMFLOAT3(-scale, -scale, -scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
		// top face
		{XMFLOAT3(scale, scale, scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
		{XMFLOAT3(scale, scale, -scale),XMFLOAT4(0.0f, 1, 0.0f, 1)},
		{XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
		{XMFLOAT3(-scale, scale, scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
		{XMFLOAT3(scale, scale, scale), XMFLOAT4(0.0f, 1, 0.0f, 1)},
		{XMFLOAT3(-scale, scale, -scale), XMFLOAT4(0.0f, 1, 0.0f, 1)}
	};

	return color_cube;
}
