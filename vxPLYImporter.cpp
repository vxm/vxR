#include "vxPLYImporter.h"

using namespace vxCore;

vxPLYImporter::vxPLYImporter(vxTriangleMeshHandle geo)
	:m_geo(geo)
{
}

vxTriangleMeshHandle vxPLYImporter::getGeometry() const
{
	return m_geo;
}

void vxPLYImporter::setGeometry(const vxTriangleMeshHandle &geo)
{
	m_geo = geo;
}

void vxPLYImporter::processPLYFile(const std::string &fileName)
{
	if (!FileUtils::fileExists(fileName))
	{
		std::cerr << "PLY: File " << fileName << " doesn't exist" << std::endl;
		return;
	}

	std::ifstream iFile(fileName);
	std::string line;

	m_geo->clear();
	// Not optional lines.
	std::getline(iFile, line);
	if(line!="ply")
	{
		std::cerr << "PLY: File " << fileName << " doesn't contain ply info" << std::endl;
		return;
	}
	
	// checking if ASCII
	std::getline(iFile, line);
	if(line!="format ascii 1.0")
	{
		std::cerr << "PLY: File " << fileName << " is not ASCII" << std::endl;
		return;
	}
	
	//optional lines
	
	// comments.
	std::getline(iFile, line);
	std::cout << "PLY: File Comments:: '" << line.substr(8) << "'" << std::endl;
	
	// element vertex.
	std::getline(iFile, line);
	auto vertexAmountTok = StringUtils::tokenizeSpace(line);
	if(vertexAmountTok.size()!=3)
	{
		std::cerr << "PLY: vertex count is unexpected:: '" << line << "'" << std::endl;
		return;
	}
	
	auto numVertex = std::stoul(vertexAmountTok[2]);
	std::cout << "PLY: Num vertex: " << numVertex << std::endl;
	unsigned int nFaces{0u};
	
	// reading properties.
	do
	{
		std::getline(iFile, line);
	
		auto lineTokens = StringUtils::tokenizeSpace(line);
		std::cout << "PLY: paramter tokens: ";
		for(auto&& t:lineTokens)
		{
			std::cout << " [" << t << "]";
		}
		std::cout << std::endl;
		
		if(lineTokens.size()==3
				&& lineTokens[0]=="element"
				&& lineTokens[1]=="face")
		{
			nFaces = std::stoi(lineTokens[2]);
			std::cout << "PLY: num faces: " << nFaces << std::endl;
		}
	}
	while(line!="end_header");
	
	m_geo->open();
	
	unsigned int k {0};
	while(std::getline(iFile, line) && k<numVertex)
	{
		std::istringstream is(line);
		const auto &vts = StringUtils::tokenizeSpace(line);
		if(vts.size()%3!=0)
		{
			std::cerr << "PLY: Line " << k << " could not be parsed as xyx scalar" << std::endl;
		}
		else
		{
			//std::cout << "PLY: X " << x << "  Y "  << y << "  Z "  << z << " found on line: " << k << std::endl;
			scalar x, y, z;
			x = std::stod(vts[0]);
			y = std::stod(vts[1]);
			z = std::stod(vts[2]);
			m_geo->addVertexTransformed(v3s{x,y,z});
		}
		
		k++;
	}
	
	std::vector<std::vector<v3s>> normals(m_geo->vertexCount());
	TriIndices indices;
	
	auto capturedFaces{0};
	// reading properties.
	do
	{
		auto lineTokens = StringUtils::tokenizeSpace(line);
		if(lineTokens.size()==4	&& lineTokens[0]=="3"s)
		{
			unsigned long a = std::stoul(lineTokens[1]);
			unsigned long b = std::stoul(lineTokens[2]);
			unsigned long c = std::stoul(lineTokens[3]);
			
			indices.emplace_back(std::array<unsigned long,3>{a,b,c});
			
			auto& newTri = m_geo->addTriangle(a,b,c);
			newTri.computeArea();
			newTri.computeNormals();
			
			normals[a].emplace_back(newTri.normal());
			normals[b].emplace_back(newTri.normal());
			normals[c].emplace_back(newTri.normal());
			
			capturedFaces++;
		}

		if(lineTokens.size()==5	&& lineTokens[0]=="4"s)
		{
			unsigned long a = std::stoul(lineTokens[1]);
			unsigned long b = std::stoul(lineTokens[2]);
			unsigned long c = std::stoul(lineTokens[3]);
			unsigned long d = std::stoul(lineTokens[4]);
			
			auto& newTriA = m_geo->addTriangle(a,b,c);
			indices.emplace_back(std::array<unsigned long,3>{a,b,c});
			newTriA.computeArea();
			newTriA.computeNormals();
			normals[a].emplace_back(newTriA.normal());
			normals[b].emplace_back(newTriA.normal());
			normals[c].emplace_back(newTriA.normal());
			
			auto& newTriB = m_geo->addTriangle(d,a,c);
			indices.emplace_back(std::array<unsigned long,3>{d,a,c});
			newTriB.computeArea();
			newTriB.computeNormals();
			normals[d].emplace_back(newTriB.normal());
			normals[a].emplace_back(newTriB.normal());
			normals[c].emplace_back(newTriB.normal());
			
			capturedFaces+=2;
		}
		
		for(auto& n:normals)
		{
			if(!n.size())
				break;
			
			auto sz = n.size();
			auto avg = n[0];
			for(int i=1;i<sz;i++)
			{
				avg+=n[i];
			}
			avg/=(scalar)sz;
			n.resize(1);
			n[0] = avg;
		}
		
		//!Averaging normals, could be part of other process.
		for(int i=0;i<indices.size();i++)
		{
			indices[i];
			normals[i];
		}

		if(lineTokens.size()>5)
		{
			std::cerr << "N-polygons are not added to geometry" << std::endl;
		}
	}
	while(std::getline(iFile, line));
	
	std::cout << "PLY: file "s 
			  << fileName 
			  << " :: '"s
			  << m_geo->vertexCount() 
			  << "' vertices "s
			  << " :: '"s
			  << m_geo->triangleCount() 
			  << "' triangles taken"s
			  << std::endl;

	m_geo->close();
	iFile.close();
}

