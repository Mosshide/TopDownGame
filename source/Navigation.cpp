#include "Navigation.h"

Node::Node(int a, int b, TerrainType t)
{
	x = a;
	y = b;
	px = -1;
	py = -1;
	c = INT_MAX;
	terrain = t;
	occupied = false;
	checkState = -1;
}

Node::~Node()
{
}

void Node::tryNewParent(int a, int b, int pc, int cost)
{
	/*if ((terrain == trnBuilding || terrain == trnBed) && (pType != trnBuilding && pType != trnBed && pType != trnEntrance))
	{
		return;
	}*/
	if (pc + cost < c)
	{
		px = a;
		py = b;
		c = pc + cost;
	}
}

SDL_Point Node::getPoint()
{
	SDL_Point v = { x, y };
	return v;
}

SDL_Point Node::getPPoint()
{
	SDL_Point v = { px, py };
	return v;
}

//////

Navigation::Navigation()
{
	startEnd[0] = 0;
	startEnd[1] = 0;

	_rect.setRGBA(red);

	_vibo.vbo = 0;
	_vibo.ibo = 0;
	_vibo.size = 0;

	vertices = nullptr;
	_shader = render->getShader();
}

Navigation::~Navigation()
{
}

void Navigation::draw()
{
	_shader->setModelView(glm::mat4(), glm::mat4(), glm::mat4());
	_shader->updateModelView();

	//Set texture ID
	glBindTexture(GL_TEXTURE_2D, textureBox->texture["blank"]);

	//_shader->enableVertexPointer();
	//_shader->enableTexCoordPointer();

	//Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vibo.vbo);

	//Update vertex buffer data
	glBufferSubData(GL_ARRAY_BUFFER, 0, _vibo.size * sizeof(VertexData3D), vertices);

	//Set texture coordinate data
	_shader->setTexCoordPointer(sizeof(VertexData3D), (GLvoid*)offsetof(VertexData3D, texCoord));

	_shader->setNormalPointer(sizeof(VertexData3D), (GLvoid*)offsetof(VertexData3D, normals));

	//Set vertex data
	_shader->setVertexPointer(sizeof(VertexData3D), (GLvoid*)offsetof(VertexData3D, position));

	_shader->setTextureColor(Color(1.f, 0.f, 0.f, .5f));


	//Draw quad using vertex data and index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vibo.ibo);
	glDrawElements(GL_TRIANGLES, _vibo.size, GL_UNSIGNED_INT, NULL);
}

void Navigation::initVBO(int w, int h)
{
	//If texture is loaded and VBO does not already exist
	if (_vibo.vbo == 0)
	{
		_vibo.size = w * h * 6;
		///
		if (vertices != nullptr) delete[] vertices;
		vertices = new VertexData3D[_vibo.size];
		for (int i = 0; i < _vibo.size; i++)
		{
			vertices[i].texCoord.s = 0;
			vertices[i].texCoord.t = 0;
			vertices[i].position.x = 0;
			vertices[i].position.y = .001f;
			vertices[i].position.z = 0;
			vertices[i].normals.x = 0;
			vertices[i].normals.y = 1;
			vertices[i].normals.z = 0;
		}
		for (int i = 0; i < _vibo.size; i += 6)
		{
			GLfloat x = float((i / 6) % wTiled * TILE_WIDTH);// +gameCamera.x;
			GLfloat z = float((i / 6) / wTiled * TILE_HEIGHT);// +gameCamera.y;
			vertices[i].position.x = x + .075;
			vertices[i].position.z = z + .075;
			vertices[i + 1].position.x = x + TILE_WIDTH - .075;
			vertices[i + 1].position.z = z + .075;
			vertices[i + 2].position.x = x + TILE_WIDTH - .075;
			vertices[i + 2].position.z = z + TILE_HEIGHT - .075;

			vertices[i + 3].position.x = x + .075;
			vertices[i + 3].position.z = z + .075;
			vertices[i + 4].position.x = x + TILE_WIDTH - .075;
			vertices[i + 4].position.z = z + TILE_HEIGHT - .075;
			vertices[i + 5].position.x = x + .075;
			vertices[i + 5].position.z = z + TILE_HEIGHT - .075;
		}

		//Vertex data
		GLuint *iData = new GLuint[_vibo.size];

		for (int i = 0; i < _vibo.size; i++) iData[i] = i;

		//Create VBO
		glGenBuffers(1, &_vibo.vbo);
		glBindBuffer(GL_ARRAY_BUFFER, _vibo.vbo);
		glBufferData(GL_ARRAY_BUFFER, _vibo.size * sizeof(VertexData3D), vertices, GL_DYNAMIC_DRAW);

		//Create IBO
		glGenBuffers(1, &_vibo.ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vibo.ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, _vibo.size * sizeof(GLuint), iData, GL_DYNAMIC_DRAW);

		//Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		delete[] iData;
	}
}

