#ifndef VXGEOGRID_H
#define VXGEOGRID_H

#include <set>
#include <vector>
#include "vxBoundingBox.h"


namespace vxCore {

using triangleIds = std::vector<unsigned long>; 
using triangleIdsRef = std::shared_ptr<triangleIds>;

/// name to be changed
struct searchResult
{
	unsigned long index;
	triangleIdsRef listRef;
};


class vxGeoGrid
{
	
	std::shared_ptr<vxBoundingBox> m_bb;
	unsigned int m_rx = {30u};
	unsigned int m_ry = {30u};
	unsigned int m_rz = {30u};
	
	std::vector<scalar> m_xvalues;
	std::vector<scalar> m_yvalues;
	std::vector<scalar> m_zvalues;
	
public:
	static const searchResult invalidResult;
	
	vxGeoGrid();
	
	std::vector<searchResult> m_members;
	std::shared_ptr<vxBoundingBox> bb() const;
	void setBb(const std::shared_ptr<vxBoundingBox> &bb);
	void close();

	unsigned int size() const;
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
	
	const searchResult getList(const vxRay &ray, v3 &sp) const;
	bool indexIsValid(const long idx) const;
	bool hasTriangles(const long idx) const;
};

}
#endif // VXGEOGRID_H
