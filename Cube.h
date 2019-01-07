#pragma once
#include "Model.h"

enum VERTEX_FORMAT
{
	TEXTURED_LIGHTED,
	TEXTURED_COLORED,
	TEXTURED_COLORED_LIGHTED
};

class Cube : public Model
{


public:
	explicit Cube(LPCSTR texture = "", VERTEX_FORMAT vertex_format = TEXTURED_LIGHTED, CB_STATE state = CB_STATE_SIMPLE);
	~Cube();

};
