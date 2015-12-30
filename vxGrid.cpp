#include <cassert>
#include <mutex>
#include <fstream>

#include "MathUtils.h"
#include "vxGrid.h"
std::mutex gridMutex;
using namespace vxCore;

#define DBL_EPSILON 1e-12
#define DRAWBBOX 0

vxGrid::vxGrid()
{
	m_size = 1.0;
	
	createGridData(5);
	initialize();
	updateBB();
	srand(time(NULL));
}

vxGrid::vxGrid(const v3 &position, scalar size)
	: m_position(position)
{
	setSize(size);
	createGridData(5);
	
	initialize();
	updateBB();
	srand(time(NULL));
}

vxGrid::vxGrid(scalar x, scalar y, scalar z, scalar size)
{ 
	m_position.set(x,y,z);
	setSize(size);
	
	createGridData(5);
	
	initialize();
	updateBB();
	srand(time(NULL));
}

vxGrid::~vxGrid()
{
}

void vxGrid::updateBB()
{
	m_bb->set(m_position, m_size);
}

void vxGrid::createGridData(const unsigned long resolution)
{
	m_resolution = resolution;
	m_c_resXres = resolution*resolution;
	m_c_resXresXres = m_c_resXres*resolution;
	m_data.resize(m_c_resXresXres);
	m_c_resDivTres = m_c_midSize/(scalar)m_resolution;
	setBoxSize();
}

void vxGrid::setResolution(unsigned long resolution)
{	
	if(resolution!=m_resolution)
	{
		createGridData(resolution);
	}
}

void vxGrid::setSize(const scalar size)
{
	m_size = size;
	m_c_midSize = size/2.0;
	setBoxSize();
}

unsigned long vxGrid::size() const
{
	return m_size;
}

void vxGrid::setPosition(const v3 &position)
{
	m_position = position;
	updateBB();
}

v3 vxGrid::position() const
{
	return m_position;
}

unsigned long vxGrid::resolution() const
{
	return m_resolution;
}

void vxGrid::setBoxSize()
{
	m_c_boxSize = m_size/scalar(m_resolution);
	m_c_midBoxSize = m_c_boxSize/2.0;
}

void vxGrid::createDiagonals(unsigned char colorIndex)
{
	unsigned long rMO=m_resolution-1;
	for (unsigned long i=0;i<m_resolution;i++)
	{
		vxAt(i,i,i).setColorIndex(colorIndex);
		vxAt(rMO-i,rMO-i,i).setColorIndex(colorIndex);
		vxAt(i,rMO-i,rMO-i).setColorIndex(colorIndex);
		vxAt(rMO-i,i,rMO-i).setColorIndex(colorIndex);
	}
}

void vxGrid::dumpFileInMemory(const std::string &fileName)
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

void vxGrid::dumpNumericTypeInMemory()
{
	using numType = int;
	void *p = (void*) &m_data[0];
	new (p) numType(std::numeric_limits<numType>::max());
}

void vxGrid::createCorners(unsigned char colorIndex)
{
	auto rMO = m_resolution - 1;
	vxAt(rMO, rMO, rMO).setColorIndex(colorIndex);
	vxAt(rMO, rMO, 0).setColorIndex(colorIndex);
	vxAt(rMO, 0, rMO).setColorIndex(colorIndex);
	vxAt(rMO, 0, 0).setColorIndex(colorIndex);
	vxAt(0, rMO, rMO).setColorIndex(colorIndex);
	vxAt(0, rMO, 0).setColorIndex(colorIndex);
	vxAt(0, 0, rMO).setColorIndex(colorIndex);
	vxAt(0, 0, 0).setColorIndex(colorIndex);
}

void vxGrid::createGround(unsigned long offset, unsigned char colorIndex)
{
	if(offset>=m_resolution)
		return;
	
	for (unsigned long i=0;i<m_resolution;i++)
		for (unsigned long j=0;j<m_resolution;j++)
		{
			vxAt(i,offset,j).setColorIndex(colorIndex);
		}
}

void vxGrid::createRoof(unsigned long offset, unsigned char colorIndex)
{
	if(offset>=m_resolution)
	{
		return;
	}
	
	for (unsigned long i=0;i<m_resolution;i++)
	{
		for (unsigned long j=0;j<m_resolution;j++)
		{
			vxAt(i,(m_resolution-offset-1),j).setColorIndex(colorIndex);
		}
	}
}

