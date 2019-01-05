#include "Collectable.h"



Collectable::Collectable()
{
}


Collectable::~Collectable()
{
}

Collectable::Collectable(ITEM_TYPE item_type)
{
}

void Collectable::collected(Character actor) const
{
	switch(item_type_)
	{
	case SPEEDBUFF: actor.set_speed_buff(100.0f, 10.0f); break;	
	case COIN: actor.set_stat_score(actor.get_stats().score + 1);
	default: ;
	}
}


