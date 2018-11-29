#include "Enemy.h"

Enemy::Enemy()
{
	_model.setTxr("rabbit");
	_model.setModel("rabbit");
	_model.setScalar(.005f);

	navigator.pathable.push_back(trnGrass);
	//navigator.pathable.push_back(trnEntrance);
	//navigator.pathable.push_back(trnBuilding);
	//_navigator.pathable.push_back(trnBed);
	navigator.speed = .0005f;

	isEmbarrassed = false;

	reset();
}

Enemy::~Enemy()
{
}

void Enemy::reset()
{
	dead = true;
	active = false;
	ascended = false;
	navigator.reachedTarget = false;
	health = 200;
	setPosition(0.f, FLT_MAX);
}

void Enemy::update()
{
	if (!dead)
	{
		if (active)
		{
			navigator.moveAlongPath(presence);

			if (navigator.reachedTarget)
			{
				dead = true;
				ascended = true;
			}
			if (health <= 0)
			{
				reset();
			}
		}

		_model.setTranslation(presence.x, .001f, presence.y);
		_model.setRotation(navigator.direction + float(M_PI / 2), glm::vec3(0, 1, 0));
	}
}

void Enemy::draw()
{
	if (!dead)
	{
		if (isEmbarrassed) _model.setColor(Color(1.f, 0.f, 0.f, 1.f));
		else _model.setColor(Color(1.f, 1.f, 1.f, 1.f));
		_model.draw();
		//_origin.draw();
	}
}