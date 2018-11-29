#pragma once
#include "ImageButton.h"
#include "Button.h"
#include "Character.h"
#include "TextEntryBox.h"
#include "NameGrabber.h"
#include "CharacterEdit.h"
#include "Level.h"
#include "Inventory.h"
#include "GameMap.h"

class Party
{
public:
	Party();
	~Party();
	void updateGame();
	void draw();
	void drawUI();
	void setLevel(Level *l);
	bool createCharacter();
	bool chargeMoney(int amount);
	bool openInventory();
	void receiveItem(Item i);
	void setMap(GameMap *m) { _map = m; }

	int health;

private:
	Level *_level;
	GameMap *_map;

	Character **_characterSort;
	int _latestCharacter;
	bool _placing;
	int _characterCost;
	bool _openInv;
	Item *_holdItem;

	UIText _moneyTxt;
	int _money;

	UIText _healthTxt;

	Button _addChar;
	Button _toggleInventory;
};

