#pragma once
#include "GameState.h"
#include "MapChooser.h"
#include "GameMap.h"
#include "EscMenu.h"
#include "NumEntryBox.h"

class MapMakingState : public GameState
{
public:
	MapMakingState();
	~MapMakingState();
	StateEnum update();
	void draw();
	void drawUI();

private:
	MapChooser _mc;
	GameMap _map;
	EscMenu _escMenu;

	NumEntryBox _widthBox;
	NumEntryBox _heightBox;
	Button _submitSize;

	UIText _cPos;
	UIText _cFront;
	UIText _cYPR;

	bool _firstPersonMode;
	glm::vec3 _firstPersonCamera;
};

