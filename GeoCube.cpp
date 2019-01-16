#include "GeoCube.h"


GeoCube::GeoCube(const LPCSTR texture, const VERTEX_FORMAT vertex_format, const CB_STATE state) : Model(state)
{
	UINT numverts; // number of vertices
	// create different types of cube with different vertex formats
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

	Model::initialize_sphere_collider(); // initialize sphere collider

	numverts_ = numverts; // set number of vertices

	origin_ = XMVectorZero(); // geo cubes origin is always zero

	load_geo_model(vertices_, numverts_, vert_size_); // load created geometry
	load_texture(texture); // load texture
}


GeoCube::~GeoCube()
{
}
