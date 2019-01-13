#pragma once
#include "Model.h"

extern const LPCSTR cube_file;

class ObjCube :
	public Model
{
	

public:
	~ObjCube();

	explicit ObjCube(CB_STATE state = CB_STATE_TIME_SCALED, COLLIDER_TYPE col_type = MESH);

private:

};

