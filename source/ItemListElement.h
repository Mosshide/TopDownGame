#pragma once
#include "ListElement.h"

class ItemListElement : public ListElement
{
public:
	ItemListElement();
	~ItemListElement();
	void draw();
	void drawClipped(fRectangle clip);
	void realign();
	void setUI(bool b);

	UIRectangle itemImg;
	UIText itemName;
	UIText itemDes;
	UIText itemCost;
};

