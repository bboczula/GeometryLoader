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
	FACE
};

class GeometryLoader : public IGeometryLoader
{
public:
	GeometryLoader();
	~GeometryLoader();
	virtual void loadFromFile(std::string filename);
	void printVertices();
	void printNormals();
	void printIndices();
	virtual int getNumOfVertices();
	virtual int getNumOfNormals();
	virtual int getNumOfIndices();
	virtual void parse(std::string& input);
	virtual FLOAT3 vertexAt(int i);
	virtual FLOAT3 normalAt(int i);
	virtual int vertexIndexAt(int i);
	virtual int normalIndexAt(int i);
private:
	void faceHandler(std::vector<std::string>& tokens);
	void tripletHandler(std::vector<std::string> triplets);
	void vertexHandler(std::vector<std::string>& tokens);
	void normalHandler(std::vector<std::string>& tokens);
	void addVertex(FLOAT3 vertex);
	void addNormal(FLOAT3 normal);
	void addIndex(short index);
	void addNormalIndex(short normalIndex);
	EKeyword translate(std::string keyword);
private:
	std::vector<FLOAT3> vertices;
	std::vector<FLOAT3> normals;
	std::vector<short> indices;
	std::vector<short> normalIndices;
};