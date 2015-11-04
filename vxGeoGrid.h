#ifndef VXGEOGRID_H
#define VXGEOGRID_H

#include <set>
#include <vector>
#include "vxBoundingBox.h"


namespace vxCore {

using listRef = std::shared_ptr<std::vector<unsigned long>>;


class vxGeoGrid
{
	std::shared_ptr<vxBoundingBox> m_bb;
	unsigned int m_rx = {5u};
	unsigned int m_ry = {5u};
	unsigned int m_rz = {5u};
	
public:
	static listRef emptyListRef;
	
	vxGeoGrid();
	
	std::vector<listRef> m_members;
	std::shared_ptr<vxBoundingBox> bb() const;
	void setBb(const std::shared_ptr<vxBoundingBox> &bb);
	
	unsigned int rx() const;
	void setRx(unsigned int rx);
	unsigned int ry() const;
	void setRy(unsigned int ry);
	unsigned int rz() const;
	void setRz(unsigned int rz);

	unsigned int lookupVoxel(const scalar a, const scalar b, const scalar c) const;
	void locateAndRegister(const vxTriRef &tri, unsigned long triangleID);
	
	listRef getList(const vxRay &ray) const;
};

}
#endif // VXGEOGRID_H
