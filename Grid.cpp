#include <cassert>
#include <fstream>
#include <mutex>

#include "Cylinder.h"
#include "Grid.h"
#include "MathUtils.h"

#include "CirclesMap.h"

std::mutex gridMutex;

using namespace vxCore;

#define DBL_EPSILON 1e-12
#define DRAWBBOX 0

Geometry *Grid::getVisibleVoxelGeo() const
{
    return m_visibleVoxelGeo;
}

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

long Grid::index(const long x, const long y, const long z) const
{
	return (x + (y * m_resolution) + (z * m_c_resXres));
}

void Grid::initialize(bool value)
{
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

	for (long i = 0; i < m_data.size(); i++)
	{
		if (vxAt(i).active())
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

bool Grid::active(long idx) const
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

VoxelData &Grid::vxAt(const long long idx) { return m_data[idx]; }

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

int Grid::throwRay(const Ray &ray, Collision &col) const
{
	///////////// THis is const and could be stored per thread and ray ////

	thread_local VoxelInfo voxel;
	voxel.size = m_c_voxelSize;

	thread_local Collision c;
	thread_local BoundingBox box; // geometries forthread_local VoxelInfo voxel;
								  // voxels
	const auto &d = ray.direction();
	const auto velX = d.xPositive() ? 1 : -1;
	const auto velY = d.yPositive() ? 1 : -1;
	const auto velZ = d.zPositive() ? 1 : -1;
	///////////////////////////////////////////////////////////////////////
                CirclesMap cm;
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
		voxel.index = index(xIndex, yIndex, zIndex);
		voxel.data = vxAt(voxel.index);

		if (voxel.data.active())
		{
            box.set(getVoxelPosition(xIndex, yIndex, zIndex), voxel.size * .98);

            if (box.throwRay(ray, c))
			{
				col = c;
                //auto color = Color::indexColor(voxel.data.byte() % 22);
                //color.saturate(1.3);


                col.setColor(cm.compute(c));



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
				if (box.throwRay(ray, c))
				{
					col = c;
					auto color = Color::indexColor(neighbour.data.byte() % 22);
                    //color.saturate(1.3);
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
    }
	while ((xIndex >= 0 && xIndex < m_resolution) &&
		   (yIndex >= 0 && yIndex < m_resolution) &&
		   (zIndex >= 0 && zIndex < m_resolution));

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
	m_bb->clear();
	m_bb->extend(v3s(m_size / 2.0, m_size / 2.0, m_size / 2.0));
	m_bb->extend(v3s(-m_size / 2.0, -m_size / 2.0, -m_size / 2.0));
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
