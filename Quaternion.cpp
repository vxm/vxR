#include "Quaternion.h"

using namespace vxCore;

Quaternion::Quaternion()
{
	
}

Quaternion::Quaternion(scalar ix, scalar iy, scalar iz, scalar iw)
	: x(ix)
	, y(iy)
	, z(iz)
	, w(iw)
{
}

Quaternion::Quaternion(const v3s &axis, const scalar angle)
{
	scalar s = sin(angle / 2.0) / axis.length();
	set(axis.x() * s, axis.y() * s, axis.z() * s, cos(angle / 2.0));
}

void Quaternion::set(const v3s &axis, const scalar angle)
{
	scalar s = sin(angle / 2.0) / axis.length();
	set(axis.x() * s, axis.y() * s, axis.z() * s, cos(angle / 2.0));
}


void Quaternion::set(scalar ix, scalar iy, scalar iz, scalar iw)
{
	x=ix;
	y=iy;
	z=iz;
	w=iw;
}
