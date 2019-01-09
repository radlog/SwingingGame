#pragma once
class Collider
{

public:
	Collider();
	virtual ~Collider();

	virtual bool check_collision(Collider col);	

};

