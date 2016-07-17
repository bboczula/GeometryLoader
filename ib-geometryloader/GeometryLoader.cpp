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

void GeometryLoader::parse(std::string& input)
{
	std::istringstream stream(input);

	// 1. Prepare the keyword
	std::string keyword;
	stream >> keyword;

	if (keyword.compare("#") == 0)
		return;

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
	return EKeyword::UNDEFINED;
}

void GeometryLoader::addVertex(FLOAT3 vertex)
{
	vertices.push_back(vertex);
}

void GeometryLoader::addNormal(FLOAT3 normal)
{
	normals.push_back(normal);
}

void GeometryLoader::addIndex(short index)
{
	indices.push_back(index);
}

void GeometryLoader::addNormalIndex(short normalIndex)
{
	normalIndices.push_back(normalIndex);
}

void GeometryLoader::loadFromFile(std::string filename)
{
	std::string line;
	std::ifstream objFile(filename);
	if (objFile.is_open())
	{
		while (std::getline(objFile, line))
		{
			parse(line);
		}
		std::cout << "loaded " << filename << ": vertices = " << vertices.size() << ", indexes = " << indices.size() << ", normals = " << normals.size() << std::endl;
		objFile.close();
	}
	else
	{
		std::cout << "ERROR: cannot open a file\n";
	}
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

void GeometryLoader::faceHandler(std::vector<std::string>& tokens)
{
	int numOfIndices = tokens.size();
	if (numOfIndices < 3)
	{
		std::cout << "ERROR: to few indices (" << numOfIndices << ") !!!" << std::endl;
		return;
	}
	if (numOfIndices > 4)
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
	int numOfSlashes;
	std::vector<std::size_t> indexPosition;
	std::vector<std::size_t> texturePosition;
	std::vector<std::string> vertexIndex;
	std::vector<std::string> normalIndex;

	int i = 0;
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
			indexPosition.push_back(triplet.find('/'));
			texturePosition.push_back(triplet.find('/', indexPosition[i] + 1));
			vertexIndex.push_back(triplet.substr(0, indexPosition[i]));

			numOfSlashes = std::count(triplet.begin(), triplet.end(), '/');
			if (numOfSlashes == 1)
			{
				// This is allowed, no normal, just texture
			}
			if (numOfSlashes == 2)
			{
				normalIndex.push_back(triplet.substr(texturePosition[i] + 1, triplet.size() - texturePosition[i] + 1));
			}
			if (numOfSlashes > 2 || numOfSlashes < 1)
			{
				std::cout << "WARNING: invalid format of face component of OBJ file" << std::endl;
			}
		}
		i++;
	}

	addIndex(std::stoi(vertexIndex[0]) - 1);
	addIndex(std::stoi(vertexIndex[1]) - 1);
	addIndex(std::stoi(vertexIndex[2]) - 1);
	
	if (numOfSlashes == 2)
	{
		addNormalIndex(std::stoi(normalIndex[0]) - 1);
		addNormalIndex(std::stoi(normalIndex[1]) - 1);
		addNormalIndex(std::stoi(normalIndex[2]) - 1);
	}
	if (triplets.size() == 4)
	{
		addIndex(std::stoi(vertexIndex[2]) - 1);
		addIndex(std::stoi(vertexIndex[0]) - 1);
		addIndex(std::stoi(vertexIndex[3]) - 1);

		if (numOfSlashes == 2)
		{
			addNormalIndex(std::stoi(normalIndex[2]) - 1);
			addNormalIndex(std::stoi(normalIndex[0]) - 1);
			addNormalIndex(std::stoi(normalIndex[3]) - 1);
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


int GeometryLoader::getNumOfVertices()
{
	return vertices.size();
}


int GeometryLoader::getNumOfNormals()
{
	return normals.size();
}


int GeometryLoader::getNumOfIndices()
{
	return indices.size();
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