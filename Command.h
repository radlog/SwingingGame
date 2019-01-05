#pragma once
#include "Character.h"

class Command
{
public:
	Command();
	~Command();

	void execute(Character actor) const;

private:
	GameData game_data_;
};

