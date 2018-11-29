#pragma once
#include "Render.h"
#include <map>

class toyBox
{
public:
	toyBox();
	~toyBox();
	VIBO load(std::string path);
	void initVBO(VIBO *vi, std::vector<VertexData3D> *vertices);
	void freeVBO(VIBO *vi);
	
	std::map<std::string, VIBO> model;
};

extern toyBox *toyBox;
