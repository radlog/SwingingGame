#pragma once
#include "BaseModel.h"
#include "Collider.h"

class MeshCollider;
class SphereCollider;

class Model :
	public BaseModel
{

	enum COLLIDER_TYPE
	{
		SPHERE,
		MESH
	};

public:
	explicit Model(LPCSTR filename, CB_STATE state = CB_STATE_SIMPLE, COLLIDER_TYPE col_type = SPHERE);
	explicit Model(CB_STATE state = CB_STATE_SIMPLE, COLLIDER_TYPE col_type = SPHERE);
	~Model();

	void draw(XMMATRIX view_projection, bool use_simple_cb, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) override;

	void add_sphere_collider(SphereCollider *col);
	void add_mesh_collider(MeshCollider *col);
	
protected:
	Collider collider_;

	void initialize_sphere_collider();
	void initialize_mesh_collider();
};

