#include "pch.h"
#include "Model.h"
#include "MeshCollider.h"

Model::Model(const LPCSTR filename, const CB_STATE state, const COLLIDER_TYPE col_type) : BaseModel(filename, state)
{
	//calculate_origin();
	origin_ = XMVectorZero();
	//min_outer_vector_ = XMVectorSplatOne();
	//max_outer_vector_ = XMVectorSet(2,2,2,0);
	switch (col_type)
	{
		case SPHERE: initialize_sphere_collider();  break;
		case MESH: Model::initialize_mesh_collider(); break;
		default: break;
	}
}

Model::Model(const CB_STATE state, const COLLIDER_TYPE col_type) : BaseModel(state)
{
	//calculate_origin();
	origin_ = XMVectorZero();
	//min_outer_vector_ = XMVectorSplatOne();
	//max_outer_vector_ = XMVectorSet(2, 2, 2, 0);
	switch(col_type)
	{
		case SPHERE: initialize_sphere_collider();  break;
		case MESH: Model::initialize_mesh_collider(); break;		
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
	float radius = 0;
	if(obj_file_model_ != nullptr) radius = dist(min_outer_vector_, max_outer_vector_);
	else radius = 1.0f;
	radius = 1.0f;
	collider_ = new SphereCollider(origin_, radius);
}

void Model::initialize_mesh_collider()
{
	if (obj_file_model_ == nullptr) return;

	collider_ = new MeshCollider(origin_, obj_file_model_->get_vertex_positions());
}

