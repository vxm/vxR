
#include "vxGrid.h"

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

inline bool vxGrid::inGrid(const vxVector3d &pnt) const
{
	return pnt.x()<=m_xmax && pnt.x()>=m_xmin
			&& pnt.y()<=m_ymax && pnt.y()>=m_ymin
			&& pnt.z()<=m_zmax && pnt.z()>=m_zmin;
}

void vxGrid::getNearestCollision(const vxVector3d &ray, vxCollision &collide)
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

void vxGrid::getNearestCollisionUsingX(const vxVector3d &ray, vxCollision &collide)
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

void vxGrid::getNearestCollisionUsingY(const vxVector3d &ray, vxCollision &collide)
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

void vxGrid::getNearestCollisionUsingZ(const vxVector3d &ray, vxCollision &collide)
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
		vxVector3d tmp = curr;
		if(MathUtils::rectAndYPlane(ray, prev.y() + ycota).x()>(prev.x() + xcota))
		{
			if(curr.x()!=prev.x())
			{
				tmp = prev;
				tmp.setX(curr.x());
			
				if(inGrid(tmp) && active(indexAtPosition(tmp)))
				{
					curr = tmp;
					found=true;
					break;
				}
			}
			
			if(curr.y()!=prev.y())
			{
				tmp.setY(curr.y());
			
				if(inGrid(tmp) && active(indexAtPosition(tmp)))
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
		
		if(inGrid(curr) && active(indexAtPosition(curr)))
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

void vxGrid::getNearestCollisionBF(const vxVector3d &ray, vxCollision &collide)
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
							if(collide.getPosition().length()<minima.getPosition().length()) minima=collide;
						}
					}
					else
					{
						collide.setColor(255,0,0);
					}
				}
			}
		}
	}
	
	//
	collide=minima;
}

int vxGrid::throwRay(const vxVector3d &ray, vxCollision &collide)
{ 
	if (!m_boundingBox) 
		return 0;
	
	// will test in 
	m_boundingBox->throwRay(ray, collide);
	
	if (collide.isValid()) 
	{
		getNearestCollision(ray, collide);
		vxBoxN *boxInstance = vxGlobal::getInstance()->getExistingtBox( collide.getPosition(), m_boxSize);
		boxInstance->throwRay( ray, collide );
		return 1;
	}
	else
	{
		collide.initialize();
		return 0;
	}
}
