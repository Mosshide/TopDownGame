#include "GameMap.h"

GameMap::GameMap()
{
	presence.x = 0;
	presence.y = 0;
	presence.w = 0;
	presence.h = 0;
	_enabled = false;
	wTiled = 0;
	hTiled = 0;
	_fileName = "*.*";
	_editing = false;
	_centered.x = 0;
	_centered.y = 0;

	_walls.setModel("wall");
	_walls.setTxr("wall");
	_walls.setScalar(.08f);

	_tiles = nullptr;
	//setTiles(15, 15);

	_highlight.setModel("square");
	_highlight.setScalar(10.f, 10.f, 10.f);

	vertices = nullptr;
	_shader = render->getShader();
	_txr = textureBox->texture["tileSet"];
}

GameMap::~GameMap()
{
	if (_tiles != nullptr)
	{
		delete[] _tiles;
	}

	freeVBO();
}

void GameMap::update()
{
	if (_enabled)
	{
		for (int i = 0; i < 5; i++)
		{
			_flies[i].update();
		}

		_highlightedTile = -1;
		GLfloat dist = 0;
		if (rayPlaneCollision(render->getCameraPos(), mouse->worldRay, glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), &dist))
		{
			glm::vec3 target = render->getCameraPos() + mouse->worldRay * dist;
			if ((target.x > 0.f) && (target.x < wTiled * TILE_WIDTH) && (target.z > 0.f) && (target.z < hTiled * TILE_HEIGHT))
			{
				_highlightedTile = (wTiled * int(target.z / TILE_HEIGHT)) +
					int(target.x / TILE_WIDTH);
			}
		}

		if (_editing)
		{
			if (keyboard->getKey('d') == keyUp) setTiles(70, 55);
			if (keyboard->getKey('s') == keyUp) save();
			//if (keyboard->getKey('c') == keyUp) _centered = gameCamera;

			_tc.update();
			if (_highlightedTile > -1)
			{
				_highlight.setTranslation((_highlightedTile % wTiled) * TILE_WIDTH + TILE_WIDTH / 2, 0.f,
					(_highlightedTile / wTiled) * TILE_HEIGHT + TILE_HEIGHT / 2);

				if (mouse->getClick(leftClick) == clickHold)
				{
					_tiles[_highlightedTile] = _tc.getTile();
					int vert = _highlightedTile * 6;
					fRectangle *clip = _tc.getTileClip(_tiles[_highlightedTile]);
					vertices[vert].texCoord.s = clip->x;
					vertices[vert].texCoord.t = clip->y;
					vertices[vert + 1].texCoord.s = clip->x + clip->w;
					vertices[vert + 1].texCoord.t = clip->y;
					vertices[vert + 2].texCoord.s = clip->x + clip->w;
					vertices[vert + 2].texCoord.t = clip->y + clip->h;

					vertices[vert + 3].texCoord.s = clip->x;
					vertices[vert + 3].texCoord.t = clip->y;
					vertices[vert + 4].texCoord.s = clip->x + clip->w;
					vertices[vert + 4].texCoord.t = clip->y + clip->h;
					vertices[vert + 5].texCoord.s = clip->x;
					vertices[vert + 5].texCoord.t = clip->y + clip->h;

					mouse->lockClick("edit tile", leftClick);
				}
			}
		}
		else
		{
			_theGoal.update();
			if (_theGoal.selected)
			{
				if (_highlightedTile > -1)
				{
					SDL_Point tile = { _highlightedTile % wTiled, _highlightedTile / wTiled };

					_theGoal.setPosition((tile.x * TILE_WIDTH) + (TILE_WIDTH / 2),
						(tile.y * TILE_HEIGHT) + (TILE_HEIGHT / 2));

					TerrainType type = gameNavigation->getTerrain(tile);
					bool okTile = gameNavigation->checkterrainOK(type, &_theGoal.placeable);
					bool occ = gameNavigation->getOccupied(tile);

					if (okTile && !occ) _theGoal.isEmbarrassed = false;
					else _theGoal.isEmbarrassed = true;
				}
			}

			GLfloat dist = 0;
			if (rayPlaneCollision(render->getCameraPos(), mouse->worldRay, glm::vec3(0, 1, 0), glm::vec3(0, 0, 0), &dist))
			{
				glm::vec3 target = render->getCameraPos() + mouse->worldRay * dist;
				_highlight.setTranslation(target.x, target.y, target.z);
			}
		}
	}
}

