#ifndef VXBROADPHASE_H
#define VXBROADPHASE_H
#include "vxRay.h"
#include "vxCollision.h"
#include "vxVector.h"
#include "vxGeometry.h"
#include "VectorUtils.h"

namespace vxCore {

using geometryHandleArray = std::vector<vxGeometryHandle>; 
using geometryHandleArrayRef = std::shared_ptr<geometryHandleArray>;


/// name to be changed
struct bpSearchResult
{
	unsigned long index;
	geometryHandleArrayRef geoRefs;
};

class vxBroadPhase
{
	std::vector<vxGeometryHandle> m_geometries;
	vxBoundingBoxHandle m_bb;
	vxBoundingBoxHandle m_cameraBB;
	
public:

	///
	/// \brief vxBroadPhase
	///Default constructor
	vxBroadPhase();

	///
	/// \brief addGeometry
	/// \param geo
	///adds a geometry handle to be viewed by broadphase
	void addGeometry(vxGeometryHandle geo);
	
	vxBoundingBoxHandle closestBox(const v3 &p) const;
	
	
	vxColor m_color{vxColor::white};
	
	//renderable interface
	bool throwRay(const vxRay &ray) const;
	int throwRay(const vxRay &ray, vxCollision &collide) const;
	bool hasCollision(const vxRay &ray) const;
	
	
public:
	
	///////////////////////////////////////////
	///// new grid interface.
	///////////////////////////////////////////
	
	static const bpSearchResult invalidResult;

	unsigned int m_rx{1u};
	unsigned int m_ry{1u};
	unsigned int m_rz{1u};
	unsigned int m_c_size{1u};
	
	std::vector<scalar> m_xvalues;
	std::vector<scalar> m_yvalues;
	std::vector<scalar> m_zvalues;

	std::vector<bpSearchResult> m_members;

	void updateCache();
	unsigned long index(unsigned int a,
						unsigned int b,
						unsigned int c) const;
	
	unsigned int rx() const;
	void setRx(unsigned int rx);

	unsigned int ry() const;
	void setRy(unsigned int ry);

	unsigned int rz() const;
	void setRz(unsigned int rz);

	unsigned long lookupVoxel(const v3 &v, int &a, int &b, int &c) const;
	void locateAndRegister(vxGeometryHandle geo);
	const bpSearchResult getList(const vxRay &ray, v3 &sp, bool skip) const;
	bool indexIsValid(const long idx) const;
	bool hasTriangles(const long idx) const;
};

using vxBroadPhaseHandler = std::shared_ptr<vxBroadPhase>;

}
#endif // VXBROADPHASE_H
