#pragma once
class Character;

class Collectable
{

	enum ITEM_TYPE {
		SPEEDBUFF,
		COIN
	};
public:
	Collectable();
	~Collectable();

	Collectable(ITEM_TYPE item_type);

	void collected(Character actor) const;

private:
	ITEM_TYPE item_type_;
};