void GameMap::draw()
{
	if (_enabled)
	{
		_shader->setModelView(glm::mat4(), glm::mat4(), glm::mat4());
		_shader->updateModelView();

		//Set texture ID
		glBindTexture(GL_TEXTURE_2D, _txr);

		//_shader->enableVertexPointer();
		//_shader->enableTexCoordPointer();

		//Bind vertex buffer
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);

		//Update vertex buffer data
		glBufferSubData(GL_ARRAY_BUFFER, 0, _numVertices * sizeof(VertexData3D), vertices);

		//Set texture coordinate data
		_shader->setTexCoordPointer(sizeof(VertexData3D), (GLvoid*)offsetof(VertexData3D, texCoord));

		_shader->setNormalPointer(sizeof(VertexData3D), (GLvoid*)offsetof(VertexData3D, normals));

		//Set vertex data
		_shader->setVertexPointer(sizeof(VertexData3D), (GLvoid*)offsetof(VertexData3D, position));

		_shader->setTextureColor(_color);


		//Draw quad using vertex data and index data
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glDrawElements(GL_TRIANGLES, _numVertices, GL_UNSIGNED_INT, NULL);

		for (int i = 0; i < hTiled; i++)
		{
			for (int ii = 0; ii < wTiled; ii++)
			{
				if (_tiles[ii + i * wTiled] == trnCobble)
				{
					_walls.setTranslation(ii * TILE_WIDTH + TILE_WIDTH / 2, 0, i * TILE_HEIGHT + TILE_HEIGHT / 2);
					_walls.draw();
				}
			}
		}

		for (int i = 0; i < 5; i++)
		{
			//_flies[i].draw();
		}

		if (_editing)
		{
			if (_highlightedTile > -1 && _highlightedTile < wTiled * hTiled)
			{
				_highlight.draw();
			}
		}
		else
		{
			_theGoal.draw();
			//_highlight.draw();
		}

		//_nav.draw();
	}
}

void GameMap::drawUI()
{
	if (_editing)
	{
		_tc.draw();
	}
}

void GameMap::enable()
{
	_enabled = true;
}

void GameMap::disable()
{
	_enabled = false;
	_editing = false;
}

void GameMap::load(std::string file)
{
	setTiles(0, 0);

	_fileName = file;

	SDL_RWops *saveFile = SDL_RWFromFile(_fileName.c_str(), "r+b");

	if (saveFile != NULL)
	{
		//retrieve data
		int w, h = 0;
		SDL_RWread(saveFile, &w, sizeof(int), 1);
		SDL_RWread(saveFile, &h, sizeof(int), 1);
		//SDL_RWread(saveFile, &_centered, sizeof(Vertex), 1);
		//gameCamera = _centered;
		_theGoal.setPosition(.5f, .5f);
		setTiles(w, h);
		//wTiled = w;
		//hTiled = h;
		//presence.w = (float)wTiled * TILE_WIDTH;
		//presence.h = (float)hTiled * TILE_HEIGHT;
		for (int i = 0; i < hTiled * wTiled; i++)
		{
			//_tiles.push_back(TerrainType(0));
			SDL_RWread(saveFile, &_tiles[i], sizeof(TerrainType), 1);
		}

		fRectangle *clip;
		for (int i = 0; i < _numVertices; i += 6)
		{
			clip = _tc.getTileClip(_tiles[i / 6]);
			vertices[i].texCoord.s = clip->x;
			vertices[i].texCoord.t = clip->y;
			vertices[i + 1].texCoord.s = clip->x + clip->w;
			vertices[i + 1].texCoord.t = clip->y;
			vertices[i + 2].texCoord.s = clip->x + clip->w;
			vertices[i + 2].texCoord.t = clip->y + clip->h;

			vertices[i + 3].texCoord.s = clip->x;
			vertices[i + 3].texCoord.t = clip->y;
			vertices[i + 4].texCoord.s = clip->x + clip->w;
			vertices[i + 4].texCoord.t = clip->y + clip->h;
			vertices[i + 5].texCoord.s = clip->x;
			vertices[i + 5].texCoord.t = clip->y + clip->h;
		}
		

		//Close file handler
		SDL_RWclose(saveFile);		
	}
	else printf("Could not load: %s\n", SDL_GetError());
}

void GameMap::save()
{
	SDL_RWops *saveFile = SDL_RWFromFile(_fileName.c_str(), "w+b");
	if (saveFile != nullptr)
	{
		//save data
		SDL_RWwrite(saveFile, &wTiled, sizeof(int), 1);
		SDL_RWwrite(saveFile, &hTiled, sizeof(int), 1);
		//SDL_RWwrite(saveFile, &_centered, sizeof(Vertex), 1);

		for (int i = 0; i < hTiled * wTiled; i++)
		{
			SDL_RWwrite(saveFile, &_tiles[i], sizeof(TerrainType), 1);
		}

		//Close file handler
		SDL_RWclose(saveFile);
	}
	else printf("Could not save: %s\n", SDL_GetError());
}

