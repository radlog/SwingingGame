#include "pch.h"
#include "Model.h"
#include "MeshCollider.h"

Model::Model(const LPCSTR filename, const CB_STATE state, const COLLIDER_TYPE col_type) : BaseModel(filename, state)
{
	calculate_origin();
	switch (col_type)
	{
		case SPHERE: initialize_sphere_collider();  break;
		case MESH: initialize_mesh_collider(); break;
		default: break;
	}
}

Model::Model(const CB_STATE state, const COLLIDER_TYPE col_type) : BaseModel(state)
{
	calculate_origin();
	switch(col_type)
	{
		case SPHERE: initialize_sphere_collider();  break;
		case MESH: initialize_mesh_collider(); break;		
		default: break;
	}
}

Model::~Model()
{
}

void Model::draw(const XMMATRIX view_projection, const bool use_simple_cb, const D3D11_PRIMITIVE_TOPOLOGY mode)
{
	BaseModel::draw(view_projection, use_simple_cb, mode);
}

void Model::add_sphere_collider(SphereCollider* col) const
{
	*collider_ = static_cast<Collider>(*col);
}

void Model::add_mesh_collider(MeshCollider* col) const
{	
	*collider_ = static_cast<Collider>(*col);
}

Collider* Model::get_collider() const
{
	return collider_;
}

void Model::initialize_sphere_collider()
{
	//calculate_origin();
	float radius = 0;
	if(obj_file_model_ != nullptr) radius = dist(min_outer_vector_, max_outer_vector_);
	else radius = 1.0f;
	collider_ = new SphereCollider(origin_, radius);
}

void Model::initialize_mesh_collider()
{
	//calculate_origin();
	if (obj_file_model_ == nullptr) return;

	//TODO:: fix the error that appears when MeshCollider is created (the program breaks on different points randomly and error message is mostly Critical error detected c0000374), sometimes even the mouse or keyboard doesn't respond anymore
	
	collider_ = new MeshCollider(obj_file_model_->get_vertex_positions());
}

