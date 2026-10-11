#include "AccelUnitTest.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

#include "AccelStructure.h"
#include "BoundingBox.h"
#include "BroadPhase.h"
#include "Bvh.h"
#include "CellMarcher.h"
#include "Cylinder.h"
#include "FileUtils.h"
#include "GeoGrid.h"
#include "Grid.h"
#include "ImageProperties.h"
#include "Light.h"
#include "MathUtils.h"
#include "Sampler.h"
#include "Scene.h"
#include "SceneParser.h"
#include "Sphere.h"
#include "TriangleMesh.h"

#ifndef VXR_SOURCE_DIR
#define VXR_SOURCE_DIR "."
#endif

using namespace vxCore;

namespace
{

using SteadyClock = std::chrono::steady_clock;

double msSince(SteadyClock::time_point start)
{
	return std::chrono::duration<double, std::milli>(SteadyClock::now() - start).count();
}

/// Deterministic generator, independent of the renderer's thread local one.
class Rng
{
	std::uint64_t s[4];

	static std::uint64_t splitmix(std::uint64_t &x)
	{
		std::uint64_t z = (x += 0x9e3779b97f4a7c15ull);
		z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ull;
		z = (z ^ (z >> 27)) * 0x94d049bb133111ebull;
		return z ^ (z >> 31);
	}
	static std::uint64_t rotl(std::uint64_t x, int k)
	{
		return (x << k) | (x >> (64 - k));
	}

public:
	explicit Rng(std::uint64_t seed)
	{
		for (auto &v : s)
		{
			v = splitmix(seed);
		}
	}
	std::uint64_t next()
	{
		const std::uint64_t result = rotl(s[1] * 5, 7) * 9;
		const std::uint64_t t = s[1] << 17;
		s[2] ^= s[0];
		s[3] ^= s[1];
		s[1] ^= s[2];
		s[0] ^= s[3];
		s[2] ^= t;
		s[3] = rotl(s[3], 45);
		return result;
	}
	/// [0, 1)
	double u() { return double(next() >> 11) * (1.0 / 9007199254740992.0); }
	double range(double a, double b) { return a + (b - a) * u(); }
	std::size_t index(std::size_t n) { return std::size_t(next() % n); }
	v3s unit()
	{
		for (;;)
		{
			v3s v(range(-1, 1), range(-1, 1), range(-1, 1));
			const auto l = v.length();
			if (l > 1e-3 && l <= 1.0)
			{
				return v / l;
			}
		}
	}
};

bool nanBits(scalar v)
{
	std::uint64_t bits;
	std::memcpy(&bits, &v, sizeof(bits));
	return ((bits >> 52) & 0x7ffu) == 0x7ffu && (bits & 0xfffffffffffffull) != 0;
}

bool closeValue(scalar a, scalar b, scalar tol)
{
	if (nanBits(a) || nanBits(b))
	{
		return nanBits(a) && nanBits(b);
	}
	return std::abs(a - b) <= tol;
}

/// v3s accessors return doubles from code built with -ffast-math, which tells
/// the optimizer they are never NaN, so look at them through get().
struct Comps
{
	scalar v[3];
	explicit Comps(const v3s &p) { p.get(v[0], v[1], v[2]); }
	bool anyNan() const { return nanBits(v[0]) || nanBits(v[1]) || nanBits(v[2]); }
};

bool closeVec(const v3s &a, const v3s &b, scalar tol)
{
	const Comps ca(a), cb(b);
	return closeValue(ca.v[0], cb.v[0], tol) && closeValue(ca.v[1], cb.v[1], tol) &&
		   closeValue(ca.v[2], cb.v[2], tol);
}

/// A ray as plain numbers: Ray copies drop the vision, so it is rebuilt.
struct RaySpec
{
	v3s origin;
	v3s dir;
	const char *kind;
};

struct Diff
{
	std::size_t casts{0};
	std::size_t hits{0};
	std::size_t hardMismatches{0}; // valid, geometry pointer or position
	std::size_t softMismatches{0}; // normal, uv or color
	std::size_t nanSkipped{0};     // hit with a NaN position, see compareHits
	std::size_t printed{0};
	std::size_t softPrinted{0};
};

const char *visionName(VisionType v)
{
	return v == VisionType::kOpaque ? "opaque" : "all";
}

/// Compares two results of the same ray. Returns true when they agree on the
/// things the renderer and the spec care about (valid, geometry, position).
bool compareHits(const char *label, const RaySpec &spec, VisionType vision,
				 const Collision &a, int ra, const Collision &b, int rb, Diff &diff,
				 bool compareColor = true)
{
	diff.casts++;
	if (ra)
	{
		diff.hits++;
	}

	// A hit with a NaN position is garbage in, garbage out: Sphere::throwRay
	// divides by the z component of the direction, so a ray with dz == 0
	// gets NaN, and the comparison "closer than" has no meaning. Not compared.
	if (ra && rb && (Comps(a.position()).anyNan() || Comps(b.position()).anyNan()))
	{
		diff.nanSkipped++;
		return true;
	}

	std::string why;
	if (ra != rb)
	{
		why = "return value differs";
	}
	else if (ra)
	{
		if (a.isValid() != b.isValid())
		{
			why = "isValid differs";
		}
		else if (a.m_geo != b.m_geo)
		{
			why = "different visible";
		}
		else if (!closeVec(a.position(), b.position(), 1e-9))
		{
			why = "position differs";
		}
	}

	if (why.empty())
	{
		if (ra)
		{
			std::string soft;
			if (!closeVec(a.normal(), b.normal(), 1e-9))
				soft += " normal";
			if (!closeValue(a.u(), b.u(), 1e-9) || !closeValue(a.v(), b.v(), 1e-9))
				soft += " uv";
			if (compareColor &&
				(!closeValue(a.color().r(), b.color().r(), 1e-9) ||
				 !closeValue(a.color().g(), b.color().g(), 1e-9) ||
				 !closeValue(a.color().b(), b.color().b(), 1e-9)))
				soft += " color";
			if (!soft.empty())
			{
				diff.softMismatches++;
				if (diff.softPrinted++ < 3)
				{
					std::cout << std::setprecision(10) << "    soft [" << label << "] differs in"
							  << soft << " (" << spec.kind << ", " << visionName(vision)
							  << ") ray " << spec.origin << " -> " << spec.dir
							  << "\n      A normal " << a.normal() << " uv " << a.u() << " "
							  << a.v() << " color " << a.color()
							  << "\n      B normal " << b.normal() << " uv " << b.u() << " "
							  << b.v() << " color " << b.color() << std::setprecision(6)
							  << std::endl;
				}
			}
		}
		return true;
	}

	diff.hardMismatches++;
	if (diff.printed++ < 5)
	{
		std::cout << std::setprecision(10);
		std::cout << "    MISMATCH [" << label << "] " << why << " ("
				  << spec.kind << ", " << visionName(vision) << ")\n"
				  << "      ray   " << spec.origin << " -> " << spec.dir << "\n"
				  << "      A ret " << ra << " pos " << a.position() << " geo "
				  << static_cast<const void *>(a.m_geo) << "\n"
				  << "      B ret " << rb << " pos " << b.position() << " geo "
				  << static_cast<const void *>(b.m_geo) << std::endl;
		std::cout << std::setprecision(6);
	}
	return false;
}

void printDiffLine(const std::string &name, const Diff &d, double msA, double msB,
				   const char *nameA, const char *nameB)
{
	std::cout << "  " << std::left << std::setw(30) << name << std::right
			  << std::setw(8) << d.casts << " casts " << std::setw(8) << d.hits
			  << " hits  mismatches " << d.hardMismatches << " (soft "
			  << d.softMismatches << ", nan " << d.nanSkipped << ")   " << nameA
			  << " " << std::fixed << std::setprecision(1) << msA << " ms, " << nameB
			  << " " << msB << " ms" << std::defaultfloat << std::setprecision(6)
			  << std::endl;
}

int envInt(const char *name, int fallback)
{
	const char *v = std::getenv(name);
	return v ? std::atoi(v) : fallback;
}

////////////////////////////////////////////////////////////////////////////
// ray sets

std::vector<RaySpec> makeRays(Rng &rng, const std::vector<BoundingBox> &boxes,
							  const BoundingBox &world, std::size_t count)
{
	std::vector<RaySpec> out;
	out.reserve(count);

	const auto inWorld = [&]() {
		return v3s(rng.range(world.minX(), world.maxX()),
				   rng.range(world.minY(), world.maxY()),
				   rng.range(world.minZ(), world.maxZ()));
	};
	const auto inBox = [&](const BoundingBox &b) {
		return v3s(rng.range(b.minX(), b.maxX()), rng.range(b.minY(), b.maxY()),
				   rng.range(b.minZ(), b.maxZ()));
	};
	const auto push = [&](const v3s &o, v3s d, const char *kind) {
		if (d.length() < 1e-9)
		{
			return;
		}
		out.push_back({o, d.unit(), kind});
	};

	while (out.size() < count)
	{
		const auto pick = rng.u();
		const BoundingBox *box =
			boxes.empty() ? nullptr : &boxes[rng.index(boxes.size())];

		if (pick < 0.30 || !box)
		{
			push(inWorld(), rng.unit(), "random");
		}
		else if (pick < 0.55)
		{
			push(inWorld(), inBox(*box) - inWorld(), "aimed");
		}
		else if (pick < 0.65)
		{
			static const v3s axes[6] = {v3s::constX,      v3s::constY,
										v3s::constZ,      v3s::constMinusX,
										v3s::constMinusY, v3s::constMinusZ};
			const auto o = rng.u() < 0.5 ? inWorld() : inBox(*box);
			push(o, axes[rng.index(6)], "axis");
		}
		else if (pick < 0.75)
		{
			push(inBox(*box), rng.unit(), "inside");
		}
		else if (pick < 0.88)
		{
			// Origin on a face plane of the box, direction inside that plane.
			v3s o = inWorld();
			v3s d = rng.unit();
			const int axis = int(rng.index(3));
			const scalar plane = rng.u() < 0.5 ? box->min()[axis] : box->max()[axis];
			o[axis] = plane;
			d[axis] = 0.0;
			push(o, d, "grazing");
		}
		else
		{
			// Aim at a corner or at an edge midpoint.
			v3s t((rng.u() < 0.5 ? box->minX() : box->maxX()),
				  (rng.u() < 0.5 ? box->minY() : box->maxY()),
				  (rng.u() < 0.5 ? box->minZ() : box->maxZ()));
			if (rng.u() < 0.5)
			{
				t[int(rng.index(3))] = 0.5 * (box->min()[0] + box->max()[0]);
			}
			push(inWorld(), t - inWorld(), "corner");
		}
	}
	return out;
}

BoundingBox expanded(const std::vector<BoundingBox> &boxes, scalar factor)
{
	BoundingBox all;
	for (const auto &b : boxes)
	{
		all.extend(b);
	}
	if (boxes.empty())
	{
		all.set(-1, -1, -1, 1, 1, 1);
	}
	const auto c = all.center();
	const auto h = (all.max() - all.min()) * (0.5 * factor);
	const scalar minHalf = 0.5;
	return BoundingBox(c.x() - std::max(h.x(), minHalf), c.y() - std::max(h.y(), minHalf),
					   c.z() - std::max(h.z(), minHalf), c.x() + std::max(h.x(), minHalf),
					   c.y() + std::max(h.y(), minHalf), c.z() + std::max(h.z(), minHalf));
}

std::vector<BoundingBox> boxesOf(const std::vector<VisibleHandle> &visibles)
{
	std::vector<BoundingBox> boxes;
	for (const auto &v : visibles)
	{
		if (!v->isHittable())
		{
			continue;
		}
		const auto bb = v->boundingBox();
		if (bb && bb->minX() <= bb->maxX() && std::abs(bb->minX()) < 1e6 &&
			std::abs(bb->maxX()) < 1e6)
		{
			boxes.push_back(*bb);
		}
	}
	return boxes;
}

////////////////////////////////////////////////////////////////////////////
// accel level: broadphase against bvh on the same visibles

int compareAccel(const std::string &name, const AccelStructure &a,
				 const AccelStructure &b, const std::vector<RaySpec> &rays)
{
	Diff diff;
	double msA = 0.0;
	double msB = 0.0;
	for (const auto vision : {VisionType::kOpaque, VisionType::kAll})
	{
		for (const auto &spec : rays)
		{
			const Ray ray(spec.origin, spec.dir, vision);
			// What Scene::throwRay hands over: an invalid, default collision.
			Collision ca;
			Collision cb;
			ca.setValid(false);
			cb.setValid(false);

			auto t0 = SteadyClock::now();
			const int ra = a.throwRay(ray, ca);
			msA += msSince(t0);
			t0 = SteadyClock::now();
			const int rb = b.throwRay(ray, cb);
			msB += msSince(t0);

			compareHits(name.c_str(), spec, vision, ca, ra, cb, rb, diff);
		}
	}
	printDiffLine(name, diff, msA, msB, a.name(), b.name());
	return diff.hardMismatches ? 1 : 0;
}

////////////////////////////////////////////////////////////////////////////
// synthetic scene with many visibles, ties and a bit of everything

struct SyntheticScene
{
	std::vector<VisibleHandle> visibles;
	std::shared_ptr<Grid> grid;
};

SyntheticScene makeSynthetic(Rng &rng)
{
	SyntheticScene s;

	const auto addSphere = [&](const v3s &p, scalar r) {
		auto sph = std::make_shared<Sphere>();
		Matrix44 m;
		m.setOrigin(p);
		sph->setTransform(m);
		sph->setRadius(r);
		s.visibles.push_back(sph);
		return sph;
	};

	for (int i = 0; i < 700; i++)
	{
		addSphere({rng.range(-6, 6), rng.range(-3, 3), rng.range(-6, 6)},
				  rng.range(0.1, 0.5));
	}
	// Exact copies of an earlier sphere: same distance, the first added wins.
	for (int i = 0; i < 12; i++)
	{
		const auto &src = std::static_pointer_cast<Sphere>(s.visibles[rng.index(100)]);
		addSphere(src->transform().origin(), src->radius());
	}
	// A cluster of nested spheres sharing a center.
	for (int i = 0; i < 6; i++)
	{
		addSphere({1.5, 0.25, -1.0}, 0.3 + 0.0 * i);
	}

	// Cylinders with no transform sit at the origin. Hits are clipped to
	// the finite height.
	for (int i = 0; i < 3; i++)
	{
		auto cyl = std::make_shared<Cylinder>();
		cyl->setRadius(0.3 + 0.1 * i);
		cyl->setHeight(0.8 + 0.4 * i);
		s.visibles.push_back(cyl);
	}

	// A voxel grid.
	s.grid = std::make_shared<Grid>(v3s::zero, 2.0);
	s.grid->setResolution(28);
	for (int i = 0; i < 300; i++)
	{
		s.grid->vxAt(long(rng.index(28)), long(rng.index(28)), long(rng.index(28)))
			.setByte(static_cast<unsigned char>(1 + rng.index(20)));
	}
	s.grid->updateCaches();
	s.visibles.push_back(s.grid);

	// Lights: a hittable one, and the ones that never are.
	auto sl = std::make_shared<SphereLight>();
	sl->setPosition(2.0, 2.0, 2.0);
	sl->setRadius(0.4);
	s.visibles.push_back(sl);
	auto sl2 = std::make_shared<SphereLight>();
	sl2->setPosition(-3.0, 1.0, 2.5);
	sl2->setRadius(0.4);
	s.visibles.push_back(sl2);
	auto dl = std::make_shared<DirectLight>();
	s.visibles.push_back(dl);
	auto pl = std::make_shared<PointLight>();
	pl->setPosition(0.5, 3.0, 0.5);
	s.visibles.push_back(pl);

	return s;
}

int testSynthetic(std::size_t rayCount)
{
	std::cout << "== synthetic scene (spheres, ties, cylinders, grid, lights)\n";
	Rng rng(20240611);
	auto scene = makeSynthetic(rng);

	BroadPhase flat;
	Bvh bvh;
	for (auto &v : scene.visibles)
	{
		flat.addVisible(v);
		bvh.addVisible(v);
	}
	flat.updateCache();
	bvh.updateCache();

	const auto &info = bvh.buildInfo();
	std::cout << "  bvh: " << info.treePrims << " in tree, " << info.nodes
			  << " nodes, " << info.leaves << " leaves, depth " << info.maxDepth + 1
			  << ", max leaf " << info.maxLeafPrims << ", " << info.unbounded
			  << " unbounded, " << info.emitters << " emitters, "
			  << info.droppedLights << " lights never hit" << std::endl;

	int failures = 0;
	if (info.maxLeafPrims > Bvh::kMaxLeafPrims)
	{
		std::cout << "  FAIL leaf has more than " << Bvh::kMaxLeafPrims << " prims\n";
		failures++;
	}
	if (info.maxDepth + 1 >= Bvh::kStackSize)
	{
		std::cout << "  FAIL tree deeper than the traversal stack\n";
		failures++;
	}
	if (info.droppedLights != 2 || info.emitters != 2)
	{
		std::cout << "  FAIL light classification: expected 2 emitters and 2 "
					 "lights never hit\n";
		failures++;
	}

	const auto boxes = boxesOf(scene.visibles);
	const auto world = expanded(boxes, 1.4);
	const auto rays = makeRays(rng, boxes, world, rayCount);
	failures += compareAccel("synthetic", flat, bvh, rays);
	return failures;
}

/// A tree where every box is the same and the centroids coincide, and a line
/// of boxes: the splits that SAH cannot make.
int testDegenerateTrees(std::size_t rayCount)
{
	std::cout << "== degenerate trees (identical boxes, collinear boxes)\n";
	Rng rng(77);
	int failures = 0;

	for (int variant = 0; variant < 2; variant++)
	{
		std::vector<VisibleHandle> visibles;
		for (int i = 0; i < 64; i++)
		{
			auto sph = std::make_shared<Sphere>();
			Matrix44 m;
			m.setOrigin(variant == 0 ? v3s(0.0, 0.0, 0.0) : v3s(i * 0.7, 0.0, 0.0));
			sph->setTransform(m);
			sph->setRadius(0.3);
			visibles.push_back(sph);
		}
		BroadPhase flat;
		Bvh bvh;
		for (auto &v : visibles)
		{
			flat.addVisible(v);
			bvh.addVisible(v);
		}
		flat.updateCache();
		bvh.updateCache();

		const auto boxes = boxesOf(visibles);
		const auto world = expanded(boxes, 1.4);
		const auto rays = makeRays(rng, boxes, world, rayCount / 4);
		failures += compareAccel(variant == 0 ? "identical boxes" : "collinear boxes",
								 flat, bvh, rays);
	}

	// An empty structure, and one with only lights that are never hit.
	{
		BroadPhase flat;
		Bvh bvh;
		auto dl = std::make_shared<DirectLight>();
		flat.addVisible(dl);
		bvh.addVisible(dl);
		flat.updateCache();
		bvh.updateCache();
		const auto rays = makeRays(rng, {}, expanded({}, 1.0), 2000);
		failures += compareAccel("only a direct light", flat, bvh, rays);
	}
	return failures;
}

////////////////////////////////////////////////////////////////////////////
// RayInv / slab test against the legacy box test

int testSlab(std::size_t rayCount)
{
	std::cout << "== slab test against the legacy box test\n";
	Rng rng(5);
	std::size_t legacyHits = 0, slabHits = 0, legacyOnly = 0, slabOnly = 0;

	for (std::size_t i = 0; i < rayCount * 5; i++)
	{
		const v3s c(rng.range(-3, 3), rng.range(-3, 3), rng.range(-3, 3));
		const v3s h(rng.range(0.05, 1.5), rng.range(0.05, 1.5), rng.range(0.05, 1.5));
		const BoundingBox box(c.x() - h.x(), c.y() - h.y(), c.z() - h.z(),
							  c.x() + h.x(), c.y() + h.y(), c.z() + h.z());
		// A box padded like the BVH pads the ones of the tree.
		const BoundingBox padded(box.minX() - 1e-9 * (1 + std::abs(box.minX())),
								 box.minY() - 1e-9 * (1 + std::abs(box.minY())),
								 box.minZ() - 1e-9 * (1 + std::abs(box.minZ())),
								 box.maxX() + 1e-9 * (1 + std::abs(box.maxX())),
								 box.maxY() + 1e-9 * (1 + std::abs(box.maxY())),
								 box.maxZ() + 1e-9 * (1 + std::abs(box.maxZ())));

		v3s o(rng.range(-5, 5), rng.range(-5, 5), rng.range(-5, 5));
		v3s d = rng.unit();
		const auto sel = rng.u();
		if (sel < 0.1)
		{
			d = v3s(1, 0, 0);
		}
		else if (sel < 0.2)
		{
			d[int(rng.index(3))] = 0.0;
			d = d.unit();
			o[int(rng.index(3))] = box.min()[int(rng.index(3))];
		}
		if (d.length() < 0.5)
		{
			continue;
		}

		const Ray ray(o, d, VisionType::kAll);
		Collision col;
		const bool legacy = box.throwRay(ray, col) != 0;
		const RayInv ri(ray);
		scalar t0, t1;
		const bool slab = padded.intersect(ri, t0, t1);
		legacyHits += legacy;
		slabHits += slab;
		legacyOnly += legacy && !slab;
		slabOnly += slab && !legacy;
	}
	std::cout << "  legacy " << legacyHits << " hits, slab " << slabHits
			  << " hits, legacy-only " << legacyOnly << ", slab-only " << slabOnly
			  << std::endl;
	if (legacyOnly)
	{
		std::cout << "  FAIL the slab test rejected rays the legacy test accepts\n";
		return 1;
	}
	return 0;
}

////////////////////////////////////////////////////////////////////////////
// the scene files

std::vector<std::string> missingInputs(const std::string &scenePath)
{
	std::vector<std::string> missing;
	std::ifstream in(scenePath);
	std::stringstream buf;
	buf << in.rdbuf();
	const auto text = buf.str();
	const std::regex re(R"re(\b(imagePath|filePath)\s*=\s*"([^"]*)")re");
	const auto dir = scenePath.substr(0, scenePath.find_last_of('/'));
	for (auto it = std::sregex_iterator(text.begin(), text.end(), re);
		 it != std::sregex_iterator(); ++it)
	{
		const std::string value = (*it)[2];
		const std::string full = value.rfind("/", 0) == 0 ? value : dir + "/" + value;
		if (!std::ifstream(full).good())
		{
			missing.push_back(full);
		}
	}
	return missing;
}

struct SceneResult
{
	bool valid;
	v3s position;
	v3s normal;
	Color color;
	scalar u, v;
	Visible *geo;
};

int testSceneFile(const std::string &file, std::size_t rayCount)
{
	const std::string path = std::string(VXR_SOURCE_DIR) + "/scenes/" + file;
	std::cout << "== scene " << file << "\n";
	if (!std::ifstream(path).good())
	{
		std::cout << "  SKIPPED: scene file not found\n";
		return 0;
	}
	const auto missing = missingInputs(path);
	if (!missing.empty())
	{
		std::cout << "  SKIPPED: missing input " << missing.front() << "\n";
		return 0;
	}

	std::string scenePath = path;
	FileUtils::path = FileUtils::removeLastFileOnRoute(scenePath);
	auto parser = std::make_shared<SceneParser>(path);
	parser->procesScene();

	auto props = std::make_shared<ImageProperties>(64, 36);
	// The scene build draws random numbers (Grid::createRandom).
	MU::seedRand(12345, ~std::uint64_t(0));
	Scene scene(props, "broadphase");
	scene.build(parser);

	int failures = 0;

	// accel level, on the scene's own visibles
	Bvh bvh;
	for (const auto &v : scene.accel().visibles())
	{
		bvh.addVisible(v);
	}
	bvh.updateCache();
	const auto &info = bvh.buildInfo();
	std::cout << "  bvh: " << info.visibles << " visibles: " << info.treePrims
			  << " in tree, " << info.unbounded << " unbounded, " << info.emitters
			  << " emitters, " << info.droppedLights << " lights never hit\n";

	Rng rng(0xBEEF + file.size());
	const auto boxes = boxesOf(scene.accel().visibles());
	const auto world = expanded(boxes, 1.5);
	auto rays = makeRays(rng, boxes, world, rayCount);

	// camera rays
	{
		Sampler sampler(2);
		const auto cam = scene.camera();
		std::size_t camRays = 0;
		for (int y = 0; y < 36; y++)
		{
			for (int x = 0; x < 64; x++)
			{
				for (int s = 0; s < 2; s++)
				{
					const Ray r = cam->ray(v2s(y / 36.0, x / 64.0), sampler);
					rays.push_back({r.origin(), r.direction(), "camera"});
					camRays++;
					sampler.next();
				}
				sampler.resetIterator();
			}
		}
		std::cout << "  " << rays.size() << " rays (" << camRays << " camera)\n";
	}

	failures += compareAccel(file + " accel level", scene.accel(), bvh, rays);

	// scene level: planes included, the same visibles, only the backend changes
	{
		std::vector<SceneResult> flatResults;
		flatResults.reserve(rays.size() * 2);
		auto t0 = SteadyClock::now();
		for (const auto vision : {VisionType::kOpaque, VisionType::kAll})
		{
			for (const auto &spec : rays)
			{
				const Ray ray(spec.origin, spec.dir, vision);
				Collision col;
				const int r = scene.throwRay(ray, col);
				flatResults.push_back({r == 1, col.position(), col.normal(),
									   col.color(), col.u(), col.v(), col.m_geo});
			}
		}
		const double msFlat = msSince(t0);

		if (!scene.switchAccel("bvh"))
		{
			std::cout << "  FAIL switchAccel(bvh)\n";
			return failures + 1;
		}

		Diff diff;
		std::size_t k = 0;
		t0 = SteadyClock::now();
		double msBvh = 0.0;
		for (const auto vision : {VisionType::kOpaque, VisionType::kAll})
		{
			for (const auto &spec : rays)
			{
				const Ray ray(spec.origin, spec.dir, vision);
				Collision col;
				const auto s0 = SteadyClock::now();
				const int r = scene.throwRay(ray, col);
				msBvh += msSince(s0);

				const auto &f = flatResults[k++];
				Collision ref;
				ref.setPosition(f.position);
				ref.setNormal(f.normal);
				ref.setColor(f.color);
				ref.setU(f.u);
				ref.setV(f.v);
				ref.setValid(f.valid);
				ref.m_geo = f.geo;
				compareHits((file + " scene level").c_str(), spec, vision, ref,
							f.valid ? 1 : 0, col, r, diff);
			}
		}
		printDiffLine(file + " scene level (planes)", diff, msFlat, msBvh,
					  "broadphase", "bvh");
		failures += diff.hardMismatches ? 1 : 0;
	}
	return failures;
}

////////////////////////////////////////////////////////////////////////////
// voxel grid: hierarchical traversal against the reference traversal

struct GridCase
{
	long res;
	int groundLayers;
	std::size_t randomVoxels;
	int blobs;
};

/// Brute force for the voxel grid: every active voxel, and every thin slab
/// over an active voxel, is tested with the same boxes the traversals use.
/// Returns the distance to the nearest hit, or -1.
scalar gridOracle(const Grid &grid, const std::vector<long long> &sortedActive, long res,
				  scalar vs, const Ray &ray)
{
	scalar best = -1.0;
	Collision c;
	BoundingBox box;
	const auto consider = [&](const BoundingBox &b) {
		c = Collision();
		if (b.throwRay(ray, c))
		{
			const scalar d = c.position().distance(ray.origin());
			if (best < 0.0 || d < best)
			{
				best = d;
			}
		}
	};
	const auto isActive = [&](long long idx) {
		return std::binary_search(sortedActive.begin(), sortedActive.end(), idx);
	};
	for (const auto idx : sortedActive)
	{
		const long x = long(idx % res), y = long((idx / res) % res), z = long(idx / (res * res));
		box.set(grid.getVoxelPosition(x, y, z), vs);
		consider(box);
		if (y + 1 < res && !isActive(idx + res))
		{
			const v3s position = grid.getVoxelPosition(x, y + 1, z);
			box.set(position, vs / 2.0);
			box.setMinY(position.y() - vs / 2.0);
			box.setMaxY(position.y() - vs / 4.0);
			consider(box);
		}
	}
	return best;
}

int testGridCase(const GridCase &gc, std::size_t rayCount)
{
	const std::string name = "grid res " + std::to_string(gc.res);
	Rng rng(1000 + gc.res);

	auto t0 = SteadyClock::now();
	Grid grid(v3s::zero, 3.2);
	grid.setResolution(gc.res);
	grid.updateBoundingBox();

	std::vector<long long> active;
	const auto put = [&](long x, long y, long z) {
		if (x < 0 || y < 0 || z < 0 || x >= gc.res || y >= gc.res || z >= gc.res)
		{
			return;
		}
		grid.vxAt(x, y, z).setByte(static_cast<unsigned char>(1 + rng.index(30)));
		active.push_back(grid.index(x, y, z));
	};
	for (int y = 0; y < gc.groundLayers; y++)
	{
		for (long z = 0; z < gc.res; z++)
		{
			for (long x = 0; x < gc.res; x++)
			{
				put(x, y, z);
			}
		}
	}
	for (std::size_t i = 0; i < gc.randomVoxels; i++)
	{
		put(long(rng.index(gc.res)), long(rng.index(gc.res)), long(rng.index(gc.res)));
	}
	for (int b = 0; b < gc.blobs; b++)
	{
		const long cx = long(rng.index(gc.res)), cy = long(rng.index(gc.res)),
				   cz = long(rng.index(gc.res));
		for (long z = 0; z < 4; z++)
			for (long y = 0; y < 4; y++)
				for (long x = 0; x < 4; x++)
					put(cx + x, cy + y, cz + z);
	}
	// voxels on the borders of the grid and on the last row of a cell
	put(0, 0, 0);
	put(gc.res - 1, gc.res - 1, gc.res - 1);
	put(gc.res - 1, 0, gc.res / 2);
	put(gc.res / 2, 7, gc.res / 3); // y = 7: dilation across a cell border
	put(gc.res / 2, 15 % gc.res, gc.res / 4);

	grid.updateCaches();
	const double msBuild = msSince(t0);

	std::cout << "  " << name << ": " << active.size() << " active voxels, "
			  << grid.occupancyCells1() << "^3 L1 cells"
			  << (grid.occupancyCells2() ? ", " + std::to_string(grid.occupancyCells2()) + "^3 L2 cells" : "")
			  << ", built in " << std::fixed << std::setprecision(0) << msBuild
			  << " ms" << std::defaultfloat << std::endl;

	const auto box = *grid.boundingBox();
	const auto half = (box.max() - box.min()) * 0.5;
	const auto center = box.center();
	const auto voxelCenter = [&](long long idx) {
		return grid.getVoxelPosition(idx);
	};

	std::vector<RaySpec> rays;
	rays.reserve(rayCount);
	while (rays.size() < rayCount)
	{
		const auto pick = rng.u();
		v3s o, d;
		const char *kind;
		if (pick < 0.25)
		{
			o = center + v3s(rng.range(-half.x(), half.x()),
							 rng.range(-half.y(), half.y()),
							 rng.range(-half.z(), half.z()));
			d = rng.unit();
			kind = "inside";
		}
		else if (pick < 0.65)
		{
			o = center + v3s(rng.range(-1.6, 1.6) * half.x() * 1.6,
							 rng.range(-1.6, 1.6) * half.y() * 1.6,
							 rng.range(-1.6, 1.6) * half.z() * 1.6);
			const auto target = voxelCenter(active[rng.index(active.size())]) +
								rng.unit() * (box.xLength() / gc.res * 0.45);
			d = target - o;
			kind = "aimed";
		}
		else if (pick < 0.80)
		{
			o = center + rng.unit() * (half.x() * 2.2);
			d = center + v3s(rng.range(-half.x(), half.x()), rng.range(-half.y(), half.y()),
							 rng.range(-half.z(), half.z())) - o;
			kind = "outside";
		}
		else if (pick < 0.90)
		{
			// axis parallel from outside, through the middle of random voxels
			const int axis = int(rng.index(3));
			const auto target = voxelCenter(active[rng.index(active.size())]);
			o = target;
			o[axis] = center[axis] + (rng.u() < 0.5 ? -1.0 : 1.0) * half[axis] * 1.5;
			d = target - o;
			d = v3s(axis == 0 ? d.x() : 0.0, axis == 1 ? d.y() : 0.0,
					axis == 2 ? d.z() : 0.0);
			kind = "axis";
		}
		else
		{
			// from inside an active voxel
			o = voxelCenter(active[rng.index(active.size())]) +
				rng.unit() * (box.xLength() / gc.res * 0.3);
			d = rng.unit();
			kind = "in voxel";
		}
		if (d.length() < 1e-9)
		{
			continue;
		}
		rays.push_back({o, d.unit(), kind});
	}

	Diff diff;
	double msRef = 0.0;
	double msNew = 0.0;
	std::size_t gated = 0;
	std::size_t oracleNewRight = 0, oracleNewWrong = 0, oracleBothWrong = 0;
	std::vector<long long> sortedActive;
	const scalar voxelSize = box.xLength() / scalar(gc.res);
	for (const auto &spec : rays)
	{
		const Ray ray(spec.origin, spec.dir, VisionType::kAll);
		// What the scene does first: the legacy bounding box test leaves the
		// entry point in the collision the grid starts from.
		Collision seed;
		if (!grid.testBoundingBox(ray, seed))
		{
			continue;
		}
		gated++;

		Collision a = seed;
		Collision b = seed;

		auto s0 = SteadyClock::now();
		const int ra = grid.throwRayReference(ray, a);
		msRef += msSince(s0);
		s0 = SteadyClock::now();
		const int rb = grid.throwRay(ray, b);
		msNew += msSince(s0);

		// The color of a hit that starts inside a voxel depends on leftovers of
		// the previous test on the thread (see Grid.cpp), so only compare it
		// for rays from outside the grid.
		if (!compareHits(name.c_str(), spec, VisionType::kAll, a, ra, b, rb, diff,
						 !box.contains(spec.origin)))
		{
			// The reference marches with its own stepping rules and is known to
			// lose a few rays: ask the brute force who is right.
			if (sortedActive.empty())
			{
				sortedActive = active;
				std::sort(sortedActive.begin(), sortedActive.end());
				sortedActive.erase(std::unique(sortedActive.begin(), sortedActive.end()),
								   sortedActive.end());
			}
			const scalar truth = gridOracle(grid, sortedActive, gc.res, voxelSize, ray);
			const auto agrees = [&](int r, const Collision &col) {
				if (truth < 0.0)
					return r == 0;
				return r == 1 && std::abs(col.position().distance(ray.origin()) - truth) < 1e-6;
			};
			const bool newOk = agrees(rb, b);
			const bool refOk = agrees(ra, a);
			if (newOk && !refOk)
				oracleNewRight++;
			else if (refOk && !newOk)
				oracleNewWrong++;
			else
				oracleBothWrong++;
			if (!newOk)
			{
				std::cout << "      oracle: nearest " << truth << ", hierarchy "
						  << (newOk ? "agrees" : "DISAGREES") << ", reference "
						  << (refOk ? "agrees" : "disagrees") << std::endl;
			}
		}
	}
	printDiffLine(name + " ref vs hier", diff, msRef, msNew, "reference",
				  "hierarchy");
	if (diff.hardMismatches)
	{
		std::cout << "    brute force verdict on the " << diff.hardMismatches
				  << " differing rays: hierarchy right and reference wrong "
				  << oracleNewRight << ", reference right and hierarchy wrong "
				  << oracleNewWrong << ", both wrong " << oracleBothWrong << std::endl;
	}
	return (oracleNewWrong + oracleBothWrong) ? 1 : 0;
}

int testGrids(std::size_t rayCount)
{
	std::cout << "== voxel grid traversal, reference against hierarchy\n";
	int failures = 0;
	const GridCase cases[] = {
		{28, 2, 60, 6},
		{100, 2, 400, 10},
		{256, 3, 1500, 20},
		{474, 2, 3000, 30},
	};
	for (const auto &gc : cases)
	{
		failures += testGridCase(gc, rayCount);
	}
	return failures;
}

} // namespace

int testMeshGrid()
{
	std::cout << "== mesh GeoGrid: reference store against CSR + CellMarcher\n";
	int failures = 0;
	const auto saved = GeoGrid::backend();

	auto fail = [&](const char *msg) {
		std::cout << "  FAIL " << msg << "\n";
		failures++;
	};

	{
		BoundingBox bb;
		bb.extend(v3s(0, 0, 0));
		bb.extend(v3s(8, 8, 8));
		const Ray ray(v3s(-1, 4.5, 4.5), v3s(1, 0, 0));
		const RayInv ri(ray);
		CellMarcher march;
		if (!march.init(bb, 8, 8, 8, ri, ray))
		{
			fail("CellMarcher init");
		}
		else
		{
			int steps = 0;
			bool order = true;
			do
			{
				if (march.idx[0] != steps || march.idx[1] != 4 || march.idx[2] != 4)
				{
					order = false;
				}
				steps++;
			} while (march.advance());
			if (steps != 8 || !order)
			{
				fail("CellMarcher +X walk");
			}
			else
			{
				std::cout << "  CellMarcher +X: 8 cells OK\n";
			}
		}
	}

	v3s p1a(0.20, 0.10, 0.10), p1b(0.30, 0.10, 0.15), p1c(0.20, 0.15, 0.12);
	v3s p2a(6.20, 0.50, 0.50), p2b(6.30, 0.50, 0.55), p2c(6.20, 0.55, 0.52);
	v3s n(-1, 0, 0);
	v2s uv(0.5, 0.5);
	TriRef triNear(p1a, p1b, p1c, n, n, n, uv, uv, uv);
	TriRef triFar(p2a, p2b, p2c, n, n, n, uv, uv, uv);
	triNear.computeArea();
	triFar.computeArea();

	auto makeBox = []() {
		auto bb = std::make_shared<BoundingBox>();
		bb->extend(v3s(0, 0, 0));
		bb->extend(v3s(8, 1, 1));
		return bb;
	};

	{
		GeoGrid::setBackend(MeshBackend::kRef);
		GeoGrid grid;
		grid.setBb(makeBox());
		grid.setResolution(8, 4, 4);
		grid.locateAndRegister(triNear, 0);
		grid.locateAndRegister(triFar, 1);
		int a, b, c;
		const auto nearIdx = grid.linearLookupVoxel(p1a, a, b, c);
		const auto farIdx = grid.linearLookupVoxel(p2a, a, b, c);
		if (!grid.hasTriangles(static_cast<long>(nearIdx)) ||
		    !grid.hasTriangles(static_cast<long>(farIdx)))
		{
			fail("ref store missing a single-cell triangle");
		}
		else if (grid.m_members[nearIdx]->index != 0 ||
		         grid.m_members[farIdx]->index != 0)
		{
			fail("ref single-cell index is no longer 0 (bug was the contract)");
		}
		else
		{
			std::cout << "  ref single-cell SearchResult::index stays 0 (cells "
			          << nearIdx << ", " << farIdx << ")\n";
		}
	}

	{
		GeoGrid::setBackend(MeshBackend::kCsr);
		GeoGrid grid;
		grid.setBb(makeBox());
		grid.setResolution(8, 4, 4);
		grid.locateAndRegister(triNear, 0);
		grid.locateAndRegister(triFar, 1);
		grid.finalize();
		int a, b, c;
		const auto nearIdx = grid.linearLookupVoxel(p1a, a, b, c);
		const auto farIdx = grid.linearLookupVoxel(p2a, a, b, c);
		unsigned nCount = 0, fCount = 0;
		const auto *nIds = grid.cellIds(nearIdx, nCount);
		const auto *fIds = grid.cellIds(farIdx, fCount);
		if (nCount != 1 || fCount != 1 || !nIds || !fIds || nIds[0] != 0 ||
		    fIds[0] != 1 || nearIdx == farIdx)
		{
			fail("csr store single-cell ids");
		}
		else
		{
			std::cout << "  csr cells " << nearIdx << " and " << farIdx
			          << " hold distinct triangle ids\n";
		}
	}

	// Dummy corners force an 8^3 AABB so both triangles sit in one cell.
	// Near triangle occupies the ray's first cell but sits in a corner the
	// ray misses. Far triangle sits on the ray. Both are single-cell inserts,
	// so the reference walk can stop when SearchResult::index stays 0.
	const v3s nearA(0.04, 0.480, 0.480), nearB(0.05, 0.480, 0.490),
	    nearC(0.04, 0.490, 0.480);
	const v3s farA(6.01, 0.478, 0.478), farB(6.01, 0.568, 0.490),
	    farC(6.01, 0.500, 0.568);

	auto buildMesh = [&]() {
		auto mesh = std::make_unique<TriangleMesh>();
		mesh->setTransform(Matrix44::identity44);
		mesh->open();
		for (int i = 0; i < 8; i++)
		{
			mesh->addUV(uv);
			mesh->addVertexNormalTransformed(n);
		}
		mesh->addVertexTransformed(v3s(0, 0, 0));
		mesh->addVertexTransformed(v3s(8, 8, 8));
		mesh->addVertexTransformed(nearA);
		mesh->addVertexTransformed(nearB);
		mesh->addVertexTransformed(nearC);
		mesh->addVertexTransformed(farA);
		mesh->addVertexTransformed(farB);
		mesh->addVertexTransformed(farC);
		mesh->addTriangle(2, 3, 4).computeArea();
		mesh->addTriangle(5, 6, 7).computeArea();
		mesh->close();
		return mesh;
	};

	const Ray probe(v3s(-1.0, 0.50, 0.50), v3s(1, 0, 0));

	GeoGrid::setBackend(MeshBackend::kRef);
	auto meshRef = buildMesh();
	Collision colRef;
	meshRef->boundingBox()->throwRay(probe, colRef);
	const int hitRef = meshRef->throwRay(probe, colRef);

	GeoGrid::setBackend(MeshBackend::kCsr);
	auto meshCsr = buildMesh();
	Collision colCsr;
	meshCsr->boundingBox()->throwRay(probe, colCsr);
	const int hitCsr = meshCsr->throwRay(probe, colCsr);

	if (hitCsr != 1)
	{
		fail("csr missed the far single-cell triangle");
	}
	else
	{
		std::cout << "  csr throwRay hits the far triangle after a near miss\n";
	}
	if (hitRef == 1)
	{
		std::cout << "  ref throwRay also hit (index-0 early-stop did not fire)\n";
	}
	else
	{
		std::cout << "  ref throwRay missed: index-0 early-stop after the near cell\n";
	}

	GeoGrid::setBackend(saved);
	std::cout << "  mesh GeoGrid: " << (failures ? "FAILED" : "OK") << "\n";
	return failures;
}

/// Linear closest-hit over every triangle. Same front-face filter as GeoGrid.
int bruteMeshClosest(TriangleMesh &mesh, const Ray &ray, Collision &col)
{
	const v3s origin = ray.origin();
	bool have = false;
	scalar best = std::numeric_limits<scalar>::max();
	Collision bestCol;
	for (auto &t : mesh.triangles())
	{
		Collision temp;
		if (!t.throwRay(ray, temp))
		{
			continue;
		}
		if (ray.direction().follows((temp.position() - origin).unit()))
		{
			continue;
		}
		const auto d = temp.position().distance(origin);
		if (d < best)
		{
			best = d;
			bestCol = temp;
			have = true;
		}
	}
	if (have)
	{
		col = bestCol;
		col.setValid(true);
		return 1;
	}
	col.setValid(false);
	return 0;
}

std::unique_ptr<TriangleMesh> makeTestMesh(const std::vector<v3s> &verts,
										   const std::vector<std::array<unsigned long, 3>> &faces,
										   const v3s &n)
{
	auto mesh = std::make_unique<TriangleMesh>();
	mesh->setTransform(Matrix44::identity44);
	mesh->open();
	const v2s uv(0.5, 0.5);
	for (std::size_t i = 0; i < verts.size(); i++)
	{
		mesh->addUV(uv);
		mesh->addVertexNormalTransformed(n);
		mesh->addVertexTransformed(verts[i]);
	}
	for (const auto &f : faces)
	{
		mesh->addTriangle(f[0], f[1], f[2]).computeArea();
	}
	mesh->close();
	return mesh;
}

int testMeshBvh()
{
	std::cout << "== mesh BVH: closest-hit oracle and CSR (expected cell diffs)\n";
	int failures = 0;
	const auto saved = GeoGrid::backend();
	auto fail = [&](const char *msg) {
		std::cout << "  FAIL " << msg << "\n";
		failures++;
	};

	const v3s n(-1, 0, 0);
	const v2s uv(0.5, 0.5);

	// Two-triangle near-cell miss / far hit from testMeshGrid. Both must hit.
	{
		const v3s nearA(0.04, 0.480, 0.480), nearB(0.05, 0.480, 0.490),
			nearC(0.04, 0.490, 0.480);
		const v3s farA(6.01, 0.478, 0.478), farB(6.01, 0.568, 0.490),
			farC(6.01, 0.500, 0.568);
		const std::vector<v3s> verts = {v3s(0, 0, 0), v3s(8, 8, 8), nearA, nearB,
										nearC,		   farA,		 farB, farC};
		const std::vector<std::array<unsigned long, 3>> faces = {{{2, 3, 4}},
																 {{5, 6, 7}}};
		const Ray probe(v3s(-1.0, 0.50, 0.50), v3s(1, 0, 0));

		GeoGrid::setBackend(MeshBackend::kCsr);
		auto meshCsr = makeTestMesh(verts, faces, n);
		Collision colCsr;
		meshCsr->boundingBox()->throwRay(probe, colCsr);
		const int hitCsr = meshCsr->throwRay(probe, colCsr);

		GeoGrid::setBackend(MeshBackend::kBvh);
		auto meshBvh = makeTestMesh(verts, faces, n);
		Collision colBvh;
		meshBvh->boundingBox()->throwRay(probe, colBvh);
		const int hitBvh = meshBvh->throwRay(probe, colBvh);

		if (hitCsr != 1)
		{
			fail("csr missed the far triangle (probe)");
		}
		if (hitBvh != 1)
		{
			fail("bvh missed a far triangle after a near-cell miss");
		}
		else if (hitCsr == 1 && !closeVec(colCsr.position(), colBvh.position(), 1e-6))
		{
			fail("bvh and csr disagree on the single far hit");
		}
		else if (hitBvh == 1)
		{
			std::cout << "  bvh hits the far triangle after a near-cell miss\n";
		}
		if (meshBvh->meshBvh().empty())
		{
			fail("bvh tree is empty after close()");
		}
	}

	// Large slanted triangle occupies early cells but hits far; a closer
	// triangle sits later. CSR returns the first occupied cell that hits
	// (the slanted one). BVH closest-hit returns the nearer wall. Reported,
	// not a -at failure.
	{
		const std::vector<v3s> verts = {
			v3s(0, 0, 0),		  v3s(8, 8, 8),		   v3s(0.05, 0.02, 0.02),
			v3s(7.0, 2.0, -0.5),  v3s(7.0, -0.5, 2.0), v3s(2.0, 0.3, 0.3),
			v3s(2.0, 0.7, 0.3),   v3s(2.0, 0.5, 0.7)};
		const std::vector<std::array<unsigned long, 3>> faces = {{{2, 3, 4}},
																 {{5, 6, 7}}};
		const Ray probe(v3s(-1.0, 0.50, 0.50), v3s(1, 0, 0));

		GeoGrid::setBackend(MeshBackend::kCsr);
		auto meshCsr = makeTestMesh(verts, faces, n);
		Collision colCsr;
		meshCsr->boundingBox()->throwRay(probe, colCsr);
		const int hitCsr = meshCsr->throwRay(probe, colCsr);

		GeoGrid::setBackend(MeshBackend::kBvh);
		auto meshBvh = makeTestMesh(verts, faces, n);
		Collision colBvh;
		meshBvh->boundingBox()->throwRay(probe, colBvh);
		const int hitBvh = meshBvh->throwRay(probe, colBvh);

		Collision colBrute;
		const int hitBrute = bruteMeshClosest(*meshBvh, probe, colBrute);

		if (hitBvh != 1 || hitBrute != 1)
		{
			fail("bvh/brute missed the closer wall on the first-cell case");
		}
		else if (!closeVec(colBvh.position(), colBrute.position(), 1e-6))
		{
			fail("bvh disagrees with closest-hit oracle on the first-cell case");
		}
		else if (hitCsr == 1 && !closeVec(colCsr.position(), colBvh.position(), 1e-4))
		{
			const auto dCsr = colCsr.position().distance(probe.origin());
			const auto dBvh = colBvh.position().distance(probe.origin());
			if (dBvh > dCsr + 1e-3)
			{
				fail("bvh hit is farther than csr (not closest-hit)");
			}
			else
			{
				std::cout << "  expected closest-vs-first-cell: csr dist " << dCsr
						  << " bvh dist " << dBvh << "\n";
			}
		}
		else
		{
			std::cout << "  first-cell vs closest: csr and bvh agreed on this ray\n";
		}
	}

	// Brute-force closest-hit vs BVH on a small grid of triangles.
	{
		std::vector<v3s> verts = {v3s(-0.5, -0.5, -0.5), v3s(4.5, 4.5, 4.5)};
		std::vector<std::array<unsigned long, 3>> faces;
		const int nx = 4, ny = 4;
		for (int j = 0; j <= ny; j++)
		{
			for (int i = 0; i <= nx; i++)
			{
				verts.emplace_back(scalar(i), scalar(j) * 0.1, scalar(j));
			}
		}
		const unsigned long base = 2;
		for (int j = 0; j < ny; j++)
		{
			for (int i = 0; i < nx; i++)
			{
				const unsigned long a = base + unsigned(j * (nx + 1) + i);
				const unsigned long b = a + 1;
				const unsigned long c = a + unsigned(nx + 1);
				const unsigned long d = c + 1;
				faces.push_back({{a, c, b}});
				faces.push_back({{b, c, d}});
			}
		}

		GeoGrid::setBackend(MeshBackend::kBvh);
		auto mesh = makeTestMesh(verts, faces, n);
		if (mesh->meshBvh().buildInfo().triangles != faces.size())
		{
			fail("bvh triangle count");
		}

		Rng rng(0xB4B4ull);
		std::size_t casts = 0, hits = 0, mismatch = 0, nanBad = 0;
		const std::size_t nRays = 400;
		for (std::size_t i = 0; i < nRays; i++)
		{
			v3s o, d;
			if (i < 8)
			{
				o = v3s(-1.0, 0.2 + 0.1 * scalar(i), 1.5);
				d = v3s(1, 0, 0);
			}
			else
			{
				o = v3s(rng.range(-2, 6), rng.range(-2, 6), rng.range(-2, 6));
				d = rng.unit();
			}
			const Ray ray(o, d);
			Collision ca, cb;
			const int ra = bruteMeshClosest(*mesh, ray, ca);
			const int rb = mesh->throwRay(ray, cb);
			casts++;
			if (ra)
			{
				hits++;
			}
			if (rb && Comps(cb.position()).anyNan())
			{
				nanBad++;
				continue;
			}
			if (ra && !rb)
			{
				mismatch++;
				if (mismatch <= 5)
				{
					std::cout << "    MISMATCH brute hit / bvh miss  ray " << o
							  << " -> " << d << "\n";
				}
			}
			else if (!ra && rb)
			{
				mismatch++;
				if (mismatch <= 5)
				{
					std::cout << "    MISMATCH brute miss / bvh hit  ray " << o
							  << " -> " << d << "\n";
				}
			}
			else if (ra && rb && !closeVec(ca.position(), cb.position(), 1e-6))
			{
				const auto da = ca.position().distance(o);
				const auto db = cb.position().distance(o);
				if (std::abs(da - db) > 1e-3)
				{
					mismatch++;
					if (mismatch <= 5)
					{
						std::cout << "    MISMATCH huge position error " << da << " vs "
								  << db << "\n";
					}
				}
			}
		}
		if (nanBad)
		{
			fail("bvh returned a NaN position");
		}
		if (mismatch)
		{
			fail("bvh disagrees with the closest-hit oracle");
		}
		std::cout << "  brute vs bvh: " << casts << " casts, " << hits
				  << " hits, mismatches " << mismatch << "\n";
	}

	// CSR vs BVH on the same grid: report closest-vs-first-cell, fail only
	// when BVH misses a CSR hit or lands much farther.
	{
		std::vector<v3s> verts = {v3s(-0.5, -0.5, -0.5), v3s(4.5, 4.5, 4.5)};
		std::vector<std::array<unsigned long, 3>> faces;
		const int nx = 3, ny = 3;
		for (int j = 0; j <= ny; j++)
		{
			for (int i = 0; i <= nx; i++)
			{
				verts.emplace_back(scalar(i), scalar(0.05 * j), scalar(j));
			}
		}
		const unsigned long base = 2;
		for (int j = 0; j < ny; j++)
		{
			for (int i = 0; i < nx; i++)
			{
				const unsigned long a = base + unsigned(j * (nx + 1) + i);
				const unsigned long b = a + 1;
				const unsigned long c = a + unsigned(nx + 1);
				const unsigned long d = c + 1;
				faces.push_back({{a, c, b}});
				faces.push_back({{b, c, d}});
			}
		}

		GeoGrid::setBackend(MeshBackend::kCsr);
		auto meshCsr = makeTestMesh(verts, faces, n);
		GeoGrid::setBackend(MeshBackend::kBvh);
		auto meshBvh = makeTestMesh(verts, faces, n);

		Rng rng(0xC5C5ull);
		std::size_t casts = 0, hits = 0, expectedDiff = 0, badMiss = 0,
					farther = 0, nanBad = 0;
		for (std::size_t i = 0; i < 300; i++)
		{
			const v3s o(rng.range(-2, 6), rng.range(-2, 6), rng.range(-2, 6));
			const v3s d = rng.unit();
			const Ray ray(o, d);
			Collision ca, cb;
			meshCsr->boundingBox()->throwRay(ray, ca);
			meshBvh->boundingBox()->throwRay(ray, cb);
			const int ra = meshCsr->throwRay(ray, ca);
			const int rb = meshBvh->throwRay(ray, cb);
			casts++;
			if (ra)
			{
				hits++;
			}
			if (rb && Comps(cb.position()).anyNan())
			{
				nanBad++;
				continue;
			}
			if (ra && !rb)
			{
				badMiss++;
			}
			else if (ra && rb && !closeVec(ca.position(), cb.position(), 1e-4))
			{
				const auto da = ca.position().distance(o);
				const auto db = cb.position().distance(o);
				if (db > da + 1e-3)
				{
					farther++;
				}
				else
				{
					expectedDiff++;
				}
			}
		}
		if (nanBad)
		{
			fail("bvh NaN vs csr");
		}
		if (badMiss)
		{
			fail("bvh missed a hit csr found");
		}
		if (farther)
		{
			fail("bvh hit farther than csr");
		}
		std::cout << "  csr vs bvh: " << casts << " casts, " << hits
				  << " csr hits, expected closest-vs-cell diffs " << expectedDiff
				  << ", bad misses " << badMiss << ", farther " << farther << "\n";
	}

	// Same file stamp and placement must reuse the tree. A rewrite would
	// move the cache file's timestamp.
	{
		GeoGrid::setBackend(MeshBackend::kBvh);
		const auto dir = std::filesystem::temp_directory_path();
		const auto ply = dir / "vxr-bvh-cache-test.ply";
		{
			std::ofstream touch(ply, std::ios::binary | std::ios::trunc);
			touch << "cache-stamp";
		}

		auto makeOne = [&]() {
			auto mesh = std::make_unique<TriangleMesh>();
			mesh->setTransform(Matrix44::identity44);
			mesh->setConstructionPath(ply.string());
			mesh->open();
			for (int i = 0; i < 3; i++)
			{
				mesh->addUV(uv);
				mesh->addVertexNormalTransformed(n);
			}
			mesh->addVertexTransformed(v3s(6.01, 0.478, 0.478));
			mesh->addVertexTransformed(v3s(6.01, 0.568, 0.490));
			mesh->addVertexTransformed(v3s(6.01, 0.500, 0.568));
			mesh->addTriangle(0, 1, 2).computeArea();
			mesh->close();
			return mesh;
		};

		auto first = makeOne();
		const Ray probe(v3s(-1.0, 0.50, 0.50), v3s(1, 0, 0));
		Collision hit;
		if (!first->throwRay(probe, hit))
		{
			fail("cache source mesh missed its triangle");
		}

		std::filesystem::path cache;
		std::error_code ec;
		for (const auto &entry : std::filesystem::directory_iterator(dir, ec))
		{
			const auto name = entry.path().filename().string();
			if (name.rfind("vxr-bvh-cache-test.ply.", 0) == 0 &&
				entry.path().extension() == ".mbvh")
			{
				cache = entry.path();
			}
		}
		if (cache.empty())
		{
			fail("bvh cache was not written");
		}
		else
		{
			const auto written = std::filesystem::last_write_time(cache, ec);
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
			auto second = makeOne();
			Collision again;
			if (!second->throwRay(probe, again))
			{
				fail("mesh loaded from bvh cache missed");
			}
			const auto after = std::filesystem::last_write_time(cache, ec);
			if (after != written)
			{
				fail("bvh cache was rebuilt instead of read");
			}
			std::filesystem::remove(cache, ec);
		}
		std::filesystem::remove(ply, ec);
	}

	GeoGrid::setBackend(saved);
	std::cout << "  mesh BVH: " << (failures ? "FAILED" : "OK") << "\n";
	return failures;
}

int AccelUnitTest::run()
{
	std::cout << "\nInit:: acceleration structure tests" << std::endl;
	const std::size_t rayCount = std::size_t(std::max(100, envInt("VXR_AT_RAYS", 20000)));
	int failures = 0;

	const auto start = SteadyClock::now();
	failures += testSlab(rayCount);
	failures += testSynthetic(rayCount);
	failures += testDegenerateTrees(rayCount);
	failures += testGrids(rayCount);
	failures += testMeshGrid();
	failures += testMeshBvh();

	if (envInt("VXR_AT_SKIP_SCENES", 0) == 0)
	{
		for (const auto *file : {"ab_grid.vx", "ab_prims.vx", "bg_statue.vx",
								 "juan_kloofendal.vx"})
		{
			failures += testSceneFile(file, rayCount);
		}
	}

	std::cout << "Acceleration structure tests: "
			  << (failures ? "FAILED (" + std::to_string(failures) + " failing checks)"
							 : std::string("PASSED"))
			  << " in " << std::fixed << std::setprecision(1)
			  << msSince(start) / 1000.0 << " s" << std::endl;
	return failures;
}
