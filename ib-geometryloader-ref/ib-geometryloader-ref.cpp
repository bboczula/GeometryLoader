// ib-geometryloader-ref.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "IGeometryLoader.h"

#include <iostream>

int _tmain(int argc, _TCHAR* argv[])
{
	IGeometryLoader* geometryLoader = CreateGeometryLoader();

	geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
	for (int i = 0; i < geometryLoader->getNumOfVertices(); i++)
	{
		FLOAT3 vertex = geometryLoader->vertexAt(i);
		std::cout << "v " << vertex << std::endl;
	}
	delete geometryLoader;

	getchar();
	return 0;
}