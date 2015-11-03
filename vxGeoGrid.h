#ifndef VXGEOGRID_H
#define VXGEOGRID_H

#include <set>
#include <vector>
#include "vxBoundingBox.h"


namespace vxCore {

using voxelMembers = std::vector<unsigned long>;

class vxGeoGrid
{
	std::shared_ptr<vxBoundingBox> m_bb;
	unsigned int m_rx = {5u};
	unsigned int m_ry = {5u};
	unsigned int m_rz = {5u};
	
public:
	
	vxGeoGrid();
	
	std::vector<voxelMembers> m_members;
	std::shared_ptr<vxBoundingBox> bb() const;
	void setBb(const std::shared_ptr<vxBoundingBox> &bb);
	
	unsigned int rx() const;
	void setRx(unsigned int rx);
	unsigned int ry() const;
	void setRy(unsigned int ry);
	unsigned int rz() const;
	void setRz(unsigned int rz);

	unsigned int lookupVoxel(const scalar a, const scalar b, const scalar c);
	void locateAndRegister(const vxTriRef &tri, unsigned long triangleID);
};

}
#endif // VXGEOGRID_H
