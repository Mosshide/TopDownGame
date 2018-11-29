#include "Level.h"

Level::Level()
{
	_currentLevel = 1;
	generateLevel(_currentLevel);
	_bounty = 0;

	_placing = false;

	_enemyDet.setPosition((render->getScreenWidth() / 2) - (_enemyDet.presence.w / 2), render->getScreenHeight() - 80.f);

	_startLvl.setText("Start Level");
	_startLvl.setPosition((render->getScreenWidth() / 2) - (_startLvl.presence.w / 2), render->getScreenHeight() - 40.f);

	_placeEnemy.setText("Place Enemy");
	_placeEnemy.setPosition((render->getScreenWidth() / 2) + 100.f, render->getScreenHeight() - 40.f);
}

Level::~Level()
{
}

void Level::update()
{
	if (_placing)
	{
		if (keyboard->getKey('\b') == keyUp)
		{
			_placing = false;
			_enemies.erase(_enemies.end() - 1);
		}
		else
		{
			SDL_Point tile = { 0, 0 };
			GLfloat dist = 0;
			if (rayPlaneCollision(render->getCameraPos(), mouse->worldRay, glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), &dist))
			{
				glm::vec3 target = render->getCameraPos() + mouse->worldRay * dist;
				if ((target.x > 0.f) && (target.x < _map->wTiled * TILE_WIDTH) && (target.z > 0.f) && (target.z < _map->hTiled * TILE_HEIGHT))
				{
					tile = { int(target.x / TILE_WIDTH), int(target.z / TILE_HEIGHT) };
				}
			}

			_enemies.back().setPosition((tile.x * TILE_WIDTH) + (TILE_WIDTH / 2),
				(tile.y * TILE_HEIGHT) + (TILE_HEIGHT / 2));
			_enemies.back().update();

			TerrainType type = gameNavigation->getTerrain(tile);
			bool okTile = gameNavigation->checkterrainOK(type, &_enemies.back().navigator.pathable);
			bool occ = gameNavigation->getOccupied(tile);

			if (okTile && !occ) _enemies.back().isEmbarrassed = false;
			else _enemies.back().isEmbarrassed = true;
			if (mouse->getClick(leftClick) == clickUp)
			{
				if (!_enemies.back().isEmbarrassed)
				{
					_enemies.back().active = true;
					_placing = false;

					//_nav->setOccupied(tile, true);
				}

				mouse->lockClick("lock in enemy", leftClick);
			}
		}
	}
	else
	{
		damage = 0;

		_placeEnemy.update();
		if (_placeEnemy.getClick(leftClick) == clickUp || keyboard->getKey('n') == keyHold)
		{
			Enemy e;
			e.dead = false;
			Vertex sp = gameNavigation->getStartPoint();
			e.setPosition(sp.x, sp.y);
			e.health = _enemyHealth;
			_enemies.push_back(e);

			_placing = true;
		}

		for (int i = 0; i < (int)_attacks.size(); i++)
		{
			_attacks[i].update();
		}

		for (int i = 0; i < (int)_enemies.size(); i++)
		{
			_enemies[i].update();
			if (_enemies[i].ascended)
			{
				damage++;
				_enemies.erase(_enemies.begin() + i);
				i--;
			}
			else if (_enemies[i].dead)
			{
				_enemies.erase(_enemies.begin() + i);
				i--;
			}
		}

		_startLvl.update();
		if (_levelStage == levelCreate)
		{
			if (_startLvl.getClick(leftClick) == clickUp)
			{
				_levelStage = levelSpawning;
				_startLvl.active = false;
			}
		}
		else if (_levelStage == levelSpawning)
		{
			_spawnCD -= gameTimer.getDelta();
			if (_spawnCD <= 0)
			{
				if (_spawned < _spawnAmount)
				{
					_spawnCD += _spawnTime;

					Enemy e;
					e.dead = false;
					Vertex sp = gameNavigation->getStartPoint();
					e.setPosition(sp.x, sp.y);
					e.health = _enemyHealth;
					e.active = true;
					_enemies.push_back(e);

					_spawned++;
				}
				else
				{
					_levelStage = levelWait;
				}
			}
		}
		else if (_levelStage == levelWait)
		{
			if (_enemies.size() > 0) generateLevel(_currentLevel + 1);
		}
	}
}

void Level::draw()
{
	for (int i = 0; i < (int)_attacks.size(); i++)
	{
		_attacks[i].draw();
	}

	for (int i = 0; i < (int)_enemies.size(); i++)
	{
		_enemies[i].draw();
	}
}

void Level::drawUI()
{
	_enemyDet.draw();
	_startLvl.draw();
	_placeEnemy.draw();
}

Enemy* Level::getClosestEnemy(Vertex v, int range)
{
	int closest = -1;
	float d = FLT_MAX;

	for (int i = 0; i < (int)_enemies.size(); i++)
	{
		if (_enemies[i].active)
		{
			float newD = distance(v, _enemies[i].presence.getVertex(0));

			if (newD <= range && newD < d)
			{
				closest = i;
				d = newD;
			}
		}
	}

	if (closest >= 0) return &_enemies[closest];
	else return nullptr;
}

void Level::receiveAttacks(Attack *a)
{
	_attacks.push_back(*a);
}

void Level::generateLevel(int l)
{
	_levelStage = levelCreate;
	_currentLevel = l;

	_spawnTime = 500;
	_spawnCD = _spawnTime;
	_spawnAmount = 10;
	_spawned = 0;
	_enemyHealth = 1 + (l * 5);

	_bounty = 10;

	_enemyDet.setText("Enemy Health: " + std::to_string(_enemyHealth));
	_startLvl.active = true;
}

int Level::getBounty()
{
	int b = _bounty;
	_bounty = 0;
	return b;
}