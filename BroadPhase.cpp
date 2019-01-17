#include "BroadPhase.h"
#include <algorithm>
#include <limits>

#define DRAW_BBOX 0
#define DRAW_OBJECT_BBOX 0

using namespace vxCore;

BroadPhase::BroadPhase() { m_bb = std::make_shared<BoundingBox>(); }

void BroadPhase::addVisible(VisibleHandle vis)
{
	vis->updateBoundingBox();

	// never add a visible twice
	auto v = std::find(m_visibles.begin(), m_visibles.end(), vis);
	if (v == m_visibles.end())
	{
		m_visibles.emplace_back(vis);
	}
}

BoundingBoxHandle BroadPhase::closestBox(const v3s &p) const
{
	BoundingBoxHandle ret;

	scalar d = std::numeric_limits<scalar>::max();

	for (const auto &geo : m_visibles)
	{
		auto t = geo->boundingBox()->center().distance(p);

		if (t < d)
		{
			ret = geo->boundingBox();
			d = t;
		}
	}

	return ret;
}

void BroadPhase::updateCache()
{
	m_bb->clear();
	for (const auto &visibl : m_visibles)
	{
		visibl->updateBoundingBox();
		auto bb = visibl->boundingBox();
		m_bb->extend(*bb);
	}

	/*	const auto dngs = m_visibles.size() * 2;

	  m_xvalues.resize(dngs);
	  m_yvalues.resize(dngs);
	  m_zvalues.resize(dngs);

	  m_bb->clear();

	  unsigned int i = 0;
	  unsigned int ii = 1;

	  /// Run through geometries and slice the
	  /// broad phase
	  for (auto visibl : m_visibles)
	  {
	  visibl->updateBoundingBox();
	  auto bb = visibl->boundingBox();

	  m_xvalues[i] = bb->minX();
	  m_xvalues[ii] = bb->maxX();

	  m_yvalues[i] = bb->minY();
	  m_yvalues[ii] = bb->maxY();

	  m_zvalues[i] = bb->minZ();
	  m_zvalues[ii] = bb->maxZ();

	  m_bb->extend(*bb);

	  ii += 2;
	  i += 2;
	  }

	  // TODO:this sort could be less naive as I am
	  // inserting min and max, min and max...
	  m_rx = VU::sortAndUnique(m_xvalues) - 1;
	  m_ry = VU::sortAndUnique(m_yvalues) - 1;
	  m_rz = VU::sortAndUnique(m_zvalues) - 1;

	#ifdef _DEBUG
	  std::cout << "Resolution: " << m_rx << " " << m_ry << " " << m_rz
	  << std::endl;

	  std::cout << "X: ";
	  for (auto x : m_xvalues)
	  {
	  std::cout << ", " << x;
	  }
	  std::cout << std::endl;

	  std::cout << "Y: ";
	  for (auto y : m_yvalues)
	  {
	  std::cout << ", " << y;
	  }
	  std::cout << std::endl;

	  std::cout << "Z: ";
	  for (auto z : m_zvalues)
	  {
	  std::cout << ", " << z;
	  }
	  std::cout << std::endl;
	#endif

	  m_c_size = m_rx * m_ry * m_rz;
	  m_members.resize(m_c_size);

	  auto idx(0ul);
	  for (auto &l : m_members)
	  {
	  l.index = idx++;
	  }

	  for (auto &visibl : m_visibles)
	  {
	  locateAndRegister(visibl);
	  }

	#if _DEBUG

	  for (auto &l : m_members)
	  {
	  std::cout << "Index: " << l.index << std::endl;
	  if (l.geoRefs != nullptr)
	  for (auto &g : *l.geoRefs)
	  {
	  std::cout << "\tgeo color: " << g->color() << std::endl;
	  }
	  }

	#endif
	*/
}

unsigned long BroadPhase::index(unsigned int a, unsigned int b,
								unsigned int c) const
{
	return ((m_ry * m_rx) * c) + (m_rx * b) + a;
}

unsigned long BroadPhase::lookupVoxel(const v3s &v, int &a, int &b,
									  int &c) const
{
	const auto less_or_equal = [](scalar lhs, scalar rhs) { return lhs <= rhs; };

	auto it = std::lower_bound(m_xvalues.begin(), m_xvalues.end() - 1u, v.x(),
							   less_or_equal);
	a = it <= m_xvalues.begin() ? 0u : it - m_xvalues.begin() - 1u;

	it = std::lower_bound(m_yvalues.begin(), m_yvalues.end() - 1u, v.y(),
						  less_or_equal);
	b = it <= m_yvalues.begin() ? 0u : it - m_yvalues.begin() - 1u;

	it = std::lower_bound(m_zvalues.begin(), m_zvalues.end() - 1u, v.z(),
						  less_or_equal);
	c = it <= m_zvalues.begin() ? 0u : it - m_zvalues.begin() - 1u;

#if _DEBUG
	if (a < 0 || b < 0 || c < 0)
	{
		std::cerr << "index out of bounds in GeoGrid " << __LINE__ << std::endl;
	}
#endif

	return index(a, b, c);
}

