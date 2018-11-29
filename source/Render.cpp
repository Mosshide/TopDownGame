#include "Render.h" 

Render::Render()
{
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

	//initialize the screen variable
	FULLSCREEN = false;
	SDL_GetDisplayBounds(0, &rctDisplay);
	if (FULLSCREEN)
	{
		SCREEN_WIDTH = rctDisplay.w;
		SCREEN_HEIGHT = rctDisplay.h;
	}
	else
	{
		SCREEN_WIDTH = 1600;//1366
		SCREEN_HEIGHT = 900;//768
	}
	SCREEN_BPP = 32;
	//initialize the sdl goodies
	window = nullptr;
	windowIcon = nullptr;

	//Use OpenGL 2.1
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

	 //Set up the screen
    if (!FULLSCREEN) window = SDL_CreateWindow("KernQuest",
							SDL_WINDOWPOS_UNDEFINED,
							SDL_WINDOWPOS_UNDEFINED,
                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
	else window = SDL_CreateWindow("KernQuest",
							SDL_WINDOWPOS_UNDEFINED,
							SDL_WINDOWPOS_UNDEFINED,
                          SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);

	//Create context
	gContext = SDL_GL_CreateContext(window);
	
	glewExperimental = GL_TRUE;
	glewInit();
	SDL_GL_SetSwapInterval(0);
	initGL();

	//Get window dimensions
	WIDTH_RATIO = (float)SCREEN_WIDTH/(float)rctDisplay.w;
	HEIGHT_RATIO = (float)SCREEN_HEIGHT/(float)rctDisplay.h;

	srand(int(time(nullptr)));

	windowIcon = IMG_Load("img/ael icon.png");
	SDL_SetWindowIcon(window, windowIcon);

	//Open the font
    font[0] = TTF_OpenFont("fonts/AozoraMinchoRegular.ttf", 18);
	font[1] = TTF_OpenFont("fonts/AozoraMinchoRegular.ttf", 12);
	font[2] = TTF_OpenFont("fonts/AozoraMinchoRegular.ttf", 50);
	TTF_SizeText(font[0], "0", nullptr, &fontSize[0]);
	TTF_SizeText(font[1], "1", nullptr, &fontSize[1]);
	TTF_SizeText(font[2], "2", nullptr, &fontSize[2]);

	BASE_FRAME_TIME = 250;
}

Render::~Render()
{
	//delete[] _v;

	stopGL();

	 //Free the surface
	SDL_DestroyWindow(window);
	SDL_FreeSurface(windowIcon);

	//Close the font
    TTF_CloseFont(font[0]);
	TTF_CloseFont(font[1]);
	TTF_CloseFont(font[2]);
}

bool Render::initGL()
{
	bool success = true;
	GLenum error = GL_NO_ERROR;

	//Initialize clear color
	glClearColor(.1f, .1f, .8f, 1.f);

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	//glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glDisable(GL_DEPTH_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


	/*glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_LINE_SMOOTH);
	glEnable(GL_POLYGON_SMOOTH);*/

	//previously in vbo draw
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);

	//Check for error
	error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error initializing OpenGL! %s\n", gluErrorString(error));
		success = false;
	}

	_cameraPos = glm::vec3(2.0f, 2.0f, 5.0f);
	_cameraFront = glm::vec3(0.0f, -2.0f, -1.0f);
	_cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

	//Load basic shader program
	if (!_shader.loadProgram("shaders/shader"))
	{
		printf("Unable to load basic shader!\n");
		return false;
	}
	

	///////Set up basic shader
	//Bind basic shader program
	_shader.bind();

	//Initialize projection
	//_shader.setProjection(glm::ortho<GLfloat>(0.0, (float)SCREEN_WIDTH / SCREEN_HEIGHT, 1.f, 0.0, -100.0, 100.0));
	_shader.setProjection(glm::perspective(glm::radians(80.0f), /*1.f*/(float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f));
	//_shader.setProjection(glm::perspective(glm::radians(80.0f), /*1.f*/(float)SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f));
	_shader.updateProjection();

	//Initialize camera
	_shader.setCamera(glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp));
	_shader.updateCamera();

	//Initialize modelview
	_shader.setModelView(glm::mat4(), glm::mat4(), glm::mat4());
	_shader.updateModelView();

	_shader.setTextureUnit(0);

	_shader.enableVertexPointer();
	_shader.enableNormalPointer();
	_shader.enableTexCoordPointer();

	_shader.unbind();


	//Load basic shader program
	if (!_UIShader.loadProgram("shaders/shaderUI"))
	{
		printf("Unable to load UI shader!\n");
		return false;
	}


	///////set up ui shader
	//Bind basic shader program
	_UIShader.bind();

	//Initialize projection
	_UIShader.setProjection(glm::ortho<GLfloat>(0.0, (GLfloat)SCREEN_WIDTH, (GLfloat)SCREEN_HEIGHT, 0.0, 1.0, -1.0));
	_UIShader.updateProjection();

	_UIShader.setTextureUnit(0);

	_UIShader.enableVertexPointer();
	_UIShader.enableTexCoordPointer();

	return success;
}

