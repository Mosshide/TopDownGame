#include "Player.h"

Player::Player()
{
	_vel = { 0.f, 0.f, 0.f };
	_acc = { 0.00000f };
	_maxVel = { .006f };
	camera = render->getCameraPos();
}

Player::~Player()
{
}

void Player::update()
{
	//camera controls
	//if (keyboard->getKey('w') == keyHold && keyboard->getKey('s') == keyNone)
	//{
	//	//cVel.y = copysign(cVel.y, 1);
	//	if (cVel.y < 0) cVel.y = 0;
	//	cVel.y = std::min(cVel.y + gameTimer.getDelta() * cAcc, cMaxVel);
	//}
	//else if (keyboard->getKey('s') == keyHold && keyboard->getKey('w') == keyNone)
	//{
	//	if (cVel.y > 0) cVel.y = 0;
	//	cVel.y = std::max(cVel.y - gameTimer.getDelta() * cAcc, -cMaxVel);
	//}
	//else cVel.y = copysign(1, cVel.y) * std::max(abs(cVel.y) - gameTimer.getDelta() * cAcc, 0.f);
	//if (keyboard->getKey('a') == keyHold && keyboard->getKey('d') == keyNone)
	//{
	//	if (cVel.x < 0) cVel.x = 0;
	//	cVel.x = std::min(cVel.x + gameTimer.getDelta() * cAcc, cMaxVel);
	//}
	//else if (keyboard->getKey('d') == keyHold && keyboard->getKey('a') == keyNone)
	//{
	//	if (cVel.x > 0) cVel.x = 0;
	//	cVel.x = std::max(cVel.x - gameTimer.getDelta() * cAcc, -cMaxVel);
	//}
	//else cVel.x = copysign(1, cVel.x) * std::max(abs(cVel.x) - gameTimer.getDelta() * cAcc, 0.f);

	//glm::vec3 front = render->getCameraFront();
	//glm::vec3 right = glm::rotate(render->getCameraFront(), (float)M_PI / 2, glm::vec3(0.f, 1.f, 0.f));
	//GLfloat deltaX = gameTimer.getDelta() * (cVel.x * right.x + cVel.y * front.x);
	//GLfloat deltaZ = gameTimer.getDelta() * (cVel.x * right.z + cVel.y * front.z);
	//render->translateCamera(glm::vec3(deltaX, 0, deltaZ));
	//render->setLight(render->getCameraPos() + glm::vec3(0, 2, 0));

	mouse->setPosition(render->getScreenWidth() / 2, render->getScreenHeight() / 2);

	render->rotateCamera(glm::vec3(mouse->dX / 100.f, mouse->dY / -100.f, 0));

	//if (keyboard->getKey(' ') == keyUp) _vel.y = .003f;
	//_vel.y -= .00001;
	//if (_yVel < -.01f) _yVel = -.01f;
	//camera.y += _yVel;
	//if (camera.y < .2f) camera.y = .2f;
	//render->setCameraTranslate(camera);

	//float forwardVelocity = 0.f;
	//float sideVelocity = 0.f;
	//if (keyboard->getKey('w') == keyHold) forwardVelocity += .01f;
	//if (keyboard->getKey('s') == keyHold) forwardVelocity -= .01f;
	//if (keyboard->getKey('a') == keyHold) sideVelocity += .01f;
	//if (keyboard->getKey('d') == keyHold) sideVelocity -= .01f;
	//glm::vec3 front = render->getCameraFront();
	//front.y = camera.y;
	//front = glm::normalize(front);
	//glm::vec3 right = glm::rotate(front, (float)M_PI / 2, glm::vec3(0.f, 1.f, 0.f));
	//GLfloat deltaX = (sideVelocity * right.x + forwardVelocity * front.x);// *(render->getCameraPos().y / 2);
	//GLfloat deltaZ = (sideVelocity * right.z + forwardVelocity * front.z);// *(render->getCameraPos().y / 2);
	//camera.x += deltaX / 7.0f;
	//camera.z += deltaZ / 7.0f;

	_model.setTranslation(position.x, position.y, position.z);
}

void Player::draw()
{
	_model.draw();
}