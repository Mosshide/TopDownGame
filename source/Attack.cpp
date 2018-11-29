#include "Attack.h"

Attack::Attack()
{
	dead = true;
	presence.w = 16;
	presence.h = 16;

	targetID = nullptr;
	stats = nullptr;

	//realign();

	_model.setTxr("dryad");
	_model.setModel("arrow");
	_model.setScalar(.01f, .05f, .05f);

	_nav.speed = .001f;
}

Attack::~Attack()
{
}

void Attack::update()
{
	if (!dead)
	{
		if (targetID != nullptr)
		{
			if (targetID->dead) targetID = nullptr;
			else
			{
				_nav.target.x = targetID->presence.x;
				_nav.target.y = targetID->presence.y;
			}
		}
		//else printf("Attack has no target.\n");
		_nav.moveTowardTarget(presence);
		_model.setTranslation(presence.x, .1f, presence.y);
		_model.setRotation(_nav.direction, glm::vec3(0, 1, 0));

		if (_nav.nextNode)
		{
			dead = true;
			if (targetID != nullptr && stats != nullptr)
			{
				targetID->health -= stats->mod[bluntDamageMod];
				targetID = nullptr;
			}
		}
	}
}

void Attack::draw()
{
	if (!dead)
	{
		_model.draw();
	}
}

//void Attack::realign()
//{
//	_ani.setPosition(presence.x - (presence.w / 2), presence.y - (presence.h / 2));
//}