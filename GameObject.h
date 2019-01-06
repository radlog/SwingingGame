#pragma once
#include <string>
#include "Transform.h"
#include "Model.h"
#include "SoundEngine.h"
#include "GameData.h"
#include "MathHelper.h"
//#include "Camera.h"

class VGTime;
class D3Dfw;

class GameObject
{

public:
	GameObject();
	virtual ~GameObject();

	explicit GameObject(LPCSTR name);
	GameObject(LPCSTR name, Model *model, const Transform transform);
	virtual void draw(XMMATRIX view_projection, bool use_default_cb = true, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	virtual void move_horizontal_forward(float speed);
	virtual void move_horizontal_backward(float speed);
	virtual void move_forward(float speed);
	virtual void move_backward(float speed);
	virtual void move_right(float speed);
	virtual void move_left(float speed);
	virtual void move_up(float speed);
	virtual void move_down(float speed);

	Transform transform;


	virtual void spawn(XMVECTOR position);
	virtual void render();
	virtual void start();
	virtual void update(VGTime timer);
	bool collided(GameObject target) const;
	LPCSTR get_name() const;
	Model* get_model() const;

	void set_model(Model *model);

	virtual void cleanup();
protected:
	LPCSTR name_;
	LPCSTR tag_ = "default_object";
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

};