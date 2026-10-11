#include <atomic>
#include <cassert>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <limits>
#include <mutex>

#include "Cylinder.h"
#include "Grid.h"
#include "MathUtils.h"
#include "PerfStats.h"

#include "CirclesMap.h"

std::mutex gridMutex;

using namespace vxCore;

#define DBL_EPSILON 1e-12
#define DRAWBBOX 0

namespace
{
// Scratch objects of the voxel traversal, one per thread. Both traversals use
// the same ones: the legacy box test leaves uv and color behind when the ray
// starts inside a voxel, and the color of that hit depends on them.
thread_local VoxelInfo tl_voxel;
thread_local Collision tl_c;
thread_local BoundingBox tl_box;
thread_local CirclesMap tl_cm;

// -1 means "not decided yet, ask the environment".
std::atomic<int> g_traversal{-1};

Grid::Traversal currentTraversal()
{
	int mode = g_traversal.load(std::memory_order_relaxed);
	if (mode < 0)
	{
		const char *env = std::getenv("VXR_GRID");
		mode = (env && std::strcmp(env, "ref") == 0)
				   ? static_cast<int>(Grid::Traversal::kReference)
				   : static_cast<int>(Grid::Traversal::kAuto);
		g_traversal.store(mode, std::memory_order_relaxed);
	}
	return static_cast<Grid::Traversal>(mode);
}
} // namespace

void Grid::setTraversal(Traversal mode)
{
	g_traversal.store(static_cast<int>(mode), std::memory_order_relaxed);
}

Grid::Traversal Grid::traversal() { return currentTraversal(); }

Geometry *Grid::getVisibleVoxelGeo() const { return m_visibleVoxelGeo; }

void Grid::setVisibleVoxelGeo(Geometry *visibleVoxelGeo)
{
	m_visibleVoxelGeo = visibleVoxelGeo;
}

Grid::Grid()
{
	m_size = 1.0;

	createGridData(5);
	initialize();
	updateBB();
}

Grid::Grid(const v3s &position, scalar size) : m_position(position)
{
	setSize(size);
	createGridData(5);

	initialize();
	updateBB();
}

Grid::Grid(scalar x, scalar y, scalar z, scalar size)
{
	m_position.set(x, y, z);
	setSize(size);

	createGridData(5);

	initialize();
	updateBB();
}

void Grid::updateBB() { m_bb->set(m_position, m_size); }

void Grid::createGridData(const long resolution)
{
	m_occValid = false;
	m_resolution = resolution;
	m_c_resXres = resolution * resolution;
	m_c_resXresXres = m_c_resXres * resolution;
	m_data.resize(m_c_resXresXres);
	m_c_resDivTres = m_c_midSize / (scalar)m_resolution;
	setBoxSize();
}

void Grid::setResolution(long resolution)
{
	if (resolution != m_resolution)
	{
		createGridData(resolution);
		updateCaches();
	}
}

void Grid::setSize(const scalar size)
{
	m_size = size;
	m_c_midSize = size / scalar(2.0);
	setBoxSize();
}

long Grid::size() const { return m_size; }

void Grid::setPosition(const v3s &position)
{
	m_position = position;
	updateBB();
}

v3s Grid::position() const { return m_position; }

long Grid::resolution() const { return m_resolution; }

void Grid::setBoxSize()
{
	m_c_voxelSize = m_size / scalar(m_resolution);
	m_c_halfVoxelSize = m_c_voxelSize / scalar(2.0);
}

void Grid::createDiagonals(unsigned char colorIndex)
{
	long rMO = m_resolution - 1;
	for (long i = 0; i < m_resolution; i++)
	{
		vxAt(i, i, i).setByte(colorIndex);
		vxAt(rMO - i, rMO - i, i).setByte(colorIndex);
		vxAt(i, rMO - i, rMO - i).setByte(colorIndex);
		vxAt(rMO - i, i, rMO - i).setByte(colorIndex);
	}
}

void Grid::dumpFileInMemory(const std::string &fileName)
{
	m_occValid = false;
	std::ifstream file(fileName, std::ios::binary);
	file.seekg(0, std::ios::end);
	std::streamsize size = file.tellg();
	file.seekg(0, std::ios::beg);

	if (size > (std::streamsize)m_data.size())
	{
		std::cout << "Error, trying to dump long file to memory " << fileName
				  << std::endl;
		return;
	}

	std::vector<char> buffer(size);
	if (file.read((char *)&(m_data[0]), size))
	{
		/* worked! */
	}

	file.close();
}

