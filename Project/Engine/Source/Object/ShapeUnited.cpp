
#include "ShapeUnited.h"
#include "../Platform/Source/FilesManager.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

#ifdef BUILD_WIN_GLES
#define GL_GLEXT_PROTOTYPES
#include "GLES2/gl2.h"
#elif BUILD_WIN_GLFW
#include <GL/glew.h>
#elif BUILD_OSX
#include "glfw3.h"
#endif

//	Mesh

Mesh::~Mesh()
{
	_countVertex = 0;
	
	if (_aVertex)
	{
		delete[] _aVertex;
		_aVertex = nullptr;
	}

	if (_aNormal)
	{
		delete[] _aNormal;
		_aNormal = nullptr;
	}

	if (_aTexCoord)
	{
		delete[] _aTexCoord;
		_aTexCoord = nullptr;
	}

	_countIndex = 0;

	if (_aIndex)
	{
		delete[] _aIndex;
		_aIndex = nullptr;
	}
}

void Mesh::initVBO()
{
	glDeleteBuffers(4, _buffer);

	glGenBuffers(4, _buffer);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[0]);
	glBufferData(GL_ARRAY_BUFFER, _countVertex * 3 * sizeof(GLfloat), _aVertex, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[1]);
	glBufferData(GL_ARRAY_BUFFER, _countVertex * 2 * sizeof(GLfloat), _aTexCoord, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, _buffer[2]);
	glBufferData(GL_ARRAY_BUFFER, _countVertex * 3 * sizeof(GLfloat), _aNormal, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _buffer[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, _countIndex * sizeof(GLushort), _aIndex, GL_STATIC_DRAW);

	_hasVBO = true;
}

//	MeshPhysic

MeshPhysic::~MeshPhysic()
{
	_count = 0;
	if (_meshes)
	{
		delete[] _meshes;
		_meshes = nullptr;
	}
}

//	ShapeUnited

struct BlockTemporary
{
	char* _name = '\0';
	const int _maxCount = 10240;
	int _countLine = 0;
	char** _charLine = nullptr;

	int _countNumber = 9;

	int _count = 0;
	float* _floatArray = nullptr;
	unsigned short int* _intArray = nullptr;

	BlockTemporary()
	{
		_charLine = new char*[_maxCount];
	}

	BlockTemporary(int countNumber)
	{
		_countNumber = countNumber;
		_charLine = new char*[_maxCount];
	}

	~BlockTemporary()
	{
		delete[] _charLine;
	}

	void parse()
	{
		_count = _countLine * _countNumber;

		if (_countNumber == 2 || _countNumber == 3)
		{
			_floatArray = new float[_count];

			for (int i = 0; i < _countLine; ++i)
			{
				int index = i * _countNumber;

				if (_countNumber == 2)
				{
					sscanf(_charLine[i], "%f %f", &_floatArray[index], &_floatArray[index + 1]);
				}
				else if (_countNumber == 3)
				{
					sscanf(_charLine[i], "%f %f %f", &_floatArray[index], &_floatArray[index + 1], &_floatArray[index + 2]);
				}
			}
		}
		else if (_countNumber == 9)
		{
			_intArray = new unsigned short int[_count];

			for (int i = 0; i < _countLine; ++i)
			{
				int intTemp[9];

				sscanf(_charLine[i], "%d/%d/%d %d/%d/%d %d/%d/%d",
					&intTemp[0],
					&intTemp[1],
					&intTemp[2],
					&intTemp[3],
					&intTemp[4],
					&intTemp[5],
					&intTemp[6],
					&intTemp[7],
					&intTemp[8]);

				int index = i * _countNumber;

				for (int i2 = 0; i2 < 9; ++i2)
				{
					_intArray[index + i2] = static_cast<unsigned short int>(intTemp[i2] - 1);
				}
			}
		}

		/*{
#ifdef WIN32
std::ofstream _WRITE_LOG("Log.txt", std::ios::app);
_WRITE_LOG << "\t_countNumber = " << _countNumber << std::endl;

			if (_countNumber == 2)
			{
				for (int i = 0; i < _count; i += _countNumber)
				{
					_WRITE_LOG  << _floatArray[i] << ' ' << _floatArray[i + 1] << std::endl;
					
				}
				
			}
			else if (_countNumber == 3)
			{
				for (int i = 0; i < _count; i += _countNumber)
				{
					_WRITE_LOG << _floatArray[i] << ' ' << _floatArray[i + 1] << ' ' << _floatArray[i + 2] << std::endl;

				}

			}
			else if (_countNumber == 9)
			{
				for (int i = 0; i < _count; i += _countNumber)
				{
					_WRITE_LOG << _intArray[i] << '/' << _intArray[i + 1] << '/' << _intArray[i + 2] << ' ' << _intArray[i + 3] << '/' << _intArray[i + 4] << '/' << _intArray[i + 5] << ' ' << _intArray[i + 6] << '/' << _intArray[i + 7] << '/' << _intArray[i + 8] << "\t//\t" << _name << std::endl;
				}
			}
#endif
		}*/
	}

