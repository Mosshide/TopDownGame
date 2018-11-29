#pragma once
#include "Inventory.h"
#include "Attack.h"
#include "TextureBox.h"
#include "UICircle.h"
#include "Level.h"

class Character : public Entity
{
public:
	Character();
	~Character();
	void update();
	void draw();
	void drawUI();
	void realign();
	void toggleInventory();
	void updateStats();

	bool active;
	bool visible;
	bool selected;
	bool readyToAttack;
	bool isEmbarrassed;
	Inventory inv;

	std::string name;
	CombatStats stats;

	Navigator navigator;
	
	Level *level;

private:
	void update_attackCD();

	TextBox _details;

	Model _model;
	Model _selection;
	//UICircle _rangeIndicator;

	Attack _attack;
	
	//Button _btn;

	int _attackCD;
};

