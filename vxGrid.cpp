#include <cassert>
#include <mutex>

#include "MathUtils.h"
#include "vxGrid.h"

std::mutex gridMutex;

vxGrid::vxGrid()
{
	m_boundingBox = std::make_unique<vxBox>(true);
	m_size=1;
	
	createGridData(5);
	initialize();
	updateBB();
	srand(time(NULL));
}

vxGrid::vxGrid(const vxVector3d &position, double size)
	: m_position(position)
{
	m_boundingBox = std::make_unique<vxBox>(true);

	setSize(size);
	createGridData(5);
	
	initialize();
	updateBB();
	srand(time(NULL));
}

vxGrid::vxGrid(double x, double y, double z, double size)
{ 
	m_boundingBox = std::make_unique<vxBox>(true);
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
	m_boundingBox->set(m_position, m_size);
	//TODO:missing
}

void vxGrid::createGridData(const unsigned int resolution)
{
	m_resolution = resolution;
	m_resXres=resolution*resolution;
	m_resXresXres=m_resXres*resolution;
	//m_data=(bool*)malloc(sizeof(bool)*m_resXresXres);
	m_data.resize(m_resXresXres, false);
	//invRes=1/resolution;
	m_resDivTres = m_midSize/(double)m_resolution;
	setBoxSize();
	
	m_xmin = m_position.x() - m_midSize;
	m_xmax = m_position.x() + m_midSize;
	m_ymin = m_position.y() - m_midSize;
	m_ymax = m_position.y() + m_midSize;
	m_zmin = m_position.z() - m_midSize;
	m_zmax = m_position.z() + m_midSize;
	
}

void vxGrid::setResolution(unsigned int resolution)
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

unsigned int vxGrid::size() const
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

unsigned int vxGrid::resolution() const
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
	unsigned int resminusone=m_resolution-1;
	for (unsigned int i=0;i<m_resolution;i++)
	{
		activate(i,i,i);
		activate(resminusone-i,resminusone-i,i);
		activate(i,resminusone-i,resminusone-i);
		activate(resminusone-i,i,resminusone-i);
	}
}

void vxGrid::createCorners()
{
	unsigned int resminusone=m_resolution-1;
	activate(resminusone, resminusone, resminusone);
	activate(resminusone, resminusone,			 0);
	activate(resminusone,	0,			resminusone);
	activate(resminusone,	0,					0);
	activate(0, resminusone, resminusone);
	activate(0, resminusone,			 0);
	activate(0,	0,			resminusone);
	activate(0,	0,					0);	}

void vxGrid::createGround(unsigned int offset)
{
	if(offset>=m_resolution)
		return;
	
	for (unsigned int i=0;i<m_resolution;i++)
		for (unsigned int j=0;j<m_resolution;j++)
		{
			activate(i,offset,j);
		}
}

void vxGrid::createEdges()
{
	unsigned int resminusone=m_resolution-1;
	for (unsigned int i=0;i<m_resolution;i++)
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
	for(auto it = begin(m_data);
		it!= end(m_data);
		++it)
	{
		if(getRandomBoolean(ratio))
		{
			*it = true;
		}
	}
}

void vxGrid::addVertices(const std::vector<vxVector3d> &verts,
						 const vxVector3d &offset,
						 const vxVector3d &scale)
{
	for(const auto& v : verts)
	{
		activate((v*scale)+offset);
	}
}

void vxGrid::initialize(bool value)
{
	for(unsigned int i=0;i<m_data.size();i++)
	{
		m_data[i]=value;
	}
}

unsigned int vxGrid::numActiveVoxels()
{
	unsigned int av{0};
	
	for(unsigned int i=0;i<m_data.size();i++)
	{
		if(m_data[i])
			av++;
	}
	
	return av;
}

int vxGrid::getNumberOfVoxels() const
{
	return m_resXresXres;
}

bool vxGrid::active(const unsigned int x, 
					const unsigned int y, 
					const unsigned int z) const
{
	return getElement(x,y,z);
}

inline bool vxGrid::active(const vxVector3d &pos) const
{
	if(	MathUtils::inRange(pos.x(), m_xmin, m_xmax)
		&& MathUtils::inRange(pos.y(), m_ymin, m_ymax)
		&& MathUtils::inRange(pos.z(), m_zmin, m_zmax))
	{
		const auto&& fPos = pos - (m_position - m_midSize);
		return getElement((unsigned int)floor(fPos.x()),
						  (unsigned int)floor(fPos.y()),
						  (unsigned int)floor(fPos.z()));
	}
	
	return false;
}

inline bool vxGrid::activeInRange(const vxVector3d &pos) const
{
	const auto&& fPos = pos - (m_position - m_midSize);
	return getElement((unsigned int)floor(fPos.x()),
					  (unsigned int)floor(fPos.y()),
					  (unsigned int)floor(fPos.z()));
}


inline bool vxGrid::active(unsigned int idx) const
{
	if (idx<m_resXresXres)
		return m_data[idx];
	else
		return false;
}

