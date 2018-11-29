#pragma once
#include "TextureBox.h"
#include "Model.h"

class Player
{
public:
	Player();
	~Player();
	void update();
	void draw();

	glm::vec3 camera;
	glm::vec3 position;

private:
	Model _model;

	V3rtex _vel;
	V3rtex _acc;
	V3rtex _maxVel;
};