	static int foundIndex(float *vertex, float *normal, float *texCoord,
		float *aVertex, float *aNormal, float *aTexCoord,
		int count)
	{
		for (int index = 0; index < count; ++index)
		{
			if ((vertex[0] == aVertex[index * 3] && vertex[1] == aVertex[(index * 3) + 1] && vertex[2] == aVertex[(index * 3) + 2]) &&
				(normal[0] == aNormal[index * 3] && normal[1] == aNormal[(index * 3) + 1] && normal[2] == aNormal[(index * 3) + 2]) &&
				(texCoord[0] == aTexCoord[index * 2] && texCoord[1] == aTexCoord[(index * 2) + 1]))
			{
				return index;
			}
		}

		return -1;
	};

	static void getMesh(Mesh& mesh, int countIndexTemporary, BlockTemporary* indexTemporarys,
		BlockTemporary& vertexTemporary, BlockTemporary& normalTemporary, BlockTemporary& textureTemporary)
	{
#ifdef WIN32
		std::ofstream _WRITE_LOG("Log.txt", std::ios::app);
		_WRITE_LOG << "\n\n\n" << std::endl;

		int countIndexT = 0;
		
		for (int iT = 0; iT < countIndexTemporary; ++iT)
		{
			countIndexT += indexTemporarys[iT]._count;
		}

		unsigned short* aIndexNew = new unsigned short[countIndexT * 3];
		float* aVertexNew = new float[countIndexT * 3];
		float* aNormalNew = new float[countIndexT * 3];
		float* aTextureNew = new float[countIndexT * 2];
		
		int indexVertexNew = 0;
		int iIndexNew = 0;

		float* aVertexT = vertexTemporary._floatArray;
		float* aTextureT = textureTemporary._floatArray;
		float* aNormalT = normalTemporary._floatArray;

		for (int iT = 0; iT < countIndexTemporary; ++iT)
		{
			BlockTemporary& indexTemporary = indexTemporarys[iT];
			_WRITE_LOG << "\n\t " << indexTemporary._name << std::endl;
			unsigned short* aIndexT = indexTemporary._intArray;
			
			for (int i = 0; i < indexTemporary._count; i+=3)
			{
				unsigned short indexV = aIndexT[i];
				float* vertex = &aVertexT[indexV * 3];

				unsigned short indexT = aIndexT[i + 1];
				float* texCoord = &aTextureT[indexT * 2];

				unsigned short indexN = aIndexT[i + 2];
				float* normal = &aNormalT[indexN * 3];

				//_WRITE_LOG << indexV << '/' << indexT << '/' << indexN << " [" << vertex [0] << ' ' << vertex[1] << ' ' << vertex[2]  << "] [" << texCoord[0] << ' ' << texCoord[1] << "] [" << normal[0] << ' ' << normal[1] << ' ' << normal[2] << "] " << std::endl;

				int index = foundIndex(vertex, normal, texCoord, aVertexNew, aNormalNew, aTextureNew, indexVertexNew);
				if (index == -1)
				{
					index = indexVertexNew;

					aVertexNew[index * 3] = vertex[0];
					aVertexNew[index * 3 + 1] = vertex[1];
					aVertexNew[index * 3 + 2] = vertex[2];

					aNormalNew[index * 3] = normal[0];
					aNormalNew[index * 3 + 1] = normal[1];
					aNormalNew[index * 3 + 2] = normal[2];

					aTextureNew[index * 2] = texCoord[0];
					aTextureNew[index * 2 + 1] = texCoord[1];

					++indexVertexNew;
				}

				aIndexNew[iIndexNew] = index;
				++iIndexNew;
			}
		}

		mesh._countIndex = iIndexNew;
		mesh._aIndex = aIndexNew;

		mesh._countVertex = indexVertexNew;
		mesh._aVertex = new float[indexVertexNew * 3];
		mesh._aNormal = new float[indexVertexNew * 3];
		mesh._aTexCoord = new float[indexVertexNew * 2];

		for (int i = 0; i < indexVertexNew; ++i)
		{
			int indexV = i * 3;

			mesh._aVertex[indexV] = aVertexNew[indexV];
			mesh._aVertex[indexV + 1] = aVertexNew[indexV + 1];
			mesh._aVertex[indexV + 2] = aVertexNew[indexV + 2];

			mesh._aNormal[indexV] = aNormalNew[indexV];
			mesh._aNormal[indexV + 1] = aNormalNew[indexV + 1];
			mesh._aNormal[indexV + 2] = aNormalNew[indexV + 2];

			int indexT = i * 2;

			mesh._aTexCoord[indexT] = aTextureNew[indexT];
			mesh._aTexCoord[indexT + 1] = aTextureNew[indexT + 1];
		}

		delete[] aVertexNew;
		delete[] aNormalNew;
		delete[] aTextureNew;
	}

#endif
};

