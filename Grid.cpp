#include <cassert>
#include <mutex>
#include <fstream>

#include "MathUtils.h"
#include "Grid.h"
std::mutex gridMutex;
using namespace vxCore;

#define DBL_EPSILON 1e-12
#define DRAWBBOX 0

Grid::Grid()
{
	m_size = 1.0;
	
	createGridData(5);
	initialize();
	updateBB();
	srand(time(NULL));
}

Grid::Grid(const v3s &position, scalar size)
	: m_position(position)
{
	setSize(size);
	createGridData(5);
	
	initialize();
	updateBB();
	srand(time(NULL));
}

Grid::Grid(scalar x, scalar y, scalar z, scalar size)
{ 
	m_position.set(x,y,z);
	setSize(size);
	
	createGridData(5);
	
	initialize();
	updateBB();
	srand(time(NULL));
}

Grid::~Grid()
{
}

void Grid::updateBB()
{
	m_bb->set(m_position, m_size);
}

void Grid::createGridData(const unsigned long resolution)
{
	m_resolution = resolution;
	m_c_resXres = resolution*resolution;
	m_c_resXresXres = m_c_resXres*resolution;
	m_data.resize(m_c_resXresXres);
	m_c_resDivTres = m_c_midSize/(scalar)m_resolution;
	setBoxSize();
}

void Grid::setResolution(unsigned long resolution)
{	
	if(resolution!=m_resolution)
	{
		createGridData(resolution);
	}
}

void Grid::setSize(const scalar size)
{
	m_size = size;
	m_c_midSize = size/scalar(2.0);
	setBoxSize();
}

unsigned long Grid::size() const
{
	return m_size;
}

void Grid::setPosition(const v3s &position)
{
	m_position = position;
	updateBB();
}

v3s Grid::position() const
{
	return m_position;
}

unsigned long Grid::resolution() const
{
	return m_resolution;
}

void Grid::setBoxSize()
{
	m_c_boxSize = m_size/scalar(m_resolution);
	m_c_midBoxSize = m_c_boxSize/scalar(2.0);
}

void Grid::createDiagonals(unsigned char colorIndex)
{
	unsigned long rMO=m_resolution-1;
	for (unsigned long i=0;i<m_resolution;i++)
	{
		vxAt(i,i,i).setByte(colorIndex);
		vxAt(rMO-i,rMO-i,i).setByte(colorIndex);
		vxAt(i,rMO-i,rMO-i).setByte(colorIndex);
		vxAt(rMO-i,i,rMO-i).setByte(colorIndex);
	}
}

void Grid::dumpFileInMemory(const std::string &fileName)
{
	std::ifstream file(fileName, std::ios::binary);
	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	
	if(size>(std::streamsize)m_data.size())
	{
		std::cout << "Error, trying to dump long file to memory "
				  << fileName << std::endl;
		return;
	}
	
	std::vector<char> buffer(size);
	if (file.read((char*)&(m_data[0]), size))
	{
		/* worked! */
	}
	
	file.close();
}

void Grid::dumpNumericTypeInMemory()
{
	using numType = int;
	void *p = (void*) &m_data[0];
	new (p) numType(std::numeric_limits<numType>::max());
}

///////////////////////////////////
///////////////////////////////////

unsigned int Grid::neighboursAlive(unsigned long long idx)
{
	long retx;
	long rety;
	long retz;
	
	getComponentsOfIndex(idx, retx, rety, retz);
	
	unsigned int ret{0u};
	
	for(auto x = retx - 1l; x < retx+2; ++x)
		for(auto y = rety - 1l; y < rety+2; ++y)
			for(auto z = retz - 1l; z < retz+2; ++z)
			{
				if(retx==x && rety==y && retz==z)
				{
					continue;
				}
				
				if(x >= (long)m_resolution
					|| y >= (long)m_resolution
					|| z >= (long)m_resolution
					|| x<0
					|| y<0 
					|| z<0)
				{
					continue;
				}
				
				auto ind = index(x, y, z);
				ret += vxAt(ind).active() ? 1u : 0u;
			}
	
	return ret;
}

