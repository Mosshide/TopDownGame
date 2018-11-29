#include "Character.h"

Character::Character()
{
	presence.x = FLT_MAX;
	presence.y = FLT_MAX;
	presence.w = TILE_WIDTH * 2.f;
	presence.h = TILE_HEIGHT * 2.f;

	name = gameNames.grabName();

	inv.visible = false;
	inv.generateItems(1, 1.f);
	updateStats();
	
	_model.setTxr("dryad");
	_model.setModel("dryad");
	_model.setScalar(.005f);
	_model.setRotation(0.f, glm::vec3(0, 1, 0));

	_selection.setTxr("dryad");
	_selection.setModel("square");
	_selection.setScalar(10.f);
	//_selection.setRotation(0.f, glm::vec3(0, 1, 0));

	active = false;
	visible = true;
	selected = true;
	readyToAttack = false;
	isEmbarrassed = false;

	_attackCD = stats.mod[speedMod];

	_attack.stats = &stats;
	_attack.dead = false;

	navigator.pathable.push_back(trnGrass);
	navigator.pathable.push_back(trnDarkGrass);
	navigator.pathable.push_back(trnStone);
	//placeable.push_back(trnBed);
	navigator.speed = .0005f;

	level = nullptr;

	//realign();
}

Character::~Character()
{

}

void Character::update()
{
	inv.visible = false;
	inv.active = false;
	
	if (active)
	{
		update_attackCD();

		inv.update();

		navigator.moveAlongPath(presence);
		realign();

		if ((mouse->getClick(leftClick) == clickUp) && _model.isPicked())
		{
			selected = true;
		}
		else if (mouse->clicked[leftClick] == "none" ||
			mouse->clicked[leftClick] == "gamemap" ||
			mouse->clicked[leftClick] == "character")
		{
			selected = false;
		}

		if (selected)
		{
			if (mouse->getClick(rightClick) == clickUp)
			{
				printf("Click Up char: %d\n", rightClick);
				GLfloat dist = 0;
				if (rayPlaneCollision(render->getCameraPos(), mouse->worldRay, glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), &dist))
				{
					glm::vec3 target = render->getCameraPos() + mouse->worldRay * dist;
					if ((target.x > 0.f) && (target.x < gameNavigation->wTiled * TILE_WIDTH) && (target.z > 0.f) && (target.z < gameNavigation->hTiled * TILE_HEIGHT))
					{
						Vertex vTarget = { target.x, target.z };
						//printf("New Path: (%f, %f) -> (%f, %f)\n", presence.x, presence.y, vTarget.x, vTarget.y);
						navigator.setPath(gameNavigation->getPath(presence.getVertex(0), vTarget, &navigator.pathable));
					}
				}
			}
		}
	}
}

void Character::draw()
{
	if (visible)
	{
		if (isEmbarrassed) _model. setColor(Color(1.f, 0.f, 0.f, 1.f));
		else if (selected) _model.setColor(Color(.5f, 1.f, .5f, 1.f));
		else if (_model.isPicked()) _model.setColor(Color(.8f, 1.f, .8f, 1.f));
		else if (!active) _model.setColor(Color(.5f, .5f, .5f, 1.f));
		else _model.setColor(Color(1.f, 1.f, 1.f, 1.f));

		if (selected) _selection.draw();
		_model.draw();

		navigator.draw();
	}
}

void Character::drawUI()
{
	if (selected || _model.isPicked()) _details.visible = true;
	else _details.visible = false;

	_details.draw();
	inv.draw();
}

void Character::update_attackCD()
{
	if (_attackCD > 0)
	{
		_attackCD -= gameTimer.getDelta();
		if (_attackCD <= 0) readyToAttack = true;
	}
	else  _attackCD = 0;

	if (readyToAttack)
	{
		if (level != nullptr)
		{
			_attack.targetID = level->getClosestEnemy(presence.getVertex(0), stats.mod[rangeMod]);
			if (_attack.targetID != nullptr)
			{
				_attackCD = 1000 - stats.mod[speedMod];

				readyToAttack = false;

				level->receiveAttacks(&_attack);
			}
		}
		else printf("Character has no reference to level\n");
	}
}

void Character::realign()
{
	//_animation.setRect(fRectangle(presence.x - (presence.w / 2), presence.y - presence.h, presence.w, presence.h));
	//_btn.setRect(fRectangle(presence.x - (TILE_WIDTH / 2), presence.y - (TILE_HEIGHT / 2), (GLfloat)TILE_WIDTH, (GLfloat)TILE_HEIGHT));
	//_selection.setRect(fRectangle(presence.x - (TILE_WIDTH / 2), presence.y - (TILE_HEIGHT / 2), (GLfloat)TILE_WIDTH, (GLfloat)TILE_HEIGHT));
	_attack.setPosition(presence.x, presence.y);
	//_rangeIndicator.setCircle(presence.x, presence.y, (GLfloat)stats.mod[rangeMod]);
	_details.setPosition(270, render->getScreenHeight() - 48);
	_model.setTranslation(presence.x, .1f, presence.y);
	_selection.setTranslation(presence.x, .001f, presence.y);
}

void Character::toggleInventory()
{
	inv.visible = true;
	inv.active = true;
}

void Character::updateStats()
{
	stats.reset();
	stats = inv.getStats();
	stats.mod[rangeMod] += 100;
	_details.setText(stats.mod[bluntDamageMod]);
}