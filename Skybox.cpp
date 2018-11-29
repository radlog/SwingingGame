#include "Skybox.h"


Skybox::Skybox()
{
	POS_TEX_VERTEX* skybox_desc = Geometry::create_textured_skybox(20.0f);
	model = Model(dx_handle->device, dx_handle->immediateContext);
	model.LoadGeoModel(skybox_desc, 36, sizeof(POS_TEX_VERTEX));
}

Skybox::~Skybox()
{
}

Skybox::Skybox(std::string name)
{
}

Skybox::Skybox(std::string name, Transform transform, Model model)
{

}

