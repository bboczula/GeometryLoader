// ib-geometryloader-ref.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#include "IGeometryLoader.h"

#include <iostream>
#include <vector>

int _tmain(int argc, _TCHAR* argv[])
{
	IGeometryLoader* geometryLoader = CreateGeometryLoader();

	geometryLoader->loadFromFile("..\\ib-geometryloader-res\\uber_monkey.obj");
	delete geometryLoader;

	getchar();
	return 0;
}