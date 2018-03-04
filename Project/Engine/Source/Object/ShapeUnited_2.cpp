
#include "ShapeUnited_2.h"
#include "../Platform/Source/FilesManager.h"

#include <fstream>
#include <iostream>
#include <stdlib.h>

enum BlockSampleTemporaryType
{
	NONE,
	VERTEX,
	NORMAL,
	TEXTURE,
	INDEX
};

//	Структура файла
struct BlockSampleTemporary
{
	int _countItem = 0;
	int _countChar = 0;
	char* _beginChar = nullptr;
	char* _endChar = nullptr;

	BlockSampleTemporaryType _type = BlockSampleTemporaryType::NONE;
	PhysicType _physicType = PhysicType::NONE;

	unsigned short int _count;
	float* _array = nullptr;
	unsigned short* _aIndex = nullptr;

	BlockSampleTemporaryType getType()
	{
		if (_type != BlockSampleTemporaryType::NONE)
		{
			return _type;
		}

		if (_countChar < 3) return BlockSampleTemporaryType::NONE;

		if (_beginChar[0] == 'v' && _beginChar[1] == ' ')
		{
			_type = BlockSampleTemporaryType::VERTEX;
		}
		else if (_beginChar[0] == 'v' && _beginChar[1] == 'n' && _beginChar[2] == ' ')
		{
			_type = BlockSampleTemporaryType::NORMAL;
		}
		else if (_beginChar[0] == 'v' && _beginChar[1] == 't' && _beginChar[2] == ' ')
		{
			_type = BlockSampleTemporaryType::TEXTURE;
		}
		else if (_beginChar[0] == 'f' && _beginChar[1] == ' ')
		{
			_type = BlockSampleTemporaryType::INDEX;
		}

		return _type;
	}

	float* getVertexArray(unsigned short int& _count, const int amountNumbers)
	{
		if (_countItem == 0 || !(amountNumbers == 2 || amountNumbers == 3))
		{
			_count = 0;
			return nullptr;
		}

		_count = _countItem * amountNumbers;
		float* array = new float[_count];

		char* lineChar = nullptr;
		int countLine = 0;
		int iChar = 0;
		int iArray = 0;

		while (iChar < _countChar)
		{
			if (!lineChar && _beginChar[iChar] == ' ')
			{
				lineChar = &_beginChar[iChar];
			}

			if (_beginChar[iChar] == '\n' || _beginChar[iChar] == '\0' || iChar == (_countChar - 1))
			{
				if (amountNumbers == 2)
				{
					sscanf(lineChar, "%f %f", &array[iArray], &array[iArray + 1]);
				}
				else if (amountNumbers == 3)
				{
					sscanf(lineChar, "%f %f %f", &array[iArray], &array[iArray + 1], &array[iArray + 2]);
				}

				iArray += amountNumbers;
				lineChar = nullptr;
				++countLine;

				if (countLine >= _countItem) break;
			}

			++iChar;
		}

		return array;
	}

	unsigned short* getIndexArray(unsigned short int& _count)
	{
		if (_count == 0)
		{
			_count = 0;
			return nullptr;
		}

		int amountNumbers = 9;
		_count = _countItem * amountNumbers;
		unsigned short* array = new unsigned short[_count];

		char* lineChar = nullptr;
		int countLine = 0;
		int iChar = 0;
		int iArray = 0;

		while (iChar < _countChar)
		{
			if (!lineChar && _beginChar[iChar] == ' ')
			{
				lineChar = &_beginChar[iChar];
			}

			if (_beginChar[iChar] == '\n' || _beginChar[iChar] == '\0' || iChar == (_countChar - 1))
			{
				sscanf(lineChar, "%d/%d/%d %d/%d/%d %d/%d/%d",
					&array[iArray],
					&array[iArray + 1],
					&array[iArray + 2],
					&array[iArray + 3],
					&array[iArray + 4],
					&array[iArray + 5],
					&array[iArray + 6],
					&array[iArray + 7],
					&array[iArray + 8]);

				iArray += amountNumbers;
				lineChar = nullptr;
				++countLine;

				if (countLine >= _countItem) break;
			}

			++iChar;
		}

		return array;
	}

