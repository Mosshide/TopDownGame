#pragma once
#include "TextureBox.h"
#include "TerrainType.h"
#include "UIRectangle.h"

class Node
{
public:
	Node(int a, int b, TerrainType t);
	~Node();
	void tryNewParent(int a, int b, int pc, int cost);
	SDL_Point getPoint();
	SDL_Point getPPoint();

	int x; //node x location
	int y; //node y location
	int px; //parent x location
	int py; //parent y location
	TerrainType terrain;
	bool occupied;
	int c; //path cost
	short checkState;
};

///////

class Navigation
{
public:
	Navigation();
	~Navigation();
	void draw();
	void initVBO(int w, int h);
	void freeVBO();
	void setNodes(int w, int h);
	std::vector<Vertex> getPath(Vertex a, Vertex b, std::vector<TerrainType> *ok);
	void setTerrain(int x, int y, TerrainType t);
	void setTerrain(SDL_Point p, TerrainType t) { return setTerrain(p.x, p.y, t); }
	TerrainType getTerrain(int x, int y);
	TerrainType getTerrain(SDL_Point p) { return getTerrain(p.x, p.y); }
	void setOccupied(int x, int y, bool o);
	void setOccupied(SDL_Point p, bool o) { return setOccupied(p.x, p.y, o); }
	bool getOccupied(int x, int y);
	bool getOccupied(SDL_Point p) { return getOccupied(p.x, p.y); }
	SDL_Point getClosestTile(GLfloat x, GLfloat y);
	SDL_Point getClosestTile(int x, int y) { return getClosestTile((GLfloat)x, (GLfloat)y); }
	SDL_Point getClosestTile(Vertex v) { return getClosestTile(v.x, v.y); }
	bool checkterrainOK(TerrainType tt, std::vector<TerrainType> *ok);
	Vertex getStartPoint();
	Vertex getEndPoint();

	SDL_Point goal;
	int wTiled;
	int hTiled;
	std::vector<Node> nodes;
	int startEnd[2];

private:
	void resetNodes();
	bool checkNode(int x, int y, int px, int py, int cost, std::vector<TerrainType> *ok, std::vector<Node*> *tnodes);
	std::vector<Vertex> refinePath(std::vector<Vertex> path, Vertex a, Vertex  b, std::vector<TerrainType> *ok);
	bool straightPath(Vertex a, Vertex b, std::vector<TerrainType> *ok);

	UIRectangle _rect;

	//render
	VertexData3D *vertices;
	Shader *_shader;
	VIBO _vibo;
};

extern Navigation *gameNavigation;