#include "Collectable.h"


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
	case SPEEDBUFF: actor.set_speed_buff(100.0f, 10.0f); delete(this); break; // buff character who collected this with a speed buff
	case COIN: actor.set_stat_score(actor.get_stats().score + 1); delete(this); break; // give character one score point for collecting this
	default: ;
	}
}



