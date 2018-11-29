#include "DesiredObject.h"

DesiredObject::DesiredObject()
{
	presence.x = 0;
	presence.y = 0;
	presence.w = 32;
	presence.h = 32;

	selected = false;
	isEmbarrassed = false;
	held = false;

	placeable.push_back(trnGrass);
	placeable.push_back(trnDarkGrass);
	placeable.push_back(trnBuilding);
	placeable.push_back(trnEntrance);

	_model.setModel("ghost");
	_model.setTxr("rabbit");
	_model.setScalar(.1f);
	//_model.setScalar(1.f);

	realign();
}

DesiredObject::~DesiredObject()
{
}

void DesiredObject::update()
{
	_model.setTranslation(presence.x, .1f, presence.y);

	if ((mouse->getClick(leftClick) == clickDown) && _model.isPicked())
	{
		selected = true;
		_old = { _model.getTranslation()[3][0], _model.getTranslation()[3][2] };
	}
	else if (selected && (mouse->getClick(leftClick) == clickUp))
	{
		selected = false;
		if (isEmbarrassed)
		{
			setPosition(_old.x, _old.y);
		}
		isEmbarrassed = false;
	}
}

void DesiredObject::draw()
{
	if (isEmbarrassed) _model.setColor(Color(1.f, 0.f, 0.f, 1.f));
	else _model.setColor(Color(1.f, 1.f, 1.f, 1.f));

	_model.draw();
}