void Grid::dumpNumericTypeInMemory()
{
	m_occValid = false;
	using numType = int;
	auto p = (void *)&m_data[0];
	new (p) numType(std::numeric_limits<numType>::max());
}

///////////////////////////////////
///////////////////////////////////

unsigned int Grid::neighboursAlive(long long idx)
{
	long retx;
	long rety;
	long retz;

	getComponentsOfIndex(idx, retx, rety, retz);

	unsigned int ret{0u};

	for (auto x = retx - 1l; x < retx + 2; ++x)
		for (auto y = rety - 1l; y < rety + 2; ++y)
			for (auto z = retz - 1l; z < retz + 2; ++z)
			{
				if (retx == x && rety == y && retz == z)
				{
					continue;
				}

				if (x >= (long)m_resolution || y >= (long)m_resolution ||
					z >= (long)m_resolution || x < 0 || y < 0 || z < 0)
				{
					continue;
				}

				auto ind = index(x, y, z);
				ret += vxAt(ind).active() ? 1u : 0u;
			}

	return ret;
}

long long Grid::legolizeColors()
{
	m_occValid = false;
	std::vector<Color> colors;

	int k = 0;
	int i = 0;

	int r = 124;

	for (auto &d : m_data)
	{
		if (d.active())
		{
			d.setByte(k % 19);
			if (i % r == 0)
			{
				k++;
			}
			i++;
		}
	}

	return 0;
}

long long Grid::playGameOfLife()
{
	m_occValid = false;
	long long idx{0ull};
	long long newLife{0ul};

	for (auto &d : m_data)
	{
		auto pop = neighboursAlive(idx);

		if (d.activeBit(7))
		{
			if (pop == 1)
			{
				markCellAsDead(d);
			}

			if (pop == 3)
			{
				markCellAsDead(d);
			}

			if (pop == 5)
			{
				markCellAsDead(d);
			}

			if (pop == 7)
			{
				markCellAsDead(d);
			}
		}
		else
		{
			if (pop == 6)
			{
				markCellForGenesis(d);
				newLife++;
			}
		}
		idx++;
	}
	return newLife;
}

void Grid::markCellAsDead(VoxelData &cell)
{
	cell.c = 0b0000'0000;
	cell.activateBit(4);
}

void Grid::markCellForGenesis(VoxelData &cell)
{
	cell.c = 0b0000'0000;
	cell.activateBit(6);
}

long long Grid::killTheDead()
{
	m_occValid = false;
	long long deadCells{0ull};
	for (auto &d : m_data)
	{
		unsigned char tm = d.c;
		if (d.activeBit(4))
		{
			d.deactivate();
			deadCells++;
			continue;
		}

		if (d.activeBit(6))
		{
			d.activate();
			continue;
		}

		if (!d.activeBit(7))
		{
			tm >>= 1;
		}

		d.c = tm;
	}

	return deadCells;
}

void Grid::createCorners(unsigned char colorIndex)
{
	auto rMO = m_resolution - 1;
	vxAt(rMO, rMO, rMO).setByte(colorIndex);
	vxAt(rMO, rMO, 0).setByte(colorIndex);
	vxAt(rMO, 0, rMO).setByte(colorIndex);
	vxAt(rMO, 0, 0).setByte(colorIndex);
	vxAt(0, rMO, rMO).setByte(colorIndex);
	vxAt(0, rMO, 0).setByte(colorIndex);
	vxAt(0, 0, rMO).setByte(colorIndex);
	vxAt(0, 0, 0).setByte(colorIndex);
}

void Grid::createGround(scalar y_threshold, unsigned char colorIndex)
{
	m_occValid = false;
	int i = 0;
	for (auto &it : m_data)
	{
		if (getVoxelPosition(i).y() < y_threshold)
		{
			it.setByte(colorIndex);
		}

		i++;
	}
}

void Grid::createRoof(long offset, unsigned char colorIndex)
{
	if (offset >= m_resolution)
	{
		return;
	}

	for (long i = 0; i < m_resolution; i++)
	{
		for (long j = 0; j < m_resolution; j++)
		{
			vxAt(i, (m_resolution - offset - 1), j).setByte(colorIndex);
		}
	}
}

