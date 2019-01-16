#pragma once
class Collider
{

public:
	Collider();
	virtual ~Collider();

	virtual bool check_collision(Collider *col); // checks collision with other collider
	void set_world_position(XMVECTOR world); // set world position of the collider origin
	XMVECTOR get_world_position(); // get world position of the collider origin
	XMVECTOR get_origin() const; // get origin of local position

protected:
	XMVECTOR origin_; // local position origin
	XMVECTOR world_; // world origin position
};

