#include "InfiniteGameState.h"

InfiniteGameState::InfiniteGameState()
{
	id = infiniteGameState;

	_paused = false;

	_map.load("maps/meme.nm");
	_map.play();
	
	_level.setMap(&_map);

	_party.setLevel(&_level);
	_party.setMap(&_map);

	render->setCamera(glm::vec3(1.5f, 5.0f, 2.0f), glm::vec3(-float(M_PI / 2), -float(M_PI / 3), 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	_text.setPosition(0, 50);
	_text.setText("0, 0, 0");
	_text1.setPosition(0, 75);
	_text1.setText("0, 0, 0");
	render->setLight(render->getCameraPos() + glm::vec3(0, 2, 0));

	
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

InfiniteGameState::~InfiniteGameState()
{
	
}

StateEnum InfiniteGameState::update()
{
	_fps.update();

	if (!_escMenu.active)
	{
		SDL_SetRelativeMouseMode(SDL_TRUE);

		_shop.update();

		_level.update();

		_party.chargeMoney(-_level.getBounty());

		_party.updateGame();

		_map.update();

		if (_shop.inventory.sellAttempt)
		{
			if (_party.openInventory())
			{
				if (_party.chargeMoney(_shop.inventory.getHighlightedPrice()))
				{
					_shop.inventory.addGold(_shop.inventory.getHighlightedPrice());
					_party.receiveItem(_shop.inventory.grabHighlightedItem());
				}
			}
		}

		_player.update();

		render->setCameraTranslate(_player.camera);
		render->setLight(_player.position);
		/////

		_text.setText(std::to_string((int)_player.camera.x) + ", " + std::to_string((int)_player.camera.y) + ", " + std::to_string((int)_player.camera.z));
		//_text1.setText(std::to_string(front.x) + ", " + std::to_string(front.y) + ", " + std::to_string(front.z));
	}
	else
	{
		SDL_SetRelativeMouseMode(SDL_FALSE);
	}

	//state changing
	if (_party.health <= 0) return mainMenuState;
	else return _escMenu.update();
}

void InfiniteGameState::draw()
{
	_map.draw();

	_level.draw();

	_party.draw();

	_player.draw();
}

void InfiniteGameState::drawUI()
{
	_level.drawUI();

	_party.drawUI();

	_shop.draw();

	_escMenu.draw();

	_text.draw();
	_text1.draw();

	_fps.draw();
}