void Grid::createEdges(unsigned char colorIndex)
{
	long resminusone = m_resolution - 1;
	for (long i = 0; i < m_resolution; i++)
	{
		vxAt(i, 0, 0).setByte(colorIndex);
		vxAt(i, resminusone, resminusone).setByte(colorIndex);
		vxAt(i, 0, resminusone).setByte(colorIndex);
		vxAt(i, resminusone, 0).setByte(colorIndex);

		vxAt(0, i, 0).setByte(colorIndex);
		vxAt(resminusone, i, resminusone).setByte(colorIndex);
		vxAt(0, i, resminusone).setByte(colorIndex);
		vxAt(resminusone, i, 0).setByte(colorIndex);

		vxAt(0, 0, i).setByte(colorIndex);
		vxAt(resminusone, resminusone, i).setByte(colorIndex);
		vxAt(resminusone, 0, i).setByte(colorIndex);
		vxAt(0, resminusone, i).setByte(colorIndex);
	}
}

void Grid::fill(unsigned char colorIndex)
{
	m_occValid = false;
	for (long i = 0; i < m_data.size(); i++)
	{
		vxAt(i).setByte(colorIndex);
	}
}

void Grid::createSphere(scalar x, scalar y, scalar z, const scalar radio,
						unsigned char colorIndex)
{
	createSphere(v3s{(scalar)x, (scalar)y, (scalar)z}, radio, colorIndex);
}

bool Grid::getRandomBoolean(scalar ratio)
{
	scalar num = pow((rand() / (scalar)RAND_MAX), 1.0 / ratio);
	return num > .5;
}

void Grid::createRandom(scalar ratio, scalar y_threshold)
{
	m_occValid = false;
	int i = 0;
	for (auto &it : m_data)
	{
		if (MU::getBoolRand(ratio) && getVoxelPosition(i).y() < y_threshold)
		{
			it.setByte((unsigned int)MU::getRand(25.0));
		}

		i++;
	}
}
#include <cstdint>

void Grid::addGeometry(const vxTriangleMeshHandle &geo)
{
	m_occValid = false;
	auto pi = reinterpret_cast<std::uintptr_t>(geo.get());
	pi %= 15;
	for (auto &&tri : geo->triangles())
	{
		{
			const auto &p = tri.p1;
			if (isInside(p))
			{
				auto &&v = vxAtPosition(p);
				v.setByte(v.byte() + pi);
			}
		}
		{
			const auto &p = tri.p2;
			if (isInside(p))
			{
				auto &&v = vxAtPosition(p);
				v.setByte(v.byte() + pi);
			}
		}
		{
			const auto &p = tri.p3;
			if (isInside(p))
			{
				auto &&v = vxAtPosition(p);
				v.setByte(v.byte() + pi);
			}
		}

		const unsigned int interp = 4u;
		for (unsigned int i = 0; i < interp; i++)
		{
			auto a = MU::lerp(tri.p1, tri.p2, (i + 1) / (scalar)interp);
			const auto &p = a;
			if (isInside(p))
			{
				auto &&v = vxAtPosition(p);
				v.setByte(v.byte() + pi);
			}
		}

		for (unsigned int i = 0; i < interp; i++)
		{
			auto a = MU::lerp(tri.p1, tri.p3, (i + 1) / (scalar)interp);
			const auto &p = a;
			if (isInside(p))
			{
				auto &&v = vxAtPosition(p);
				v.setByte(v.byte() + pi);
			}
		}

		for (unsigned int i = 0; i < interp; i++)
		{
			auto a = MU::lerp(tri.p2, tri.p3, (i + 1) / (scalar)interp);
			const auto &p = a;
			if (isInside(p))
			{
				auto &&v = vxAtPosition(p);
				v.setByte(v.byte() + pi);
			}
		}
	}
}

long long Grid::index(const long x, const long y, const long z) const
{
	return (x + (y * m_resolution) + (z * m_c_resXres));
}

void Grid::initialize(bool value)
{
	m_occValid = false;
	if (value)
	{
		for (long i = 0; i < m_data.size(); i++)
		{
			vxAt(i).activate();
		}
	}
	else
	{
		for (long i = 0; i < m_data.size(); i++)
		{
			vxAt(i).deactivate();
		}
	}
}

long Grid::numActiveVoxels()
{
	long av{0};

	// Reads m_data directly: the non const vxAt() drops the occupancy grids.
	for (const auto &d : m_data)
	{
		if (d.active())
			av++;
	}

	return av;
}

