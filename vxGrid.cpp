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

void vxGrid::createSphere(int x, int y, int z, const double radio)
{
	createSphere(vxVector3d(x,y,z), radio);
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
		const auto& v1 = (v*scale)+offset;
		setElement(indexAtPosition(v1), true);
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
		const auto& fPos = pos - (m_position - m_midSize);
		return getElement((unsigned int)floor(fPos.x()),
						  (unsigned int)floor(fPos.y()),
						  (unsigned int)floor(fPos.z()));
	}
	
	return false;
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

inline vxVector3d vxGrid::getVoxelPosition(const unsigned int x, 
										   const unsigned int y, 
										   const unsigned int z) const
{
	double retx = (m_position.x() - m_midSize) + (x * m_boxSize) + m_resDivTres;
	double rety = (m_position.y() - m_midSize) + (y * m_boxSize) + m_resDivTres;
	double retz = (m_position.z() - m_midSize) + (z * m_boxSize) + m_resDivTres;
	return vxVector3d(retx, rety, retz);
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
	return pnt.x()<=(m_xmax+tol)
			&& pnt.x()>=(m_xmin-tol)
			&& pnt.y()<=(m_ymax+tol)
			&& pnt.y()>=(m_ymin-tol)
			&& pnt.z()<=(m_zmax+tol)
			&& pnt.z()>=(m_zmin-tol);
}

int vxGrid::getNearestCollision(const vxRayXYZ &ray, vxCollision &collide) const
{
	if(!m_boundingBox->hasCollision(ray))
	{
		return 0;
	}
	
	switch(ray.mainAxis())
	{
	case vxVector3d::axis::kZ:
		return getNearestCollisionUsingZ(ray, collide);
		break;
	case vxVector3d::axis::kX:
		return getNearestCollisionUsingX(ray, collide);
		break;
	case vxVector3d::axis::kY:
		return getNearestCollisionUsingY(ray, collide);
		break;
	}

	return 0;
}

int vxGrid::getNearestCollisionUsingX(const vxRayXYZ &ray, vxCollision &collide) const
{
	double vel = -m_boxSize;
	vxVector3d curr(m_xmax, 0.0, 0.0);
	vxVector3d prev;
	bool found = false;
	double x = m_xmin;
	
	auto zcota = copysign(m_midBoxSize, ray.z());
	auto ycota = copysign(m_midBoxSize, ray.y());
	
	while(x >= m_xmin)
	{
		auto pnt = MathUtils::rectAndXPlane(ray, x);
		prev = curr;
		curr = pnt.asIntPosition()+.5;
		vxVector3d tmp = curr;
		if(prev.z() != curr.z() || prev.y() != curr.y())
		{
			if(MathUtils::z_forRectAndYPlane(ray, prev.y() + ycota)>(prev.z() + zcota))
			{
				if(curr.z()!=prev.z())
				{
					tmp = prev;
					tmp[2] = curr[2];
				
					if(active(tmp))
					{
						curr = tmp;
						found=true;
						break;
					}
				}

				tmp[1] = curr[1];
				
				if(active(tmp))
				{
					curr = tmp;
					found=true;
					break;
				}
			}
			else
			{
				if(curr.y()!=prev.y())
				{
					tmp = prev;
					tmp[1] = curr[1];
				
					if(active(tmp))
					{
						curr = tmp;
						found=true;
						break;
					}
				}
				
				tmp[2] = curr[2];
			
				if(active(tmp))
				{
					curr = tmp;
					found=true;
					break;
				}
			}
		}
		
		if(active(curr))
		{
			found=true;
			break;
		}

		x+= vel;
	}
	
	if(found)
	{
		collide.setPosition(curr);
		collide.setValid();
	}

	return (int)found;
}

int vxGrid::getNearestCollisionUsingY(const vxRayXYZ &, vxCollision &collide) const
{
	assert(true);
	return 0;
}


int vxGrid::getNearestCollisionUsingZ(const vxRayXYZ &ray, vxCollision &collide) const
{
	const double &vel = m_boxSize;
	vxVector3d curr(m_zmin, 0.0, 0.0);
	vxVector3d prev;
	bool found = false;
	double z = m_zmin;
	
	auto xcota = copysign(m_midBoxSize, ray.x());
	auto ycota = copysign(m_midBoxSize, ray.y());
	
	while(z <= m_zmax)
	{
		auto pnt = MathUtils::rectAndZPlane(ray, z);
		prev = curr;
		curr = pnt.asIntPosition()+.5;
		vxVector3d tmp = curr;
		if(prev.x() != curr.x() || prev.y() != curr.y())
		{
			if(MathUtils::x_forRectAndYPlane(ray, prev.y() + ycota)>(prev.x() + xcota))
			{
				if(curr.x()!=prev.x())
				{
					tmp = prev;
					tmp[0] = curr[0];
				
					if(active(tmp))
					{
						curr = tmp;
						found=true;
						break;
					}
				}

				tmp[1] = curr[1];
				
				if(active(tmp))
				{
					curr = tmp;
					found=true;
					break;
				}
			}
			else
			{
				if(curr.y()!=prev.y())
				{
					tmp = prev;
					tmp[1] = curr[1];
				
					if(active(tmp))
					{
						curr = tmp;
						found=true;
						break;
					}
				}
				
				tmp[0] = curr[0];
			
				if(active(tmp))
				{
					curr = tmp;
					found=true;
					break;
				}
		}
		}
		
		if(active(curr))
		{
			found=true;
			break;
		}

		z+= vel;
	}
	
	if(found)
	{
		collide.setPosition(curr);
		collide.setValid();
	}

	return (int)found;
}


int vxGrid::throwRay(const vxRayXYZ &ray, vxCollision &collide) const
{ 
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
}

bool vxGrid::hasCollision(const vxRayXYZ &ray) const
{
	auto found = false;
	const vxVector3d&& v = ray.unit();
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

