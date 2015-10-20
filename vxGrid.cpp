#include <cassert>
#include <mutex>
#include <fstream>

#include "MathUtils.h"
#include "vxGrid.h"
std::mutex gridMutex;

#define DBL_EPSILON 1e-12
#define DRAWBBOX 0

vxGrid::vxGrid()
{
	m_size=1;
	
	createGridData(5);
	initialize();
	updateBB();
	srand(time(NULL));
}

vxGrid::vxGrid(const vxVector3d &position, double size)
	: m_position(position)
	, m_boundingBox(position, size)
{
	setSize(size);
	createGridData(5);
	
	initialize();
	updateBB();
	srand(time(NULL));
}

vxGrid::vxGrid(double x, double y, double z, double size)
{ 
	m_boundingBox.set(vxVector3d(x,y,z), size);
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
	m_boundingBox.set(m_position, m_size + DBL_EPSILON);
	//TODO:missing
}

void vxGrid::createGridData(const unsigned long resolution)
{
	m_resolution = resolution;
	m_resXres=resolution*resolution;
	m_resXresXres=m_resXres*resolution;
	m_data.resize(m_resXresXres);
	m_resDivTres = m_midSize/(double)m_resolution;
	setBoxSize();
	
	m_xmin = m_position.x() - m_midSize;
	m_xmax = m_position.x() + m_midSize;
	m_ymin = m_position.y() - m_midSize;
	m_ymax = m_position.y() + m_midSize;
	m_zmin = m_position.z() - m_midSize;
	m_zmax = m_position.z() + m_midSize;
	
}

void vxGrid::setResolution(unsigned long resolution)
{	
	if(resolution!=m_resolution)
	{
		createGridData(resolution);
	}
}

void vxGrid::setSize(const double size)
{
	m_size=size;
	m_midSize = size/2.0;
	setBoxSize();
}

unsigned long vxGrid::size() const
{
	return m_size;
}

void vxGrid::setPosition(const vxVector3d position)
{
	m_position=position;
}

vxVector3d vxGrid::position() const
{
	return m_position;
}

unsigned long vxGrid::resolution() const
{
	return m_resolution;
}

void vxGrid::setBoxSize()
{
	m_boxSize = m_size/double(m_resolution);
	m_midBoxSize = m_boxSize/2.0;
}


void vxGrid::createDiagonals()
{
	unsigned long resminusone=m_resolution-1;
	for (unsigned long i=0;i<m_resolution;i++)
	{
		activate(i,i,i);
		activate(resminusone-i,resminusone-i,i);
		activate(i,resminusone-i,resminusone-i);
		activate(resminusone-i,i,resminusone-i);
	}
}

void vxGrid::dumpFileInMemory(const std::string &fileName)
{
	std::ifstream file(fileName, std::ios::binary);
	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);
	
	if(size>m_data.size())
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

void vxGrid::createCorners()
{
	unsigned long resminusone=m_resolution-1;
	activate(resminusone, resminusone, resminusone);
	activate(resminusone, resminusone,			 0);
	activate(resminusone,	0,			resminusone);
	activate(resminusone,	0,					0);
	activate(0, resminusone, resminusone);
	activate(0, resminusone,			 0);
	activate(0,	0,			resminusone);
	activate(0,	0,					0);	
}

void vxGrid::createGround(unsigned long offset)
{
	if(offset>=m_resolution)
		return;
	
	for (unsigned long i=0;i<m_resolution;i++)
		for (unsigned long j=0;j<m_resolution;j++)
		{
			vxAt(i,offset,j).setColorIndex(2);
		}
}

void vxGrid::createRoof(unsigned long offset)
{
	if(offset>=m_resolution)
		return;
	
	for (unsigned long i=0;i<m_resolution;i++)
		for (unsigned long j=0;j<m_resolution;j++)
		{
			vxAt(i,(m_resolution-offset-1),j).activate();
			vxAt(i,(m_resolution-offset-1),j).setColorIndex(10);
		}
}