long Grid::getNumberOfVoxels() const { return m_c_resXresXres; }

bool Grid::active(const long x, const long y, const long z) const
{
	return getElement(x, y, z);
}

bool Grid::active(const v3s &pos) const
{
	if (isInside(pos))
	{
		const auto &&fPos = pos - (m_position - m_c_midSize);
		return getElement((long)floor(fPos.x()), (long)floor(fPos.y()),
						  (long)floor(fPos.z()));
	}

	return false;
}

bool Grid::activeInRange(const v3s &pos) const
{
	const auto &&fPos = pos - (m_position - m_c_midSize);
	return getElement((long)floor(fPos.x()), (long)floor(fPos.y()),
					  (long)floor(fPos.z()));
}

bool Grid::active(long long idx) const
{
	if (idx < m_c_resXresXres)
	{
		return vxAt(idx).active();
	}

	return false;
}

void Grid::activate(const long x, const long y, const long z)
{
	setElement(x, y, z, true);
}

bool Grid::activate(const v3s &pos)
{
	if (!isInside(pos))
	{
		return false;
	}

	const auto &&offsetPos = pos - (m_position - m_c_midSize);
	setElement((long)floor(offsetPos.x()), (long)floor(offsetPos.y()),
			   (long)floor(offsetPos.z()), true);

	return true;
}

void Grid::deactivate(const long x, const long y, const long z)
{
	setElement(x, y, z, false);
}

bool Grid::getElement(const long x, const long y, const long z) const
{
	return active(index(x, y, z));
}

void Grid::setElement(const long x, const long y, const long z, bool value)
{
	vxAt(x, y, z).activate(value);
}

unsigned char Grid::elementColorIndex(const long x, const long y,
									  const long z) const
{
	return (index(x, y, z)) % 8;
}

void Grid::setElementColorIndex(const long x, const long y, const long z,
								const unsigned char c)
{
	vxAt(x, y, z).setByte(c);
}

void Grid::setElement(long long idx, bool value) { vxAt(idx).activate(value); }

void Grid::getComponentsOfIndex(const long idx, long &retx, long &rety,
								long &retz) const
{
	retz = idx / m_c_resXres;
	rety = (idx % m_c_resXres) / m_resolution;
	retx = idx % m_resolution;
}

long long Grid::indexAtPosition(const v3s &pos) const
{
	auto p = ((pos + m_c_midSize) / m_c_voxelSize).floorVector();

	return index((long)p.x(), (long)p.y(), (long)p.z());
}

v3s Grid::getVoxelPosition(const long iX, const long iY, const long iZ) const
{
	return v3s(m_bb->minX() + (iX * m_c_voxelSize),
			   m_bb->minY() + (iY * m_c_voxelSize),
			   m_bb->minZ() + (iZ * m_c_voxelSize)) +
		   (m_c_halfVoxelSize);
}

v3s Grid::getVoxelPosition(long long idx) const
{
	long retx;
	long rety;
	long retz;

	getComponentsOfIndex(idx, retx, rety, retz);

	return getVoxelPosition(retx, rety, retz);
}

VoxelData &Grid::vxAtPosition(const v3s &position)
{
	auto idx = indexAtPosition(position);
	return vxAt(idx >= m_c_resXresXres ? 0 : idx);
}

VoxelData Grid::vxAtPosition(const v3s &position) const
{
	auto idx = indexAtPosition(position);
	return vxAt(idx >= m_c_resXresXres ? 0 : idx);
}

VoxelData &Grid::vxAt(const long iX, const long iY, const long iZ)
{
	return vxAt(index(iX, iY, iZ));
}

VoxelData Grid::vxAt(const long iX, const long iY, const long iZ) const
{
	return vxAt(index(iX, iY, iZ));
}

VoxelData &Grid::vxAt(const long long idx)
{
	// The caller may write through the reference.
	m_occValid = false;
	return m_data[idx];
}

VoxelData Grid::vxAt(const long long idx) const { return m_data[idx]; }

bool Grid::bitInBufferData(const long long idx) const
{
	auto byte = m_data[idx / 8];
	unsigned char ch = byte.c;
	const unsigned char sh = (7 - (idx % 8));
	// Doing this so we loose any left information
	// and bit gets filtered.
	ch <<= sh;
	ch >>= sh;
	return (bool)ch;
}