void Render::stopGL()
{
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);

	_shader.disableVertexPointer();
	_shader.disableTexCoordPointer();

	_shader.unbind();
	_shader.disableVertexPointer();
	_shader.disableTexCoordPointer();
}

SDL_Surface* Render::_surfRender_text(std::string textureText, SDL_Color textColor, TTF_Font* ft)
{
	//cout << textureText << endl;
	//SDL_Texture* tempT;

	//Render text surface
	SDL_Surface* textSurface = TTF_RenderText_Blended(ft, textureText.c_str(), textColor);
	if( textSurface == nullptr )
	{
		printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError() );
	}

	//Create texture from surface pixels
    /*tempT = SDL_CreateTextureFromSurface(renderer, textSurface);
	if(tempT == nullptr)
	{
		printf("Unable to create texture from rendered text! SDL Error: \n");
	}
	else
	{
		SDL_SetTextureBlendMode(tempT, SDL_BLENDMODE_BLEND);
	}*/

	//Get rid of old surface
	//SDL_FreeSurface(textSurface);
	
	//Return success
	return textSurface;
}

void Render::renderGame()
{
	updateCamera();

	SDL_GL_SwapWindow(window);
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}


//
void Render::setScreenSize(int w, int h)
{
	if (w < 0) w = SCREEN_WIDTH;
	if (h < 0) h = SCREEN_HEIGHT;
	
	SDL_SetWindowSize(window, w, h);
}

void Render::setFullscreen(bool f)
{
	if (f) SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
	else SDL_SetWindowFullscreen(window, 0);
}

int Render::getScreenWidth()
{
	return SCREEN_WIDTH;
}

int Render::getScreenHeight()
{
	return SCREEN_HEIGHT;
}