	BlockSampleTemporaryType getData()
	{
		BlockSampleTemporaryType type = getType();
		
		switch (type)
		{
		case BlockSampleTemporaryType::VERTEX:
			_array = getVertexArray(_count, 3);
				break;
		case BlockSampleTemporaryType::TEXTURE:
			_array = getVertexArray(_count, 2);
			break;
		case BlockSampleTemporaryType::NORMAL:
			_array = getVertexArray(_count, 3);
			break;
		case BlockSampleTemporaryType::INDEX:
			_aIndex = getIndexArray(_count);
			break;
		}

		return type; 
	}
};

struct Mesh_2Temporary_2
{
	PhysicType _physicType = PhysicType::NONE;
	BlockSampleTemporary* _vertex;
	BlockSampleTemporary* _texture;
	BlockSampleTemporary* _normal;
	BlockSampleTemporary* _index;

	void setPhysicType(PhysicType physicType)
	{
		_physicType = physicType == PhysicType::NONE ? physicType  : PhysicType::NONE;
	}
};

//	Mesh_2

Mesh_2::~Mesh_2()
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

//	ShapeUnited_2

ShapeUnited_2::ShapeUnited_2()
{

}

ShapeUnited_2::~ShapeUnited_2()
{
	_countMesh_2 = 0;

	if (_aMesh_2)
	{
		delete[] _aMesh_2;
		_aMesh_2 = nullptr;
	}
}

