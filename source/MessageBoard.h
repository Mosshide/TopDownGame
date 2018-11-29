#pragma once
#include <map>

enum VariableType
{
	INTEGER,
	FLOATER,
	DOUBLER,
	STRING,
	CHARER,
	VERTEX,
	V3RTEX,
	VERTEXDATA3D,
	VERTEXDATA2D,
	INDEXDATA3D,
	TEXCOORD,
	SDL_POINT,
	SDL_RECT,
	GLM_VEC3,
	GLM_MAT4
};

struct Message
{
	void *data;
	VariableType type;
};

class MessageBoard
{
public:
	~MessageBoard();
	void send(std::string msgName, Message message);
	Message receive(std::string msgName);

private:
	std::map<std::string, Message> messages;
};

extern MessageBoard messageBoard;
