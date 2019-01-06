#pragma once
#include <string>
#include "Transform.h"
#include "Model.h"
#include "SoundEngine.h"
#include "GameData.h"
#include "MathHelper.h"
#include "d3dfw.h"
//#include "Camera.h"

class VGTime;
class D3Dfw;

class GameObject 
{



public:
	GameObject();
	virtual ~GameObject();

	explicit GameObject(LPCSTR name, LPCSTR tag = "default");
	GameObject(LPCSTR name, Transform transform, Model *model, LPCSTR tag = "default");
	void draw(XMMATRIX view_projection, bool use_default_cb = true, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	Transform transform;
	

	virtual void spawn(XMVECTOR position);
	virtual void render();
	virtual void start();
	virtual void update(VGTime timer);
	bool collided(GameObject target) const;
	LPCSTR get_name() const;
	Model* get_model();
	
	virtual void cleanup();
protected:
	LPCSTR name_;
	LPCSTR tag_;
	Model *model_;
	SoundEngine sound_;
	float collision_radius_;
	GameData game_data_;
	D3Dfw* dx_handle_;

	double air_time_ = 0;
	bool is_grounded_;
	bool is_kinetic_;

	ID3D11Device* device_;
	ID3D11DeviceContext* immediate_context_;

	enum COLLISION_TYPE
	{
		DEFAULT = 0,
		GROUND = 1,
		MODEL = 2,
		POLY = 3,
		CHARACTER = 4,
		PLAYER = 5
	};

	static LPCSTR get_collision_type(COLLISION_TYPE collision_type);

};