ShapeUnited::~ShapeUnited()
{
	if (_physic)
	{
		delete _physic;
		_physic = nullptr;
	}
}

void ShapeUnited::load(const string& name)
{
#ifdef WIN32
	std::ofstream _WRITE_LOG("Log.txt");
//#endif

	char* data = FilesManager::loadTextFile(name.c_str());

	if (!data)
	{
		return;
	}

	int len = strlen(data);
	int iChar = 0;

	BlockTemporary vertexTemporary(3);
	BlockTemporary normalTemporary(3);
	BlockTemporary textureTemporary(2);

	const int maxCountIndexTemporary = 10;
	BlockTemporary* currentIndexTemporary = nullptr;

	int countIndexTemporary = 0;
	BlockTemporary indexTemporary[maxCountIndexTemporary];

	int countIndexPhysicTemporary = 0;
	BlockTemporary indexPhysicTemporary[maxCountIndexTemporary];
	
	// Разделение данных на блоки
	while (iChar < len)
	{
		if (data[iChar] == 'v' && data[iChar + 1] == ' ' && data[iChar + 2] == ' ')
		{
			vertexTemporary._charLine[vertexTemporary._countLine] = &data[iChar + 3];
			++vertexTemporary._countLine;
			iChar += 3;
		}

		if (data[iChar] == 'v' && data[iChar + 1] == 'n' && data[iChar + 2] == ' ')
		{
			normalTemporary._charLine[normalTemporary._countLine] = &data[iChar + 3];
			++normalTemporary._countLine;
			iChar += 3;
		}

		if (data[iChar] == 'v' && data[iChar + 1] == 't' && data[iChar + 2] == ' ')
		{
			textureTemporary._charLine[textureTemporary._countLine] = &data[iChar + 3];
			++textureTemporary._countLine;
			iChar += 3;
		}

		if (data[iChar] == 'f' && data[iChar + 1] == ' ')
		{
			if (!currentIndexTemporary)
			{
				char* name = '\0';
				int iCharTemp = iChar - 1;
				data[iCharTemp] = '\0';

				while (iCharTemp >= 0 && data[iCharTemp] != ' ')
				{
					--iCharTemp;
				}

				name = &data[iCharTemp + 1];

				if (name[0] == '_')
				{
					currentIndexTemporary = &indexPhysicTemporary[countIndexPhysicTemporary];
					++countIndexPhysicTemporary;

					if (countIndexPhysicTemporary >= maxCountIndexTemporary)
					{
						break;
						// LOG
					}
				}
				else
				{
					currentIndexTemporary = &indexTemporary[countIndexTemporary];
					++countIndexTemporary;

					if (countIndexPhysicTemporary >= maxCountIndexTemporary)
					{
						break;
						// LOG
					}
				}
				
				currentIndexTemporary->_name = name;
			}

			currentIndexTemporary->_charLine[currentIndexTemporary->_countLine] = &data[iChar + 2];
			++currentIndexTemporary->_countLine;
			iChar += 2;
		}

		if (currentIndexTemporary && (data[iChar] == '#' && data[iChar + 1] == ' '))
		{
			currentIndexTemporary = nullptr;
			iChar += 2;
		}

		if (data[iChar] == '\n')
		{
			data[iChar] = '\0';
		}

		++iChar;
	}


//#ifdef WIN32
//	std::ofstream _WRITE_LOG("Log.txt");

	/*_WRITE_LOG << "\n//\tcount vertex = " << vertexTemporary._countLine << std::endl;
	for (int i = 0; i < vertexTemporary._countLine; ++i)
	{
		_WRITE_LOG << vertexTemporary._charLine[i] << std::endl;
	}

	_WRITE_LOG << "\n//\tcount normal = " << normalTemporary._countLine << std::endl;
	for (int i = 0; i < normalTemporary._countLine; ++i)
	{
		_WRITE_LOG << normalTemporary._charLine[i] << std::endl;
	}

	_WRITE_LOG << "\n//\tcount texture = " << textureTemporary._countLine << std::endl;
	for (int i = 0; i < textureTemporary._countLine; ++i)
	{
		_WRITE_LOG << textureTemporary._charLine[i] << std::endl;
	}

	_WRITE_LOG << "\n//\tcount index temp = " << countIndexTemporary << std::endl;
	for (int iIndexes = 0; iIndexes < countIndexTemporary; ++iIndexes)
	{
		BlockTemporary& temporary = indexTemporary[iIndexes];

		_WRITE_LOG << "\n//\tcount index = " << temporary._countLine << " name = " << temporary._name << std::endl;
		for (int i = 0; i < temporary._countLine; ++i)
		{
			_WRITE_LOG << temporary._charLine[i] << std::endl;
		}
	}

	_WRITE_LOG << "\n//\tPHYSIC count index temp = " << countIndexPhysicTemporary << std::endl;
	for (int iIndexes = 0; iIndexes < countIndexPhysicTemporary; ++iIndexes)
	{
		BlockTemporary& temporary = indexPhysicTemporary[iIndexes];

		_WRITE_LOG << "\n//\tcount index = " << temporary._countLine << " name = " << temporary._name << std::endl;
		for (int i = 0; i < temporary._countLine; ++i)
		{
			_WRITE_LOG << temporary._charLine[i] << std::endl;
		}
	}*/

	vertexTemporary.parse();
	normalTemporary.parse();
	textureTemporary.parse();

	for (int i = 0; i < countIndexTemporary; ++i)
	{
		indexTemporary[i].parse();
	}

	for (int i = 0; i < countIndexPhysicTemporary; ++i)
	{
		indexPhysicTemporary[i].parse();
	}

	BlockTemporary::getMesh(*this, countIndexTemporary, indexTemporary, vertexTemporary, normalTemporary, textureTemporary);

	if (countIndexPhysicTemporary > 0)
	{
		_physic = new MeshPhysic();

		_physic->_count = countIndexPhysicTemporary;
		_physic->_meshes = new Mesh[countIndexPhysicTemporary];

		for (int i = 0; i < countIndexPhysicTemporary; ++i)
		{
			BlockTemporary::getMesh(_physic->_meshes[i], 1, &indexPhysicTemporary[i], vertexTemporary, normalTemporary, textureTemporary);
		}
	}

	{
		/*
		unsigned short int _countVertex = 0;
		float* _aVertex = nullptr;
		float* _aNormal = nullptr;
		float* _aTexCoord = nullptr;

		unsigned short int _countIndex = 0;
		unsigned short* _aIndex = nullptr;
		*/

		_WRITE_LOG << "\t// VERTEX _countVertex: " << _countVertex << " (" << (_countVertex / 3) << ") " << std::endl;
		for (int i = 0; i < _countVertex * 3; i+=3)
		{
			_WRITE_LOG << _aVertex[i] << ' ' << _aVertex[i + 1] << ' ' << _aVertex[i + 2] << std::endl;
		}

		_WRITE_LOG << "\t// NORMAL _countVertex: " << _countVertex << " (" << (_countVertex / 3) << ") " << std::endl;
		for (int i = 0; i < _countVertex * 3; i+=3)
		{
			_WRITE_LOG << _aNormal[i] << ' ' << _aNormal[i + 1] << ' ' << _aNormal[i + 2] << std::endl;
		}

		_WRITE_LOG << "\t// TEXTURE _countVertex: " << _countVertex << std::endl;
		for (int i = 0; i < _countVertex * 2; i += 2)
		{
			_WRITE_LOG << _aTexCoord[i] << ' ' << _aTexCoord[i + 1] << std::endl;
		}

		_WRITE_LOG << "\t// INDEX _countIndex: " << _countIndex << " (" << (_countVertex / 3) << ") " << std::endl;
		for (int i = 0; i < _countIndex; i += 3)
		{
			_WRITE_LOG << _aIndex[i] << ' ' << _aIndex[i + 1] << ' ' << _aIndex[i + 2] << std::endl;
		}
	}

#endif
}
