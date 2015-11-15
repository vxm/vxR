#ifndef VXGEOGRID_H
#define VXGEOGRID_H

#include <set>
#include <vector>
#include "vxBoundingBox.h"


namespace vxCore {

using triangleIds = std::vector<unsigned long>; 
using triangleIdsRef = std::shared_ptr<triangleIds>;

class vxGeoGrid
{
	
	std::shared_ptr<vxBoundingBox> m_bb;
	unsigned int m_rx = {5u};
	unsigned int m_ry = {5u};
	unsigned int m_rz = {5u};
	
	std::vector<scalar> m_xvalues;
	std::vector<scalar> m_yvalues;
	std::vector<scalar> m_zvalues;
	
public:
	static triangleIdsRef emptyListRef;
	
	vxGeoGrid();
	
	std::vector<triangleIdsRef> m_members;
	std::shared_ptr<vxBoundingBox> bb() const;
	void setBb(const std::shared_ptr<vxBoundingBox> &bb);
	void close();
	
	unsigned long index(unsigned int a,
						unsigned int b,
						unsigned int c) const;
	
	unsigned long numVoxels() const;
	unsigned int rx() const;
	void setRx(unsigned int rx);
	unsigned int ry() const;
	void setRy(unsigned int ry);
	unsigned int rz() const;
	void setRz(unsigned int rz);

	unsigned long lookupVoxel(const v3 &v, int &a, int &b, int &c) const;
	void locateAndRegister(const vxTriRef &tri, unsigned long triangleID);
	
	const triangleIdsRef getList(const vxRay &ray, const v3 &sp) const;
	bool indexIsValid(const long idx) const;
	bool hasTriangles(const long idx) const;
};

}
#endif // VXGEOGRID_H