void Grid::createSphere(const v3s &center, const scalar radio,
						unsigned char colorIndex)
{
	long x, y, z;

	for (x = 0; x < m_resolution; x++)
	{
		for (y = 0; y < m_resolution; y++)
		{
			for (z = 0; z < m_resolution; z++)
			{
				auto &voxel = vxAt(x, y, z);
				if (center.distance(getVoxelPosition(x, y, z)) < radio)
				{
					voxel.setByte(colorIndex);
				}
			}
		}
	}
}

void Grid::createBox(const BoundingBox &bb, unsigned char colorIndex)
{
	long x, y, z;

	for (x = 0; x < m_resolution; x++)
	{
		for (y = 0; y < m_resolution; y++)
		{
			for (z = 0; z < m_resolution; z++)
			{
				auto &voxel = vxAt(x, y, z);
				auto pos = getVoxelPosition(x, y, z);
				if (bb.contains(pos))
				{
					voxel.setByte(colorIndex);
				}
			}
		}
	}
}

bool Grid::isInside(const v3s &pnt, scalar tol) const
{
	return m_bb->contains(pnt, tol);
}

bool Grid::isInside(const v3s &pnt) const { return m_bb->contains(pnt); }

VoxelInfo Grid::neighbourVoxel(const VoxelInfo &orig,
							   const std::array<int, 3> &moves) const
{
	VoxelInfo inf = orig;

	inf.x += moves[0];
	inf.y += moves[1];
	inf.z += moves[2];

	inf.index = (orig.index + moves[0]) + (moves[1] * m_resolution) +
				(moves[2] * m_c_resXresXres);

	inf.data = vxAt(inf.index);

	return inf;
}

bool Grid::throwRay(const Ray &ray) const
{
#if DRAWBBOX
	return m_bb->throwRay(ray);
#else
	Collision col;
	return throwRay(ray, col) == 1;
#endif
}

int Grid::throwRayReference(const Ray &ray, Collision &col) const
{
	///////////// THis is const and could be stored per thread and ray ////

	PERF_COUNT(kGridCast);
	auto &voxel = tl_voxel;
	voxel.size = m_c_voxelSize;

	auto &c = tl_c;
	auto &box = tl_box; // geometries for voxels
	const auto &d = ray.direction();
	const auto velX = d.xPositive() ? 1 : -1;
	const auto velY = d.yPositive() ? 1 : -1;
	const auto velZ = d.zPositive() ? 1 : -1;
	auto &cm = tl_cm;
	///////////////////////////////////////////////////////////////////////

	auto sp = m_bb->closestPointInside(col.position(), m_c_halfVoxelSize / 10.0);

	long xIndex = 0;
	long yIndex = 0;
	long zIndex = 0;

	auto idx = indexAtPosition(sp);
	getComponentsOfIndex(idx, xIndex, yIndex, zIndex);

	scalar xVal =
		m_bb->minX() + ((xIndex + (d.xPositive() ? 1 : 0)) * m_c_voxelSize);
	scalar yVal =
		m_bb->minY() + ((yIndex + (d.yPositive() ? 1 : 0)) * m_c_voxelSize);
	scalar zVal =
		m_bb->minZ() + ((zIndex + (d.zPositive() ? 1 : 0)) * m_c_voxelSize);

	do
	{
		PERF_COUNT(kGridVoxelStep);
		voxel.index = index(xIndex, yIndex, zIndex);
		voxel.data = vxAt(voxel.index);

		if (voxel.data.active())
		{
			box.set(getVoxelPosition(xIndex, yIndex, zIndex), voxel.size);

			PERF_COUNT(kGridVoxelBoxTest);
			if (box.throwRay(ray, c))
			{
				const auto maxDistance = col.maxDistance();
				col = c;
				col.setMaxDistance(maxDistance);
				auto color1 = Color::indexColor(voxel.data.byte() % 22);
				auto color2 = Color::indexColor(
					(voxel.data.byte() + yIndex + xIndex + zIndex) % 22);
				col.setColor(cm.compute(c, color1, color2));
				col.setValid(true);
				return 1;
			}
		}
		else if (yIndex > 0)
		{
			auto neighbour = neighbourVoxel(voxel, {0, -1, 0});

			if (neighbour.data.active())
			{
				neighbour.position = getVoxelPosition(voxel.index);

				box.set(neighbour.position, neighbour.size / 2.0);
				box.setMinY(neighbour.position.y() - m_c_voxelSize / 2.0);
				box.setMaxY(neighbour.position.y() - m_c_voxelSize / 4.0);

				PERF_COUNT(kGridVoxelBoxTest);
				if (box.throwRay(ray, c))
				{
					const auto maxDistance = col.maxDistance();
					col = c;
					col.setMaxDistance(maxDistance);
					auto color = Color::indexColor(neighbour.data.byte() % 22);
					col.setColor(color);
					col.setValid(true);
					return 1;
				}
			}
		}

		long txIndex = 0;
		long tyIndex = 0;
		long tzIndex = 0;

		v3s v = MU::rayAndXPlane(ray, xVal);
		getComponentsOfIndex(
			indexAtPosition(v + v3s(velX * m_c_halfVoxelSize / 10.0, 0, 0)),
			txIndex, tyIndex, tzIndex);

		if (yIndex == tyIndex && zIndex == tzIndex)
		{
			xVal += velX * m_c_voxelSize;
			xIndex += velX;
		}
		else
		{
			v = MU::rayAndYPlane(ray, yVal);
			getComponentsOfIndex(
				indexAtPosition(v + v3s(0, velY * m_c_halfVoxelSize / 10.0, 0)),
				txIndex, tyIndex, tzIndex);
			if (xIndex == txIndex && zIndex == tzIndex)
			{
				yVal += velY * m_c_voxelSize;
				yIndex += velY;
			}
			else
			{
				zVal += velZ * m_c_voxelSize;
				zIndex += velZ;
			}
		}
	} while ((xIndex >= 0 && xIndex < m_resolution) &&
			 (yIndex >= 0 && yIndex < m_resolution) &&
			 (zIndex >= 0 && zIndex < m_resolution));

	col.setValid(false);
	return 0;
}

