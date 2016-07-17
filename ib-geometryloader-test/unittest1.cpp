#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\ib-geometryloader\GeometryLoader.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ibgeometryloadertest
{		
	TEST_CLASS(FileTesting)
	{
		IGeometryLoader* geometryLoader;

		TEST_METHOD_INITIALIZE(SetupGeometryLoader)
		{
			geometryLoader = CreateGeometryLoader();
		}

		TEST_METHOD_CLEANUP(CleanupGeometryLoader)
		{
			if (geometryLoader)
				delete geometryLoader;
		}
		
		TEST_METHOD(BlenderCubeNumOfVertices)
		{
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
			Assert::AreEqual(8, geometryLoader->getNumOfVertices());
		}

		TEST_METHOD(BlenderCubeNumOfNormals)
		{
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
			Assert::AreEqual(6, geometryLoader->getNumOfNormals());
		}

		TEST_METHOD(BlenderCubeNumOfIndices)
		{
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
			Assert::AreEqual(36, geometryLoader->getNumOfVertexIndices());
		}

		TEST_METHOD(WilliamsCubeNumOfVertices)
		{
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
			Assert::AreEqual(8, geometryLoader->getNumOfVertices());
		}

		TEST_METHOD(WilliamsCubeNumOfNormals)
		{
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
			Assert::AreEqual(6, geometryLoader->getNumOfNormals());
		}

		TEST_METHOD(WilliamsCubeNumOfIndices)
		{
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\cube.obj");
			Assert::AreEqual(36, geometryLoader->getNumOfVertexIndices());
		}
	};

	TEST_CLASS(ParserTesting)
	{
		IGeometryLoader* geometryLoader;

		TEST_METHOD_INITIALIZE(SetupGeometryLoader)
		{
			geometryLoader = CreateGeometryLoader();
		}

		TEST_METHOD_CLEANUP(CleanupGeometryLoader)
		{
			if (geometryLoader)
				delete geometryLoader;
		}

		TEST_METHOD(ValidVertexHandler)
		{
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
			Assert::AreEqual(0, geometryLoader->getNumOfVertexIndices());

			FLOAT3 result = geometryLoader->vertexAt(0);
			Assert::AreEqual(0.5f, result.x);
			Assert::AreEqual(0.5f, result.y);
			Assert::AreEqual(0.5f, result.z);

			FLOAT3 result1 = geometryLoader->vertexAt(1);
			Assert::AreEqual(0.5f, result1.x);
			Assert::AreEqual(-0.5f, result1.y);
			Assert::AreEqual(0.5f, result1.z);
		}

		TEST_METHOD(InalidVertexHandler)
		{
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
			Assert::AreEqual(0, geometryLoader->getNumOfVertexIndices());
		}

		TEST_METHOD(ValidNormalHandler)
		{
			std::vector<std::string> input;
			input.push_back({ "vn 0.500 0.500 0.500" });
			input.push_back({ "vn 0.500 -0.500 0.500" });

			for (int i = 0; i < input.size(); i++)
				geometryLoader->parse(input[i]);

			Assert::AreEqual(0, geometryLoader->getNumOfVertices());
			Assert::AreEqual(2, geometryLoader->getNumOfNormals());
			Assert::AreEqual(0, geometryLoader->getNumOfVertexIndices());

			FLOAT3 normal = geometryLoader->normalAt(1);
			Assert::AreEqual(0.5f, normal.x);
			Assert::AreEqual(-0.5f, normal.y);
			Assert::AreEqual(0.5f, normal.z);
		}

		TEST_METHOD(FaceWithoutNormals)
		{
			std::vector<std::string> input;
			input.push_back({ "v 0.500 0.500 0.500" });
			input.push_back({ "v 0.500 -0.500 0.500" });
			input.push_back({ "v 0.500 -0.500 0.500" });
			input.push_back({ "f 1/17 2/18 3/13" });

			for (int i = 0; i < input.size(); i++)
				geometryLoader->parse(input[i]);

			Assert::AreEqual(3, geometryLoader->getNumOfVertices());
			Assert::AreEqual(0, geometryLoader->getNumOfNormals());
			Assert::AreEqual(3, geometryLoader->getNumOfVertexIndices());

			Assert::AreEqual(0, geometryLoader->vertexIndexAt(0));
			Assert::AreEqual(1, geometryLoader->vertexIndexAt(1));
			Assert::AreEqual(2, geometryLoader->vertexIndexAt(2));
		}

		TEST_METHOD(VariousFaceFormats)
		{
			std::vector<std::string> input;
			input.push_back({ "v 0.500 0.500 0.500" });
			input.push_back({ "v 0.500 -0.500 0.500" });
			input.push_back({ "v 0.500 -0.500 0.500" });
			input.push_back({ "f 1 2 3" });
			input.push_back({ "f 1/1 2/23 3/2" });
			input.push_back({ "f 1//2 2//32 3//24" });
			input.push_back({ "f 1/1/2 2/2/32 3/3/24" });

			for (int i = 0; i < input.size(); i++)
				geometryLoader->parse(input[i]);

			Assert::AreEqual(12, geometryLoader->getNumOfVertexIndices());
			Assert::AreEqual(1, geometryLoader->normalIndexAt(0));
			Assert::AreEqual(31, geometryLoader->normalIndexAt(1));
			Assert::AreEqual(23, geometryLoader->normalIndexAt(2));
			Assert::AreEqual(6, geometryLoader->getNumOfNormalIndices());
		}
	};

	TEST_CLASS(PerformanceTesting)
	{
		IGeometryLoader* geometryLoader;

		TEST_METHOD_INITIALIZE(SetupGeometryLoader)
		{
			geometryLoader = CreateGeometryLoader();
		}

		TEST_METHOD_CLEANUP(CleanupGeometryLoader)
		{
			if (geometryLoader)
				delete geometryLoader;
		}

		TEST_METHOD(BlenderMonkeyIntegrity)
		{
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\uber_monkey.obj");
			Assert::AreEqual(7958, geometryLoader->getNumOfVertices());
			Assert::AreEqual(15731, geometryLoader->getNumOfNormals());
			Assert::AreEqual(47232, geometryLoader->getNumOfVertexIndices());
		}

		TEST_METHOD(BlenderFuturisticCarIntegrity)
		{
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\futuristic_car.obj");
			Assert::AreEqual(4850, geometryLoader->getNumOfVertices());
			Assert::AreEqual(3652, geometryLoader->getNumOfNormals());
			Assert::AreEqual(26916, geometryLoader->getNumOfVertexIndices());
		}

		TEST_METHOD(BlenderMinecraftIntegrity)
		{
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\minecraft.obj");
			Assert::AreEqual(156841, geometryLoader->getNumOfVertices());
			Assert::AreEqual(0, geometryLoader->getNumOfNormals());
			Assert::AreEqual(742434, geometryLoader->getNumOfVertexIndices());
		}

		TEST_METHOD(BlenderF15Integrity)
		{
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\f15.obj");
			Assert::AreEqual(48421, geometryLoader->getNumOfVertices());
			Assert::AreEqual(0, geometryLoader->getNumOfNormals());
			Assert::AreEqual(136980, geometryLoader->getNumOfVertexIndices());
		}

		TEST_METHOD(Cinema4DCastleIntegrity)
		{
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\castle.obj");
			Assert::AreEqual(28348, geometryLoader->getNumOfVertices());
			Assert::AreEqual(0, geometryLoader->getNumOfNormals());
			Assert::AreEqual(142776, geometryLoader->getNumOfVertexIndices());
		}

		TEST_METHOD(Max3DSInteriorIntegrity)
		{
			geometryLoader->loadFromFile("..\\ib-geometryloader-res\\interior.obj");
			Assert::AreEqual(108156, geometryLoader->getNumOfVertices());
			Assert::AreEqual(484448, geometryLoader->getNumOfNormals());
			Assert::AreEqual(645144, geometryLoader->getNumOfVertexIndices());
		}
	};
}