unsigned long long Grid::playGameOfLife()
{
	unsigned long long idx{0ull};
	unsigned long long newLife{0ul};
	
	for(auto& d:m_data)
	{
		auto pop = neighboursAlive(idx);
		
		if(d.activeBit(7))
		{
			if(pop<5 || pop>5)
			{
				markCellAsDead(d);
			}
			
		}
		else
		{
			if(pop>3 && pop<5)
			{
				markCellForGenesis(d);
				newLife++;
			}
		}
		idx++;
	}
	return newLife;
}

void Grid::markCellAsDead(vx &cell)
{
	cell.c = 0b0000'0000;
	cell.activateBit(4);
}

void Grid::markCellForGenesis(vx &cell)
{
	cell.c = 0b0000'0000;
	cell.activateBit(6);
}

unsigned long long Grid::killTheDead()
{
	unsigned long long deadCells{0ull};
	for(auto& d:m_data)
	{
		unsigned char tm = d.c;
		if(d.activeBit(4))
		{
			d.deactivate();
			deadCells++;
			continue;
		}
		
		if(d.activeBit(6))
		{
			d.activate();
			continue;
		}
		
		if(!d.activeBit(7))
		{
			tm>>=1;
		}
		
		d.c = tm;
	}
	
	return deadCells;
}



void Grid::createCorners(unsigned char colorIndex)
{
	auto rMO = m_resolution - 1;
	vxAt(rMO, rMO, rMO).setByte(colorIndex);
	vxAt(rMO, rMO, 0).setByte(colorIndex);
	vxAt(rMO, 0, rMO).setByte(colorIndex);
	vxAt(rMO, 0, 0).setByte(colorIndex);
	vxAt(0, rMO, rMO).setByte(colorIndex);
	vxAt(0, rMO, 0).setByte(colorIndex);
	vxAt(0, 0, rMO).setByte(colorIndex);
	vxAt(0, 0, 0).setByte(colorIndex);
}

void Grid::createGround(unsigned long offset, unsigned char colorIndex)
{
	if(offset>=m_resolution)
		return;
	
	for (unsigned long i=0;i<m_resolution;i++)
		for (unsigned long j=0;j<m_resolution;j++)
		{
			vxAt(i,offset,j).setByte(colorIndex);
		}
}

void Grid::createRoof(unsigned long offset, unsigned char colorIndex)
{
	if(offset>=m_resolution)
	{
		return;
	}
	
	for (unsigned long i=0;i<m_resolution;i++)
	{
		for (unsigned long j=0;j<m_resolution;j++)
		{
			vxAt(i,(m_resolution-offset-1),j).setByte(colorIndex);
		}
	}
}

void Grid::createEdges(unsigned char colorIndex)
{
	unsigned long resminusone=m_resolution-1;
	for (unsigned long i=0;i<m_resolution;i++)
	{
		vxAt(i,0,0).setByte(colorIndex);
		vxAt(i,resminusone,resminusone).setByte(colorIndex);
		vxAt(i,0,resminusone).setByte(colorIndex);
		vxAt(i,resminusone,0).setByte(colorIndex);
		
		vxAt(0,i,0).setByte(colorIndex);
		vxAt(resminusone,i,resminusone).setByte(colorIndex);
		vxAt(0,i,resminusone).setByte(colorIndex);
		vxAt(resminusone,i,0).setByte(colorIndex);
		
		vxAt(0,0,i).setByte(colorIndex);
		vxAt(resminusone,resminusone,i).setByte(colorIndex);
		vxAt(resminusone,0,i).setByte(colorIndex);
		vxAt(0,resminusone,i).setByte(colorIndex);
	}
}

