#pragma once
#include <string>
#include "Transform.h"
#include "Model.h"
#include "SoundEngine.h"
#include "GameData.h"
#include "MathHelper.h"
#include "SphereCollider.h"
#include "MeshCollider.h"


class Model;
class VGTime;
class D3Dfw;

extern const float push_back_speed = 2.0f;

// tag defines the collision type and how interaction between collided objects work
enum TAG
{
	DEFAULT = 0,
	GROUND = 1,
	MODEL = 2,
	POLY = 3,
	CHARACTER = 4,
	PLAYER = 5,
	LAVA = 6
};

// most important class to create and handle all entities within the game
class GameObject
{



public:
	GameObject();
	virtual ~GameObject();

	/**
	 * \brief initialises a gameobject with a name
	 * \param name name of the object
	 * \param tag
	 */
	explicit GameObject(LPCSTR name, TAG tag = DEFAULT);
	/**
	 * \brief initialises a gameobject with name, mode, transform
	 * \param name name of the object
	 * \param model gameobject's model
	 * \param transform initial gameobject's transform
	 * \param tag
	 */
	GameObject(LPCSTR name, Model *model, Transform *transform, TAG tag = DEFAULT);
	/**
	 * \brief draw the objects model with provided view_projection, default constant buffer flag, topology mode
	 * \param view_projection pass in cameras view projection
	 * \param use_default_cb whether to use constant buffer or not
	 * \param mode topology to draw
	 */
	virtual void draw(XMMATRIX view_projection, bool use_default_cb = false, D3D11_PRIMITIVE_TOPOLOGY mode = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	virtual bool translate(XMVECTOR direction, float speed); // moves the object in a direction at a given speed
	virtual void rotate_fixed(float pitch, float yaw, float roll); // rotates the object at given axis rotations pitch, yaw, roll with rotation lock at 89 degrees to prevent inverted axis
	virtual void rotate(float pitch, float yaw, float roll); // rotates the object at given axis rotations pitch, yaw, roll

	vector<GameObject*> get_children() const; // returns the list of children this object has
	void set_kinetic(bool kinetic);
	bool get_kinetic() const;
	void set_grounded(bool grounded); // sets object grounded
	bool get_grounded() const;
	virtual SphereCollider* get_sphere_collider();
	virtual MeshCollider* get_mesh_collider();
	TAG get_tag() const;
	bool check_collision(GameObject *target);




	Transform* get_transform() const;
	void set_transform(Transform* transform);

	Collider* get_last_collision();
	virtual void spawn(XMVECTOR position);
	virtual void render(); // render frame
	virtual void start();
	virtual void update_transform(XMMATRIX *world);
	virtual void update(VGTime *timer); // update gameobject's physics and interactions with the game world
	//bool collided(GameObject target) const; // says whether the object collided with another gameobject
	LPCSTR get_name() const; // return objects name
	Model* get_model() const; // return objects model

	void set_model(Model *model); // manually set the objects model
	void set_parent(GameObject *parent);
	void remove_parent(GameObject *child);
	void add_child(GameObject *child); // add a child object
	bool remove_child(GameObject *child); // remove a child safely(only if it is in the children list)

	void update_collision_tree(XMMATRIX *world, float scale);
	void update_constant_buffer_time_scaled(XMMATRIX world_view_projection, const XMMATRIX view_projection, const XMVECTOR directional_light_vector,
		const XMVECTOR directional_light_color, const XMVECTOR ambient_light_color, const float game_time);

	virtual void cleanup(); // cleanup pointers to prevent memory leaks
protected:
	GameObject *parent_;
	vector<GameObject*> children_; // pointer to children list of gameobject

	LPCSTR name_; // name
	TAG tag_; // tag
	Model *model_; // model
	//SoundEngine sound_; // sound engine instance
	GameData game_data_; // game data that hold track of running game values
	D3Dfw* dx_handle_; // directX handle 

	double air_time_ = 0; // time the object is in air
	bool is_grounded_ = false; // whether the object has ground under its feet
	bool is_kinetic_ = true; // kinetic objects are not affected by physics and or gravity

	ID3D11Device* device_; // pointer to the hardware device
	ID3D11DeviceContext* immediate_context_; // pointer to the device context

	SphereCollider *sphere_collider_;
	MeshCollider *mesh_collider_;
	Transform *transform_; // the transform of the object

};