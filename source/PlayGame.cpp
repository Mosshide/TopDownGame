#include "PlayGame.h"

PlayGame::PlayGame()
{
	//Initialize all SDL subsystems
    SDL_Init(SDL_INIT_EVERYTHING);

	//init png code
	IMG_Init( IMG_INIT_PNG );

	//Initialize SDL_ttf
    TTF_Init();

	_quit = false;
}

PlayGame::~PlayGame()
{
	delete render;
	delete _currentState;
	delete textureBox;
	delete modelBox;
	delete gameNavigation;

    //Quit SDL_ttf
    TTF_Quit();

	//Quit SDL_IMG
	IMG_Quit();

	//Quit SDL_mixer
    Mix_CloseAudio();

    //Quit SDL
    SDL_Quit();
}

void PlayGame::runGame()
{
	render = new Render;
	textureBox = new TextureBox;
	modelBox = new ModelBox;
	gameNavigation = new Navigation;
	_currentState = new MainMenuState;
	UserInput input;

	while(!_quit)
	{
		gameTimer.update();

		_quit = input.getInput();

		_stateRequest = _currentState->update();

		glEnable(GL_DEPTH_TEST);
		_currentState->draw();
		//render->getShader()->unbind();
		render->getUIShader()->bind();
		glDisable(GL_DEPTH_TEST);
		_currentState->drawUI();
		//render->getUIShader()->unbind();
		render->getShader()->bind();

		render->renderGame();

		if (_stateRequest == quitState) _quit = true;
		else if (_stateRequest != _currentState->id)
		{
			delete _currentState;

			switch (_stateRequest)
			{
			case mainMenuState:
				_currentState = new MainMenuState;
				break;
			case infiniteGameState:
				_currentState = new InfiniteGameState;
				break;
			case mapMakingState:
				_currentState = new MapMakingState;
				break;
			}
		}
	}
}