void Grid::fill(unsigned char colorIndex)
{
	for(unsigned long i=0;i<m_data.size();i++)
	{
		vxAt(i).setByte(colorIndex);
	}	
}

void Grid::createSphere(scalar x, scalar y, scalar z, const scalar radio, unsigned char colorIndex)
{
	createSphere(v3s{(scalar)x,(scalar)y,(scalar)z}, radio, colorIndex);
}

bool Grid::getRandomBoolean(scalar ratio)
{
	scalar num = pow((rand()/(scalar)RAND_MAX), 1.0/ratio);
	return num>.5;
}

void Grid::createRandom(scalar ratio, scalar y_threshold)
{
	int i=0;
	for(auto& it:m_data)
	{
		if(MU::getBoolRand(ratio) && getVoxelPosition(i).y()>y_threshold)
		{
			it.setByte((unsigned int)MU::getRand(25.0));
		}
		
		i++;
	}
}

void Grid::addGeometry(const vxTriangleMeshHandle geo)
{
	for(auto&& tri: geo->triangles())
	{
		{
			const auto& p{tri.p1};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setByte(v.byte()+1);
			}
		}
		{
			const auto& p{tri.p2};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setByte(v.byte()+1);
			}
		}
		{
			const auto& p{tri.p3};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setByte(v.byte()+1);
			}
		}
		
		const unsigned int interp = 4u;
		for(uint i=0; i<interp; i++)
		{
			auto a = MU::lerp(tri.p1, tri.p2, (i+1)/(scalar)interp);
			const auto& p{a};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setByte(v.byte()+1);
			}
		}
		
		for(uint i=0; i<interp; i++)
		{
			auto a = MU::lerp(tri.p1, tri.p3, (i+1)/(scalar)interp);
			const auto& p{a};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setByte(v.byte()+1);
			}
		}
		
		for(uint i=0; i<interp; i++)
		{
			auto a = MU::lerp(tri.p2, tri.p3, (i+1)/(scalar)interp);
			const auto& p{a};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setByte(v.byte()+1);
			}
		}
	}
}

unsigned long Grid::index(const unsigned long x, const unsigned long y, const unsigned long z) const
{
	return (x+(y*m_resolution)+(z*m_c_resXres));
}

void Grid::initialize(bool value)
{
	if(value)
	{
		for(unsigned long i=0;i<m_data.size();i++)
		{
			vxAt(i).activate();
		}
	}
	else
	{
		for(unsigned long i=0;i<m_data.size();i++)
		{
			vxAt(i).deactivate();
		}
	}
}

unsigned long Grid::numActiveVoxels()
{
	unsigned long av{0};
	
	for(unsigned long i=0;i<m_data.size();i++)
	{
		if(vxAt(i).active())
			av++;
	}
	
	return av;
}

long Grid::getNumberOfVoxels() const
{
	return m_c_resXresXres;
}

bool Grid::active(const unsigned long x, 
					const unsigned long y, 
					const unsigned long z) const
{
	return getElement(x,y,z);
}

 bool Grid::active(const v3s &pos) const
{
	if(inGrid(pos))
	{
		const auto&& fPos = pos - (m_position - m_c_midSize);
		return getElement((unsigned long)floor(fPos.x()),
						  (unsigned long)floor(fPos.y()),
						  (unsigned long)floor(fPos.z()));
	}
	
	return false;
}

 bool Grid::activeInRange(const v3s &pos) const
{
	const auto&& fPos = pos - (m_position - m_c_midSize);
	return getElement((unsigned long)floor(fPos.x()),
					  (unsigned long)floor(fPos.y()),
					  (unsigned long)floor(fPos.z()));
}


 bool Grid::active(unsigned long idx) const
{
	if (idx<m_c_resXresXres)
		return vxAt(idx).active();
	else
		return false;
}

 void Grid::activate(const unsigned long x, 
							 const unsigned long y, 
							 const unsigned long z)
{
	setElement(x,y,z,true);
}

