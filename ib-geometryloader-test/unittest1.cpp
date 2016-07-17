#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\ib-geometryloader\GeometryLoader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ibgeometryloadertest
{		
	TEST_CLASS(FileTesting)
	{
	public:
		
		TEST_METHOD(BlenderCubeNumOfVertices)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
			Assert::AreEqual(8, geometryLoader->getNumOfVertices());
			delete geometryLoader;
		}

		TEST_METHOD(BlenderCubeNumOfNormals)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
			Assert::AreEqual(6, geometryLoader->getNumOfNormals());
			delete geometryLoader;
		}

		TEST_METHOD(BlenderCubeNumOfIndices)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
			Assert::AreEqual(36, geometryLoader->getNumOfIndices());
			delete geometryLoader;
		}

		TEST_METHOD(WilliamsCubeNumOfVertices)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
			Assert::AreEqual(8, geometryLoader->getNumOfVertices());
			delete geometryLoader;
		}

		TEST_METHOD(WilliamsCubeNumOfNormals)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
			Assert::AreEqual(6, geometryLoader->getNumOfNormals());
			delete geometryLoader;
		}

		TEST_METHOD(WilliamsCubeNumOfIndices)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
			Assert::AreEqual(36, geometryLoader->getNumOfIndices());
			delete geometryLoader;
		}
	};

	TEST_CLASS(ParserTesting)
	{
		TEST_METHOD(ValidVertexHandler)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			std::vector<std::string> input;
			input.push_back({ "v 0.500 0.500 0.500" });
			input.push_back({ "v 0.500 -0.500 0.500" });
			input.push_back({ "v -1 0.500 0.500" });
			input.push_back({ "v 0.000000 0.000000 -1.000000" });
			input.push_back({ "v 0 0 -1" });
			input.push_back({ "v 0.999999 99.999999 99999.000000" });

			for (auto s : input)
				geometryLoader->parse(s);

			Assert::AreEqual(6, geometryLoader->getNumOfVertices());
			Assert::AreEqual(0, geometryLoader->getNumOfNormals());
			Assert::AreEqual(0, geometryLoader->getNumOfIndices());

			FLOAT3 result = geometryLoader->vertexAt(0);
			Assert::AreEqual(0.5f, result.x);
			Assert::AreEqual(0.5f, result.y);
			Assert::AreEqual(0.5f, result.z);

			FLOAT3 result1 = geometryLoader->vertexAt(1);
			Assert::AreEqual(0.5f, result1.x);
			Assert::AreEqual(-0.5f, result1.y);
			Assert::AreEqual(0.5f, result1.z);

			delete geometryLoader;
		}

		TEST_METHOD(InalidVertexHandler)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			std::vector<std::string> input;
			input.push_back({ "va 0.500 0.500 0.500" });
			input.push_back({ "vb 0.500 -0.500 0.500" });
			input.push_back({ "0 cube" });
			input.push_back({ "# comment" });
			input.push_back({ "usemtl file.mtl" });
			input.push_back({ "totally invalid input" });

			for (int i = 0; i < input.size(); i++)
				geometryLoader->parse(input[i]);

			Assert::AreEqual(0, geometryLoader->getNumOfVertices());
			Assert::AreEqual(0, geometryLoader->getNumOfNormals());
			Assert::AreEqual(0, geometryLoader->getNumOfIndices());

			delete geometryLoader;
		}

		TEST_METHOD(ValidNormalHandler)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			std::vector<std::string> input;
			input.push_back({ "vn 0.500 0.500 0.500" });
			input.push_back({ "vn 0.500 -0.500 0.500" });

			for (int i = 0; i < input.size(); i++)
				geometryLoader->parse(input[i]);

			Assert::AreEqual(0, geometryLoader->getNumOfVertices());
			Assert::AreEqual(2, geometryLoader->getNumOfNormals());
			Assert::AreEqual(0, geometryLoader->getNumOfIndices());

			FLOAT3 normal = geometryLoader->normalAt(1);
			Assert::AreEqual(0.5f, normal.x);
			Assert::AreEqual(-0.5f, normal.y);
			Assert::AreEqual(0.5f, normal.z);

			delete geometryLoader;
		}

		TEST_METHOD(FaceWithoutNormals)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			std::vector<std::string> input;
			input.push_back({ "v 0.500 0.500 0.500" });
			input.push_back({ "v 0.500 -0.500 0.500" });
			input.push_back({ "v 0.500 -0.500 0.500" });
			input.push_back({ "f 1/17 2/18 3/13" });

			for (int i = 0; i < input.size(); i++)
				geometryLoader->parse(input[i]);

			Assert::AreEqual(3, geometryLoader->getNumOfVertices());
			Assert::AreEqual(0, geometryLoader->getNumOfNormals());
			Assert::AreEqual(3, geometryLoader->getNumOfIndices());

			Assert::AreEqual(0, geometryLoader->vertexIndexAt(0));
			Assert::AreEqual(1, geometryLoader->vertexIndexAt(1));
			Assert::AreEqual(2, geometryLoader->vertexIndexAt(2));

			delete geometryLoader;
		}
	};

	TEST_CLASS(PerformanceTesting)
	{
		TEST_METHOD(BlenderMonkeyIntegrity)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\uber_monkey.obj");
			Assert::AreEqual(7958, geometryLoader->getNumOfVertices());
			Assert::AreEqual(15731, geometryLoader->getNumOfNormals());
			Assert::AreEqual(47232, geometryLoader->getNumOfIndices());
			delete geometryLoader;
		}

		TEST_METHOD(BlenderFuturisticCarIntegrity)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\futuristic_car.obj");
			Assert::AreEqual(4850, geometryLoader->getNumOfVertices());
			Assert::AreEqual(3652, geometryLoader->getNumOfNormals());
			Assert::AreEqual(26916, geometryLoader->getNumOfIndices());
			delete geometryLoader;
		}

		TEST_METHOD(BlenderMinecraftIntegrity)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\minecraft.obj");
			Assert::AreEqual(156841, geometryLoader->getNumOfVertices());
			Assert::AreEqual(0, geometryLoader->getNumOfNormals());
			Assert::AreEqual(742434, geometryLoader->getNumOfIndices());
			delete geometryLoader;
		}

		TEST_METHOD(BlenderF15Integrity)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\f15.obj");
			Assert::AreEqual(48421, geometryLoader->getNumOfVertices());
			Assert::AreEqual(0, geometryLoader->getNumOfNormals());
			Assert::AreEqual(136980, geometryLoader->getNumOfIndices());
			delete geometryLoader;
		}

		TEST_METHOD(Cinema4DCastleIntegrity)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\castle.obj");
			Assert::AreEqual(28348, geometryLoader->getNumOfVertices());
			Assert::AreEqual(0, geometryLoader->getNumOfNormals());
			Assert::AreEqual(142776, geometryLoader->getNumOfIndices());
			delete geometryLoader;
		}

		TEST_METHOD(Max3DSInteriorIntegrity)
		{
			IGeometryLoader* geometryLoader = CreateGeometryLoader();
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\interior.obj");
			Assert::AreEqual(108156, geometryLoader->getNumOfVertices());
			Assert::AreEqual(484448, geometryLoader->getNumOfNormals());
			Assert::AreEqual(645144, geometryLoader->getNumOfIndices());
			delete geometryLoader;
		}
	};
}