void ShapeUnited_2::load(const string& name)
{
	char* data = FilesManager::loadTextFile(name.c_str());

	if (!data)
	{
		return;
	}

	int len = strlen(data);
	int iChar = 0;
	int beginChar = 0;
	int countMesh_2Temporary_2 = 0;
	char* curentName = nullptr;
	PhysicType physicType = PhysicType::NONE;

	int countBlockSample = 0;
	const int maxBlockSample = 10 * 4;	// Максимальное кол-во поверхностей * 4 составляющие
	BlockSampleTemporary blockSamples[maxBlockSample];
	BlockSampleTemporary* currentBlockSample = nullptr;

	// Разделение данных на блоки
	while (iChar < len)
	{
		if (data[iChar] == '#')
		{

			if (data[iChar + 1] == ' ' &&
				data[iChar + 2] == 'o' &&
				data[iChar + 3] == 'b' &&
				data[iChar + 4] == 'j' &&
				data[iChar + 5] == 'e' &&
				data[iChar + 6] == 'c' &&
				data[iChar + 7] == 't' &&
				data[iChar + 8] == ' ')
			{
				char typeChar = data[iChar + 9];
				if (typeChar == '&')
				{
					_physicType = PhysicType::CONVEX;
				}
				else if (typeChar == '#')
				{
					_physicType = PhysicType::TRIANGLE;
				}
			}

			int offsetBackChar = 1;
			if (currentBlockSample && iChar > offsetBackChar)
			{
				data[iChar - offsetBackChar] = '\0';
				currentBlockSample->_endChar = &data[iChar - offsetBackChar];
				currentBlockSample->_countChar = iChar - currentBlockSample->_countChar - offsetBackChar;
				currentBlockSample = nullptr;
			}
		}

		if ((data[iChar] == 'v' || data[iChar] == 'f') && (iChar > 1 && data[iChar - 1] == '\n'))
		{
			if (!currentBlockSample)
			{
				if (countBlockSample >= maxBlockSample)
				{
					// LOG
					break;
				}

				if (data[iChar] == 'f')
				{
					++countMesh_2Temporary_2;
				}

				currentBlockSample = &blockSamples[countBlockSample];
				currentBlockSample->_beginChar = &data[iChar];
				currentBlockSample->_countChar = iChar;
				currentBlockSample->_physicType = physicType;
				++countBlockSample;
			}

			++currentBlockSample->_countItem;
		}

		++iChar;
	}

#ifdef WIN32
	std::ofstream _WRITE_LOG("Log.txt");
	_WRITE_LOG << "//\tcountBlockSample = " << countBlockSample << std::endl;

	for (int i = 0; i < countBlockSample; ++i)
	{
		_WRITE_LOG << "\n\n\t//\t_countItem = " << blockSamples[i]._countItem << " _countChar = " << blockSamples[i]._countChar << " -------------------------------------------------------------------------------------------------------" << std::endl;
		
		switch (blockSamples[i]._physicType)
		{
		case PhysicType::CONVEX:
			_WRITE_LOG << "CONVEX" << std::endl;
			break;
		case PhysicType::TRIANGLE:
			_WRITE_LOG << "TRIANGLE" << std::endl;
			break;
		default:
			_WRITE_LOG << "NONE" << std::endl;
			break;
		}

		_WRITE_LOG << blockSamples[i]._beginChar << std::endl;

	}
#endif
	
	return;

	// Преобразование данных и компоновка поверхности

	Mesh_2Temporary_2* aMesh_2Temporary_2 = new Mesh_2Temporary_2[countMesh_2Temporary_2];

	int lastMesh_2Vertex = 0;
	int lastMesh_2Texture = 0;
	int lastMesh_2Normal = 0;
	int lastMesh_2Index = 0;

	for (int i = 0; i < countBlockSample; ++i)
	{
		switch (blockSamples[i].getData())
		{
		case BlockSampleTemporaryType::VERTEX:
		{
			aMesh_2Temporary_2[lastMesh_2Vertex]._vertex = &blockSamples[i];
			aMesh_2Temporary_2[lastMesh_2Vertex].setPhysicType(blockSamples[i]._physicType);
			++lastMesh_2Vertex;
		} break;
		case BlockSampleTemporaryType::NORMAL:
		{
			aMesh_2Temporary_2[lastMesh_2Normal]._normal = &blockSamples[i];
			aMesh_2Temporary_2[lastMesh_2Normal].setPhysicType(blockSamples[i]._physicType);
			++lastMesh_2Normal;
		} break;
		case BlockSampleTemporaryType::TEXTURE:
		{
			aMesh_2Temporary_2[lastMesh_2Texture]._texture = &blockSamples[i];
			aMesh_2Temporary_2[lastMesh_2Texture].setPhysicType(blockSamples[i]._physicType);
			++lastMesh_2Texture;
		} break;
		case BlockSampleTemporaryType::INDEX:
		{
			aMesh_2Temporary_2[lastMesh_2Index]._index = &blockSamples[i];
			aMesh_2Temporary_2[lastMesh_2Index].setPhysicType(blockSamples[i]._physicType);
			++lastMesh_2Index;
		} break;
		}

	}

	countMesh_2Temporary_2 = lastMesh_2Vertex;
	countMesh_2Temporary_2 = lastMesh_2Normal < countMesh_2Temporary_2 ? lastMesh_2Normal : countMesh_2Temporary_2;
	countMesh_2Temporary_2 = lastMesh_2Texture < countMesh_2Temporary_2 ? lastMesh_2Texture : countMesh_2Temporary_2;
	countMesh_2Temporary_2 = lastMesh_2Index < countMesh_2Temporary_2 ? lastMesh_2Index : countMesh_2Temporary_2;

	unsigned short int countMesh_2 = 0;
	unsigned short int countPhysicMesh_2 = 0;

	for (int iMesh_2 = 0; iMesh_2 < countMesh_2Temporary_2; iMesh_2++)
	{
		if (aMesh_2Temporary_2[iMesh_2]._physicType == PhysicType::NONE)
		{
			++countMesh_2;
		}
		else
		{
			++countPhysicMesh_2;
		}
	}

	_countMesh_2 = countMesh_2;
	_aMesh_2 = new Mesh_2[_countMesh_2];

	_countPhysicMesh_2 = countPhysicMesh_2;
	_aPhysicMesh_2 = new Mesh_2[_countPhysicMesh_2];

	int iMesh_2 = 0;
	int iPhysicMesh_2 = 0;

	for (int i = 0; i < countMesh_2Temporary_2; i++)
	{
		if (aMesh_2Temporary_2[iMesh_2]._physicType == PhysicType::NONE)
		{
			getMesh_2(_aMesh_2[iMesh_2], aMesh_2Temporary_2[i]);
			++iMesh_2;
		}
		else
		{
			getMesh_2(_aPhysicMesh_2[iPhysicMesh_2], aMesh_2Temporary_2[i]);
			++iPhysicMesh_2;
		}
	}
}