bool Grid::activate(const v3s &pos)
{
	if(!inGrid(pos))
	{
		return false;
	}
	
	const auto&& offsetPos = pos - (m_position - m_c_midSize);
	setElement((unsigned long)floor(offsetPos.x()),
			   (unsigned long)floor(offsetPos.y()),
			   (unsigned long)floor(offsetPos.z()), true);
	
	return true;
}


 void Grid::deactivate(const unsigned long x, const unsigned long y, const unsigned long z)
{
	setElement(x,y,z,false);
}

 bool Grid::getElement(const unsigned long x, 
							   const unsigned long y, 
							   const unsigned long z) const
{
	return active(index(x,y,z));
}

 void Grid::setElement(const unsigned long x, 
							   const unsigned long y, 
							   const unsigned long z, 
							   bool value)
{
	vxAt(x,y,z).activate(value);
}


 unsigned char Grid::elementColorIndex(const unsigned long x, 
											   const unsigned long y, 
											   const unsigned long z) const
{
	return (index(x,y,z))%8;
}

 void Grid::setElementColorIndex(const unsigned long x, 
										 const unsigned long y, 
										 const unsigned long z, 
										 const unsigned char c)
{
	vxAt(x,y,z).setByte(c);
}

 void Grid::setElement(unsigned long idx, bool value)
{
	vxAt(idx).activate(value);
}

void Grid::getComponentsOfIndex(const unsigned long long idx,
								  long &retx, 
								  long &rety, 
								  long &retz) const
{
	retz = idx / m_c_resXres;
	rety = (idx % m_c_resXres) / m_resolution;
	retx = idx % m_resolution;
}

 unsigned long Grid::indexAtPosition(const v3s &pos) const
{
	auto p = ((pos - m_position + m_c_midSize)/m_c_boxSize).floorVector();
	
	return index((unsigned long)p.x(),(unsigned long)p.y(),(unsigned long)p.z());
}

 v3s Grid::getVoxelPosition(const unsigned long iX, 
								   const unsigned long iY, 
								   const unsigned long iZ) const
{
	return v3s(m_bb->minX()+(iX*m_c_boxSize),
			  m_bb->minY()+(iY*m_c_boxSize),
			  m_bb->minZ()+(iZ*m_c_boxSize)) + (m_c_midBoxSize);
	
}

 v3s Grid::getVoxelPosition(unsigned long long idx) const
{
	long retx;
	long rety;
	long retz;
	
	getComponentsOfIndex(idx, retx, rety, retz);
	
	return getVoxelPosition(retx, rety, retz);
}

 vx& Grid::vxAtPosition(const v3s &position)
{
	const auto&& idx = indexAtPosition(position);
	return vxAt(idx>=m_c_resXresXres ? 0 : idx);
}

 vx Grid::vxAtPosition(const v3s &position) const
{
	const auto&& idx = indexAtPosition(position);
	return vxAt(idx>=m_c_resXresXres ? 0:idx);
}

vx &Grid::vxAt(const unsigned long iX, 
				 const unsigned long iY, 
				 const unsigned long iZ)
{
	return vxAt(index(iX,iY,iZ));
}


vx Grid::vxAt(const unsigned long iX, 
				const unsigned long iY, 
				const unsigned long iZ) const
{
	return vxAt(index(iX,iY,iZ));
}


 vx &Grid::vxAt(const unsigned long idx)
{
	return m_data[idx];
}

 vx Grid::vxAt(const unsigned long idx) const
{
	return m_data[idx];
}

bool Grid::bitInBufferData(const unsigned long idx) const
{
	auto byte = m_data[idx/8];
	unsigned char ch = byte.c;
	const unsigned char sh = (7 - (idx%8));
	//Doing this so we loose any left information
	// and bit gets filtered.
	ch<<=sh;
	ch>>=sh;
	return (bool)ch;
}

