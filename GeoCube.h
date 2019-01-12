#pragma once
#include "BaseModel.h"

enum VERTEX_FORMAT
{
	TEXTURED_LIGHTED,
	TEXTURED_COLORED,
	TEXTURED_COLORED_LIGHTED
};

class GeoCube : public Model
{


public:
	explicit GeoCube(LPCSTR texture = "", VERTEX_FORMAT vertex_format = TEXTURED_LIGHTED, CB_STATE state = CB_STATE_SIMPLE);
	~GeoCube();

};
