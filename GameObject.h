#pragma once
#include <string>
#include "Transform.h"
#include "Model.h"
#include "SoundEngine.h"
#include "GameData.h"
#include "d3dfw.h"
//#include "Camera.h"

class VGTime;
class d3dfw;

class GameObject 
{

public:
	GameObject();
	virtual ~GameObject();

	GameObject(LPCSTR name);
	GameObject(LPCSTR name, Transform transform, Model model);
	void Draw(XMMATRIX view_projection, bool use_default_cb = true, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	Transform transform;
	

	virtual void spawn(XMVECTOR position);
	virtual void render();
	virtual void start();
	virtual void update(VGTime timer);
	virtual bool collided(GameObject target);
	virtual LPCSTR get_name();
	Model* get_model();
	
	void Cleanup();
protected:
	LPCSTR name;
	Model model;
	SoundEngine sound;
	float collision_radius;
	GameData game_data;
	d3dfw* dx_handle;

	double air_time = 0;
	bool is_grounded;
	bool is_kinetic;

	ID3D11Device* device;
	ID3D11DeviceContext* immediateContext;

};