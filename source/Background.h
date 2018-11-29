#pragma once
#include "UIRectangle.h"
#include "Entity.h"

class Background : public Entity
{
public:
	Background();
	~Background();
	void update();
	void draw();
	void drawClipped(fRectangle clip);
	void setPosition(GLfloat x, GLfloat y);
	void setSize(GLfloat w, GLfloat h);
	void setRGBA(Color c);
	void setBorder(int b);
	void realign();

private:
	UIRectangle _back;
	UIRectangle _front;
	int _border;
};

