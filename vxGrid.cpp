
#include "vxGrid.h"

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
{
	setSize(size);
	createGridData(5);
	
	initialize();
	updateBB();
	srand(time(NULL));
}

vxGrid::vxGrid(double x, double y, double z, double size)
{ 
	m_position.set(x,y,z);
	setSize(size);
	
	createGridData(5);
	
	initialize();
	updateBB();
	srand(time(NULL));
}

vxGrid::~vxGrid()
{}

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

void vxGrid::setPosition(const vxVector3d position)
{
	m_position=position;
}

void vxGrid::setBoxSize()
{
	m_boxSize = m_size/double(m_resolution);
	m_midBoxSize = m_boxSize/2.0;
}

void vxGrid::updateBB()
{
	//TODO:c++14 make unique
	m_boundingBox.reset(vxGlobal::getBox(m_position, m_size));
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

bool vxGrid::active(unsigned int idx) const
{
	if (idx<m_resXresXres)
		return m_data[idx];
	else
		return false;
}

void vxGrid::activate(const int x, const int y, const int z)
{
	setElement(x,y,z,true);
}

void vxGrid::deactivate(const int x, const int y, const int z)
{
	setElement(x,y,z,false);
}

bool vxGrid::getElement(int x, int y, int z) const
{
	auto p = x+(y*m_resolution)+(z*m_resXres);
	return m_data[p];
}

void vxGrid::setElement(int x, int y, int z, bool value)
{
	m_data[x+(y*m_resolution)+(z*m_resXres)]=value;
}

vxVector3d vxGrid::getVoxelPosition(unsigned int idx) const
{
	int retz = idx / m_resXres;
	int rety = (idx%m_resXres) / m_resolution;
	int retx = idx % m_resolution;
	
	return getVoxelPosition(retx, rety, retz);
}

unsigned int vxGrid::indexAtPosition(const vxVector3d &position) const
{
	vxVector3d pos = position - (m_position-m_midSize); 
	unsigned int idx = floor(pos.x());
	idx += floor(pos.y()) * m_resolution;
	idx += floor(pos.z()) * m_resXres;
	return idx;
}

vxVector3d vxGrid::getVoxelPosition(int x, int y, int z) const
{
	double retx = (m_position.x() - m_midSize) + (x * m_boxSize) + m_resDivTres;
	double rety = (m_position.y() - m_midSize) + (y * m_boxSize) + m_resDivTres;
	double retz = (m_position.z() - m_midSize) + (z * m_boxSize) + m_resDivTres;
	return vxVector3d(retx, rety, retz);
}

bool vxGrid::active(int x, int y, int z) const
{
	return getElement(x,y,z);
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
	return pnt.x()<=(m_xmax+tol) && pnt.x()>=(m_xmin-tol)
			&& pnt.y()<=(m_ymax+tol) && pnt.y()>=(m_ymin-tol)
			&& pnt.z()<=(m_zmax+tol) && pnt.z()>=(m_zmin-tol);
}

void vxGrid::getNearestCollision(const vxRayXYZ &ray, vxCollision &collide)
{
	switch(ray.mainAxis())
	{
	case vxVector3d::axis::kZ:
		getNearestCollisionUsingZ(ray, collide);
		break;
	case vxVector3d::axis::kX:
		getNearestCollisionUsingX(ray, collide);
		break;
	case vxVector3d::axis::kY:
		getNearestCollisionUsingY(ray, collide);
		break;
	}
}

void vxGrid::getNearestCollisionUsingX(const vxRayXYZ &ray, vxCollision &collide)
{
	collide.initialize();
	
	vxBoxN *boxInstance = nullptr;
	vxVector3d min(m_zmax, m_ymax, m_xmax);
	
	bool found = false;
	double x=m_xmin;
	while(x<m_xmax && !found)
	{
		auto pnt = MathUtils::rectAndXPlane(ray, x);
		if(!inGrid(pnt))
		{
			x+= m_boxSize;
			continue;
		}
		
		auto idx = indexAtPosition(pnt);
		
		vxVector3d prev = min;
		min = getVoxelPosition(idx);
		if(active(idx))
		{
			if(min.z()!=prev.z())
			{
				auto prvVoxl = vxVector3d(prev.x(), prev.y(), min.z());
				if(inGrid(prvVoxl))
				{
					x+= m_boxSize;
					
					auto idx2 = indexAtPosition(prvVoxl);
					if(active(idx2))
					{
						min = prvVoxl;
					}
				}
			}
			if(min.y()!=prev.y())
			{
				auto prvVoxl = vxVector3d(prev.x(), min.y(), prev.z());
				if(inGrid(prvVoxl))
				{
					auto idx2 = indexAtPosition(prvVoxl);
					if(active(idx2))
					{
						min = prvVoxl;
					}
				}
			}
			
			found=true;
			continue;
		}
		
		x+= m_boxSize;
	}
	
	if(found)
	{
		boxInstance = vxGlobal::getInstance()->getExistingtBox( min, m_boxSize);
		boxInstance->throwRay( ray, collide );
	}
}

void vxGrid::getNearestCollisionUsingY(const vxRayXYZ &ray, vxCollision &collide)
{
	collide.initialize();
	
	vxBoxN *boxInstance = nullptr;
	vxVector3d min(m_zmax, m_ymax, m_xmax);
	
	bool found = false;
	double y=m_zmin;
	while(y<m_ymax && !found)
	{
		auto pnt = MathUtils::rectAndYPlane(ray, y);
		if(!inGrid(pnt))
		{
			y+= m_boxSize;
			continue;
		}
		
		auto idx = indexAtPosition(pnt);
		
		vxVector3d prev = min;
		min = getVoxelPosition(idx);
		if(active(idx))
		{
			if(min.x()!=prev.x())
			{
				auto prvVoxl = vxVector3d(min.x(), prev.y(), prev.z());
				if(inGrid(prvVoxl))
				{
					y+= m_boxSize;
					
					auto idx2 = indexAtPosition(prvVoxl);
					if(active(idx2))
					{
						min = prvVoxl;
					}
				}
			}
			else if(min.z()!=prev.z())
			{
				auto prvVoxl = vxVector3d(prev.x(), prev.y(), min.z());
				if(inGrid(prvVoxl))
				{
					y+= m_boxSize;
					
					auto idx2 = indexAtPosition(prvVoxl);
					if(active(idx2))
					{
						min = prvVoxl;
					}
				}
			}
			
			found=true;
			continue;
		}
		
		y+= m_boxSize;
	}
	
	if(found)
	{
		boxInstance = vxGlobal::getInstance()->getExistingtBox( min, m_boxSize);
		boxInstance->throwRay( ray, collide );
	}
}

void vxGrid::getNearestCollisionUsingZ(const vxRayXYZ &ray, vxCollision &collide)
{
	collide.initialize();
	vxVector3d curr(m_zmin, 0.0, 0.0);
	vxVector3d prev;
	bool found = false;
	double z = m_zmin;
	
	auto xcota = copysign(m_midBoxSize, ray.x());
	auto ycota = copysign(m_midBoxSize, ray.y());
	
	while(z <= m_zmax && !found)
	{
		auto pnt = MathUtils::rectAndZPlane(ray, z);
	
		prev = curr;
		curr = getVoxelPosition(indexAtPosition(pnt));
		if(!inGrid(curr, m_boxSize))
		{
			// can we jump to the real next z?
			z+= m_boxSize;
			continue;
		}
		
		vxVector3d tmp = curr;
		if(prev.x() != curr.x() || prev.y() != curr.y())
		{
			if(MathUtils::rectAndYPlane(ray, prev.y() + ycota).x()>(prev.x() + xcota))
			{
				if(curr.x()!=prev.x())
				{
					tmp = prev;
					tmp.setX(curr.x());
				
					if(inGrid(tmp, m_boxSize) && active(indexAtPosition(tmp)))
					{
						curr = tmp;
						found=true;
						break;
					}
				}
				
				if(curr.y()!=prev.y())
				{
					tmp.setY(curr.y());
				
					if(inGrid(tmp, m_boxSize) && active(indexAtPosition(tmp)))
					{
						curr = tmp;
						found=true;
						break;
					}
				}
			}
			else
			{
				if(curr.y()!=prev.y())
				{
					tmp = prev;
					tmp.setY(curr.y());
				
					if(inGrid(tmp) && active(indexAtPosition(tmp)))
					{
						curr = tmp;
						found=true;
						break;
					}
				}			
				
				if(curr.x()!=prev.x())
				{
					tmp.setX(curr.x());
				
					if(inGrid(tmp) && active(indexAtPosition(tmp)))
					{
						curr = tmp;
						found=true;
						break;
					}
				}
			}
		}
		
		if(inGrid(curr, m_boxSize) && active(indexAtPosition(curr)))
		{
			found=true;
			break;
		}

		z+= m_boxSize;

	}
	
	
	if(found)
	{
		collide.setPosition(curr);
		collide.setValid();
	}
}





void vxGrid::getNearestCollisionBF(const vxRayXYZ &ray, vxCollision &collide)
{
	collide.initialize();
	
	bool pri=true;
	
	vxCollision minima;
	
	vxBoxN *caja;
	
	for(unsigned int x=0;x<m_resolution;x++)
	{
		for(unsigned int y=0;y<m_resolution;y++)
		{
			for(unsigned int z=0;z<m_resolution;z++)
			{
				if (getElement(x,y,z))
				{
					caja = vxGlobal::getInstance()->getExistingtBox( getVoxelPosition(x, y, z), m_boxSize);
					caja->throwRay( ray, collide );
					
					if (collide.isValid()) 
					{
						
						if (pri)
						{
							minima=collide;
							
							pri=false;
						}
						else
						{
							if(collide.position().length()<minima.position().length()) minima=collide;
						}
					}
					else
					{
						collide.setColor(1.0,0,0);
					}
				}
			}
		}
	}
	
	//
	collide=minima;
}

int vxGrid::throwRay(const vxRayXYZ &ray, vxCollision &collide)
{ 
	if (!m_boundingBox) 
		return 0;
	
	// will test in 
	m_boundingBox->throwRay(ray, collide);
	
	if (collide.isValid()) 
	{
		getNearestCollision(ray, collide);
		vxBoxN *boxInstance = vxGlobal::getInstance()->getExistingtBox( collide.position(), m_boxSize);
		boxInstance->throwRay( ray, collide );
		return 1;
	}
	else
	{
		collide.initialize();
		return 0;
	}
}



bool vxGrid::hasCollision(const vxVector3d &origin, const vxRayXYZ &ray)
{
	auto itDoes = false;

	auto incX = copysign(m_boxSize, ray.x());
	auto incY = copysign(m_boxSize, ray.y());
	auto incZ = copysign(m_boxSize, ray.z());

	vxVector3d next(origin.x() + incX,
					origin.y() + incY,
					origin.z() + incZ);

	while(inGrid(next, m_boxSize))
	{
		auto idx = indexAtPosition(next);
		if(active(idx))
		{
			itDoes = true;
			break;
		}
		
		next.setX(next.x() + incX);
		next.setY(next.y() + incY);
		next.setZ(next.z() + incZ);
	}
	
	return itDoes;
}

