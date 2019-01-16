#include "pch.h"
#include "Model.h"
#include "MeshCollider.h"
#include "MathHelper.h"

Model::Model(const LPCSTR filename, const CB_STATE state) : BaseModel(filename, state), sphere_collider_(nullptr),mesh_collider_(nullptr),radius_(0)
{
	calculate_origin(); // get models origin vector
	Model::initialize_sphere_collider(); // initialize sphere collider
	Model::initialize_mesh_collider(); // initialize mesh collider
}

Model::Model(const CB_STATE state) : BaseModel(state), sphere_collider_(nullptr), mesh_collider_(nullptr), radius_(0)
{
	calculate_origin(); // get models origin vector
	Model::initialize_sphere_collider(); // initialize sphere collider
	Model::initialize_mesh_collider(); // initialize mesh collider
}

Model::~Model()
{
}

// return the sphere collider
SphereCollider* Model::get_bounding_sphere() const
{
	return sphere_collider_;
}

// return the mesh collider
MeshCollider* Model::get_mesh_collider() const
{
	return mesh_collider_;
}

// initialize sphere collider
void Model::initialize_sphere_collider()
{
	// get radius from model max and min outer vector
	if (obj_file_model_ != nullptr) radius_ = dist(min_outer_vector_, max_outer_vector_);
	else radius_ = initial_radius_; // if model nullptr then set initial radius
	sphere_collider_ = new SphereCollider(origin_, radius_); // initialize sphere collider with origin and radius
}

// initialize mesh collider
void Model::initialize_mesh_collider()
{
	if (obj_file_model_ == nullptr) return; // do not use mesh collider if there is no obj model

	mesh_collider_ = new MeshCollider(origin_, obj_file_model_->get_vertex_positions(),radius_); // initialize mesh collider with origin and radius
}