void vxGrid::createEdges()
{
	unsigned long resminusone=m_resolution-1;
	for (unsigned long i=0;i<m_resolution;i++)
	{
		activate(i,0,0);
		activate(i,resminusone,resminusone);
		activate(i,0,resminusone);
		activate(i,resminusone,0);
		
		activate(0,i,0);
		activate(resminusone,i,resminusone);
		activate(0,i,resminusone);
		activate(resminusone,i,0);
		
		activate(0,0,i);
		activate(resminusone,resminusone,i);
		activate(resminusone,0,i);
		activate(0,resminusone,i);
	}
}

void vxGrid::fill()
{
	for(unsigned long i=0;i<m_data.size();i++)
	{
		vxAt(i).activate();
	}	
}

void vxGrid::createSphere(double x, double y, double z, const double radio)
{
	createSphere(vxVector3d{(double)x,(double)y,(double)z}, radio);
}

bool vxGrid::getRandomBoolean(double ratio)
{
	double num = pow((rand()/(double)RAND_MAX), 1.0/ratio);
	return num>.5;
}

void vxGrid::createRandom(double ratio)
{
	for(auto &&it = begin(m_data);it!= end(m_data);	++it)
	{
		if(getRandomBoolean(ratio))
		{
			it->activate();
		}
	}
}

void vxGrid::addGeometry(const std::shared_ptr<vxGeometry> geo,
										const vxVector3d &offset,
										const vxVector3d &scale)
{
	for(auto &tri: geo->m_triangles)
	{
		{
			const auto&& p{(tri.p1*scale)+offset};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setColorIndex(v.colorIndex()+1);
			}
		}
		{
			const auto&& p{(tri.p2*scale)+offset};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setColorIndex(v.colorIndex()+1);
			}
		}
		{
			const auto&& p{(tri.p3*scale)+offset};
			if(inGrid(p))
			{
				auto&& v = vxAtPosition(p);
				v.setColorIndex(v.colorIndex()+1);
			}
		}
		
		const unsigned int interp = 4u;
		for(uint i=0; i<interp; i++)
		{
			auto a = MU::lerp(tri.p1, tri.p2, (i+1)/(double)interp);
			if(inGrid(a))
			{
				auto& v = vxAtPosition(a);
				v.setColorIndex(7);
			}
		}
		
		for(uint i=0; i<interp; i++)
		{
			auto a = MU::lerp(tri.p1, tri.p3, (i+1)/(double)interp);
			if(inGrid(a))
			{
				auto& v = vxAtPosition(a);
				v.setColorIndex(7);
			}
		}
		
		for(uint i=0; i<interp; i++)
		{
			auto a = MU::lerp(tri.p2, tri.p3, (i+1)/(double)interp);
			if(inGrid(a))
			{
				auto& v = vxAtPosition(a);
				v.setColorIndex(7);
			}
		}
	}
}

unsigned long vxGrid::index(const unsigned long x, const unsigned long y, const unsigned long z) const
{
	return x+(y*m_resolution)+(z*m_resXres);
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
	return m_resXresXres;
}

bool vxGrid::active(const unsigned long x, 
					const unsigned long y, 
					const unsigned long z) const
{
	return getElement(x,y,z);
}

inline bool vxGrid::active(const vxVector3d &pos) const
{
	if(inGrid(pos))
	{
		const auto&& fPos = pos - (m_position - m_midSize);
		return getElement((unsigned long)floor(fPos.x()),
						  (unsigned long)floor(fPos.y()),
						  (unsigned long)floor(fPos.z()));
	}
	
	return false;
}

inline bool vxGrid::activeInRange(const vxVector3d &pos) const
{
	const auto&& fPos = pos - (m_position - m_midSize);
	return getElement((unsigned long)floor(fPos.x()),
					  (unsigned long)floor(fPos.y()),
					  (unsigned long)floor(fPos.z()));
}


