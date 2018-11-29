#include "Item.h"

Item::Item()
{
	reset();
}

Item::~Item()
{
}

void Item::reset()
{
	level = 0;
	name = "";
	description = "";
	_basePrice = 0;
	price = 0;
}

void Item::generate(int l, ItemType i)
{
	type = i;
	_txr = "shirtIcon";
	level = l;
	name = gameNames.grabWeaponName();
	description = "Generated item.";
	_basePrice = rand() % 20 + l * 5;
	price = _basePrice;
	stats.mod[bluntDamageMod] = _basePrice;
	stats.mod[speedMod] = _basePrice;

	switch (type)
	{
	case itemTypeHead:
		_txr = "headArmorIcon";
		break;
	case itemTypeBack:
		_txr = "backArmorIcon";
		break;
	case itemTypeHand:
		_txr = "handArmorIcon";
		break;
	case itemTypeChest:
		_txr = "chestArmorIcon";
		break;
	case itemTypeLegs:
		_txr = "legArmorIcon";
		break;
	case itemTypeFoot:
		_txr = "footArmorIcon";
		break;
	case itemTypeWeapon:
		_txr = "weaponIcon";
		break;
	case itemTypeRing:
		_txr = "ringIcon";
		break;
	case itemTypeTrinket:
		_txr = "trinketIcon";
		break;
	}
}

void Item::modPrice(float m)
{
	price = int(_basePrice * m);
}

ListElement* Item::getListElement()
{
	ItemListElement *toAdd = new ItemListElement;
	toAdd->itemImg.setTexture(_txr);
	toAdd->itemName.setText(name);
	toAdd->itemDes.setText(description);
	toAdd->itemCost.setText(std::to_string(price));

	return toAdd;
}