#pragma once
#include "BaseModel.h"
#include "Collider.h"

class MeshCollider;
class SphereCollider;


class Model :
	public BaseModel
{

public:
	explicit Model(LPCSTR filename, CB_STATE state = CB_STATE_TIME_SCALED); // initialize model with filename, constant buffer state
	explicit Model(CB_STATE state = CB_STATE_TIME_SCALED); // initialize model with constant buffer state
	~Model();

	SphereCollider* get_bounding_sphere() const; // return bounding sphere collider
	MeshCollider* get_mesh_collider() const; // return mesh collider

protected:
	SphereCollider* sphere_collider_; // bounding sphere collider
	MeshCollider* mesh_collider_; // mesh collider
	float radius_; // initial radius of the model collision
	virtual void initialize_sphere_collider(); // initialize sphere collider
	virtual void initialize_mesh_collider(); // initialize mesh collider
	
	const float initial_radius_ = 0.2f;
};

