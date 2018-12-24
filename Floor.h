#pragma once
class Floor : public GameObject
{
public:
	Floor();
	~Floor();

	Floor(LPCSTR texture, int tiles = 200, float scale = 1.0f);
	Plane get_collider();

private:
	Plane plane_collider;
	POS_TEX_NORM_COL_VERTEX *plane_vertices;
	unsigned int *plane_indices;
	int tiles;
	float scale;
};

