#include "Model.h"

Model::Model()
{
	_shader = render->getShader();
	_vibo = { 0, 0 };
	_txr = textureBox->texture["blank"];
	_bounds = TILE_WIDTH / 2;
	
	_color = Color(white, .1f);
}

Model::Model(const Model& v)
{
	_shader = render->getShader();
	_vibo = v._vibo;
	_scalar = v._scalar;
	_rotation = v._rotation;
	_translation = v._translation;
	_color = v._color;
	_txr = v._txr;
	_bounds = v._bounds;
}

Model& Model::operator=(const Model& v)
{
	_vibo = v._vibo;
	_scalar = v._scalar;
	_rotation = v._rotation;
	_translation = v._translation;
	_color = v._color;
	_txr = v._txr;
	_bounds = v._bounds;

	return *this;
}

Model::~Model()
{
}

void Model::setModel(std::string m)
{
	if (m == "none") _vibo = { 0, 0, 0 };
	else _vibo = modelBox->model[m];
}

void Model::setTxr(std::string t)
{
	_txr = textureBox->texture[t];
}

void Model::setColor(Color c)
{
	_color = c;
}

void Model::setBounds(GLfloat b)
{
	_bounds = b;
}

void Model::setTranslation(float x, float y, float z)
{
	//_translation = glm::translate(glm::vec3(x, y, z));
	_translation = glm::mat4(1, 0, 0, 0,
							0, 1, 0, 0,
							0, 0, 1, 0,
							x, y, z, 1);
	//_transformation = _translation * _scalar * _rotation;
}

void Model::setRotation(float angle, float x, float y, float z)
{
	_rotation = glm::rotate(angle, glm::vec3(x, y, z));
	//_transformation = _translation * _scalar * _rotation;
}

void Model::setScalar(float x, float y, float z)
{
	_scalar = glm::scale(glm::vec3(x, y, z));
	//_transformation = _translation * _scalar * _rotation;
}

void Model::setTransformation(glm::mat4 m)
{
	_transformation = m;
}

bool Model::isPicked()
{
	GLfloat dist = 0;
	if (rayPlaneCollision(render->getCameraPos(), mouse->worldRay, mouse->worldRay, glm::vec3(_translation[3]), &dist))
	{
		glm::vec3 target = render->getCameraPos() + mouse->worldRay * dist;
		V3rtex a = { target.x, target.y, target.z};
		V3rtex b = { _translation[3][0], _translation[3][1], _translation[3][2] };

		if (distanc3(a, b) < _bounds) return true;
	}

	return false;
}

void Model::draw()
{
	_shader->setModelView(_translation, _rotation, _scalar);
	//_shader->updateModelView();

	//Set texture ID
	glBindTexture(GL_TEXTURE_2D, _txr);
	/*GLenum error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error binding model texture model! %s\n", gluErrorString(error));
	}*/

	//Update vertex buffer data
	glBindBuffer(GL_ARRAY_BUFFER, _vibo.vbo);	
	/*error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error binding model vbo! %s\n", gluErrorString(error));
	}*/

	//Set texture coordinate data
	_shader->setPointers();
	/*error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error setting model vertex pointer! %s\n", gluErrorString(error));
	}*/

	_shader->setTextureColor(_color);
	/*error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error setting model color! %s\n", gluErrorString(error));
	}*/

	//Draw quad using vertex data and index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _vibo.ibo);
	/*error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error binding model ibo! %s\n", gluErrorString(error));
	}*/

	glDrawElements(GL_TRIANGLES, _vibo.size, GL_UNSIGNED_INT, NULL);
	/*error = glGetError();
	if (error != GL_NO_ERROR)
	{
		printf("Error drawing model! %s\n", gluErrorString(error));
	}*/
}