void Grid::updateCaches() { rebuildOccupancy(); }

void Grid::rebuildOccupancy()
{
	const long r = m_resolution;
	const long c1 = (r + 7) / 8;
	// The second level only pays from 128 voxels per side on.
	const long c2 = r >= 128 ? (c1 + 7) / 8 : 0;

	m_occCells1 = c1;
	m_occCells2 = c2;
	m_occ1.assign(static_cast<std::size_t>(c1) * c1 * c1, 0);
	m_occ2.assign(static_cast<std::size_t>(c2) * c2 * c2, 0);

	const VoxelData *data = m_data.data();
	for (long z = 0; z < r; z++)
	{
		for (long y = 0; y < r; y++)
		{
			const VoxelData *row = data + (z * r + y) * r;
			unsigned char *cellRow = &m_occ1[((z >> 3) * c1 + (y >> 3)) * c1];
			// The row of cells one layer up, when this is the last voxel layer
			// of its cell and there is one more layer: an empty voxel on top
			// of an active one can be hit, so those cells are not empty.
			unsigned char *upRow = nullptr;
			if ((y & 7) == 7 && y + 1 < r)
			{
				upRow = &m_occ1[((z >> 3) * c1 + ((y + 1) >> 3)) * c1];
			}

			for (long xc = 0; xc < c1; xc++)
			{
				const long x0 = xc * 8;
				const long x1 = std::min(x0 + 8, r);
				unsigned char any = 0;
				for (long x = x0; x < x1; x++)
				{
					any |= row[x].c;
				}
				if (any)
				{
					cellRow[xc] = 1;
					if (upRow)
					{
						upRow[xc] = 1;
					}
				}
			}
		}
	}

	if (c2)
	{
		for (long cz = 0; cz < c1; cz++)
		{
			for (long cy = 0; cy < c1; cy++)
			{
				for (long cx = 0; cx < c1; cx++)
				{
					if (m_occ1[(cz * c1 + cy) * c1 + cx])
					{
						m_occ2[(((cz >> 3) * c2) + (cy >> 3)) * c2 + (cx >> 3)] = 1;
					}
				}
			}
		}
	}

	m_occValid = true;
}