inline bool vxGrid::active(unsigned long idx) const
{
	if (idx<m_resXresXres)
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

bool vxGrid::activate(const vxVector3d &pos)
{
	if(!inGrid(pos))
	{
		return false;
	}
	
	const auto&& offsetPos = pos - (m_position - m_midSize);
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

inline vxVector3d vxGrid::getVoxelPosition(unsigned long idx) const
{
	long retz = idx / m_resXres;
	long rety = (idx%m_resXres) / m_resolution;
	long retx = idx % m_resolution;
	
	return getVoxelPosition(retx, rety, retz);
}

inline vx& vxGrid::vxAtPosition(const vxVector3d &position)
{
	const auto&& pos = (position - (m_position-m_midSize)).floorVector(); 
	const auto&& idx = index(pos.x(),pos.y(),pos.z());
	return vxAt(idx>=m_resXresXres ? 0:idx);
}

vx &vxGrid::vxAt(const unsigned long iX, const unsigned long iY, const unsigned long iZ)
{
	return vxAt(index(iX,iY,iZ));
}

inline vx vxGrid::vxAtPosition(const vxVector3d &position) const
{
	const auto&& pos = (position - (m_position-m_midSize)).floorVector(); 
	const auto&& idx = index(pos.x(),pos.y(),pos.z());
	return vxAt(idx>=m_resXresXres ? 0:idx);
}

vx vxGrid::vxAt(const unsigned long iX, 
				const unsigned long iY, 
				const unsigned long iZ) const
{
	return vxAt(index(iX,iY,iZ));
}

inline unsigned long vxGrid::indexAtPosition(const vxVector3d &position) const
{
	vxVector3d pos = position - (m_position-m_midSize); 
	unsigned long idx = floor(pos.x());
	idx += floor(pos.y()) * m_resolution;
	idx += floor(pos.z()) * m_resXres;
	return idx<m_resXresXres ? idx: 0;
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

inline vxVector3d vxGrid::getVoxelPosition(const unsigned long iX, 
										   const unsigned long iY, 
										   const unsigned long iZ) const
{
	double x = (m_position.x() - m_midSize) + (iX * m_boxSize) + m_resDivTres;
	double y = (m_position.y() - m_midSize) + (iY * m_boxSize) + m_resDivTres;
	double z = (m_position.z() - m_midSize) + (iZ * m_boxSize) + m_resDivTres;
	return vxVector3d{x,y,z};
}

void vxGrid::createSphere(const vxVector3d &center, const double radio)
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
					voxel.setColorIndex((unsigned char)
										MU::getRand(24));
					voxel.activate();
				}
			}
		}
	}
}

inline bool vxGrid::inGrid(const vxVector3d &pnt, double tol) const
{
	return std::islessequal(pnt.x(),m_xmax+tol)
			&& std::isgreaterequal(pnt.x(),m_xmin-tol)
			&& std::islessequal(pnt.y(),m_ymax+tol)
			&& std::isgreaterequal(pnt.y(),m_ymin-tol)
			&& std::islessequal(pnt.z(),m_zmax+tol)
			&& std::isgreaterequal(pnt.z(),m_zmin-tol);
}

inline bool vxGrid::inGrid(const vxVector3d &pnt) const
{
	return std::islessequal(pnt.x(),m_xmax)
			&& std::isgreaterequal(pnt.x(),m_xmin)
			&& std::islessequal(pnt.y(),m_ymax)
			&& std::isgreaterequal(pnt.y(),m_ymin)
			&& std::islessequal(pnt.z(),m_zmax)
			&& std::isgreaterequal(pnt.z(),m_zmin);
}

vxVector3d vxGrid::nextVoxel(const vxRay &ray, vxVector3d &exactCollision)
{
	const auto&& modPos = ray.origin();

	const auto&& minX = floor(modPos[0]);
	const auto&& minY = floor(modPos[1]);
	const auto&& minZ = floor(modPos[2]);

	const auto&& directionX = ray.direction().x();
	const auto&& directionY = ray.direction().y();
	const auto&& directionZ = ray.direction().z();

	const auto&& xSigned = std::signbit(directionX);
	const auto&& ySigned = std::signbit(directionY);
	const auto&& zSigned = std::signbit(directionZ);

	const auto&& maxX = xSigned ? minX : minX+1.0;
	const auto&& maxY = ySigned ? minY : minY+1.0;
	const auto&& maxZ = zSigned ? minZ : minZ+1.0;

	const auto&& xCollision = MU::rayAndXPlane(ray, maxX);
	const auto&& yCollision = MU::rayAndYPlane(ray, maxY);
	const auto&& zCollision = MU::rayAndZPlane(ray, maxZ);

	if( MU::inRange(xCollision.z(), minZ, minZ+1.0)
		&& MU::inRange(xCollision.y(), minY, minY+1.0))
	{
		exactCollision = xCollision;
		return xCollision.floorVector()+vxVector3d(xSigned ? -.5 : .5, .5,.5);
	}

	if( MU::inRange( yCollision.z(), minZ, minZ+1.0))
	{
		exactCollision = yCollision;
		return yCollision.floorVector()+vxVector3d(.5,ySigned ? -.5 : .5,.5);
	}

	exactCollision = zCollision;
	return zCollision.floorVector()+vxVector3d(.5,.5,zSigned ? -.5 : .5);
}