inline void vxGrid::activate(const unsigned int x, 
					  const unsigned int y, 
					  const unsigned int z)
{
	setElement(x,y,z,true);
}

void vxGrid::activate(const vxVector3d &pos)
{
	if(	MathUtils::inRange(pos.x(), m_xmin, m_xmax)
		&& MathUtils::inRange(pos.y(), m_ymin, m_ymax)
		&& MathUtils::inRange(pos.z(), m_zmin, m_zmax))
	{
		const auto&& offsetPos = pos - (m_position - m_midSize);
		setElement((unsigned int)floor(offsetPos.x()),
						  (unsigned int)floor(offsetPos.y()),
						  (unsigned int)floor(offsetPos.z()), true);
	}
}


inline void vxGrid::deactivate(const unsigned int x, const unsigned int y, const unsigned int z)
{
	setElement(x,y,z,false);
}

inline bool vxGrid::getElement(const unsigned int x, 
								const unsigned int y, 
								const unsigned int z) const
{
	auto p = x+(y*m_resolution)+(z*m_resXres);
	return m_data[p];
}

inline void vxGrid::setElement(const unsigned int x, 
						const unsigned int y, 
						const unsigned int z, 
						bool value)
{
	m_data[x+(y*m_resolution)+(z*m_resXres)]=value;
}

inline void vxGrid::setElement(unsigned int idx, bool value)
{
	if(idx<m_data.size())
	{
		m_data[idx] = value;
	}
}

inline vxVector3d vxGrid::getVoxelPosition(unsigned int idx) const
{
	int retz = idx / m_resXres;
	int rety = (idx%m_resXres) / m_resolution;
	int retx = idx % m_resolution;
	
	return getVoxelPosition(retx, rety, retz);
}

inline unsigned int vxGrid::indexAtPosition(const vxVector3d &position) const
{
	vxVector3d pos = position - (m_position-m_midSize); 
	unsigned int idx = floor(pos.x());
	idx += floor(pos.y()) * m_resolution;
	idx += floor(pos.z()) * m_resXres;
	return idx;
}

inline vxVector3d vxGrid::getVoxelPosition(const unsigned int iX, 
										   const unsigned int iY, 
										   const unsigned int iZ) const
{
	double x = (m_position.x() - m_midSize) + (iX * m_boxSize) + m_resDivTres;
	double y = (m_position.y() - m_midSize) + (iY * m_boxSize) + m_resDivTres;
	double z = (m_position.z() - m_midSize) + (iZ * m_boxSize) + m_resDivTres;
	return vxVector3d{x,y,z};
}