void vxGrid::createEdges(unsigned char colorIndex)
{
	unsigned long resminusone=m_resolution-1;
	for (unsigned long i=0;i<m_resolution;i++)
	{
		vxAt(i,0,0).setColorIndex(colorIndex);
		vxAt(i,resminusone,resminusone).setColorIndex(colorIndex);
		vxAt(i,0,resminusone).setColorIndex(colorIndex);
		vxAt(i,resminusone,0).setColorIndex(colorIndex);
		
		vxAt(0,i,0).setColorIndex(colorIndex);
		vxAt(resminusone,i,resminusone).setColorIndex(colorIndex);
		vxAt(0,i,resminusone).setColorIndex(colorIndex);
		vxAt(resminusone,i,0).setColorIndex(colorIndex);
		
		vxAt(0,0,i).setColorIndex(colorIndex);
		vxAt(resminusone,resminusone,i).setColorIndex(colorIndex);
		vxAt(resminusone,0,i).setColorIndex(colorIndex);
		vxAt(0,resminusone,i).setColorIndex(colorIndex);
	}
}

void vxGrid::fill(unsigned char colorIndex)
{
	for(unsigned long i=0;i<m_data.size();i++)
	{
		vxAt(i).setColorIndex(colorIndex);
	}	
}

void vxGrid::createSphere(scalar x, scalar y, scalar z, const scalar radio, unsigned char colorIndex)
{
	createSphere(v3{(scalar)x,(scalar)y,(scalar)z}, radio, colorIndex);
}

bool vxGrid::getRandomBoolean(scalar ratio)
{
	scalar num = pow((rand()/(scalar)RAND_MAX), 1.0/ratio);
	return num>.5;
}

void vxGrid::createRandom(scalar ratio, scalar y_threshold)
{
	int i=0;
	for(auto& it:m_data)
	{
		if(MU::getBoolRand(ratio) && getVoxelPosition(i).y()>y_threshold)
		{
			it.setColorIndex((unsigned int)MU::getRand(24));
		}
		
		i++;
	}
}

void vxGrid::addGeometry(const vxTriangleMeshHandle geo)
{
	for(auto&& tri: geo->m_triangles)
	{
		{
			const auto& p{tri.p1};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setColorIndex(v.colorIndex()+1);
			}
		}
		{
			const auto& p{tri.p2};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setColorIndex(v.colorIndex()+1);
			}
		}
		{
			const auto& p{tri.p3};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setColorIndex(v.colorIndex()+1);
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
				v.setColorIndex(v.colorIndex()+1);
			}
		}
		
		for(uint i=0; i<interp; i++)
		{
			auto a = MU::lerp(tri.p1, tri.p3, (i+1)/(scalar)interp);
			const auto& p{a};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setColorIndex(v.colorIndex()+1);
			}
		}
		
		for(uint i=0; i<interp; i++)
		{
			auto a = MU::lerp(tri.p2, tri.p3, (i+1)/(scalar)interp);
			const auto& p{a};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setColorIndex(v.colorIndex()+1);
			}
		}
	}
}

unsigned long vxGrid::index(const unsigned long x, const unsigned long y, const unsigned long z) const
{
	return x+(y*m_resolution)+(z*m_c_resXres);
}

void vxGrid::initialize(bool value)
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

unsigned long vxGrid::numActiveVoxels()
{
	unsigned long av{0};
	
	for(unsigned long i=0;i<m_data.size();i++)
	{
		if(vxAt(i).active())
			av++;
	}
	
	return av;
}

long vxGrid::getNumberOfVoxels() const
{
	return m_c_resXresXres;
}

bool vxGrid::active(const unsigned long x, 
					const unsigned long y, 
					const unsigned long z) const
{
	return getElement(x,y,z);
}

inline bool vxGrid::active(const v3 &pos) const
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

inline bool vxGrid::activeInRange(const v3 &pos) const
{
	const auto&& fPos = pos - (m_position - m_c_midSize);
	return getElement((unsigned long)floor(fPos.x()),
					  (unsigned long)floor(fPos.y()),
					  (unsigned long)floor(fPos.z()));
}


