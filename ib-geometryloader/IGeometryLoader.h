#pragma once

#ifdef IB_GEOMETRYLOADER_EXPORTS
#define IB_GEOMETRYLOADER_API __declspec(dllexport)
#else
#define IB_GEOMETRYLOADER_API __declspec(dllimport)
#endif

#include <string>

struct IB_GEOMETRYLOADER_API FLOAT3
{
	float x;
	float y;
	float z;
	friend std::ostream& operator<<(std::ostream& os, const FLOAT3& f);
};

std::ostream& operator<<(std::ostream& os, const FLOAT3& f)
{
	os << '(' << f.x << ',' << f.y << ',' << f.z << ')';
	return os;
}

struct IB_GEOMETRYLOADER_API FLOAT2
{
	float x;
	float y;
	friend std::ostream& operator<<(std::ostream& os, const FLOAT2& f);
};

std::ostream& operator<<(std::ostream& os, const FLOAT2& f)
{
	os << '(' << f.x << ',' << f.y << ')';
	return os;
}

class IB_GEOMETRYLOADER_API IGeometryLoader
{
public:
	virtual void loadFromFile(const std::string& filename) = 0;
	virtual void parse(std::string& input) = 0;
	virtual int getNumOfVertices() = 0;
	virtual int getNumOfNormals() = 0;
	virtual int getNumOfTexCoords() = 0;
	virtual int getNumOfVertexIndices() = 0;
	virtual int getNumOfNormalIndices() = 0;
	virtual FLOAT3 vertexAt(int i) = 0;
	virtual FLOAT3 normalAt(int i) = 0;
	virtual int vertexIndexAt(int i) = 0;
	virtual int normalIndexAt(int i) = 0;
	virtual FLOAT2 texCoordAt(int i) = 0;
	virtual int textureIndexAt(int i) = 0;
};

IB_GEOMETRYLOADER_API IGeometryLoader* CreateGeometryLoader();