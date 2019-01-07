#pragma once
// creates a textured plane as floor with a plane collider
class Floor : public GameObject
{
public:
	Floor();
	~Floor();

	/**
	 * \brief initialises a Floor object with texture, tiles, scale
	 * \param texture floors texture
	 * \param tiles floors tile size 
	 * \param scale defines the doubled size of the value as distance between each vertex
	 */
	explicit Floor(LPCSTR texture, int tiles = 200, float scale = 1.0f);
	PlaneCollider get_collider(); // returns the floors plane collider
	
private:
	PlaneCollider plane_collider_; // plane collider
	POS_TEX_NORM_COL_VERTEX *plane_vertices_; // pointer to textured, coloured vertices with normal values of the floor
	unsigned int *plane_indices_; // pointer to vertex indices
	int tiles_; // number of tiles
	float scale_; // doubled distance between each vertex
};