inline bool vxGrid::active(unsigned long idx) const
{
	if (idx<m_c_resXresXres)
		return vxAt(idx).active();
	else
		return false;
}

inline void vxGrid::activate(const unsigned long x, 
							 const unsigned long y, 
							 const unsigned long z)
{
	setElement(x,y,z,true);
}

bool vxGrid::activate(const v3 &pos)
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


inline void vxGrid::deactivate(const unsigned long x, const unsigned long y, const unsigned long z)
{
	setElement(x,y,z,false);
}

inline bool vxGrid::getElement(const unsigned long x, 
							   const unsigned long y, 
							   const unsigned long z) const
{
	return active(index(x,y,z));
}

inline void vxGrid::setElement(const unsigned long x, 
							   const unsigned long y, 
							   const unsigned long z, 
							   bool value)
{
	vxAt(x,y,z).activate(value);
}


inline unsigned char vxGrid::elementColorIndex(const unsigned long x, 
											   const unsigned long y, 
											   const unsigned long z) const
{
	return (index(x,y,z))%8;
}

inline void vxGrid::setElementColorIndex(const unsigned long x, 
										 const unsigned long y, 
										 const unsigned long z, 
										 const unsigned char c)
{
	vxAt(x,y,z).setColorIndex(c);
}

inline void vxGrid::setElement(unsigned long idx, bool value)
{
	vxAt(idx).activate(value);
}

void vxGrid::getComponentsOfIndex(const unsigned long idx,
								  unsigned long &retx, 
								  unsigned long &rety, 
								  unsigned long &retz) const
{
	retz = idx / m_c_resXres;
	rety = (idx % m_c_resXres) / m_resolution;
	retx = idx % m_resolution;
}

inline unsigned long vxGrid::indexAtPosition(const v3 &pos) const
{
	auto midSize = m_size / (scalar)2.0;
	auto boxSize = m_size / (scalar)m_resolution;
	auto p = ((pos - m_position + midSize)/boxSize).floorVector();
	
	return index((unsigned long)p.x(),(unsigned long)p.y(),(unsigned long)p.z());
}

inline v3 vxGrid::getVoxelPosition(const unsigned long iX, 
								   const unsigned long iY, 
								   const unsigned long iZ) const
{
	auto boxSize = m_size / scalar(m_resolution);
	auto midBoxSize = boxSize/2.0;

	return v3(m_bb->minX()+(iX*boxSize),
			  m_bb->minY()+(iY*boxSize),
			  m_bb->minZ()+(iZ*boxSize)) + (midBoxSize);
			
}

inline v3 vxGrid::getVoxelPosition(unsigned long idx) const
{
	unsigned long retx;
	unsigned long rety;
	unsigned long retz;
	
	getComponentsOfIndex(idx, retx, rety, retz);
	
	return getVoxelPosition(retx, rety, retz);
}

inline vx& vxGrid::vxAtPosition(const v3 &position)
{
	const auto&& idx = indexAtPosition(position);
	return vxAt(idx>=m_c_resXresXres ? 0 : idx);
}

inline vx vxGrid::vxAtPosition(const v3 &position) const
{
	const auto&& idx = indexAtPosition(position);
	return vxAt(idx>=m_c_resXresXres ? 0:idx);
}

vx &vxGrid::vxAt(const unsigned long iX, 
				 const unsigned long iY, 
				 const unsigned long iZ)
{
	return vxAt(index(iX,iY,iZ));
}


vx vxGrid::vxAt(const unsigned long iX, 
				const unsigned long iY, 
				const unsigned long iZ) const
{
	return vxAt(index(iX,iY,iZ));
}


inline vx &vxGrid::vxAt(const unsigned long idx)
{
	return m_data[idx];
}

inline vx vxGrid::vxAt(const unsigned long idx) const
{
	return m_data[idx];
}

bool vxGrid::bitInBufferData(const unsigned long idx) const
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

void vxGrid::createSphere(const v3 &center, 
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
					voxel.setColorIndex(colorIndex);
				}
			}
		}
	}
}

