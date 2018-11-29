#include "Origin.h"

Origin::Origin()
{
	V3rtex a = { 0, 0, -.1f };
	V3rtex b = { 1.f, 0, 0 };
	V3rtex c = { 0, 0, .1f };
	_vbo[0].setTriangle(a, b, c);
	_vbo[0].setRGBA(Color(red));
	a = { -.1f, 0, 0 };
	b = { 0, 1.f, 0 };
	c = { .1f, 0, 0 };
	_vbo[1].setTriangle(a, b, c); 
	_vbo[1].setRGBA(Color(green));
	a = { 0, -.1f, 0 };
	b = { 0, 0, 1.f };
	c = { 0, .1f, 0 };
	_vbo[2].setTriangle(a, b, c);
	_vbo[2].setRGBA(Color(blue));
}

Origin::~Origin()
{
}

void Origin::draw()
{
	for (int i = 0; i < 3; i++)
	{
		_vbo[i].draw();
	}
}