void Navigation::freeVBO()
{
	//Free VBO and IBO
	if (_vibo.vbo != 0)
	{
		glDeleteBuffers(1, &_vibo.vbo);
		glDeleteBuffers(1, &_vibo.ibo);
		_vibo.vbo = 0;
		_vibo.ibo = 0;
	}
}


void Navigation::setNodes(int w, int h)
{
	freeVBO();
	wTiled = w;
	hTiled = h;
	nodes.clear();
	for (int i = 0; i < hTiled; i++)
	{
		for (int ii = 0; ii < wTiled; ii++)
		{
			Node n(ii, i, trnGrass);
			nodes.push_back(n);
		}
	}

	startEnd[0] = 0;
	startEnd[1] = (int)nodes.size() - 1;

	initVBO(w, h);

	//setTerrain(_startPnt.x, _startPnt.y, trnStart);
	//setTerrain(_endPnt.x, _endPnt.y, trnEnd);
}

void Navigation::resetNodes()
{
	int s = (int)nodes.size();
	for (int i = 0; i < s; i++)
	{
		if (nodes[i].c != INT_MAX)
		{
			nodes[i].px = -1;
			nodes[i].py = -1;
			nodes[i].c = INT_MAX;
			nodes[i].checkState = -1;
		}
	}
}

std::vector<Vertex> Navigation::getPath(Vertex a, Vertex b, std::vector<TerrainType> *ok)
{
	int time = gameTimer.getTotal();
	resetNodes();
	printf("Time to reset nodes: %d\n", SDL_GetTicks() - time);

	//go
	std::vector<Vertex> path;
	SDL_Point p = getClosestTile(a);
	SDL_Point q = getClosestTile(b);
	printf("New Path: %d, %d -> %d, %d\n", p.x, p.y, q.x, q.y);

	//make sure end isnt out of bounds
	if (q.x < wTiled && q.x >= 0 && q.y < hTiled && q.y >= 0)
	{
		//make sure start and end points are walkable tiles
		if (checkterrainOK(nodes[p.x + p.y * wTiled].terrain, ok) && checkterrainOK(nodes[q.x + q.y * wTiled].terrain, ok))
		{
			//if not changing tile
			if (p.x == q.x && p.y == q.y)
			{
				Vertex tpoint = b;
				path.push_back(tpoint);
				printf("Start and end are same tile.\n");
			}
			else
			{
				SDL_Point t = { p.x, p.y };

				//init first node
				nodes[p.x + p.y * wTiled].c = 0;
				std::vector<Node*> tnodes;
				tnodes.push_back(&nodes[p.x + p.y * wTiled]);

				printf("Time to get to while loop: %d\n", SDL_GetTicks() - time);
				//while there are more nodes to check
				while (!tnodes.empty())
				{
					int currTNode = 0;
					int currTNodeC = 0;

					t.x = tnodes[currTNode]->x;
					t.y = tnodes[currTNode]->y;
					currTNodeC = tnodes[currTNode]->c;
					//find node closest to end
					for (int i = 1; i < (int)tnodes.size(); i++)
					{
						if (currTNodeC + (distance(t, q) * (10 / TILE_WIDTH)) > 
							tnodes[i]->c + (distance(tnodes[i]->getPoint(), q) * (10 / TILE_WIDTH)))
						{
							currTNode = i;
							currTNodeC = tnodes[i]->c;
							t.x = tnodes[i]->x;
							t.y = tnodes[i]->y;
						}
					}

					nodes[t.x + t.y * wTiled].checkState = 1;

					//vertical/horizontal
					bool upRightDownLeft[4];
					if (t.x - 1 > -1) upRightDownLeft[3] = checkNode(t.x - 1, t.y, t.x, t.y, 10, ok, &tnodes);
					if (t.x + 1 < wTiled) upRightDownLeft[1] = checkNode(t.x + 1, t.y, t.x, t.y, 10, ok, &tnodes);
					if (t.y - 1 > -1) upRightDownLeft[0] = checkNode(t.x, t.y - 1, t.x, t.y, 10, ok, &tnodes);
					if (t.y + 1 < hTiled) upRightDownLeft[2] = checkNode(t.x, t.y + 1, t.x, t.y, 10, ok, &tnodes);
					//diagonals
					if (t.x - 1 > -1 && t.y - 1 > -1 && upRightDownLeft[3] && upRightDownLeft[0]) 
						checkNode(t.x - 1, t.y - 1, t.x, t.y, 14, ok, &tnodes);
					if (t.x + 1 < wTiled && t.y + 1 < hTiled && upRightDownLeft[1] && upRightDownLeft[2])
						checkNode(t.x + 1, t.y + 1, t.x, t.y, 14, ok, &tnodes);
					if (t.x + 1 < wTiled && t.y - 1 > -1 && upRightDownLeft[0] && upRightDownLeft[1])
						checkNode(t.x + 1, t.y - 1, t.x, t.y, 14, ok, &tnodes);
					if (t.x - 1 > -1 && t.y + 1 < hTiled && upRightDownLeft[2] && upRightDownLeft[3])
						checkNode(t.x - 1, t.y + 1, t.x, t.y, 14, ok, &tnodes);

					//if the current node is the end
					if (t.x == q.x && t.y == q.y)
					{
						printf("Time to find end node: %d\n", SDL_GetTicks() - time);

						if (nodes[t.x + t.y * wTiled].px != -1 && nodes[t.x + t.y * wTiled].py != -1)
						{
							path.push_back(b);
							int ttx = nodes[t.x + t.y * wTiled].px;
							int tty = nodes[t.x + t.y * wTiled].py;
							t.x = ttx;
							t.y = tty;
						}
						//until there is no more parent node
						while (nodes[t.x + t.y * wTiled].px != -1 && nodes[t.x + t.y * wTiled].py != -1)
						{
							//add point to the path
							Vertex tpoint = { t.x * TILE_WIDTH + (TILE_WIDTH / 2),
								t.y * TILE_HEIGHT + (TILE_HEIGHT / 2) };
							path.push_back(tpoint);
							int ttx = nodes[t.x + t.y * wTiled].px;
							int tty = nodes[t.x + t.y * wTiled].py;
							t.x = ttx;
							t.y = tty;
						}
						//add final points to the path
						//Vertex tpoint = { t.x * TILE_WIDTH + (TILE_WIDTH / 2),
											//t.y * TILE_HEIGHT + (TILE_HEIGHT / 2) };
						path.push_back(a);
						//path = refinePath(path, a, b, ok);

						///path refinement


						///
						break;
					}
					else tnodes.erase(tnodes.begin() + currTNode);
				}
			}
		}
		else printf("Start and end not pathable.\n");
	}
	else printf("Out of bounds.\n");

	/*for (int i = 0; i < (int)path.size(); i++)
	{
		printf("%d, %d\n", path[i].x, path[i].y);
	}*/
	printf("Time to generate path: %d\n", SDL_GetTicks() - time);
	return path;
}

