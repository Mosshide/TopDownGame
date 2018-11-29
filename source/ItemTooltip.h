#pragma once
#include "Textbox.h"
#include "Item.h"

class ItemTooltip : public Entity
{
public:
	ItemTooltip();
	~ItemTooltip();
	void update();
	void draw();
	void setItem(Item *it);
	void sustain();
	void realign();

	bool visible;

private:
	Background _bg;
	UIText _name;
	UIText _desc;
	UIText _mod[12];
};