fRectangle Render::getScreen()
{
	return fRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

float Render::getWidthRatio()
{
	return WIDTH_RATIO;
}

float Render::getHeightRatio()
{
	return HEIGHT_RATIO;
}

bool Render::getFullscreen()
{
	return FULLSCREEN;
}

void Render::setBaseFrameTime(int t)
{
	BASE_FRAME_TIME = t;
}

int Render::getBaseFrameTime()
{
	return BASE_FRAME_TIME;
}

GLfloat Render::getFontSize(int f)
{
	return (GLfloat)fontSize[f];
}

GLfloat Render::getTextWidth(std::string t, int f)
{
	int tw = 0;
	TTF_SizeText(font[f], t.c_str(), &tw, nullptr);
	return (GLfloat)tw;
}

void Render::setLight(glm::vec3 pos)
{
	_shader.setLight(pos);
	_shader.updateLight();
}

void Render::setCamera(glm::vec3 pos, glm::vec3 YPR, glm::vec3 up)
{
	_cameraPos = pos;
	_yawPitchRoll = YPR;
	_cameraFront = glm::vec3(cos(_yawPitchRoll.x) * cos(_yawPitchRoll.y), sin(_yawPitchRoll.y), sin(_yawPitchRoll.x) * cos(_yawPitchRoll.y));
	_cameraUp = up;
}

void Render::translateCamera(glm::vec3 p)
{
	_cameraPos += p;
}

void Render::rotateCamera(glm::vec3 q)
{
	_yawPitchRoll += q;
	if (_yawPitchRoll.x < 0) while (_yawPitchRoll.x < 0) _yawPitchRoll.x += 2 * (float)M_PI;
	if (_yawPitchRoll.x > 2 * (float)M_PI) while (_yawPitchRoll.x > 2 * (float)M_PI) _yawPitchRoll.x -= 2 * (float)M_PI;

	//if pitch becomes parallel to "look at"'s up vec: bad things
	//if (_yawPitchRoll.y < -(float)M_PI / 2) _yawPitchRoll.y = -(float)M_PI / 2;
	//if (_yawPitchRoll.y > (float)M_PI / 2) _yawPitchRoll.y = (float)M_PI / 2;
	if (_yawPitchRoll.y < -(float)M_PI / 2 + .15) _yawPitchRoll.y = -(float)M_PI / 2 + .15;
	if (_yawPitchRoll.y >(float)M_PI / 2 - .15) _yawPitchRoll.y = (float)M_PI / 2 - .15;

	if (_yawPitchRoll.z < -(float)M_PI / 2) _yawPitchRoll.z = -(float)M_PI / 2;
	if (_yawPitchRoll.z > (float)M_PI / 2) _yawPitchRoll.z = (float)M_PI / 2;

	_cameraFront = glm::vec3(cos(_yawPitchRoll.x) * cos(_yawPitchRoll.y), sin(_yawPitchRoll.y), sin(_yawPitchRoll.x) * cos(_yawPitchRoll.y));
}

void Render::setCameraTranslate(glm::vec3 p)
{
	_cameraPos = p;
}

void Render::setCameraRotate(glm::vec3 q)
{
	_yawPitchRoll = q;
	if (_yawPitchRoll.x < 0) while (_yawPitchRoll.x < 0) _yawPitchRoll.x += 2 * (float)M_PI;
	if (_yawPitchRoll.x > 2 * (float)M_PI) while (_yawPitchRoll.x > 2 * (float)M_PI) _yawPitchRoll.x -= 2 * (float)M_PI;

	if (_yawPitchRoll.y < -(float)M_PI / 2) while (_yawPitchRoll.y < -(float)M_PI / 2) _yawPitchRoll.y += (float)M_PI;
	if (_yawPitchRoll.y >(float)M_PI / 2) while (_yawPitchRoll.y > (float)M_PI / 2) _yawPitchRoll.y -= (float)M_PI;

	if (_yawPitchRoll.z < -(float)M_PI / 2) while (_yawPitchRoll.z < -(float)M_PI / 2) _yawPitchRoll.z += (float)M_PI;
	if (_yawPitchRoll.z >(float)M_PI / 2) while (_yawPitchRoll.z >(float)M_PI / 2) _yawPitchRoll.z -= (float)M_PI;
	
	_cameraFront = glm::vec3(cos(_yawPitchRoll.x) * cos(_yawPitchRoll.y), sin(_yawPitchRoll.y), sin(_yawPitchRoll.x) * cos(_yawPitchRoll.y));
}

void Render::updateCamera()
{
	_shader.setCamera(glm::lookAt(_cameraPos, _cameraPos + _cameraFront, _cameraUp));
	_shader.updateCamera();
}

Shader* Render::getShader()
{
	return &_shader;
}

UIShader* Render::getUIShader()
{
	return &_UIShader;
}

//void Render::initWorldVBO()
//{
//	//If VBO does not already exist
//	if (_worldVbo == 0)
//	{
//		//Vertex data
//		VertexData2D *vData = new VertexData2D[_numWorldVertices];
//		GLuint *iData = new GLuint[_numWorldVertices];
//
//		//Set rendering indices
//		iData[0] = 0;
//		iData[1] = 1;
//		iData[2] = 2;
//		iData[3] = 3;
//
//		//Create VBO
//		glGenBuffers(1, &_worldVbo);
//		glBindBuffer(GL_ARRAY_BUFFER, _worldVbo);
//		glBufferData(GL_ARRAY_BUFFER, _numWorldVertices * sizeof(VertexData2D), vData, GL_DYNAMIC_DRAW);
//
//		//Create IBO
//		glGenBuffers(1, &_worldIbo);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _worldIbo);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numWorldVertices * sizeof(GLuint), iData, GL_DYNAMIC_DRAW);
//
//		//Unbind buffers
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//		delete[] vData;
//		delete[] iData;
//	}
//}
//
//void Render::freeWorldVBO()
//{
//	//Free VBO and IBO
//	if (_worldVbo != 0)
//	{
//		glDeleteBuffers(1, &_worldVbo);
//		glDeleteBuffers(1, &_worldIbo);
//		_worldVbo = 0;
//		_worldIbo = 0;
//	}
//}
//
//void Render::initUIVBO()
//{
//	//If VBO does not already exist
//	if (_UIVbo == 0)
//	{
//		//Vertex data
//		VertexData2D *vData = new VertexData2D[_numUIVertices];
//		GLuint *iData = new GLuint[_numUIVertices];
//
//		//Set rendering indices
//		iData[0] = 0;
//		iData[1] = 1;
//		iData[2] = 2;
//		iData[3] = 3;
//
//		//Create VBO
//		glGenBuffers(1, &_UIVbo);
//		glBindBuffer(GL_ARRAY_BUFFER, _UIVbo);
//		glBufferData(GL_ARRAY_BUFFER, _numUIVertices * sizeof(VertexData2D), vData, GL_DYNAMIC_DRAW);
//
//		//Create IBO
//		glGenBuffers(1, &_UIIbo);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _UIIbo);
//		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numUIVertices * sizeof(GLuint), iData, GL_DYNAMIC_DRAW);
//
//		//Unbind buffers
//		glBindBuffer(GL_ARRAY_BUFFER, 0);
//		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
//
//		delete[] vData;
//		delete[] iData;
//	}
//}
//
//void Render::freeUIVBO()
//{
//	//Free VBO and IBO
//	if (_UIVbo != 0)
//	{
//		glDeleteBuffers(1, &_UIVbo);
//		glDeleteBuffers(1, &_UIIbo);
//		_UIVbo = 0;
//		_UIIbo = 0;
//	}
//}

Render *render;

char _buffer[33];
GLfloat TILE_WIDTH = .15;
GLfloat TILE_HEIGHT = .15;
const int MAX_CHARACTERS = 100;
const int MAX_ENEMIES = 20;
const int MAX_ATTACKS = 1000;
const int TOOLTIP_DELAY = 1000;