void GameMap::edit()
{
	_editing = true;
	_enabled = true;
}

void GameMap::play()
{
	_editing = false;
	_enabled = true;
	for (int i = 0; i < hTiled; i++)
	{
		for (int ii = 0; ii < wTiled; ii++)
		{
			gameNavigation->setTerrain(ii, i, _tiles[ii + i * wTiled]);
		}
	}
}

void GameMap::setTiles(int w, int h)
{
	if (_tiles != nullptr)
	{
		TerrainType *temp = new TerrainType[w * h];
		for (int i = 0; i < h; i++)
		{
			for (int ii = 0; ii < w; ii++)
			{
				if (i < hTiled && ii < wTiled) temp[ii + i * w] = _tiles[ii + i * wTiled];
				else temp[ii + i * w] = trnGrass;
			}
		}

		delete[] _tiles;
		_tiles = temp;
	}
	else
	{
		_tiles = new TerrainType[w * h];
		for (int i = 0; i < h; i++)
		{
			for (int ii = 0; ii < w; ii++)
			{
				_tiles[ii + i * w] = trnGrass;
			}
		}
	}
	
	wTiled = w;
	hTiled = h;
	presence.w = (float)wTiled * TILE_WIDTH;
	presence.h = (float)hTiled * TILE_HEIGHT;

	///
	if (vertices != nullptr) delete[] vertices;
	_numVertices = wTiled * hTiled * 6;
	vertices = new VertexData3D[_numVertices];
	for (int i = 0; i < _numVertices; i++)
	{
		vertices[i].texCoord.s = 0;
		vertices[i].texCoord.t = 0;
		vertices[i].position.x = 0;
		vertices[i].position.y = 0;
		vertices[i].position.z = 0;
		vertices[i].normals.x = 0;
		vertices[i].normals.y = 1;
		vertices[i].normals.z = 0;
	}
	fRectangle *clip;
	for (int i = 0; i < _numVertices; i += 6)
	{
		clip = _tc.getTileClip(_tiles[i / 6]);
		vertices[i].texCoord.s = clip->x;
		vertices[i].texCoord.t = clip->y;
		vertices[i + 1].texCoord.s = clip->x + clip->w;
		vertices[i + 1].texCoord.t = clip->y;
		vertices[i + 2].texCoord.s = clip->x + clip->w;
		vertices[i + 2].texCoord.t = clip->y + clip->h;

		vertices[i + 3].texCoord.s = clip->x;
		vertices[i + 3].texCoord.t = clip->y;
		vertices[i + 4].texCoord.s = clip->x + clip->w;
		vertices[i + 4].texCoord.t = clip->y + clip->h;
		vertices[i + 5].texCoord.s = clip->x;
		vertices[i + 5].texCoord.t = clip->y + clip->h;

		GLfloat x = float((i / 6) % wTiled * TILE_WIDTH);// +gameCamera.x;
		GLfloat z = float((i / 6) / wTiled * TILE_HEIGHT);// +gameCamera.y;
		vertices[i].position.x = x;
		vertices[i].position.z = z;
		vertices[i + 1].position.x = x + TILE_WIDTH;
		vertices[i + 1].position.z = z;
		vertices[i + 2].position.x = x + TILE_WIDTH;
		vertices[i + 2].position.z = z + TILE_HEIGHT;

		vertices[i + 3].position.x = x;
		vertices[i + 3].position.z = z;
		vertices[i + 4].position.x = x + TILE_WIDTH;
		vertices[i + 4].position.z = z + TILE_HEIGHT;
		vertices[i + 5].position.x = x;
		vertices[i + 5].position.z = z + TILE_HEIGHT;
	}
	freeVBO();
	initVBO();

	gameNavigation->setNodes(wTiled, hTiled);
}

void GameMap::initVBO()
{
	//If texture is loaded and VBO does not already exist
	if (_vbo == 0)
	{
		//Vertex data
		GLuint *iData = new GLuint[_numVertices];

		for (int i = 0; i < _numVertices; i++) iData[i] = i;
		
		//Create VBO
		glGenBuffers(1, &_vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		glBufferData(GL_ARRAY_BUFFER, _numVertices * sizeof(VertexData3D), vertices, GL_DYNAMIC_DRAW);

		//Create IBO
		glGenBuffers(1, &_ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numVertices * sizeof(GLuint), iData, GL_DYNAMIC_DRAW);

		//Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		delete[] iData;
	}
}

void GameMap::freeVBO()
{
	//Free VBO and IBO
	if (_vbo != 0)
	{
		glDeleteBuffers(1, &_vbo);
		glDeleteBuffers(1, &_ibo);
		_vbo = 0;
		_ibo = 0;
	}
}