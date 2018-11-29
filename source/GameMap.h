#pragma once
#include "TextureBox.h"
#include "Navigation.h"
#include "UserInput.h"
#include "TileChooser.h"
#include "Firefly.h"
#include <iostream>
#include <fstream>
#include "DesiredObject.h"

class GameMap : public Entity
{
public:
	GameMap();
	~GameMap();
	void update();
	void draw();
	void drawUI();
	void enable();
	void disable();
	void load(std::string file);
	void save();
	void edit();
	void play();
	void setTiles(int w, int h);
	//Navigation *getNav() { return &_nav; }
	void initVBO();
	void freeVBO();

	int wTiled;
	int hTiled;

private:
	bool _enabled;
	std::string _fileName;
	bool _editing;

	Firefly _flies[5];

	//VBO _map;
	TileChooser _tc;
	TerrainType *_tiles;
	Model _highlight;
	int _highlightedTile;

	Vertex _centered;

	DesiredObject _theGoal;

	//rendering
	VertexData3D *vertices;
	int _numVertices;
	Shader *_shader;
	Color _color;
	glm::mat4 _rotation;
	fRectangle _clip;
	GLuint _vbo;
	GLuint _ibo;
	GLuint _txr;

	Model _walls;
};