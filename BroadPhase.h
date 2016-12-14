#ifndef VXBROADPHASE_H
#define VXBROADPHASE_H
#include "Ray.h"
#include "Collision.h"
#include "Vector.h"
#include "Geometry.h"
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

class BroadPhase
{
	std::vector<vxGeometryHandle> m_geometries;
	BoundingBoxHandle m_bb;
	BoundingBoxHandle m_cameraBB;
	using collision_geometryH = std::pair<Collision, vxGeometryHandle>;
	
public:

	///
	/// \brief vxBroadPhase
	///Default constructor
	BroadPhase();

	///
	/// \brief addGeometry
	/// \param geo
	///adds a geometry handle to be viewed by broadphase
	void addGeometry(vxGeometryHandle geo);
	
	BoundingBoxHandle closestBox(const v3s &p) const;
	
	
	Color m_color{Color::white};
	
	//renderable interface
	bool throwRay(const Ray &ray) const;
	int throwRay(const Ray &ray, Collision &collide) const;
	bool hasCollision(const Ray &ray) const;
	
	
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
	
	unsigned long lookupVoxel(const v3s &v, int &a, int &b, int &c) const;
	void locateAndRegister(vxGeometryHandle geo);
	
	const bpSearchResult getList(const Ray &ray, 
								 v3s &sp, 
								 v3s &fp) const;
	
	bool indexIsValid(const long idx) const;
};

using vxBroadPhaseHandle = std::unique_ptr<BroadPhase>;

}
#endif // VXBROADPHASE_H
