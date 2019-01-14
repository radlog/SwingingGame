#pragma once
#include "BaseModel.h"
#include "Collider.h"

class MeshCollider;
class SphereCollider;


class Model :
	public BaseModel
{


public:
	explicit Model(LPCSTR filename, CB_STATE state = CB_STATE_TIME_SCALED);
	explicit Model(CB_STATE state = CB_STATE_TIME_SCALED);
	~Model();

	void draw(XMMATRIX view_projection, bool use_simple_cb = false, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST) override;

	SphereCollider* get_bounding_sphere() const;
	MeshCollider* get_mesh_collider() const;

protected:
	SphereCollider* sphere_collider_;
	MeshCollider* mesh_collider_;
	virtual void initialize_sphere_collider();
	virtual void initialize_mesh_collider();
};

