// ib-geometryloader-ref.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "IGeometryLoader.h"

#include <iostream>
#include <vector>

int _tmain(int argc, _TCHAR* argv[])
{
	IGeometryLoader* geometryLoader = CreateGeometryLoader();

	//geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
	std::vector<std::string> insturctions;
	insturctions.push_back("v 1 2 3");
	insturctions.push_back("v 4 5 6");
	insturctions.push_back("v 1 1 1");
	insturctions.push_back("f 1 2 3");
	insturctions.push_back("f 1/9 2/9 3/9");
	insturctions.push_back("f 1//9 2//9 3//9");
	insturctions.push_back("f 1/8/10 2/8/10 3/8/10");

	for each (std::string i in insturctions)
	{
		geometryLoader->parse(i);
	}

	for (int i = 0; i < geometryLoader->getNumOfVertices(); i++)
	{
		FLOAT3 vertex = geometryLoader->vertexAt(i);
		std::cout << "v " << vertex << std::endl;
	}
	delete geometryLoader;

	getchar();
	return 0;
}