bool Navigation::checkNode(int x, int y, int px, int py, int cost, std::vector<TerrainType> *ok, std::vector<Node*> *tnodes)
{
	//make sure node is walkable
	if (checkterrainOK(nodes[x + y * wTiled].terrain, ok))
	{
		//if node hasnt been checked
		if (nodes[x + y * wTiled].checkState == -1)
		{
			//add to vector of nodes to check the surroundings of
			tnodes->push_back(&nodes[x + y * wTiled]);
			nodes[x + y * wTiled].checkState = 0;
		}
		//check if a new parent node is warranted
		nodes[x + y * wTiled].tryNewParent(px, py, nodes[px + py * wTiled].c, cost);

		return true;
	}
	else return false;
}

void Navigation::setTerrain(int x, int y, TerrainType t)
{
	if (x < 0 || x >= wTiled || y < 0 || y >= hTiled)
	{
		printf("set terrain out of bounds");
	}
	else
	{
		nodes[x + y * wTiled].terrain = t;
	}
}

TerrainType Navigation::getTerrain(int x, int y)
{
	if (x < 0 || x >= wTiled || y < 0 || y >= hTiled)
	{
		printf("set terrain out of bounds\n");
		return TERRAIN_TYPE_SIZE;
	}
	else return nodes[x + y * wTiled].terrain;
}

void Navigation::setOccupied(int x, int y, bool o)
{
	if (x < 0 || x >= wTiled || y < 0 || y >= hTiled)
	{
		printf("set occupied out of bounds");
	}
	else
	{
		nodes[x + y * wTiled].occupied = o;
	}
}

bool Navigation::getOccupied(int x, int y)
{
	if (x < 0 || x >= wTiled || y < 0 || y >= hTiled)
	{
		printf("set terrain out of bounds\n");
		return true;
	}
	else return nodes[x + y * wTiled].occupied;
}

SDL_Point Navigation::getClosestTile(GLfloat x, GLfloat y)
{
	SDL_Point p = { int(x / TILE_WIDTH), int(y / TILE_HEIGHT) };

	if (p.x < 0) p.x = 0;
	else if (p.x >= wTiled) p.x = wTiled - 1;
	if (p.y < 0) p.y = 0;
	else if (p.y >= hTiled) p.y = hTiled - 1;

	return p;
}

bool Navigation::checkterrainOK(TerrainType tt, std::vector<TerrainType> *ok)
{
	for (int i = 0; i < (int)ok->size(); i++)
	{
		if (tt == (*ok)[i]) return true;
	}

	return false;
}

