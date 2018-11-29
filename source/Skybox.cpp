#include "Skybox.h"

Skybox::Skybox()
{
	_shader = render->getUIShader();
	_vbo = 0;
	_ibo = 0;
	_txr = textureBox->texture["blank"];
	_numVertices = 36;
	initVBO();

	/*for (int i = 0; i < _numVertices; i++)
	{
		vertices[i].position.x = 0;
		vertices[i].position.y = 0;
		vertices[i].position.z = 0;

		vertices[i].texCoord.s = 0.f;
		vertices[i].texCoord.t = 0.f;
	}*/

	setRGBA(Color(1.f, 1.f, 1.f, 1.f));
}

Skybox::~Skybox()
{
}

void Skybox::update()
{

}

void Skybox::draw()
{
	//Set texture ID
	glBindTexture(GL_TEXTURE_2D, _txr);

	//Bind vertex buffer
	glBindBuffer(GL_ARRAY_BUFFER, _vbo);

	//Set texture coordinate data
	_shader->setTexCoordPointer(sizeof(VertexData3D), (GLvoid*)offsetof(VertexData3D, texCoord));

	//Set vertex data
	_shader->setVertexPointer(sizeof(VertexData3D), (GLvoid*)offsetof(VertexData3D, position));

	_shader->setTextureColor(_color);

	//Draw quad using vertex data and index data
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
	glDrawElements(GL_TRIANGLES, _numVertices, GL_UNSIGNED_INT, NULL);
}

void Skybox::initVBO()
{
	//If texture is loaded and VBO does not already exist
	if (_vbo == 0)
	{
		float points[] = {
			-10.0f,  10.0f, -10.0f,
			-10.0f, -10.0f, -10.0f,
			10.0f, -10.0f, -10.0f,
			10.0f, -10.0f, -10.0f,
			10.0f,  10.0f, -10.0f,
			-10.0f,  10.0f, -10.0f,

			-10.0f, -10.0f,  10.0f,
			-10.0f, -10.0f, -10.0f,
			-10.0f,  10.0f, -10.0f,
			-10.0f,  10.0f, -10.0f,
			-10.0f,  10.0f,  10.0f,
			-10.0f, -10.0f,  10.0f,

			10.0f, -10.0f, -10.0f,
			10.0f, -10.0f,  10.0f,
			10.0f,  10.0f,  10.0f,
			10.0f,  10.0f,  10.0f,
			10.0f,  10.0f, -10.0f,
			10.0f, -10.0f, -10.0f,

			-10.0f, -10.0f,  10.0f,
			-10.0f,  10.0f,  10.0f,
			10.0f,  10.0f,  10.0f,
			10.0f,  10.0f,  10.0f,
			10.0f, -10.0f,  10.0f,
			-10.0f, -10.0f,  10.0f,

			-10.0f,  10.0f, -10.0f,
			10.0f,  10.0f, -10.0f,
			10.0f,  10.0f,  10.0f,
			10.0f,  10.0f,  10.0f,
			-10.0f,  10.0f,  10.0f,
			-10.0f,  10.0f, -10.0f,

			-10.0f, -10.0f, -10.0f,
			-10.0f, -10.0f,  10.0f,
			10.0f, -10.0f, -10.0f,
			10.0f, -10.0f, -10.0f,
			-10.0f, -10.0f,  10.0f,
			10.0f, -10.0f,  10.0f
		};
		GLuint vbo;
		glGenBuffers(1, &vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &points, GL_STATIC_DRAW);

		GLuint vao;
		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

		//Vertex data
		VertexData3D *vData = new VertexData3D[_numVertices];
		GLuint *iData = new GLuint[_numVertices];

		////Set rendering indices
		//iData[0] = 0;
		//iData[1] = 1;
		//iData[2] = 2;

		////Create VBO
		//glGenBuffers(1, &_vbo);
		//glBindBuffer(GL_ARRAY_BUFFER, _vbo);
		//glBufferData(GL_ARRAY_BUFFER, _numVertices * sizeof(VertexData3D), vData, GL_DYNAMIC_DRAW);

		////Create IBO
		//glGenBuffers(1, &_ibo);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _ibo);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER, _numVertices * sizeof(GLuint), iData, GL_DYNAMIC_DRAW);

		////Unbind buffers
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//delete[] vData;
		//delete[] iData;
	}
}

void Skybox::freeVBO()
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

void Skybox::setRGBA(Color c)
{
	_color = c;
}