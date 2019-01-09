#pragma once
#include <string>
#include "Transform.h"
#include "Model.h"
#include "SoundEngine.h"
#include "GameData.h"
#include "MathHelper.h"
#include "SphereCollider.h"

class Model;
class VGTime;
class D3Dfw;

// most important class to create and handle all entities within the game
class GameObject
{

	// tag defines the collision type and how interaction between collided objects work
	enum TAG
	{
		DEFAULT = 0,
		GROUND = 1,
		MODEL = 2,
		POLY = 3,
		CHARACTER = 4,
		PLAYER = 5
	};

public:
	GameObject();
	virtual ~GameObject();

	/**
	 * \brief initialises a gameobject with a name
	 * \param name name of the object
	 */
	explicit GameObject(LPCSTR name, TAG tag = DEFAULT);
	/**
	 * \brief initialises a gameobject with name, mode, transform
	 * \param name name of the object
	 * \param model gameobject's model
	 * \param transform initial gameobject's transform
	 */
	GameObject(LPCSTR name, Model *model, const Transform transform, TAG tag = DEFAULT);
	/**
	 * \brief draw the objects model with provided view_projection, default constant buffer flag, topology mode
	 * \param view_projection pass in cameras view projection
	 * \param use_default_cb whether to use constant buffer or not
	 * \param mode topology to draw
	 */
	virtual void draw(XMMATRIX view_projection, bool use_default_cb = true, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	virtual void translate(XMVECTOR direction, float speed); // moves the object in a direction at a given speed
	virtual void move_horizontal_forward(float speed); // moves the object forward horizontally ignoring the up/down rotation at a given speed 
	virtual void move_horizontal_backward(float speed); // moves the object backward horizontally ignoring the up/down rotation at a given speed 
	virtual void move_forward(float speed); // moves the object forward at a given speed 
	virtual void move_backward(float speed); // moves the object backward at a given speed 
	virtual void move_right(float speed); // moves the object right at a given speed 
	virtual void move_left(float speed); // moves the object left at a given speed 
	virtual void move_up(float speed); // moves the object up at a given speed 
	virtual void move_down(float speed); // moves the object down at a given speed 
	virtual void rotate_fixed(float pitch, float yaw, float roll); // rotates the object at given axis rotations pitch, yaw, roll with rotation lock at 89 degrees to prevent inverted axis
	virtual void rotate(float pitch, float yaw, float roll); // rotates the object at given axis rotations pitch, yaw, roll

	vector<GameObject*> get_children() const; // returns the list of children this object has
	void set_kinetic(bool kinetic);

	// TODO: make transform private or find out how to make it an interface or how to make a better abstraction 
	Transform transform; // the transform of the object



	virtual void spawn(XMVECTOR position);
	virtual void render(); // render frame
	virtual void start();
	virtual void update_transform(XMMATRIX *world);
	virtual void update(VGTime timer); // update gameobject's physics and interactions with the game world
	//bool collided(GameObject target) const; // says whether the object collided with another gameobject
	LPCSTR get_name() const; // return objects name
	Model* get_model() const; // return objects model

	void set_model(Model *model); // manually set the objects model
	void add_child(GameObject *child); // add a child object
	bool remove_child(GameObject *child); // remove a child safely(only if it is in the children list)

	void set_grounded(bool grounded); // sets object grounded

	void set_collider(Collider col);
	void update_collision_tree(XMMATRIX *world, float scale);

	virtual void cleanup(); // cleanup pointers to prevent memory leaks
protected:
	vector<GameObject*> children_; // pointer to children list of gameobject
	LPCSTR name_; // name
	TAG tag_; // tag
	Model *model_; // model
	//SoundEngine sound_; // sound engine instance
	GameData game_data_; // game data that hold track of running game values
	D3Dfw* dx_handle_; // directX handle 

	double air_time_ = 0; // time the object is in air
	bool is_grounded_ = false; // whether the object has ground under its feet
	bool is_kinetic_ = false; // kinetic objects are not affected by physics and or gravity

	ID3D11Device* device_; // pointer to the hardware device
	ID3D11DeviceContext* immediate_context_; // pointer to the device context

	Collider collider_;
	SphereCollider sphere_collider_;

};