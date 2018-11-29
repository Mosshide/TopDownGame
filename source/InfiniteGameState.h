#pragma once
#include "GameState.h"
#include "Render.h"
#include "UserInput.h"
#include "EscMenu.h"
#include "Navigation.h"
#include "Party.h"
#include "ListContainer.h"
#include "Shop.h"
#include "GameMap.h"
#include <algorithm>
#include "FPSCounter.h"
#include "Player.h"

class InfiniteGameState : public GameState
{
public:
	InfiniteGameState();
	~InfiniteGameState();
	StateEnum update();
	void draw();
	void drawUI();

private:
	bool _paused;

	FPSCounter _fps;
	EscMenu _escMenu;
	GameMap _map;
	Level _level;
	Party _party;
	Shop _shop;

	UIText _text;
	UIText _text1;

	bool _firstPersonMode;

	Player _player;
};