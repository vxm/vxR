#ifndef VXGEOGRID_H
#define VXGEOGRID_H

#include <set>
#include <vector>
#include "BoundingBox.h"

namespace vxCore {

using triangleIds = std::vector<unsigned long>; 
using triangleIdsRef = std::shared_ptr<triangleIds>;

/// name to be changed
struct searchResult
{
	unsigned long index;
	triangleIdsRef listRef;
};

///
/// \brief The vxGeoGrid class
///
class GeoGrid final
{
	///
	/// \brief m_bb
	///
	std::shared_ptr<BoundingBox> m_bb;
	//TODO: above 11 this becomes problematic, investigate why.
	unsigned int m_rx = {11u};
	unsigned int m_ry = {5u};
	unsigned int m_rz = {11u};
	
	scalar m_xSlice;
	scalar m_ySlice;
	scalar m_zSlice;

	scalar m_c_xBoxSize;
	scalar m_c_yBoxSize;
	scalar m_c_zBoxSize;
	
	std::vector<scalar> m_xvalues;
	std::vector<scalar> m_yvalues;
	std::vector<scalar> m_zvalues;

public:
	static const searchResult invalidResult;
	///
	/// \brief vxGeoGrid
	///
	GeoGrid();
	///
	/// \brief vxGeoGrid
	/// \param rx
	/// \param ry
	/// \param rz
	///
	GeoGrid(unsigned int rx, unsigned int ry, unsigned int rz)
		:m_rx(rx)
		,m_ry(ry)
		,m_rz(rz)
	{}
	///
	/// \brief updateCache
	///
	void updateCache();
	///
	/// \brief setResolution
	/// \param x
	/// \param y
	/// \param z
	///
	void setResolution(unsigned int x, unsigned int y, unsigned int z);
	///
	/// \brief m_members
	///
	std::vector<searchResult> m_members;
	///
	/// \brief bb
	/// \return 
	///
	std::shared_ptr<BoundingBox> bb() const;
	///
	/// \brief setBb
	/// \param bb
	///
	void setBb(const std::shared_ptr<BoundingBox> &bb);
	///
	/// \brief size
	/// \return 
	///
	unsigned long size() const;
	///
	/// \brief index
	/// \param a
	/// \param b
	/// \param c
	/// \return 
	///
	unsigned long index(unsigned int a,
						unsigned int b,
						unsigned int c) const;
	///
	/// \brief numVoxels
	/// \return 
	///
	unsigned long numVoxels() const;
	///
	/// \brief rx
	/// \return 
	///
	unsigned int rx() const;
	///
	/// \brief setRx
	/// \param rx
	///
	void setRx(unsigned int rx);
	///
	/// \brief ry
	/// \return 
	///
	unsigned int ry() const;
	///
	/// \brief setRy
	/// \param ry
	///
	void setRy(unsigned int ry);
	///
	/// \brief rz
	/// \return 
	///
	unsigned int rz() const;
	///
	/// \brief setRz
	/// \param rz
	///
	void setRz(unsigned int rz);
	///
	/// \brief linearLookupVoxel
	/// \param v
	/// \param a
	/// \param b
	/// \param c
	/// \return 
	///
	unsigned long linearLookupVoxel(const v3s &v, int &a, int &b, int &c) const;
	///
	/// \brief lookupVoxel
	/// \param v
	/// \param a
	/// \param b
	/// \param c
	/// \return 
	///
	unsigned long lookupVoxel(const v3s &v, int &a, int &b, int &c) const;
	///
	/// \brief locateAndRegister
	/// \param tri
	/// \param triangleID
	///
	void locateAndRegister(const TriRef &tri, unsigned long triangleID);
	///
	/// \brief getList
	/// \param ray
	/// \param sp
	/// \return 
	///
	const searchResult getList(const Ray &ray, v3s &sp) const;
	///
	/// \brief indexIsValid
	/// \param idx
	/// \return 
	///
	bool indexIsValid(const long idx) const;
	///
	/// \brief hasTriangles
	/// \param idx
	/// \return 
	///
	bool hasTriangles(const long idx) const;
};

}
#endif // VXGEOGRID_H
