#ifndef VXGEOGRID_H
#define VXGEOGRID_H

#include <vector>
#include "vxBoundingBox.h"


namespace vxCore {

using voxelMembers = std::vector<long>;

class vxGeoGrid
{
	std::shared_ptr<vxBoundingBox> m_bb;
	unsigned int m_rx = {5u};
	unsigned int m_ry = {5u};
	unsigned int m_rz = {5u};
	
public:
	
	vxGeoGrid();
	
	std::vector<voxelMembers> m_members{125};
	
	std::shared_ptr<vxBoundingBox> bb() const;

	void setBb(const std::shared_ptr<vxBoundingBox> &bb);
	
	unsigned int rx() const;
	void setRx(unsigned int rx);
	unsigned int ry() const;
	void setRy(unsigned int ry);
	unsigned int rz() const;
	void setRz(unsigned int rz);

	unsigned int lookupVoxel(const scalar a, const scalar b, const scalar c)
	{
		const auto x = (unsigned int)floor(m_rx * a);
		const auto y = (unsigned int)floor(m_ry * b);
		const auto z = (unsigned int)floor(m_rz * c);

		return ((m_ry * m_rx) * z) + (m_rx * y) + x;
	}
	
	void locateTriangle(const vxTriRef &tri,
						unsigned long triangleID)
	{
		auto r1x = MU::scaleTo01(m_bb->minX(), m_bb->maxX(), tri.p1.x());
		auto r1y = MU::scaleTo01(m_bb->minY(), m_bb->maxY(), tri.p1.y());
		auto r1z = MU::scaleTo01(m_bb->minZ(), m_bb->maxZ(), tri.p1.z());

		auto vx1 = lookupVoxel(r1x, r1y, r1z);
		
		auto r2x = MU::scaleTo01(m_bb->minX(), m_bb->maxX(), tri.p2.x());
		auto r2y = MU::scaleTo01(m_bb->minY(), m_bb->maxY(), tri.p2.y());
		auto r2z = MU::scaleTo01(m_bb->minZ(), m_bb->maxZ(), tri.p2.z());
		
		auto vx2 = lookupVoxel(r2x, r2y, r2z);
		
		auto r3x = MU::scaleTo01(m_bb->minX(), m_bb->maxX(), tri.p3.x());
		auto r3y = MU::scaleTo01(m_bb->minY(), m_bb->maxY(), tri.p3.y());
		auto r3z = MU::scaleTo01(m_bb->minZ(), m_bb->maxZ(), tri.p3.z());
		
		auto vx3 = lookupVoxel(r3x, r3y, r3z);
	}
};

}
#endif // VXGEOGRID_H