int foundIndex(float *vertex, float *normal, float *texCoord,
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

	return USHRT_MAX;
}

void ShapeUnited_2::getMesh_2(Mesh_2& mesh, Mesh_2Temporary_2& meshTemporary)
{

#ifdef WIN32
	std::ofstream _WRITE_LOG("Log.txt");

	_WRITE_LOG << "//_vertex->_count = "  << meshTemporary._vertex->_count << '(' << (meshTemporary._vertex->_count / 3) << ")\t"  << '\t' << (int)meshTemporary._physicType << std::endl;

	for (int i = 0; i < meshTemporary._vertex->_count; i+=3)
	{
		float* vertex = meshTemporary._vertex->_array;
		_WRITE_LOG << "v\t" << vertex[i] << '\t' << vertex[i + 1] << '\t' << vertex[i + 2] << std::endl;

	}

	_WRITE_LOG << "\n//_normal->_count = " << meshTemporary._normal->_count << '(' << (meshTemporary._normal->_count / 3) << ")\t" << std::endl;

	for (int i = 0; i < meshTemporary._normal->_count; i += 3)
	{
		float* normal = meshTemporary._normal->_array;
		_WRITE_LOG << "vn\t" << normal[i] << '\t' << normal[i + 1] << '\t' << normal[i + 2] << std::endl;

	}

	_WRITE_LOG << "\n//_texture->_count = " << meshTemporary._texture->_count << '(' << (meshTemporary._texture->_count / 2) << ")\t" << std::endl;

	for (int i = 0; i < meshTemporary._texture->_count; i += 3)
	{
		float* texture = meshTemporary._texture->_array;
		_WRITE_LOG << "vt\t" << texture[i] << '\t' << texture[i + 1] << std::endl;

	}

	_WRITE_LOG << "\n//_index->_count = " << meshTemporary._index->_count << '(' << (meshTemporary._index->_count / 9) << ")\t" << std::endl;

	for (int i = 0; i < meshTemporary._index->_count; i += 9)
	{
		unsigned short* index = meshTemporary._index->_aIndex;
		_WRITE_LOG << "f\t" << index[i] << '/' << index[i + 1] << '/' << index[i + 2] << '\t' << index[i + 3] << '/' << index[i + 4] << '/' << index[i + 5] << '\t' << index[i + 6] << '/' << index[i + 7] << '/' << index[i + 8] << std::endl;

	}
#endif

	int countIndex = meshTemporary._index->_count / 3;

	float* aVertexNew = new float[countIndex * 3];
	float* aNormalNew = new float[countIndex * 3];
	float* aTexCoordNew = new float[countIndex * 2];
	unsigned short* aIndexNew = new unsigned short[countIndex];
	for (int i = 0; i < countIndex; ++i) aIndexNew[i] = USHRT_MAX;

	int iIndex = 0;
	int countVertexNew = 0;

	float* aVertex = meshTemporary._vertex->_array;
	float* aNormal = meshTemporary._normal->_array;
	float* aTexCoord = meshTemporary._texture->_array;
	unsigned short* aIndex = meshTemporary._index->_aIndex;

	for (int i = 0; i < meshTemporary._index->_count; i += 3)
	{
		int indexV = aIndex[i];
		float *vertex = &aVertex[indexV * 3];

		int indexT = aIndex[i + 1];
		float *texCoord = &aTexCoord[indexT * 2];

		int indexN = aIndex[i + 2];
		float *normal = &aNormal[indexN * 3];

		int index = foundIndex(vertex, normal, texCoord, aVertexNew, aNormalNew, aTexCoordNew, countVertexNew);
		if (index == USHRT_MAX)
		{
			index = countVertexNew;

			aVertexNew[index * 3] = vertex[0];
			aVertexNew[index * 3 + 1] = vertex[1];
			aVertexNew[index * 3 + 2] = vertex[2];

			aNormalNew[index * 3] = normal[0];
			aNormalNew[index * 3 + 1] = normal[1];
			aNormalNew[index * 3 + 2] = normal[2];

			aTexCoordNew[index * 2] = texCoord[0];
			aTexCoordNew[index * 2 + 1] = texCoord[1];

			++countVertexNew;
		}

		aIndexNew[iIndex] = index;
		++iIndex;
	}

	mesh._countVertex = countVertexNew;
	mesh._aIndex = aIndexNew;
	mesh._countIndex = countIndex;

	mesh._aVertex = new float[countVertexNew * 3];
	mesh._aNormal = new float[countVertexNew * 3];
	mesh._aTexCoord = new float[countVertexNew * 2];

	for (int i = 0; i < countVertexNew * 3; ++i) mesh._aVertex[i] = aVertexNew[i];
	for (int i = 0; i < countVertexNew * 3; ++i) mesh._aNormal[i] = aNormalNew[i];
	for (int i = 0; i < countVertexNew * 2; ++i) mesh._aTexCoord[i] = aTexCoordNew[i];

	check(mesh);
}

