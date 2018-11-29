#include "MapMakingState.h"

MapMakingState::MapMakingState()
{
	id = mapMakingState;
	_mc.enable();
	_escMenu.state = mapMakingState;

	_widthBox.setPosition(300.f, 400.f);
	_heightBox.setPosition(300.f, 450.f);
	_submitSize.setText("Set Size");
	_submitSize.setPosition(400.f, 350.f);

	_firstPersonMode = false;
	render->setCamera(glm::vec3(2.0f, 1.0f, 5.0f), glm::vec3(-float(M_PI / 2), -float(M_PI / 3), 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	render->setLight(render->getCameraPos() + glm::vec3(0, 2, 0));
	_cPos.setText("0, 0, 0");
	_cFront.setText("0, 0, 0");
	_cFront.setPosition(0.f, 30.f);
	_cYPR.setText("0, 0, 0");
	_cYPR.setPosition(0.f, 60.f);
}

MapMakingState::~MapMakingState()
{
}

StateEnum MapMakingState::update()
{
	if (!_escMenu.active)
	{
		if (keyboard->getKey('f') == keyUp)
		{
			_firstPersonMode = !_firstPersonMode;
			SDL_SetRelativeMouseMode((SDL_bool)_firstPersonMode);
			_firstPersonCamera = render->getCameraPos();
		}

		if (_firstPersonMode)
		{
			mouse->setPosition(render->getScreenWidth() / 2, render->getScreenHeight() / 2);


			render->rotateCamera(glm::vec3(mouse->dX / 100.f, mouse->dY / -100.f, 0));
			render->setLight(render->getCameraPos() + glm::vec3(0, 2, 0));

			float forwardVelocity = 0.f;
			float sideVelocity = 0.f;
			if (keyboard->getKey('w') == keyHold) forwardVelocity += .01f;
			if (keyboard->getKey('s') == keyHold) forwardVelocity -= .01f;
			if (keyboard->getKey('a') == keyHold) sideVelocity += .01f;
			if (keyboard->getKey('d') == keyHold) sideVelocity -= .01f;
			glm::vec3 front = render->getCameraFront();
			glm::vec3 right = glm::rotate(render->getCameraFront(), (float)M_PI / 2, glm::vec3(0.f, 1.f, 0.f));
			GLfloat deltaX = (sideVelocity * right.x + forwardVelocity * front.x) * (render->getCameraPos().y / 2);
			GLfloat deltaZ = (sideVelocity * right.z + forwardVelocity * front.z) * (render->getCameraPos().y / 2);
			_firstPersonCamera.x += deltaX;
			_firstPersonCamera.z += deltaZ;

			_firstPersonCamera.y -= .01f;
			if (_firstPersonCamera.y < .2f) _firstPersonCamera.y = .2f;

			render->setCameraTranslate(_firstPersonCamera);
			render->setLight(render->getCameraPos() + glm::vec3(0, 2, 0));
		}
		else
		{
			if (mouse->getClick(rightClick) == clickHold)
			{
				glm::vec3 front = render->getCameraFront();
				glm::vec3 right = glm::rotate(render->getCameraFront(), (float)M_PI / 2, glm::vec3(0.f, 1.f, 0.f));
				GLfloat deltaX = ((mouse->dX / 100.f) * right.x + (mouse->dY / 100.f) * front.x) * (render->getCameraPos().y / 2);
				GLfloat deltaZ = ((mouse->dX / 100.f) * right.z + (mouse->dY / 100.f) * front.z) * (render->getCameraPos().y / 2);
				render->translateCamera(glm::vec3(deltaX, 0, deltaZ));
				render->setLight(render->getCameraPos() + glm::vec3(0, 2, 0));
				mouse->lockClick("move camera", rightClick);
			}

			if (mouse->getClick(middleClick) == clickHold)
			{
				render->rotateCamera(glm::vec3(mouse->dX / 50.f, mouse->dY / -50.f, 0));
				render->setLight(render->getCameraPos() + glm::vec3(0, 2, 0));
				mouse->lockClick("move camera", middleClick);
			}

			if (mouse->wheel != 0)
			{
				render->translateCamera(glm::vec3(0, -mouse->wheel * (render->getCameraPos().y / 20), 0));
				render->setLight(render->getCameraPos() + glm::vec3(0, 2, 0));
			}
		}

		_mc.update();
		if (_mc.mapName != "")
		{
			_map.load("maps/" + _mc.mapName);
			_mc.disable();
			_map.edit();
		}

		_map.update();



		glm::vec3 cP = render->getCameraPos();
		_cPos.setText(std::to_string((int)cP.x) + ", " + std::to_string((int)cP.y) + ", " + std::to_string((int)cP.z));
		glm::vec3 cF = render->getCameraFront();
		_cFront.setText(std::to_string(cF.x) + ", " + std::to_string(cF.y) + ", " + std::to_string(cF.z));
		glm::vec3 cYPR = render->getCameraYPR();
		_cYPR.setText(std::to_string(cYPR.x) + ", " + std::to_string(cYPR.y) + ", " + std::to_string(cYPR.z));
	}
	else
	{
		_widthBox.update();
		_heightBox.update();
		_submitSize.update();

		if (_submitSize.getClick(leftClick) == clickUp)
		{
			_map.setTiles(_widthBox.value, _heightBox.value);
		}
	}

	if (_mc.cancel) return mainMenuState;
	else return _escMenu.update();
}

void MapMakingState::draw()
{
	_map.draw();
}

void MapMakingState::drawUI()
{
	_map.drawUI();

	_mc.draw();
	_escMenu.draw();

	if (_escMenu.active)
	{
		_widthBox.draw();
		_heightBox.draw();
		_submitSize.draw();
	}

	_cPos.draw();
	_cFront.draw();
	_cYPR.draw();
}