void BroadPhase::locateAndRegister(VisibleHandle vis)
{
	auto bb = vis->boundingBox();
	int a1, b1, c1;
	auto idx1 = lookupVoxel(bb->min() + smallScalar, a1, b1, c1);
	int a2, b2, c2;
	auto idx2 = lookupVoxel(bb->max() - smallScalar, a2, b2, c2);

#ifdef _DEBUG
	auto cnt = bb->center();
	std::cout << std::endl << std::endl << std::endl;
	std::cout << "bb->min() " << bb->min() << std::endl;

	std::cout << "a1 " << a1 << std::endl;
	std::cout << "b1 " << b1 << std::endl;
	std::cout << "c1 " << c1 << std::endl;

	std::cout << "bb->max() " << bb->max() << std::endl;
	std::cout << "a2 " << a1 << std::endl;
	std::cout << "b2 " << b1 << std::endl;
	std::cout << "c2 " << c1 << std::endl;

	if (!indexIsValid(idx1) || !indexIsValid(idx2))
	{
		std::cerr << "Adding gmetry to an invalid " << std::endl;
	}
#endif

	if (idx1 == idx2)
	{
		if (m_members[idx1].geoRefs == nullptr)
		{
			m_members[idx1].geoRefs = std::make_shared<VisibleHandleArray>();
		}

		m_members[idx1].geoRefs->emplace_back(vis);
		return;
	}

	for (auto x = a1; x <= a2; x++)
	{
		for (auto y = b1; y <= b2; y++)
		{
			for (auto z = c1; z <= c2; z++)
			{
				auto idx = index(x, y, z);

				if (m_members[idx].geoRefs == nullptr)
				{
					m_members[idx].geoRefs = std::make_shared<VisibleHandleArray>();
				}

				// this index could be very redundant. //TODO:check
				m_members[idx].geoRefs->push_back(vis);
			}
		}
	}
}

const bpSearchResult BroadPhase::getList(const Ray &ray, v3s &sp, v3s &fp) const
{
	long retVal{static_cast<long>(m_c_size)};

	const auto &d = ray.direction();
	const auto &p = ray.origin();

	auto velX = d.xPositive() ? 1 : 0;
	auto velY = d.yPositive() ? 1 : 0;
	auto velZ = d.zPositive() ? 1 : 0;

	int x;
	int y;
	int z;

	do
	{
		retVal = lookupVoxel(sp, x, y, z);

		auto xVal = m_xvalues[x + velX] - p.x();
		auto yVal = m_yvalues[y + velY] - p.y();
		auto zVal = m_zvalues[z + velZ] - p.z();

		v3s intersectX = MU::rectAndXPlane(d, xVal);
		if (fabs(intersectX.y()) <= fabs(yVal) &&
			fabs(intersectX.z()) <= fabs(zVal))
		{
			fp = p + intersectX + v3s((velX ? 1.0 : -1.0) / 100.0, 0.0, 0.0);
		}

		v3s intersectY = MU::rectAndYPlane(d, yVal);
		if (fabs(intersectY.x()) <= fabs(xVal) &&
			fabs(intersectY.z()) <= fabs(zVal))
		{
			fp = p + intersectY + v3s(0.0, (velY ? 1.0 : -1.0) / 100.0, 0.0);
		}

		v3s intersectZ = MU::rectAndZPlane(d, zVal);
		if (fabs(intersectZ.x()) <= fabs(xVal) &&
			fabs(intersectZ.y()) <= fabs(yVal))
		{
			fp = p + intersectZ + v3s(0.0, 0.0, (velZ ? 1.0 : -1.0) / 100.0);
		}

		if (m_members[retVal].geoRefs != nullptr)
		{
			return m_members[retVal];
		}

		sp = fp;
	} while (indexIsValid(retVal) && m_bb->contains(sp));

	return bpSearchResult{m_c_size, nullptr};
}

bool BroadPhase::indexIsValid(const long idx) const
{
	return !(idx < 0l || idx >= m_c_size);
}

bool BroadPhase::throwRay(const Ray &ray) const
{
	Collision col;
	return throwRay(ray, col);
}

