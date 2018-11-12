#pragma once
class Collectable
{

	enum Item_Type {
		Speedbuff,
		Accumulator,
		Coin
	};
public:
	Collectable();
	~Collectable();

	Collectable(Item_Type item_type);

	void collected();

};

