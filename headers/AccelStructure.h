#ifndef VXACCELSTRUCTURE_H
#define VXACCELSTRUCTURE_H

#include <memory>
#include <string>
#include <vector>

#include "Collision.h"
#include "Ray.h"
#include "visible.h"

namespace vxCore
{

///
/// The scene level spatial structure. Scene owns one and only talks to it
/// through this interface, so a backend can be picked by name at run time
/// (--accel <name>) and two backends can be compared on the same scene.
///
/// To add a backend: derive from this class and add one line to the table
/// in AccelStructure.cpp.
///
class AccelStructure
{
public:
	virtual ~AccelStructure() = default;

	/// Name it is registered with.
	virtual const char *name() const = 0;
	/// Adds a visible, it must not be added twice.
	virtual void addVisible(VisibleHandle vis) = 0;
	/// Called once after every visible has been added.
	virtual void updateCache() = 0;
	/// Closest hit among the visibles. Planes and the dome are handled
	/// by Scene. Returns 1 when collide is valid.
	virtual int throwRay(const Ray &ray, Collision &collide) const = 0;
	/// Same as throwRay, but the caller already has a hit (a plane) at
	/// distance maxDist and only cares about a visible that is at that distance
	/// or closer. A backend may use it to skip work and then return 0, or may
	/// return a farther hit, the caller compares distances either way. The
	/// default ignores the bound. maxDist is a distance, never ray.length().
	virtual int throwRayBounded(const Ray &ray, Collision &collide,
								scalar maxDist) const
	{
		(void)maxDist;
		return throwRay(ray, collide);
	}
	virtual bool hasCollision(const Ray &ray) const = 0;

	/// Every visible that was added, in insertion order.
	virtual const std::vector<VisibleHandle> &visibles() const = 0;

	/// Returns nullptr when the name is not registered.
	static std::unique_ptr<AccelStructure> create(const std::string &name);
	static std::vector<std::string> names();
	static const char *defaultName();
};

using AccelStructureHandle = std::unique_ptr<AccelStructure>;

} // namespace vxCore
#endif // VXACCELSTRUCTURE_H
