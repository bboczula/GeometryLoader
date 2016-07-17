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

class IB_GEOMETRYLOADER_API IGeometryLoader
{
public:
	virtual void loadFromFile(std::string filename) = 0;
	virtual void parse(std::string& input) = 0;
	virtual int getNumOfVertices() = 0;
	virtual int getNumOfNormals() = 0;
	virtual int getNumOfVertexIndices() = 0;
	virtual int getNumOfNormalIndices() = 0;
	virtual FLOAT3 vertexAt(int i) = 0;
	virtual FLOAT3 normalAt(int i) = 0;
	virtual int vertexIndexAt(int i) = 0;
	virtual int normalIndexAt(int i) = 0;
};

IB_GEOMETRYLOADER_API IGeometryLoader* CreateGeometryLoader();