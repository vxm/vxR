#ifndef VXR_CELLMARCHER_H
#define VXR_CELLMARCHER_H

#include "BoundingBox.h"
#include "Ray.h"

#include <cmath>

namespace vxCore
{

/// Amanatides–Woo DDA over a uniform axis-aligned grid whose resolution may
/// differ per axis (GeoGrid). Grid.cpp has a cubic special case plus L1/L2
/// occupancy skips; those stay in Grid. This is the shared step that GeoGrid
/// uses. Do not switch Grid over to this header until that path is A/B'd on
/// its own.
struct CellMarcher
{
	long idx[3]{0, 0, 0};
	long res[3]{1, 1, 1};
	long step[3]{1, 1, 1};
	scalar tMax[3]{};
	scalar tDelta[3]{};
	scalar tExit{0};
	bool ok{false};

	bool init(const BoundingBox &bb, unsigned rx, unsigned ry, unsigned rz,
	          const RayInv &ri, const Ray &ray)
	{
		scalar tEnter = 0;
		if (!bb.intersect(ri, tEnter, tExit))
		{
			ok = false;
			return false;
		}

		const scalar mn[3] = {bb.minX(), bb.minY(), bb.minZ()};
		const scalar sz[3] = {bb.xLength(), bb.yLength(), bb.zLength()};
		if (!(sz[0] > 0.0) || !(sz[1] > 0.0) || !(sz[2] > 0.0) || rx == 0 ||
		    ry == 0 || rz == 0)
		{
			ok = false;
			return false;
		}

		res[0] = static_cast<long>(rx);
		res[1] = static_cast<long>(ry);
		res[2] = static_cast<long>(rz);

		const scalar o[3] = {ri.ox, ri.oy, ri.oz};
		const scalar inv[3] = {ri.ix, ri.iy, ri.iz};
		const int neg[3] = {ri.sx, ri.sy, ri.sz};
		const v3s dir = ray.direction();
		const scalar d[3] = {dir.x(), dir.y(), dir.z()};

		for (int a = 0; a < 3; a++)
		{
			const scalar vs = sz[a] / scalar(res[a]);
			const scalar invVs = scalar(res[a]) / sz[a];
			long i = static_cast<long>(
			    std::floor((o[a] + d[a] * tEnter - mn[a]) * invVs));
			if (i < 0)
			{
				i = 0;
			}
			if (i >= res[a])
			{
				i = res[a] - 1;
			}
			idx[a] = i;
			step[a] = neg[a] ? -1 : 1;
			tDelta[a] = vs * (inv[a] < 0.0 ? -inv[a] : inv[a]);
			tMax[a] = (mn[a] + (idx[a] + (neg[a] ? 0 : 1)) * vs - o[a]) * inv[a];
		}
		ok = true;
		return true;
	}

	/// Move to the next cell. False when the ray leaves the grid.
	bool advance()
	{
		const int ax = (tMax[0] < tMax[1]) ? (tMax[0] < tMax[2] ? 0 : 2)
		                                   : (tMax[1] < tMax[2] ? 1 : 2);
		idx[ax] += step[ax];
		if (idx[ax] < 0 || idx[ax] >= res[ax])
		{
			ok = false;
			return false;
		}
		tMax[ax] += tDelta[ax];
		return true;
	}

	unsigned long linearIndex(unsigned rx, unsigned ry) const
	{
		return (static_cast<unsigned long>(ry) * rx) *
		           static_cast<unsigned long>(idx[2]) +
		       static_cast<unsigned long>(rx) *
		           static_cast<unsigned long>(idx[1]) +
		       static_cast<unsigned long>(idx[0]);
	}
};

} // namespace vxCore

#endif // VXR_CELLMARCHER_H
