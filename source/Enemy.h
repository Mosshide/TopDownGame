#pragma once
#include "Model.h"
#include "CombatStats.h"
#include "Navigator.h"
#include "Entity.h"
#include "Searcher.h"

class Enemy : public Entity
{
public:
	Enemy();
	~Enemy();
	void reset();
	void update();
	void draw();
	//void setPath(std::vector<Vertex> p);
	std::vector<TerrainType> getPathable() { return navigator.pathable; }
	
	bool dead;
	bool active;
	bool ascended;
	bool isEmbarrassed;

	int health;
	Navigator navigator;
	Searcher searcher;

private:
	Model _model;
};

