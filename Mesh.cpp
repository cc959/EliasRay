#include "Mesh.h"

#include <chrono>

#define currentTime chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now().time_since_epoch()).count()

Mesh::Mesh(vector<vec3> &positions, vector<vec2> &uvs, vector<vec3> &normals)
{
	vector<vec3> tangents;
	vector<vec3> bitangents;
	try
	{
		for (int i = 0; i < positions.size(); i += 3)
		{
			// Shortcuts for vertices
			vec3 &v0 = positions[i + 0];
			vec3 &v1 = positions[i + 1];
			vec3 &v2 = positions[i + 2];

			// Shortcuts for UVs
			vec2 &uv0 = uvs[i + 0];
			vec2 &uv1 = uvs[i + 1];
			vec2 &uv2 = uvs[i + 2];

			// Edges of the triangle : position delta
			vec3 deltaPos1 = v1 - v0;
			vec3 deltaPos2 = v2 - v0;

			// UV delta
			vec2 deltaUV1 = uv1 - uv0;
			vec2 deltaUV2 = uv2 - uv0;

			float r = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV1.y * deltaUV2.x);
			vec3 tangent = (deltaPos1 * deltaUV2.y - deltaPos2 * deltaUV1.y) * r;
			vec3 bitangent = (deltaPos2 * deltaUV1.x - deltaPos1 * deltaUV2.x) * r;

			// Set the same tangent for all three vertices of the triangle.
			tangents.push_back(tangent);
			tangents.push_back(tangent);
			tangents.push_back(tangent);

			// Same thing for bitangents
			bitangents.push_back(bitangent);
			bitangents.push_back(bitangent);
			bitangents.push_back(bitangent);
		}
	}
	catch (int e)
	{
		cout << "Error occurred while computing tangents and bitangents :( Error code: " << e << "\n";
	}

	*this = Mesh(positions, uvs, normals, tangents, bitangents);
}