#define NAIVE_BB_METHOD 1
int BroadPhase::throwRay(const Ray &ray, Collision &collide) const
{
#if DRAW_BBOX
	if (m_bb->throwRay(ray, collide))
	{
		collide.setColor(m_color);
		collide.setValid(true);

		return 1;
	}
#else
#if NAIVE_BB_METHOD

	/*	// check if this is really faster.
	 * if (!m_bb->throwRay(ray, collide))
	  {
	  collide.setValid(false);
	  return 0;
	  }
	*/

	/*	// draw a margin
	  if(collide.u()<.001
	  || collide.u()>.999
	  || collide.v()<.001
	  || collide.v()>.999)
	  {
	  collide.setColor(vxColor::white);
	  collide.setValid(true);
	  collide.setUV(v2s(0.5,0.5));
	  return 1;
	  }
	*/
	Collision temp = collide;
	temp.setMaxDistance(std::numeric_limits<scalar>::max());

	for (const auto &visbl : m_visibles)
	{ /*
		 // if ray comes from an object which cannot see itself.
		 // aka convex.
		 if (visbl.get() == collide.m_geo &&
		   collide.m_geo->type() == VisibleType::kOtherOpaque)
		 {
		   continue;
		 }*/

		/// if ray can only see opaque and this is a light we move on.
		if (ray.m_vision == VisionType::kOpaque &&
			visbl->type() == VisibleType::kLight)
		{
			continue;
		}

		// if it's own hit falls outside it's bounding box.
		if (!visbl->testBoundingBox(ray, temp))
		{
			continue;
		}

#if !DRAW_OBJECT_BBOX
		visbl->throwRay(ray, temp);
#endif
		auto s = temp.position().distance(ray.origin());

		if (s < temp.maxDistance() && temp.isValid())
		{
			temp.setMaxDistance(s);
			collide = temp;
			collide.m_geo = visbl.get();
		}
	}

	return collide.isValid() ? 1 : 0;
#else

	if (!m_bb->throwRay(ray, collide))
	{
		collide.setValid(false);
		return 0;
	}

	// draw a margin
	if (collide.u() < .001 || collide.u() > .999 || collide.v() < .001 ||
		collide.v() > .999)
	{
		collide.setColor(vxColor::white);
		collide.setValid(true);
		collide.setUV(v2s(0.5, 0.5));
		return 1;
	}

	std::vector<collision_geometryH> cols;
	cols.reserve(10u);
	auto sp = collide.position() + collide.normal().inverted().tiny();

	auto prev{m_c_size};
	bpSearchResult bbxs;

	std::set<vxGeometryHandle> viewedGeos;
	vxCollision tmpCol{collide};

	bool thersHit = false;

	do
	{
		auto fp = sp;

		bbxs = getList(ray, sp, fp);

		if (bbxs.index == prev || bbxs.geoRefs == nullptr)
		{
			collide.setValid(false);
			return 0;
		}

		prev = bbxs.index;

		for (auto geo : *bbxs.geoRefs)
		{
			tmpCol.setPosition(sp);

			if (viewedGeos.insert(geo).second)
			{
				geo->throwRay(ray, tmpCol);
			}

			if (tmpCol.isValid())
			{
				cols.emplace_back(tmpCol, geo);
			}
		}

		int i = 0;
		while (!thersHit && i < cols.size())
		{
			if (ray.isCloser(fp, cols[i].first.position()))
			{
				thersHit = true;
			}
			i++;
		}
	} while (!thersHit);

	if (cols.size())
	{
		auto mind = (cols[0].first.position() - ray.origin()).length();
		collide = cols[0].first;
		for (auto &c : cols)
		{
			if (ray.distance(c.first.position()) < mind)
			{
				collide = c.first;
			}
		}

		collide.setColor(vxColor::indexColor(cols.size()));
		collide.setValid(true);
		collide.setUV(v2s(0.5, 0.5));
		return 1;
	}
	collide.setValid(false);
#endif
#endif
	return 0;
}

bool BroadPhase::hasCollision(const Ray &ray) const
{
	for (auto &visbl : m_visibles)
	{
		/// if ray can only see opaque and this is a light we move on.
		if (ray.m_vision == VisionType::kOpaque &&
			visbl->type() == VisibleType::kLight)
		{
			continue;
		}

		Collision col;

		// if it's own hit falls outside it's bounding box.
		if (visbl->boundingBox()->throwRay(ray, col) == 1)
		{
			if (visbl->throwRay(ray, col) == 1)
			{
				return true;
			}
		}
	}

	return false;
}
