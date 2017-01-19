#include "stdafx.h"

#include "GeometryLoader.h"

IGeometryLoader* CreateGeometryLoader()
{
	return new GeometryLoader;
}

GeometryLoader::GeometryLoader()
{
}

GeometryLoader::~GeometryLoader()
{
}

void GeometryLoader::loadFromFile(const std::string& filename)
{
	std::ifstream objFile(filename);
	if (objFile.is_open())
	{
		std::string line;
		while (std::getline(objFile, line))
		{
			parse(line);
		}
		objFile.close();
		printStatistics(filename);
	}
	else
	{
		std::cout << "ERROR: cannot open a file\n";
	}
}

void GeometryLoader::parse(std::string& input)
{
	std::istringstream stream(input);

	// 1. Prepare the keyword
	std::string keyword;
	stream >> keyword;

	if (keyword.compare("#") == 0)
	{
		return;
	}

	// 2. Prepare tokens
	std::vector<std::string> tokens;
	tokens.reserve(4);

	std::string temp;
	while (stream >> temp)
	{
		tokens.push_back(temp);
	}

	switch (translate(keyword))
	{
	case EKeyword::UNDEFINED:
		break;
	case EKeyword::VERTEX:
		vertexHandler(tokens);
		break;
	case EKeyword::NORMAL:
		normalHandler(tokens);
		break;
	case EKeyword::FACE:
		faceHandler(tokens);
		break;
	case EKeyword::TEXCOORD:
		textureHandler(tokens);
	default:
		break;
	}
}

EKeyword GeometryLoader::translate(std::string keyword)
{
	if (keyword.compare("v") == 0)
		return EKeyword::VERTEX;
	else if (keyword.compare("vn") == 0)
		return EKeyword::NORMAL;
	else if (keyword.compare("f") == 0)
		return EKeyword::FACE;
	else if (keyword.compare("vt") == 0)
		return EKeyword::TEXCOORD;
	return EKeyword::UNDEFINED;
}

void GeometryLoader::faceHandler(std::vector<std::string>& tokens)
{
	const int MIN_NUM_OF_INDICES{ 3 };
	const int MAX_NUM_OF_INDICES{ 4 };

	int numOfIndices = tokens.size();
	if (numOfIndices < MIN_NUM_OF_INDICES)
	{
		std::cout << "ERROR: to few indices (" << numOfIndices << ") !!!" << std::endl;
		return;
	}
	if (numOfIndices > MAX_NUM_OF_INDICES)
	{
		std::cout << "WARNING: a face is a complex plygon (" << numOfIndices << ")." << std::endl;
		return;
	}

	try
	{
		tripletHandler(tokens);
	}
	catch (std::invalid_argument e)
	{
		std::cout << "ERROR: " << e.what() << std::endl;
	}
}

void GeometryLoader::tripletHandler(std::vector<std::string> triplets)
{
	std::vector<std::string> vertexIndex;
	std::vector<std::string> normalIndex;
	std::vector<std::string> textureIndex;

	int numOfVertices = vertices.size();
	for each (auto triplet in triplets)
	{
		if (std::stoi(triplet) > numOfVertices)
		{
			std::ostringstream msg;
			msg << "Vertex index(" << std::stoi(triplet) << ") is out of range (max " << numOfVertices << ")";
			throw std::invalid_argument(msg.str().c_str());
		}
		else
		{
			std::istringstream tripletStream;
			tripletStream.str(triplet);

			std::string temp;
			std::vector<std::string> another;
			const int CHUNK_SIZE{ 3 };
			another.reserve(CHUNK_SIZE);
			while (std::getline(tripletStream, temp, '/'))
			{
				another.push_back(temp);
			}

			switch (another.size())
			{
			case 3:
				if (!another[2].empty())
					normalIndex.push_back(another[2]);
			case 2:
				if (!another[1].empty())
					textureIndex.push_back(another[1]);
			case 1:
				if (!another[0].empty())
					vertexIndex.push_back(another[0]);
			default:
				break;
			}
		}
	}

	addIndex(std::stoi(vertexIndex[0]) - 1);
	addIndex(std::stoi(vertexIndex[1]) - 1);
	addIndex(std::stoi(vertexIndex[2]) - 1);

	if (!normalIndex.empty())
	{
		addNormalIndex(std::stoi(normalIndex[0]) - 1);
		addNormalIndex(std::stoi(normalIndex[1]) - 1);
		addNormalIndex(std::stoi(normalIndex[2]) - 1);
	}

	if (!textureIndex.empty())
	{
		addTextureIndex(std::stoi(textureIndex[0]) - 1);
		addTextureIndex(std::stoi(textureIndex[1]) - 1);
		addTextureIndex(std::stoi(textureIndex[2]) - 1);
	}

	if (triplets.size() == 4)
	{
		addIndex(std::stoi(vertexIndex[2]) - 1);
		addIndex(std::stoi(vertexIndex[0]) - 1);
		addIndex(std::stoi(vertexIndex[3]) - 1);

		if (!normalIndex.empty())
		{
			addNormalIndex(std::stoi(normalIndex[2]) - 1);
			addNormalIndex(std::stoi(normalIndex[0]) - 1);
			addNormalIndex(std::stoi(normalIndex[3]) - 1);
		}

		if (!textureIndex.empty())
		{
			addTextureIndex(std::stoi(textureIndex[2]) - 1);
			addTextureIndex(std::stoi(textureIndex[0]) - 1);
			addTextureIndex(std::stoi(textureIndex[3]) - 1);
		}

	}
}