unsigned int vxGrid::getNearestCollision(const vxRay &ray, vxCollision &collide) const
{
	// If we start inside we dont look out for the bb.
	if(m_boundingBox.contains(ray.origin()))
	{
		collide.setPosition(ray.origin());
	}
	
	vxRay r{collide.position(), ray.direction()};
	bool found{false};
	auto&& vx{r.origin()+ray.origin()};

	const auto&& directionX = r.direction().x();
	const auto&& directionY = r.direction().y();
	const auto&& directionZ = r.direction().z();

	const auto&& xSigned = std::signbit(directionX);
	const auto&& ySigned = std::signbit(directionY);
	const auto&& zSigned = std::signbit(directionZ);
	do
	{
		vxVector3d exactCollision{0.0,0.0,0.0};
		const auto&& min = r.origin().floorVector();
		const auto&& maxX = xSigned ? min.x() : min.x()+1.0;
		const auto&& xCollision = MU::rayAndXPlane(r, maxX);
	
		if( MU::inRange(xCollision.z(), min.z(), min.z()+1.0)
			&& MU::inRange(xCollision.y(), min.y(), min.y()+1.0))
		{
			exactCollision = xCollision;
			vx = exactCollision.floorVector()
					+vxVector3d(xSigned ? -0.5 : 0.5, 0.5,0.5);
		}
		else 
		{
			const auto&& maxY = ySigned ? min.y() : min.y()+1.0;
			const auto&& yCollision = MU::rayAndYPlane(r, maxY);
			if( MU::inRange( yCollision.z(), min.z(), min.z()+1.0))
			{
				exactCollision = yCollision;
				vx = exactCollision.floorVector()
						+vxVector3d(0.5, ySigned ? -0.5 : 0.5,0.5);
			}
			else
			{
				const auto&& maxZ = zSigned ? min.z() : min.z()+1.0;
				const auto&& zCollision = MU::rayAndZPlane(r, maxZ);
				exactCollision = zCollision;
				vx = exactCollision.floorVector()
						+vxVector3d(0.5, 0.5, zSigned ? -0.5 : 0.5);
			}
		}
		
		if(active(vx))
		{
			found = true;
			break;
		}
		r.setOrigin(exactCollision + r.direction()/10000.0);
	}
	while(inGrid(vx));

	collide.setPosition(found ? vx : std::move(collide.position()));
	collide.setValid(found);
	return found ? 1 : 0;
}

bool vxGrid::throwRay(const vxRay &ray) const
{ 
#if DRAWBBOX
	return m_boundingBox.throwRay(ray);
#else
	vxCollision col;
	getNearestCollision(ray,col);
	return col.isValid();
#endif
}

int vxGrid::throwRay(const vxRay &ray, vxCollision &collide) const
{ 
#if DRAWBBOX
	if(m_boundingBox.throwRay(ray, collide))
	{
		collide.setColor(vxColor::white);
	}
	
	return collide.isValid();
#else

	if (getNearestCollision(ray, collide))
	{
		std::cout << "aqui llega" << std::endl;
		const auto&& pos = collide.position();
		const auto&& iap = indexAtPosition(pos);
		const auto&& c = vxColor::indexColor(vxAt(iap).colorIndex());
		collide.setColor(c);
		auto box = vxThreadPool::threadBox(std::this_thread::get_id());
		box.set(pos);
		return box.throwRay(ray,collide);
	}
	
	collide.setValid(false);
	return 0;
#endif
}

bool vxGrid::hasCollision(const vxRay &ray) const
{
#if DRAWBBOX
	return m_boundingBox.hasCollision(ray);
#else
	auto found = false;
	const auto&& v{ray.direction().unit()};
	vxVector3d next{ray.origin()+v};

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