void ShapeUnited_2::check(Mesh_2& mesh)
{
#ifdef WIN32
	std::ofstream _WRITE_LOG("Log.txt");
	_WRITE_LOG << "//-------------------------" << std::endl;

	_WRITE_LOG << "\nVERTEXS: " << (mesh._aVertex ? "YES" : "NO") << "\t COUNT = " << mesh._countVertex << std::endl;
	if (mesh._aVertex)
	{
	for (int i = 0; i < mesh._countVertex * 3; i = i + 3)
	{
		_WRITE_LOG << '\t' << '[' << mesh._aVertex[i] << '\t' << mesh._aVertex[i + 1] << '\t' << mesh._aVertex[i + 2] << ']' << std::endl;
	}
	}

	_WRITE_LOG << "\nNORMALS: " << (mesh._aNormal ? "YES" : "NO") << std::endl;
	if (mesh._aNormal)
	{
	for (int i = 0; i < mesh._countVertex * 3; i = i + 3)
	{
		_WRITE_LOG << '\t' << '[' << mesh._aNormal[i] << '\t' << mesh._aNormal[i + 1] << '\t' << mesh._aNormal[i + 2] << ']' << std::endl;
	}
	}

	_WRITE_LOG << "\nTEXCOORDS: " << (mesh._aVertex ? "YES" : "NO") << std::endl;
	if (mesh._aTexCoord)
	{
	for (int i = 0; i < mesh._countVertex * 2; i = i + 2)
	{
		_WRITE_LOG << '\t' << '[' << mesh._aTexCoord[i] << '\t' << mesh._aTexCoord[i + 1] << ']' << std::endl;
	}
	}

	_WRITE_LOG << "\nINDEXES: " << (mesh._aIndex ? "YES" : "NO") << "\t COUNT = " << mesh._countIndex << std::endl;
	if (mesh._aIndex)
	{
	for (int i = 0; i < mesh._countIndex; i = i + 3)
	{
		_WRITE_LOG << '\t' << '[' << mesh._aIndex[i] << '\t' << mesh._aIndex[i + 1] << '\t' << mesh._aIndex[i + 2] << ']' << std::endl;
	}
	}
#endif
}