void vxGrid::createSphere(const vxVector3d &center, const double radio)
{
	unsigned int x, y, z;
	
	for(x=0;x<m_resolution;x++)
	{
		for(y=0;y<m_resolution;y++)
		{
			for(z=0;z<m_resolution;z++)
			{
				if(center.distance(getVoxelPosition(x, y, z))<radio)
				{
					setElement(x,y,z,true);
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


unsigned int vxGrid::getNearestCollision(const vxRay &ray, vxCollision &collide) const
{
	if(m_boundingBox->throwRay(ray, collide) == 0)
	{
		return 0;
	}
	
	//NOTE:could be done checking normals.
	switch(ray.direction().mainAxis())
	{
	case vxVector3d::axis::kX:
		return getNearestCollisionUsingX(ray, collide);
		break;
	case vxVector3d::axis::kY:
		return getNearestCollisionUsingY(ray, collide);
		break;
	case vxVector3d::axis::kZ:
		return getNearestCollisionUsingZ(ray, collide);
		break;
	}

	return 0;
}

unsigned int vxGrid::getNearestCollisionUsingX(const vxRay &ray, vxCollision &collide) const
{
	const auto&& vel = collide.position().aaVector()/2.0;
	vxVector3d cIter{collide.position().asIntPosition()+vel};
	vxVector3d storedIter{cIter};
	bool found = false;
	double x = floor(cIter.x());
	
	while(!found && MathUtils::inRange(x, m_xmin, m_xmax))
	{
		storedIter = cIter;
		cIter = MathUtils::rectAndXPlane(ray.direction(), x).asIntPosition()+.5;
		if(storedIter.z() != cIter.z() || storedIter.y() != cIter.y())
		{
			vxVector3d deviated;
			if(MathUtils::z_forRectAndYPlane(
										collide.position(),
										storedIter.y() + vel[1])>(storedIter.z() + vel[2]))
			{
				if(cIter.z()!=storedIter.z())
				{
					deviated.set(storedIter[0], storedIter[1], cIter[2]);
					if(active(deviated))
					{
						cIter = deviated;
						found=true;
						break;
					}
				}

				deviated[1] = cIter[1];
				if(active(deviated))
				{
					cIter = deviated;
					found=true;
					break;
				}
			}
			else
			{
				if(cIter.y()!=storedIter.y())
				{
					deviated.set(storedIter[0], cIter[1], storedIter[2]);
					if(active(deviated))
					{
						cIter = deviated;
						found=true;
						break;
					}
				}
				
				deviated[2] = cIter[2];
				if(active(deviated))
				{
					cIter = deviated;
					found=true;
					break;
				}
			}
		}
		
		if(active(cIter))
		{
			found=true;
			break;
		}

		x+= vel[0];
	}
	
	if(found)
	{
		collide.setPosition(cIter);
		collide.setValid();
	}

	return (int)found;
}

unsigned int vxGrid::getNearestCollisionUsingY(const vxRay &ray, vxCollision &collide) const
{
	const auto&& vel = collide.position().aaVector()/2.0;
	vxVector3d cIter{collide.position().asIntPosition()+vel};
	vxVector3d storedIter{cIter};
	bool found = false;
	double y = floor(cIter.y());
	
	while(MathUtils::inRange(y, m_ymin, m_ymax))
	{
		auto pnt = MathUtils::rectAndYPlane(ray.direction(), y);
		storedIter = cIter;
		cIter = pnt.asIntPosition()+.5;
		if(storedIter.x() != cIter.x() || storedIter.y() != cIter.y())
		{
			vxVector3d deviated;
			if(MathUtils::x_forRectAndYPlane(
										collide.position(),
										storedIter.y() + vel[1])<=(storedIter.x() + vel[0]))
			{
				if(cIter.x()!=storedIter.x())
				{
					deviated.set(cIter[0], storedIter[1], storedIter[1]);
					if(active(deviated))
					{
						cIter = deviated;
						found=true;
						break;
					}
				}

				deviated[1] = cIter[1];
				if(active(deviated))
				{
					cIter = deviated;
					found=true;
					break;
				}
			}
			else
			{
				if(cIter.y()!=storedIter.y())
				{
					deviated.set(storedIter[0], cIter[1], storedIter[1]);
					if(active(deviated))
					{
						cIter = deviated;
						found=true;
						break;
					}
				}
				
				deviated[0] = cIter[0];
				if(active(deviated))
				{
					cIter = deviated;
					found=true;
					break;
				}
			}
		}
		
		if(active(cIter))
		{
			found=true;
			break;
		}

		y+= vel[1];
	}
	
	if(found)
	{
		collide.setPosition(cIter);
		collide.setValid();
	}

	return (int)found;
}

unsigned int vxGrid::getNearestCollisionUsingZ(const vxRay &ray, vxCollision &collide) const
{
	const auto&& vel = collide.position().aaVector()/2.0;
	vxVector3d cIter{collide.position().asIntPosition()-(vel)};
	vxVector3d storedIter;
	bool found = false;
	double z = floor(cIter.z());
	
	while(MathUtils::inRange(z, m_zmin, m_zmax))
	{
		storedIter = cIter;
		cIter = MathUtils::rectAndZPlane(ray.direction(), z).asIntPosition()+.5;
		if(storedIter.x() != cIter.x() || storedIter.y() != cIter.y())
		{
			vxVector3d deviated;
			if(MathUtils::x_forRectAndYPlane(
										collide.position(),
										storedIter.y() + vel[1])>(storedIter.x() + vel[0]))
			{
				if(cIter.x()!=storedIter.x())
				{
					deviated.set(cIter[0], storedIter[1], storedIter[2]);
					if(active(deviated))
					{
						cIter = deviated;
						found=true;
						break;
					}
				}

				deviated[1] = cIter[1];
				if(active(deviated))
				{
					cIter = deviated;
					found=true;
					break;
				}
			}
			else
			{
				if(cIter.y()!=storedIter.y())
				{
					deviated.set(storedIter[0], cIter[1], storedIter[2]);
					if(active(deviated))
					{
						cIter = deviated;
						found=true;
						break;
					}
				}
				
				deviated[0] = cIter[0];
				if(active(deviated))
				{
					cIter = deviated;
					found=true;
					break;
				}
			}
		}
		
		if(active(cIter))
		{
			found=true;
			break;
		}

		z+= vel[2];
	}
	
	if(found)
	{
		collide.setPosition(cIter);
		collide.setValid();
	}

	return (int)found;
}

#define DRAWBBOX 0
int vxGrid::throwRay(const vxRay &ray, vxCollision &collide) const
{ 
#if DRAWBBOX
	const auto&& geometry = vxGlobal::getInstance()->getExistingBox();
	const auto&& instance = geometry->at(position(), size());
	return instance->throwRay( ray, collide );
#else
	if (getNearestCollision(ray, collide))
	{	
		const auto&& geometry = vxGlobal::getInstance()->getExistingBox();
		const auto&& instance = geometry->at(collide.position(), 1.0);
		return instance->throwRay( ray, collide );
	}
	else
	{
		return 0;
	}
#endif
}

bool vxGrid::hasCollision(const vxRay &ray) const
{
	auto found = false;
	const vxVector3d&& v = ray.direction().unit();
	vxVector3d next(ray.origin()+v);

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
}

