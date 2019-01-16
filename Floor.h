#pragma once
// creates a textured plane as floor with a plane collider
class Floor : public Model
{
public:
	//Floor();
	~Floor();

	/**
	 * \brief initialises a Floor object with texture, tiles, scale
	 * \param texture floors texture
	 * \param tiles floors tile size 
	 * \param scale defines the doubled size of the value as distance between each vertex
	 */
	explicit Floor(int tiles = 200, float scale = 1.0f);
	//Plane get_collider(); // returns the floors plane collider
	Transform* get_transform() const; // get floor models transform
	int get_tiles() const; // get number of tiles
	float get_scale() const; // get scaling
protected:
	POS_TEX_NORM_COL_VERTEX *plane_vertices_; // pointer to textured, coloured vertices with normal values of the floor
	unsigned int *plane_indices_; // pointer to vertex indices
	int tiles_; // number of tiles
	float scale_; // doubled distance between each vertex
	Transform *transform_; // transform of the model
	void initialize_mesh_collider() override; // initialize mesh collider
	void initialize_sphere_collider() override; // initialize sphere collider
};