void GeometryLoader::vertexHandler(std::vector<std::string>& tokens)
{
	addVertex(FLOAT3{ std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2]) });
}

void GeometryLoader::normalHandler(std::vector<std::string>& tokens)
{
	addNormal(FLOAT3{ std::stof(tokens[0]), std::stof(tokens[1]), std::stof(tokens[2]) });
}

void GeometryLoader::textureHandler(std::vector<std::string>& tokens)
{
	addTextureCoordinate(FLOAT2{ std::stof(tokens[0]), std::stof(tokens[1]) });
}

void GeometryLoader::printStatistics(const std::string& filename)
{
	std::cout << "loaded " << filename << ": vertices = " << vertices.size()
		<< ", indexes = " << indices.size() << ", normals = " << normals.size()
		<< ", textureCoordinates = " << textureCoordinates.size() << std::endl;
}

void GeometryLoader::addVertex(FLOAT3 vertex)
{
	vertices.push_back(vertex);
}

void GeometryLoader::addNormal(FLOAT3 normal)
{
	normals.push_back(normal);
}

void GeometryLoader::addTextureCoordinate(FLOAT2 texture)
{
	textureCoordinates.push_back(texture);
}

void GeometryLoader::addIndex(short index)
{
	indices.push_back(index);
}

void GeometryLoader::addNormalIndex(short normalIndex)
{
	normalIndices.push_back(normalIndex);
}

void GeometryLoader::addTextureIndex(short textureIndex)
{
	textureIndices.push_back(textureIndex);
}

void GeometryLoader::printVertices()
{
	for each (auto vertex in vertices)
	{
		std::cout << "v" << vertex << std::endl;
	}
}

void GeometryLoader::printNormals()
{
	for each(auto normal in normals)
	{
		std::cout << "vn" << normal << std::endl;
	}
}

void GeometryLoader::printIndices()
{
	for each(auto index in indices)
	{
		std::cout << "f " << index << "//" << std::endl;
	}
}

int GeometryLoader::getNumOfVertices()
{
	return vertices.size();
}

int GeometryLoader::getNumOfNormals()
{
	return normals.size();
}

int GeometryLoader::getNumOfTexCoords()
{
	return textureCoordinates.size();
}

int GeometryLoader::getNumOfVertexIndices()
{
	return indices.size();
}

int GeometryLoader::getNumOfNormalIndices()
{
	return normalIndices.size();
}

FLOAT3 GeometryLoader::vertexAt(int i)
{
	return vertices[i];
}

FLOAT3 GeometryLoader::normalAt(int i)
{
	return normals[i];
}

int GeometryLoader::vertexIndexAt(int i)
{
	return indices[i];
}

int GeometryLoader::normalIndexAt(int i)
{
	return normalIndices[i];
}

int GeometryLoader::textureIndexAt(int i)
{
	return textureIndices[i];
}

FLOAT2 GeometryLoader::texCoordAt(int i)
{
	return textureCoordinates[i];
}