inline bool vxGrid::inGrid(const v3 &pnt, scalar tol) const
{
	return std::islessequal(pnt.x(),m_xmax+tol)
			&& std::isgreaterequal(pnt.x(),m_xmin-tol)
			&& std::islessequal(pnt.y(),m_ymax+tol)
			&& std::isgreaterequal(pnt.y(),m_ymin-tol)
			&& std::islessequal(pnt.z(),m_zmax+tol)
			&& std::isgreaterequal(pnt.z(),m_zmin-tol);
}

inline bool vxGrid::inGrid(const v3 &pnt) const
{
	return m_bb->contains(pnt);
}

Voxel vxGrid::nextVoxel(const vxRay &ray, v3 &sp) const
{
	Voxel retVal;
	retVal.size = ((scalar)m_size/(scalar)m_resolution);
	
	const auto& d = ray.direction();
	const auto& p = ray.origin();
	
	auto velX = d.xPositive() ? 1 : 0;
	auto velY = d.yPositive() ? 1 : 0;
	auto velZ = d.zPositive() ? 1 : 0;
	
	unsigned long retx;
	unsigned long rety;
	unsigned long retz;
	
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
		
		auto boxSize = m_size/(scalar)m_resolution;
		
		getComponentsOfIndex(retVal.index, retx, rety, retz);
		
		auto xVal = m_bb->minX() + (retx + velX) * boxSize - p.x();
		auto yVal = m_bb->minY() + (rety + velY) * boxSize - p.y();
		auto zVal = m_bb->minZ() + (retz + velZ) * boxSize - p.z();
		
		v3 intersectX = MU::rectAndXPlane(d, xVal);
		if(fabs(intersectX.y()) <= fabs(yVal)
				&& fabs(intersectX.z()) <= fabs(zVal))
		{
			sp = p + intersectX + v3((velX ? 1.0 : -1.0)/100.0, 0.0, 0.0);
			continue;
		}
		
		v3 intersectY = MU::rectAndYPlane(d, yVal);
		if(fabs(intersectY.x()) <= fabs(xVal)
				&& fabs(intersectY.z()) <= fabs(zVal))
		{
			sp = p + intersectY + v3(0.0, (velY ? 1.0 : -1.0)/100.0, 0.0);
			continue;
		}
		
		v3 intersectZ = MU::rectAndZPlane(d, zVal);
		if(fabs(intersectZ.x()) <= fabs(xVal)
				&& fabs(intersectZ.y()) <= fabs(yVal))
		{
			sp = p + intersectZ + v3(0.0, 0.0, (velZ ? 1.0 : -1.0)/100.0);
			continue;
		}
		
	}
	while(!retVal.data.active() 
		  && (inside = m_bb->contains(sp)));

	retVal.data.deactivate();
	retVal.index = m_c_resXresXres;
	
	return retVal;
}

bool vxGrid::throwRay(const vxRay &ray) const
{ 
#if DRAWBBOX
	return m_bb->throwRay(ray);
#else
	vxCollision col;
	return throwRay(ray,col) == 1;
#endif
}

int vxGrid::throwRay(const vxRay &ray, vxCollision &col) const
{ 
#if DRAWBBOX
	if(m_bb->throwRay(ray, col))
	{
		col.setColor(m_baseColor);
		col.setValid(true);
		
		return 1;
	}
#else
	
	vxCollision tmp;
	if(!m_bb->throwRay(ray, tmp))
	{
		return 0;
	}
	
	auto&& p = ray.origin();
	
	auto sp =  tmp.position()
			+ (tmp.normal().inverted() / (scalar)10000.0);
	
	auto prev = m_c_resXresXres;
	vxBoundingBox box;
	
	//Should this be cached?
	Voxel voxel;
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
			box.throwRay(ray,col);
			
			const auto& c = vxColor::indexColor(voxel.data.colorIndex());
			//const auto& c = vxColor::indexColor(voxel.data.active()+2);
			
			col.setColor(c);
			return 1;
		}
	}
	while(!voxel.data.active());
	
#endif
	
	col.setValid(false);
	return 0;
}

bool vxGrid::hasCollision(const vxRay &ray) const
{
#if DRAWBBOX
	return m_bb->hasCollision(ray);
#else
	auto found = false;
	const auto&& v{ray.direction().unit()};
	v3 next{ray.origin()+v};
	
	while(inGrid(next))
	{
		if(active(next))
		{
			found = true;
			break;
		}
		
		next+=v;
	}
	
	return found;
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

