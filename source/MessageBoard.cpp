#include "MessageBoard.h"

MessageBoard::~MessageBoard()
{
	for (std::map<std::string, Message>::iterator it = messages.begin(); it != messages.end(); ++it)
	{
		switch ((&it->second)->type)
		{
		case INTEGER:
			delete static_cast<int*>((&it->second)->data);
			break;
		case FLOATER:
			delete static_cast<int*>((&it->second)->data);
			break;
		case DOUBLER:
			delete static_cast<int*>((&it->second)->data);
			break;
		case STRING:
			delete static_cast<int*>((&it->second)->data);
			break;
		case CHARER:
			delete static_cast<int*>((&it->second)->data);
			break;
		case VERTEX:
			delete static_cast<int*>((&it->second)->data);
			break;
		case V3RTEX:
			delete static_cast<int*>((&it->second)->data);
			break;
		case VERTEXDATA3D:
			delete static_cast<int*>((&it->second)->data);
			break;
		case VERTEXDATA2D:
			delete static_cast<int*>((&it->second)->data);
			break;
		case INDEXDATA3D:
			delete static_cast<int*>((&it->second)->data);
			break;
		case TEXCOORD:
			delete static_cast<int*>((&it->second)->data);
			break;
		case SDL_POINT:
			delete static_cast<int*>((&it->second)->data);
			break;
		case SDL_RECT:
			delete static_cast<int*>((&it->second)->data);
			break;
		case GLM_VEC3:
			delete static_cast<int*>((&it->second)->data);
			break;
		case GLM_MAT4:
			delete static_cast<int*>((&it->second)->data);
			break;
		}
	}
}

void MessageBoard::send(std::string msgName, Message message)
{
	messages[msgName] = message;
}

Message MessageBoard::receive(std::string msgName)
{
	return messages[msgName];
}

MessageBoard messageBoard;