#pragma once
#include "TerrainType.h"
#include "Navigation.h"

class Searcher
{
public:
	Searcher();
	~Searcher();
	void setTile(int x, int y);

	SDL_Point currentTile;
	SDL_Point nextTile;
};

