#include "vxGlobal.h"

std::shared_ptr<vxLegoBlock> vxGlobal::m_lego = std::make_shared<vxLegoBlock>();

vxGlobal::vxGlobal()
{
	m_num1 = new vxBoxN1;
	m_num4 = new vxBoxN4; 
	m_num5 = new vxBoxN5;
	m_num8 = new vxBoxN8;
	m_num20 = new vxBoxN20;
	m_num17 = new vxBoxN17;
	m_num12 = new vxBoxN12;
	m_num16 = new vxBoxN16;
	m_num24 = new vxBoxN24;
}

vxGlobal *vxGlobal::getInstance()
{
	static vxGlobal a;
	return &a;
}

vxBoxN *vxGlobal::getExistingBox(const vxVector3d &position, const double size)
{
	double apot=size/2.0;
	if (position.z()-apot>=0)
	{
		if (position.y()-apot>=0)
		{
			if (position.x()-apot>=0) 
			{
				m_num1->set(position.x(),position.y(),position.z(),size);
				return m_num1;
			}
			if (position.x()+apot<=0)
			{
				m_num4->set(position.x(),position.y(),position.z(),size);
				return m_num4;
			}
			if (position.x()-apot<=0 && position.x()+apot>=0) 
			{
				m_num12->set(position.x(),position.y(),position.z(),size);
				return m_num12;
			}
		}
		if (position.y()+apot<=0)
		{
			if (position.x()-apot>=0) 
			{
				m_num5->set(position.x(),position.y(),position.z(),size);
				return m_num5;
			}
			if (position.x()+apot<=0) 
			{
				m_num8->set(position.x(),position.y(),position.z(),size);
				return m_num8;
			}
			if (position.x()-apot<=0 && position.x()+apot>=0) 
			{
				m_num16->set(position.x(),position.y(),position.z(),size);
				return m_num16;
			}
		}		
		if (position.y()-apot<=0 && position.y()+apot>=0) 
		{
			if (position.x()-apot<=0 && position.x()+apot>=0) 
			{
				m_num24->set(position.x(),position.y(),position.z(),size);
				return m_num24;
			}
			if (position.x()+apot<=0) 
			{
				m_num20->set(position.x(),position.y(),position.z(),size);
				return m_num20;
			}
			if (position.x()-apot>=0) 
			{
				m_num17->set(position.x(),position.y(),position.z(),size);
				return m_num17;
			}
		}
	}
	//TODO: return invalidty, now it's taken a random 
	// valid one.
	return m_num17;
}


//TODO: this needs to be questioned.
std::shared_ptr<vxLegoBlock> 
vxGlobal::getExistingLegoBlock(const vxVector3d &position, const double size)
{
	return vxGlobal::m_lego;
}

vxBoxN *vxGlobal::getBox(const double x, const double y, const double z, const double size)
{
	auto apot=size/2.0;
	if (z-apot>=0)
	{
		if (y-apot>=0)
		{
			if (x-apot>=0) return new vxBoxN1(x,y,z,size);
			if (x+apot<=0) return new vxBoxN4(x,y,z,size);
			if (x-apot<=0 && x+apot>=0) return new vxBoxN12(x,y,z,size);
		}
		if (y+apot<=0)
		{
			if (x-apot>=0) return new vxBoxN5(x,y,z,size);
			if (x+apot<=0) return new vxBoxN8(x,y,z,size);
			if (x-apot<=0 && x+apot>=0) return new vxBoxN16(x,y,z,size);
		}		
		if (y-apot<=0 && y+apot>=0) 
		{
			if (x-apot<=0 && x+apot>=0) return new vxBoxN24(x,y,z,size);
			if (x+apot<=0) return new vxBoxN20(x,y,z,size);
			if (x-apot>=0) return new vxBoxN17(x,y,z,size);
		}
	}
	
	return nullptr;
}

vxBoxN *vxGlobal::getBox(const vxVector3d &position, const double size)
{
	return getBox( position.x(), position.y(), position.z(), size);
}
