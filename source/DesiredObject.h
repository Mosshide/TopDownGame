#pragma once
#include "Entity.h"
#include "TerrainType.h"
//#include "Level.h"
#include "UICircle.h"
#include "Model.h"

class DesiredObject : public Entity
{
public:
	DesiredObject();
	~DesiredObject();
	void update();
	void draw();

	bool selected;
	bool held;

	std::vector<TerrainType> placeable;

	//Level *level;
	bool isEmbarrassed;

private:
	Model _model;
	Vertex _old;
};

