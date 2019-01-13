#pragma once
#include "BaseModel.h"
#include "Collider.h"

class MeshCollider;
class SphereCollider;

	enum COLLIDER_TYPE
	{
		SPHERE,
		MESH,
		NOTHING
	};


class Model :
	public BaseModel
{


public:
	explicit Model(LPCSTR filename, CB_STATE state = CB_STATE_SIMPLE, COLLIDER_TYPE col_type = NOTHING);
	explicit Model(CB_STATE state = CB_STATE_SIMPLE, COLLIDER_TYPE col_type = NOTHING);
	~Model();

	void draw(XMMATRIX view_projection, bool use_simple_cb, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) override;

	void add_sphere_collider(SphereCollider *col) const;
	void add_mesh_collider(MeshCollider *col) const;
	Collider* get_collider() const;

protected:
	Collider *collider_;

	void initialize_sphere_collider();
	virtual void initialize_mesh_collider();
};

