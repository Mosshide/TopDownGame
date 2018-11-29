#include "toyBox.h"

toyBox::toyBox()
{
	model["arrow"] = load("models/arrow.obj");
	model["ghost"] = load("models/scary ghost.obj");
	model["dryad"] = load("models/dryad.obj");
	model["wall"] = load("models/wall.obj");
	model["square"] = load("models/square.obj");
	model["rabbit"] = load("models/rabbit.obj");
	model["cube"] = load("models/cube.obj");
}

toyBox::~toyBox()
{
	for (std::map<std::string, VIBO>::iterator it = model.begin(); it != model.end(); ++it)
	{
		freeVBO(&it->second);
	}
}

VIBO toyBox::load(std::string path)
{
	VIBO vibo = { 0, 0, 0 };
	std::vector<VertexData3D> vertices;
	std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
	std::vector<V3rtex> temp_vertices;
	std::vector<TexCoord> temp_uvs;
	std::vector<V3rtex> temp_normals;

	FILE * file;
	errno_t err;
	if ((err = fopen_s(&file, path.c_str(), "r")) != 0) {
		char buf[128];
		strerror_s(buf, sizeof buf, err);
		fprintf_s(stderr, "cannot open file '%s': %s\n",
			path.c_str(), buf);
	}
	else
	{
		while (true) {
			char lineHeader[128];
			// read the first word of the line
			int res = fscanf_s(file, "%s", lineHeader, (unsigned int)_countof(lineHeader));
			if (res == EOF) break; // EOF = End Of File. Quit the loop.

			if (strcmp(lineHeader, "v") == 0) {
				V3rtex vertex;
				fscanf_s(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
				temp_vertices.push_back(vertex);
			}
			else if (strcmp(lineHeader, "vt") == 0) {
				TexCoord uv;
				fscanf_s(file, "%f %f\n", &uv.s, &uv.t);
				temp_uvs.push_back(uv);
			}
			else if (strcmp(lineHeader, "vn") == 0) {
				V3rtex normal;
				fscanf_s(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
				temp_normals.push_back(normal);
			}
			else if (strcmp(lineHeader, "f") == 0) {
				VertexData3D v;
				unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
				int matches = fscanf_s(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);
				if (matches != 9) {
					printf("File can't be read by our simple parser : ( Try exporting with other options )\n");
					break;
				}

				for (int i = 0; i < 3; i++)
				{
					v.position = temp_vertices[vertexIndex[i] - 1];
					v.texCoord = temp_uvs[uvIndex[i] - 1];
					v.texCoord.t = 1.f - v.texCoord.t;
					v.normals = temp_normals[normalIndex[i] - 1];
					vertices.push_back(v);
				}
			}
		}
	}

	initVBO(&vibo, &vertices);

	return vibo;
}

void toyBox::initVBO(VIBO *vi, std::vector<VertexData3D> *vertices)
{
	//If texture is loaded and VBO does not already exist
	if (vi->vbo == 0)
	{
		vi->size = (GLuint)vertices->size();

		//Vertex data
		//VertexData3D *vData = new VertexData3D[_numVertices];
		GLuint *iData = new GLuint[vi->size];

		//Set rendering indices
		for (int i = 0; i < (int)vi->size; i++) iData[i] = i;

		//Create VBO
		glGenBuffers(1, &vi->vbo);
		glBindBuffer(GL_ARRAY_BUFFER, vi->vbo);
		glBufferData(GL_ARRAY_BUFFER, vertices->size() * sizeof(VertexData3D), vertices->data(), GL_DYNAMIC_DRAW);

		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			printf("Error creating vbo! %s\n", gluErrorString(error));
		}

		//Create IBO
		glGenBuffers(1, &vi->ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vi->ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, vi->size * sizeof(GLuint), iData, GL_DYNAMIC_DRAW);

		error = glGetError();
		if (error != GL_NO_ERROR)
		{
			printf("Error creating ibo! %s\n", gluErrorString(error));
		}

		//Unbind buffers
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		//delete[] vData;
		delete[] iData;
	}
}

void toyBox::freeVBO(VIBO *vi)
{
	//Free VBO and IBO
	if (vi->vbo != 0)
	{
		glDeleteBuffers(1, &vi->vbo);
		glDeleteBuffers(1, &vi->ibo);
		vi->vbo = 0;
		vi->ibo = 0;
	}
}

toyBox *toyBox;
