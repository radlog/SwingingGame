#pragma once
#include <string>
#include "Transform.h"
#include "Model.h"
#include "SoundEngine.h"
#include "GameData.h"
//#include "Camera.h"

class GameObject {
public:
	GameObject();
	virtual ~GameObject();

	GameObject(std::string name);
	GameObject(std::string name, Transform transform, Model model);
	void Draw(XMMATRIX view_projection);
	void Draw(XMMATRIX view_projection, D3D11_PRIMITIVE_TOPOLOGY mode);


	Transform transform;
	
	virtual void spawn(XMVECTOR position);
	virtual void render();
	virtual void start();
	virtual void update();
	virtual void collided(XMVECTOR target);
	virtual std::string get_name();

	void Cleanup();
protected:
	std::string name;
	Model model;
	SoundEngine sound;
	float collision_radius;
	GameData game_data;
	//Camera camera;
};