int Grid::throwRay(const Ray &ray, Collision &col) const
{
	if (!m_occValid || currentTraversal() == Traversal::kReference)
	{
		return throwRayReference(ray, col);
	}

	// Entry and exit of the ray in the grid box. It does not use what the box
	// test left in col. A ray the legacy box test accepted but this one does
	// not (it only touches an edge) is left to the reference.
	const RayInv ri(ray);
	scalar tEnter, tExit;
	if (!m_bb->intersect(ri, tEnter, tExit))
	{
		return throwRayReference(ray, col);
	}

	PERF_COUNT(kGridCast);

	auto &c = tl_c;
	auto &box = tl_box;
	auto &cm = tl_cm;

	const long res = m_resolution;
	const scalar vs = m_c_voxelSize;
	const scalar invVs = scalar(1.0) / vs;
	const scalar mn[3] = {m_bb->minX(), m_bb->minY(), m_bb->minZ()};
	const scalar o[3] = {ri.ox, ri.oy, ri.oz};
	const scalar inv[3] = {ri.ix, ri.iy, ri.iz};
	const int neg[3] = {ri.sx, ri.sy, ri.sz};
	const v3s dir = ray.direction();
	const scalar d[3] = {dir.x(), dir.y(), dir.z()};

	// Amanatides and Woo, in the units of the ray direction. idx is the voxel,
	// tMax the parameter at which the ray leaves it along each axis.
	long idx[3];
	scalar tMax[3];
	scalar tDelta[3];
	long step[3];

	const auto voxelOf = [&](scalar p, int a) {
		const long i = static_cast<long>(std::floor((p - mn[a]) * invVs));
		return i < 0 ? 0 : (i >= res ? res - 1 : i);
	};
	const auto reseed = [&](int a) {
		tMax[a] = (mn[a] + (idx[a] + (neg[a] ? 0 : 1)) * vs - o[a]) * inv[a];
	};

	for (int a = 0; a < 3; a++)
	{
		idx[a] = voxelOf(o[a] + d[a] * tEnter, a);
		step[a] = neg[a] ? -1 : 1;
		tDelta[a] = vs * (inv[a] < 0.0 ? -inv[a] : inv[a]);
		reseed(a);
	}

	// Leaves the cell of side 2^shift voxels that holds idx, at the point where
	// the ray crosses its boundary. False when that is outside of the grid.
	const auto skipCell = [&](int shift) {
		const long side = 1L << shift;
		long cs[3];
		scalar tOut = std::numeric_limits<scalar>::max();
		int ax = 0;
		for (int a = 0; a < 3; a++)
		{
			cs[a] = (idx[a] >> shift) << shift;
			const long bound = neg[a] ? cs[a] : cs[a] + side;
			const scalar t = (mn[a] + bound * vs - o[a]) * inv[a];
			if (t < tOut)
			{
				tOut = t;
				ax = a;
			}
		}

		const long next = neg[ax] ? cs[ax] - 1 : cs[ax] + side;
		if (next < 0 || next >= res)
		{
			return false;
		}
		for (int a = 0; a < 3; a++)
		{
			if (a == ax)
			{
				idx[a] = next;
				continue;
			}
			// the other two stay inside the cell they were in
			long i = voxelOf(o[a] + d[a] * tOut, a);
			i = i < cs[a] ? cs[a] : (i > cs[a] + side - 1 ? cs[a] + side - 1 : i);
			idx[a] = i < res ? i : res - 1;
		}
		for (int a = 0; a < 3; a++)
		{
			reseed(a);
		}
		return true;
	};

	for (;;)
	{
		if (m_occCells2)
		{
			const long cell =
				((idx[2] >> 6) * m_occCells2 + (idx[1] >> 6)) * m_occCells2 +
				(idx[0] >> 6);
			if (!m_occ2[cell])
			{
				PERF_COUNT(kGridSkipL2);
				if (!skipCell(6))
				{
					break;
				}
				continue;
			}
		}

		const long cell1 =
			((idx[2] >> 3) * m_occCells1 + (idx[1] >> 3)) * m_occCells1 +
			(idx[0] >> 3);
		if (!m_occ1[cell1])
		{
			PERF_COUNT(kGridSkipL1);
			if (!skipCell(3))
			{
				break;
			}
			continue;
		}

		PERF_COUNT(kGridVoxelStep);
		const long xIndex = idx[0];
		const long yIndex = idx[1];
		const long zIndex = idx[2];
		const long long vi = index(xIndex, yIndex, zIndex);
		const VoxelData data = m_data[vi];

		if (data.active())
		{
			box.set(getVoxelPosition(xIndex, yIndex, zIndex), vs);

			PERF_COUNT(kGridVoxelBoxTest);
			if (box.throwRay(ray, c))
			{
				const auto maxDistance = col.maxDistance();
				col = c;
				col.setMaxDistance(maxDistance);
				auto color1 = Color::indexColor(data.byte() % 22);
				auto color2 = Color::indexColor(
					(data.byte() + yIndex + xIndex + zIndex) % 22);
				col.setColor(cm.compute(c, color1, color2));
				col.setValid(true);
				return 1;
			}
		}
		else if (yIndex > 0)
		{
			// Empty voxel on top of an active one: a thin slab in the bottom of
			// it is hit. The occupancy grids already count these voxels.
			const VoxelData below = m_data[vi - res];
			if (below.active())
			{
				const v3s position = getVoxelPosition(xIndex, yIndex, zIndex);

				box.set(position, vs / 2.0);
				box.setMinY(position.y() - vs / 2.0);
				box.setMaxY(position.y() - vs / 4.0);

				PERF_COUNT(kGridVoxelBoxTest);
				if (box.throwRay(ray, c))
				{
					const auto maxDistance = col.maxDistance();
					col = c;
					col.setMaxDistance(maxDistance);
					auto color = Color::indexColor(below.byte() % 22);
					col.setColor(color);
					col.setValid(true);
					return 1;
				}
			}
		}

		// next voxel along the ray
		const int ax = (tMax[0] < tMax[1])
						   ? (tMax[0] < tMax[2] ? 0 : 2)
						   : (tMax[1] < tMax[2] ? 1 : 2);
		idx[ax] += step[ax];
		if (idx[ax] < 0 || idx[ax] >= res)
		{
			break;
		}
		tMax[ax] += tDelta[ax];
	}

	col.setValid(false);
	return 0;
}

