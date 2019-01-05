#pragma once
class Floor : public GameObject
{
public:
	Floor();
	~Floor();

	explicit Floor(LPCSTR texture, int tiles = 200, float scale = 1.0f);
	Plane get_collider();

	void cleanup() override;
private:
	Plane plane_collider_;
	POS_TEX_NORM_COL_VERTEX *plane_vertices_;
	unsigned int *plane_indices_;
	int tiles_;
	float scale_;
};