Mesh::Mesh(vector<vec3> &positions, vector<vec2> &uvs, vector<vec3> &normals, vector<vec3> &tangents, vector<vec3> &bitangents)
{
	try
	{
		glGenBuffers(1, &positionbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, positionbuffer);
		glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(vec3), &positions[0], GL_STATIC_DRAW);

		glGenBuffers(1, &uvbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glBufferData(GL_ARRAY_BUFFER, uvs.size() * sizeof(vec2), &uvs[0], GL_STATIC_DRAW);

		glGenBuffers(1, &normalbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glBufferData(GL_ARRAY_BUFFER, normals.size() * sizeof(vec3), &normals[0], GL_STATIC_DRAW);

		glGenBuffers(1, &tangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
		glBufferData(GL_ARRAY_BUFFER, tangents.size() * sizeof(vec3), &tangents[0], GL_STATIC_DRAW);

		glGenBuffers(1, &bitangentbuffer);
		glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
		glBufferData(GL_ARRAY_BUFFER, bitangents.size() * sizeof(vec3), &bitangents[0], GL_STATIC_DRAW);

		vertexCount = positions.size();
	}
	catch (int e)
	{
		cout << "Error enabling Array Attributes. Error code: " << e << "\n";
	}
}

Mesh::Mesh(const string &path)
{
	ifstream file(path);

	if (!file.is_open())
	{
		cout << "Error loading \"" << path << "\" maybe wrong file path?\n";
	}

	vector<int> positionIndices, uvIndices, normalIndices;
	vector<vec3> temp_positions;
	vector<vec2> temp_uvs;
	vector<vec3> temp_normals;

	vector<vec3> positions;
	vector<vec2> uvs;
	vector<vec3> normals;

	try
	{
		while (!file.eof())
		{
			char line[128];
			file.getline(line, 128);

			stringstream s;
			s << line;

			string prefix;
			s >> prefix;

			if (prefix == "v")
			{
				float x, y, z;
				s >> x >> y >> z;
				temp_positions.push_back(vec3(x, y, z));
			}
			else if (prefix == "vt")
			{
				float x, y;
				s >> x >> y;
				temp_uvs.push_back(vec2(x, 1 - y));
			}
			else if (prefix == "vn")
			{
				float x, y, z;
				s >> x >> y >> z;
				temp_normals.push_back(vec3(x, y, z));
			}
			else if (prefix == "f")
			{
				vector<string> nums;

				while (!s.eof())
				{
					char c = s.get();
					if (c == ' ' || c == '/')
						nums.push_back("");
					else
						nums.back().append(1, c);
				}

				nums.back().pop_back();

				positionIndices.push_back(stoi(nums[0]));
				uvIndices.push_back(stoi(nums[1]));
				normalIndices.push_back(stoi(nums[2]));

				positionIndices.push_back(stoi(nums[3]));
				uvIndices.push_back(stoi(nums[4]));
				normalIndices.push_back(stoi(nums[5]));

				positionIndices.push_back(stoi(nums[6]));
				uvIndices.push_back(stoi(nums[7]));
				normalIndices.push_back(stoi(nums[8]));
			}
		}

		for (int v : positionIndices)
			positions.push_back(temp_positions[v - 1]);

		for (int u : uvIndices)
			uvs.push_back(temp_uvs[u - 1]);

		for (int n : normalIndices)
			normals.push_back(temp_normals[n - 1]);
	}
	catch (int e)
	{
		cout << "Error reading \"" << path << "\" Perhaps exported from blender incorrectly. Make sure to triangulate faces, include UVs and normals! Error code: " << e << "\n";
	}

	*this = Mesh(positions, uvs, normals);

	cout << "Successfully loaded the model \"" << path << "\" :)\n";
}

Mesh::Mesh()
{
	positionbuffer = uvbuffer = normalbuffer = tangentbuffer = bitangentbuffer = 0;
}

bool Mesh::EnableArrayAttributes()
{
	try
	{
		// 1st attribute buffer : positions
		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, positionbuffer);
		glVertexAttribPointer(
			0,		  // attribute 0.
			3,		  // size
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0,		  // stride
			(void *)0 // array buffer offset
		);

		// 2nd attribute buffer : uvs
		glEnableVertexAttribArray(1);
		glBindBuffer(GL_ARRAY_BUFFER, uvbuffer);
		glVertexAttribPointer(
			1,		  // attribute.
			2,		  // size
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0,		  // stride
			(void *)0 // array buffer offset
		);

		// 2nd attribute buffer : normals
		glEnableVertexAttribArray(2);
		glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
		glVertexAttribPointer(
			2,		  // attribute.
			3,		  // size
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0,		  // stride
			(void *)0 // array buffer offset
		);

		// 4th attribute buffer : tangents
		glEnableVertexAttribArray(3);
		glBindBuffer(GL_ARRAY_BUFFER, tangentbuffer);
		glVertexAttribPointer(
			3,		  // attribute
			3,		  // size
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0,		  // stride
			(void *)0 // array buffer offset
		);

		// 5th attribute buffer : bitangents
		glEnableVertexAttribArray(4);
		glBindBuffer(GL_ARRAY_BUFFER, bitangentbuffer);
		glVertexAttribPointer(
			4,		  // attribute
			3,		  // size
			GL_FLOAT, // type
			GL_FALSE, // normalized?
			0,		  // stride
			(void *)0 // array buffer offset
		);
	}
	catch (int e)
	{
		cout << "Error enabling Array Attributes. Error code: " << e << "\n";
		return false;
	}
	return true;
}

bool Mesh::DisableArrayAttributes()
{
	try
	{
		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);
		glDisableVertexAttribArray(3);
		glDisableVertexAttribArray(4);
	}
	catch (int e)
	{
		cout << "Error disabling Array Attributes. Error code: " << e << "\n";
		return false;
	}
	return true;
}

Mesh Mesh::quad;

template <class U>
void Mesh::Copy(U &&other)
{
	if (&other != this)
	{
		Release();

		GlStorage::Copy(other.id);
		positionbuffer = other.positionbuffer;
		uvbuffer = other.uvbuffer;
		normalbuffer = other.normalbuffer;
		tangentbuffer = other.tangentbuffer;
		bitangentbuffer = other.bitangentbuffer;
		vertexCount = other.vertexCount;
	}
}

Mesh::Mesh(Mesh &&other)
{
	Copy(other);
}

Mesh::Mesh(const Mesh &other)
{
	Copy(other);
}

Mesh &Mesh::operator=(Mesh &&other)
{
	Copy(other);
	return *this;
}

Mesh &Mesh::operator=(const Mesh &other)
{
	Copy(other);
	return *this;
}

bool Mesh::Release()
{
	if (GlStorage::Release())
	{
		glDeleteBuffers(1, &positionbuffer);
		glDeleteBuffers(1, &normalbuffer);
		glDeleteBuffers(1, &uvbuffer);
		glDeleteBuffers(1, &tangentbuffer);
		glDeleteBuffers(1, &bitangentbuffer);
		return true;
	}
	return false;
}

Mesh::~Mesh()
{
	Release();
}
