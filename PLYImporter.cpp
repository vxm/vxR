#include "PLYImporter.h"
#include <map>

using namespace vxCore;

PLYImporter::PLYImporter(vxTriangleMeshHandle geo) : m_geo(geo) {}

vxTriangleMeshHandle PLYImporter::getGeometry() const { return m_geo; }

void PLYImporter::setGeometry(const vxTriangleMeshHandle &geo) { m_geo = geo; }

bool PLYImporter::processPLYFile(const std::string &fileName)
{
	if (!FileUtils::fileExists(fileName))
	{
		std::cerr << "PLY: File " << fileName << " doesn't exist" << std::endl;
		return false;
	}

	std::ifstream iFile(fileName);
	std::string line;

	m_geo->clear();
	// Not optional lines.
	std::getline(iFile, line);
	if (line != "ply")
	{
		std::cerr << "PLY: File " << fileName << " doesn't contain ply info"
				  << std::endl;
		return false;
	}

	// checking if ASCII
	std::getline(iFile, line);
	if (line != "format ascii 1.0")
	{
		std::cerr << "PLY: File " << fileName << " is not ASCII" << std::endl;
		return false;
	}

	// optional lines

	// comments.
	std::getline(iFile, line);
	std::cout << "PLY: File Comments:: '" << line.substr(8) << "'" << std::endl;

	// element vertex.
	std::getline(iFile, line);
	auto vertexAmountTok = StringUtils::tokenizeSpace(line);
	if (vertexAmountTok.size() != 3)
	{
		std::cerr << "PLY: vertex count is unexpected:: '" << line << "'"
				  << std::endl;
		return false;
	}

	auto numVertex = std::stoul(vertexAmountTok[2]);
	std::cout << "PLY: Num vertex: " << numVertex << std::endl;
	unsigned int nFaces{0u};

	std::map<std::string, std::string> properties;
	// reading properties.
	do
	{
		std::getline(iFile, line);

		auto &&lineTokens = StringUtils::tokenizeSpace(line);
		const auto ts = lineTokens.size();

		if (ts > 1)
		{
			if (lineTokens[0] == "property" && lineTokens[1] != "list")
			{
				// Needs to be three but not in case is a list.
				if (ts != 3)
				{
					std::cerr << "Property description is not correct." << std::endl;
				}

				std::cout << "PLY: paramter: " << lineTokens[2] << " of type, "
						  << lineTokens[1] << std::endl;

				properties[lineTokens[2]] = lineTokens[1];
			}
		}

		// In this same block there must be
		// another block face of properties
		if (ts == 3 && lineTokens[0] == "element" && lineTokens[1] == "face")
		{
			nFaces = std::stoi(lineTokens[2]);
			std::cout << "PLY: num faces: " << nFaces << std::endl;
		}
	} while (line != "end_header");

	m_geo->open();

	std::cout << "Reading vertex data" << std::endl;

	// Reading vertex data.
	unsigned int k{0};
	while (std::getline(iFile, line) && k < numVertex)
	{
		auto &&vts = StringUtils::tokenizeSpace(line);
		if (vts.size() != properties.size())
		{
			std::cerr << "PLY: Line " << k << " could not be parsed as xyx scalar"
					  << std::endl;
			return false;
		}
		else
		{
			scalar x, y, z;
			switch (properties.size())
			{
			case 3:
				x = std::stod(vts[0]);
				y = std::stod(vts[1]);
				z = std::stod(vts[2]);
				m_geo->addVertexTransformed({x, y, z});
				m_geo->addUV({.5, .5});
				break;
			case 6:
				x = std::stod(vts[0]);
				y = std::stod(vts[1]);
				z = std::stod(vts[2]);
				m_geo->addVertexTransformed({x, y, z});
				x = std::stod(vts[3]);
				y = std::stod(vts[4]);
				z = std::stod(vts[5]);
				m_geo->addVertexNormalTransformed({x, y, z});
				x = MU::getRand();
				y = MU::getRand();
				m_geo->addUV({x, y});
				break;
			case 8:
				x = std::stod(vts[0]);
				y = std::stod(vts[1]);
				z = std::stod(vts[2]);
				m_geo->addVertexTransformed({x, y, z});
				x = std::stod(vts[3]);
				y = std::stod(vts[4]);
				z = std::stod(vts[5]);
				m_geo->addVertexNormalTransformed({x, y, z});
				x = std::stod(vts[6]);
				y = std::stod(vts[7]);
				m_geo->addUV({x, y});
				break;
			default:
				break;
			}

			if (x > 10.0)
			{
				std::cout << "big x value " << x << std::endl;
			}
		}

		k++;
	}

	std::cout << "Reading face data" << std::endl;

	auto capturedFaces{0};
	// reading properties.
	do
	{
		auto lineTokens = StringUtils::tokenizeSpace(line);
		if (lineTokens.size() == 4 && lineTokens[0] == "3"s)
		{
			unsigned long a = std::stoul(lineTokens[1]);
			unsigned long b = std::stoul(lineTokens[2]);
			unsigned long c = std::stoul(lineTokens[3]);

			auto &newTri = m_geo->addTriangle(a, b, c);
			newTri.computeArea();

			if (newTri.area() > 1.0)
			{
				std::cout << "big area " << newTri.area() << std::endl;
			}

			capturedFaces++;
		}

		if (lineTokens.size() == 5 && lineTokens[0] == "4"s)
		{
			std::cerr << "Quads are not allowed right now." << std::endl;

			unsigned long a = std::stoul(lineTokens[1]);
			unsigned long b = std::stoul(lineTokens[2]);
			unsigned long c = std::stoul(lineTokens[3]);
			unsigned long d = std::stoul(lineTokens[4]);

			auto &newTriA = m_geo->addTriangle(a, b, c);
			newTriA.computeArea();

			auto &newTriB = m_geo->addTriangle(d, a, c);
			newTriB.computeArea();

			capturedFaces += 2;
		}

		if (lineTokens.size() > 5)
		{
			std::cerr << "N-polygons are not added to geometry" << std::endl;
		}
	} while (std::getline(iFile, line));

	std::cout << "PLY: file "s << fileName << " :: '"s << m_geo->vertexCount()
			  << "' vertices "s
			  << " :: '"s << m_geo->triangleCount() << "' triangles taken"s
			  << std::endl;

	m_geo->close();
	auto bb = m_geo->boundingBox();
	std::cout << "final bb min " << bb->minX() << " " << bb->minY() << " "
			  << bb->minZ() << std::endl;
	std::cout << "final bb max " << bb->maxX() << " " << bb->maxY() << " "
			  << bb->maxZ() << std::endl;

	iFile.close();

	return true;
}