void Grid::createSphere(const v3s &center, 
						  const scalar radio, 
						  unsigned char colorIndex)
{
	unsigned long x, y, z;
	
	for(x=0;x<m_resolution;x++)
	{
		for(y=0;y<m_resolution;y++)
		{
			for(z=0;z<m_resolution;z++)
			{
				auto& voxel = vxAt(x, y, z);
				if(center.distance(getVoxelPosition(x, y, z))<radio)
				{
					voxel.setByte(colorIndex);
				}
			}
		}
	}
}

bool Grid::inGrid(const v3s &pnt, scalar tol) const
{
	return m_bb->contains(pnt, tol);
}

bool Grid::inGrid(const v3s &pnt) const
{
	return m_bb->contains(pnt);
}

VoxelInfo Grid::nextVoxel(const Ray &ray, v3s &sp) const
{
	VoxelInfo retVal;
	retVal.size = m_c_boxSize;
	
	const auto& d = ray.direction();
	const auto& p = ray.origin();
	
	scalar velX = d.xPositive() ? 1.0 : 0.0;
	scalar velY = d.yPositive() ? 1.0 : 0.0;
	scalar velZ = d.zPositive() ? 1.0 : 0.0;
	
	auto&& xProgress = p + v3s((velX ? m_c_midBoxSize : -m_c_midBoxSize), 0.0, 0.0);
	auto&& yProgress = p + v3s(0.0, (velY ? m_c_midBoxSize : -m_c_midBoxSize), 0.0);
	auto&& zProgress = p + v3s(0.0, 0.0, (velZ ? m_c_midBoxSize : -m_c_midBoxSize));
	
	long retx;
	long rety;
	long retz;
	
	bool inside = true;
	do
	{
		retVal.index = indexAtPosition(sp);
		retVal.data = vxAt(retVal.index);
		
		if(retVal.data.active())
		{
			retVal.position = getVoxelPosition(retVal.index);
			return retVal;
		}
		
		if(retVal.index >= m_c_resXresXres)
		{
			return retVal;
		}
		
		getComponentsOfIndex(retVal.index, retx, rety, retz);
		
		scalar xVal = m_bb->minX() + (retx + velX) * m_c_boxSize - p.x();
		scalar yVal = m_bb->minY() + (rety + velY) * m_c_boxSize - p.y();
		scalar zVal = m_bb->minZ() + (retz + velZ) * m_c_boxSize - p.z();
		
		v3s&& intersectX = MU::rectAndXPlane(d, xVal);
		if(fabs(intersectX.y()) <= fabs(yVal)
				&& fabs(intersectX.z()) <= fabs(zVal))
		{
			sp = intersectX + xProgress;
			continue;
		}
		
		v3s&& intersectY = MU::rectAndYPlane(d, yVal);
		if(fabs(intersectY.x()) <= fabs(xVal)
				&& fabs(intersectY.z()) <= fabs(zVal))
		{
			sp = intersectY + yProgress;
			continue;
		}
		
		v3s&& intersectZ = MU::rectAndZPlane(d, zVal);
		if(fabs(intersectZ.x()) <= fabs(xVal)
				&& fabs(intersectZ.y()) <= fabs(yVal))
		{
			sp = intersectZ + zProgress;
			continue;
		}
		
	}
	while(!retVal.data.active() 
		  && (inside = m_bb->contains(sp)));
	
	retVal.data.deactivate();
	retVal.index = m_c_resXresXres;
	
	return retVal;
}

bool Grid::throwRay(const Ray &ray) const
{ 
#if DRAWBBOX
	return m_bb->throwRay(ray);
#else
	Collision col;
	return throwRay(ray,col) == 1;
#endif
}

