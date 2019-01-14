#include "pch.h"
#include "ObjCube.h"

const LPCSTR cube_file = "assets/cube.obj";


ObjCube::~ObjCube()
{
}

ObjCube::ObjCube(const CB_STATE state) : Model(cube_file, state)
{

}
