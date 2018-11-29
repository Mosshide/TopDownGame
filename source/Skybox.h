#pragma once
#include "TextureBox.h"

class Skybox
{
public:
	Skybox();
	~Skybox();
	void update();
	void draw();
	void initVBO();
	void freeVBO();
	void setRGBA(Color c);

private:
	UIShader *_shader;
	GLuint _vbo;
	GLuint _ibo;
	int _numVertices;

	GLuint _txr;
	Color _color;
};