bool Grid::hasCollision(const Ray &ray) const
{
#if DRAWBBOX
	return m_bb->hasCollision(ray);
#else
	Collision col;
	return false; // throwRay(ray, col) == 1;
#endif
}

void Grid::updateBoundingBox()
{
	const auto &c = m_position;
	const auto h = m_size / scalar(2.0);
	m_bb->clear();
	m_bb->extend(v3s(c.x() + h, c.y() + h, c.z() + h));
	m_bb->extend(v3s(c.x() - h, c.y() - h, c.z() - h));
}

void VoxelData::activate(bool active) { active ? activate() : deactivate(); }

bool VoxelData::active() const { return (bool)c; }

bool VoxelData::activeBit(unsigned int bit) const
{
	switch (bit)
	{
	case 0:
		return c & 0b0000'0001;
	case 1:
		return c & 0b0000'0010;
	case 2:
		return c & 0b0000'0100;
	case 3:
		return c & 0b0000'1000;
	case 4:
		return c & 0b0001'0000;
	case 5:
		return c & 0b0010'0000;
	case 6:
		return c & 0b0100'0000;
	case 7:
		return c & 0b1000'0000;
	}

	return c & 0b0000'0000;
}

void VoxelData::activateBit(unsigned int bit)
{
	switch (bit)
	{
	case 0:
		c |= 0b0000'0001;
		break;
	case 1:
		c |= 0b0000'0010;
		break;
	case 2:
		c |= 0b0000'0100;
		break;
	case 3:
		c |= 0b0000'1000;
		break;
	case 4:
		c |= 0b0001'0000;
		break;
	case 5:
		c |= 0b0010'0000;
		break;
	case 6:
		c |= 0b0100'0000;
		break;
	case 7:
		c |= 0b1000'0000;
		break;
	}
}

void VoxelData::deactivateBit(unsigned int bit)
{
	switch (bit)
	{
	case 0:
		c &= ~0b0000'0001;
		break;
	case 1:
		c &= ~0b0000'0010;
		break;
	case 2:
		c &= ~0b0000'0100;
		break;
	case 3:
		c &= ~0b0000'1000;
		break;
	case 4:
		c &= ~0b0001'0000;
		break;
	case 5:
		c &= ~0b0010'0000;
		break;
	case 6:
		c &= ~0b0100'0000;
		break;
	case 7:
		c &= ~0b1000'0000;
		break;
	}
}

void VoxelData::activate() { activateBit(7); }

void VoxelData::deactivate()
{
	c = 0u;
	/*
	deactivateBit(7);
	deactivateBit(6);
	deactivateBit(5);
	deactivateBit(4);

	activateBit(0);
	activateBit(1);
	activateBit(2);*/
}

unsigned char VoxelData::byte() const { return c; }

void VoxelData::setByte(const unsigned char ci)
{
	c = ci;
	activate();
}
