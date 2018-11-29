#pragma once
#include "Render.h"
#include "Button.h"
#include "GameState.h"
#include "TextureBox.h"
#include "Origin.h"
#include "Model.h"

class MainMenuState : public GameState
{
public:
	MainMenuState();
	~MainMenuState();
	StateEnum update();
	void draw();
	void drawUI();

private:
	Button _startButton;
	Button _mapButton;
	Button _quitButton;

	UIRectangle _bg[2];

	GLfloat bgAlphaMod;
};