Vertex Navigation::getStartPoint()
{
	Vertex p;
	if (wTiled > 0)
	{
		p = { (startEnd[0] % wTiled) * TILE_WIDTH + (TILE_WIDTH / 2),
			(startEnd[0] / wTiled) * TILE_HEIGHT + (TILE_HEIGHT / 2) };
	}
	else
	{
		p = { 0, 0 };
	}
	return p;
}

Vertex Navigation::getEndPoint()
{
	Vertex p;
	if (wTiled > 0)
	{
		p = { (startEnd[1] % wTiled) * (TILE_WIDTH) + (TILE_WIDTH / 2),
			(startEnd[1] / wTiled) * (TILE_HEIGHT) + (TILE_HEIGHT / 2) };
	}
	else
	{
		p = { 0, 0 };
	}
	return p;
}

std::vector<Vertex> Navigation::refinePath(std::vector<Vertex> path, Vertex a, Vertex  b, std::vector<TerrainType> *ok)
{
	if ((int)path.size() > 0)
	{
		path[0] = a;
		/*printf("path[%d] : %d, %d\n", 0, path[0].x, path[0].y);
		for (int i = 1; i < (int)path.size(); i++)
		{
			if (path[i - 1].x > path[i].x + (3 * TILE_WIDTH / 4))
			{
				path[i].x = path[i].x + (3 * TILE_WIDTH / 4);
			}
			else if (path[i - 1].x > path[i].x + (TILE_WIDTH / 4))
			{
				path[i].x = path[i - 1].x;
			}
			
			if (path[i - 1].y > path[i].y + TILE_HEIGHT)
			{
				path[i].y = path[i].y + TILE_HEIGHT;
			}
			else if (path[i - 1].y > path[i].y)
			{
				path[i].y = path[i - 1].y;
			}
			printf("path[%d] : %d, %d\n", i, path[i].x, path[i].y);
		}*/

		std::vector<Vertex> shortcut;
		shortcut.push_back(path[0]);
		for (int i = 1; i < (int)path.size(); i++)
		{
			if (i + 1 < (int)path.size())
			{
				if (!straightPath(shortcut.back(), path[i + 1], ok))
				{
					shortcut.push_back(path[i]);
					//printf("col\n");
				}
			}
			else
			{
				shortcut.push_back(path[i]);
			}
		}
		if ((int)shortcut.size() > 1) shortcut.pop_back();

		/*for (int i = 0; i < (int)shortcut.size(); i++)
		{
			printf("shortcut[%d] : %d, %d\n", i, shortcut[i].x, shortcut[i].y);
		}*/
		//printf("end\n");

		return shortcut;
	}

	else return path;
}

bool Navigation::straightPath(Vertex a, Vertex b, std::vector<TerrainType> *ok)
{
	std::vector<SDL_Point> checkTiles;
	SDL_Point p = getClosestTile(a);
	SDL_Point q = getClosestTile(b);
	if (a.x == b.x && a.y == b.y) return true;
	else if (a.x == b.x)
	{
		if (q.y > p.y)
		{
			for (int i = p.y; i < q.y; i++)
			{
				SDL_Point a = { p.x, i };
				checkTiles.push_back(a);
			}
		}
		else
		{
			for (int i = q.y; i < p.y; i++)
			{
				SDL_Point a = { p.x, i };
				checkTiles.push_back(a);
			}
		}
	}
	else if (a.y == b.y)
	{
		if (q.x > p.x)
		{
			for (int i = p.x; i < q.x; i++)
			{
				SDL_Point a = { i, p.y };
				checkTiles.push_back(a);
			}
		}
		else
		{
			for (int i = q.x; i < p.x; i++)
			{
				SDL_Point a = { i, p.y };
				checkTiles.push_back(a);
			}
		}
	}
	else
	{
		int dx = abs(q.x - p.x);
		int dy = abs(q.y - p.y);
		int tx = p.x;
		int ty = p.y;
		int n = 1 + dx + dy;
		int x_inc = (q.x > p.x) ? 1 : -1;
		int y_inc = (q.y > p.y) ? 1 : -1;
		int error = dx - dy;
		dx *= 2;
		dy *= 2;

		for (int i = n; i > 0; --i)
		{
			SDL_Point a = { tx, ty };
			checkTiles.push_back(a);

			if (error > 0)
			{
				tx += x_inc;
				error -= dy;
			}
			else
			{
				ty += y_inc;
				error += dx;
			}
		}
	}

	for (int i = 0; i < (int)checkTiles.size(); i++)
	{
		if (!checkterrainOK(nodes[checkTiles[i].x + checkTiles[i].y * wTiled].terrain, ok))
		{
			return false;
		}
	}
	return true;
}

Navigation *gameNavigation;