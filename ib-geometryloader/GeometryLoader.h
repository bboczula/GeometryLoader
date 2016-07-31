#pragma once

#include "IGeometryLoader.h"

#include <fstream>
#include <vector>
#include <iostream>
#include <exception>
#include <sstream>

enum EKeyword
{
	UNDEFINED,
	VERTEX,
	NORMAL,
	FACE,
	TEXCOORD
};

class GeometryLoader : public IGeometryLoader
{
public:
	GeometryLoader();
	~GeometryLoader();
	virtual void loadFromFile(const std::string& filename);
	void printVertices();
	void printNormals();
	void printIndices();
	virtual int getNumOfVertices();
	virtual int getNumOfNormals();
	virtual int getNumOfTexCoords();
	virtual int getNumOfVertexIndices();
	virtual int getNumOfNormalIndices();
	virtual void parse(std::string& input);
	virtual FLOAT3 vertexAt(int i);
	virtual FLOAT3 normalAt(int i);
	virtual int vertexIndexAt(int i);
	virtual int normalIndexAt(int i);
	virtual int textureIndexAt(int i);
	virtual FLOAT2 texCoordAt(int i);
private:
	void faceHandler(std::vector<std::string>& tokens);
	void tripletHandler(std::vector<std::string> triplets);
	void vertexHandler(std::vector<std::string>& tokens);
	void normalHandler(std::vector<std::string>& tokens);
	void textureHandler(std::vector<std::string>& tokens);
	void addVertex(FLOAT3 vertex);
	void addNormal(FLOAT3 normal);
	void addTextureCoordinate(FLOAT2 texCoord);
	void addIndex(short index);
	void addNormalIndex(short normalIndex);
	void addTextureIndex(short normalIndex);
	EKeyword translate(std::string keyword);
	void printStatistics(const std::string& filename);
private:
	std::vector<FLOAT3> vertices;
	std::vector<FLOAT3> normals;
	std::vector<FLOAT2> textureCoordinates;
	std::vector<short> indices;
	std::vector<short> normalIndices;
	std::vector<short> textureIndices;
};