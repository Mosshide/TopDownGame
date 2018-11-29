#pragma once
#include "Entity.h"
#include "UITriangle.h"

class Origin : public Entity
{
public:
	Origin();
	~Origin();
	void update() {};
	void draw();

protected:
	UITriangle _vbo[3];
};

