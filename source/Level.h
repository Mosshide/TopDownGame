#pragma once
#include <string>
#include "Enemy.h"
#include "LevelStats.h"
#include "Navigation.h"
#include "Attack.h"
#include "ImageButton.h"
#include "GameMap.h"

enum LevelStage
{
	levelCreate,
	levelSpawning,
	levelWait
};

class Level
{
public:
	Level();
	~Level();
	void update();
	void draw();
	void drawUI();
	Enemy* getClosestEnemy(Vertex v, int range);
	void receiveAttacks(Attack *a);
	void generateLevel(int l);
	int getBounty();
	void setMap(GameMap *m) { _map = m; }

	int damage;

private:
	GameMap *_map;
	std::vector<Enemy> _enemies;
	UIText _enemyDet;
	Button _startLvl;
	Button _placeEnemy;

	std::vector<Attack> _attacks;

	LevelStage _levelStage;
	int _currentLevel;
	int _spawnCD;
	int _spawnTime;
	int _spawnAmount;
	int _spawned;
	int _enemyHealth;
	bool _placing;

	int _bounty;

	std::vector<Vertex> _cachePath;
};

