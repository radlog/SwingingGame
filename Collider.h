#pragma once
class Collider
{

public:
	Collider();
	virtual ~Collider();

	virtual bool check_collision(Collider *col);
	void set_world_position(XMVECTOR world);
	XMVECTOR get_world_position();
	XMVECTOR get_origin() const;

protected:
	XMVECTOR origin_;
	XMVECTOR world_;
};