int Grid::throwRay(const Ray &ray, Collision &col) const
{ 
#if DRAWBBOX
	if(m_bb->throwRay(ray, col))
	{
		col.setColor(m_baseColor);
		col.setValid(true);
		
		return 1;
	}
#else
	auto&& sp = col.position()
			+ (col.normal().inverted().tiny());
	
	auto prev = m_c_resXresXres;
	BoundingBox box;
	
	//Should this be cached?
	VoxelInfo voxel;
	do
	{
		voxel = nextVoxel(ray, sp);
		
		if(prev==voxel.index
				|| voxel.x >= m_resolution
				|| voxel.y >= m_resolution
				|| voxel.z >= m_resolution)
		{
			break;
		}
		
		prev = voxel.index;
		
		if(voxel.data.active())
		{
			box.set(voxel.position, voxel.size);
			
			if(box.throwRay(ray,col))
			{
				return 1;
			}
			return 1;
			
			//col.setColor(Color::indexColor(voxel.data.byte())/1.0);
			//col.setColor(Color::green);
		}
	}
	while(!voxel.data.active());
	
#endif
	
	col.setValid(false);
	return 0;
}

bool Grid::hasCollision(const Ray &ray) const
{
#if DRAWBBOX
	return m_bb->hasCollision(ray);
#else
	Collision col;
	return throwRay(ray,col)==1;
#endif
}

/*
std::cout << "minX " << m_xmin << std::endl;
std::cout << "minY " << m_ymin << std::endl;
std::cout << "minZ " << m_zmin << std::endl;

std::cout << "maxX " << m_xmax << std::endl;
std::cout << "maxY " << m_ymax << std::endl;
std::cout << "maxZ " << m_zmax << std::endl;
*/


void vx::activate(bool active)
{
	active ? activate() : deactivate();
}

bool vx::active() const
{
	return (bool)c;
}

bool vx::activeBit(unsigned int bit) const
{
	switch(bit)
	{
	case 0:
		return c&0b0000'0001;
				break;
	case 1:
		return c&0b0000'0010;
				break;
	case 2:
		return c&0b0000'0100;
				break;
	case 3:
		return c&0b0000'1000;
				break;
	case 4:
		return c&0b0001'0000;
				break;
	case 5:
		return c&0b0010'0000;
				break;
	case 6:
		return c&0b0100'0000;
				break;
	case 7:
		return c&0b1000'0000;
				break;
	}
	
	return c&0b0000'0000;
}

void vx::activateBit(unsigned int bit)
{
	switch(bit)
	{
	case 0:
		c|=0b0000'0001;
				break;
	case 1:
		c|=0b0000'0010;
				break;
	case 2:
		c|=0b0000'0100;
				break;
	case 3:
		c|=0b0000'1000;
				break;
	case 4:
		c|=0b0001'0000;
				break;
	case 5:
		c|=0b0010'0000;
				break;
	case 6:
		c|=0b0100'0000;
				break;
	case 7:
		c|=0b1000'0000;
				break;
	}
}

void vx::deactivateBit(unsigned int bit)
{
	switch(bit)
	{
	case 0:
		c&=~0b0000'0001;
				break;
	case 1:
		c&=~0b0000'0010;
				break;
	case 2:
		c&=~0b0000'0100;
				break;
	case 3:
		c&=~0b0000'1000;
				break;
	case 4:
		c&=~0b0001'0000;
				break;
	case 5:
		c&=~0b0010'0000;
				break;
	case 6:
		c&=~0b0100'0000;
				break;
	case 7:
		c&=~0b1000'0000;
				break;
	}
}

void vx::activate()
{
	activateBit(7);
}

void vx::deactivate()
{
	deactivateBit(7);
	deactivateBit(6);
	deactivateBit(5);
	deactivateBit(4);
	
	activateBit(0);
	activateBit(1);
	activateBit(2);
}

unsigned char vx::byte() const
{
	return c;
}

void vx::setByte(const unsigned char ci)
{
	c = ci;
	activate();
}
