#include "Cube.h"


Cube::Cube(LPCSTR texture, const VERTEX_FORMAT vertex_format, const CB_STATE state) : Model(state)
{
	UINT numverts;
	switch(vertex_format) 
	{ 
		case TEXTURED_LIGHTED: 
			vertices_ = Geometry::cube_ptn(&numverts);
			vert_size_ = sizeof(POS_TEX_NORM_VERTEX);

			break;
		case TEXTURED_COLORED: 
			vertices_ = Geometry::cube_ptc(&numverts);
			vert_size_ = sizeof(POS_COL_TEX_VERTEX);
			break;
		case TEXTURED_COLORED_LIGHTED: 
			vertices_ = Geometry::cube_ptnc(&numverts);
			vert_size_ = sizeof(POS_TEX_NORM_COL_VERTEX);
			break;
		default: break; 
	}

	initialize_sphere_collider();

	numverts_ = numverts;

	origin_ = XMVectorSet(0, 0, 0, 0);

	load_geo_model(vertices_, numverts_, vert_size_);
	load_texture();
}


Cube::~Cube()
{
}
