#ifndef VXBROADPHASE_H
#define VXBROADPHASE_H
#include "Ray.h"
#include "Collision.h"
#include "Vector.h"
#include "Geometry.h"
#include "VectorUtils.h"

namespace vxCore {

using VisibleHandleArray = std::vector<VisibleHandle>; 
using VisibleHandleArrayRef = std::shared_ptr<VisibleHandleArray>;


/// name to be changed
struct bpSearchResult
{
	unsigned long index;
	VisibleHandleArrayRef geoRefs;
};

class BroadPhase
{
	std::vector<VisibleHandle> m_visibles;
	BoundingBoxHandle m_bb;
	BoundingBoxHandle m_cameraBB;
	using collision_visibleH = std::pair<Collision, VisibleHandle>;
	
public:

	///
	/// \brief vxBroadPhase
	///Default constructor
	BroadPhase();

	///
	/// \brief addVisible
	/// \param geo
	///adds a Visible handle to be viewed by broadphase
	void addVisible(VisibleHandle vis);
	
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
	void locateAndRegister(VisibleHandle vis);
	
	const bpSearchResult getList(const Ray &ray, 
								 v3s &sp, 
								 v3s &fp) const;
	
	bool indexIsValid(const long idx) const;
};

using vxBroadPhaseHandle = std::unique_ptr<BroadPhase>;

}
#endif // VXBROADPHASE_H
