#ifndef VXPERFSTATS_H
#define VXPERFSTATS_H

#include <array>
#include <chrono>
#include <cstddef>
#include <cstdint>
#include <iosfwd>
#include <string>
#include <vector>

namespace vxCore
{

///
/// Performance counters and phase timers used to compare acceleration
/// structures (see tools/ab_bench.py).
///
/// Counters are plain thread local integers. A render thread merges its own
/// block into the totals exactly once, at the end of RenderProcess::render()
/// (PerfStats::flushThread). Nothing is atomic on the hot path.
///
/// Cost when off: PERF_COUNT is one load of a bool that never changes while
/// the render threads run. Build with -DVXR_PERF_STATS=OFF (defines
/// VXR_NO_PERF_STATS) and every call site compiles to nothing.
///
/// A new acceleration structure must count what it does itself:
///   kAccelNodeVisit  every node / visible / bucket it looks at,
///   kAccelBoxTest    every AABB test it performs at the scene level,
///   kAccelBoxReject  every AABB test that rejected the ray,
///   kAccelPrimTest   every call into a leaf's throwRay,
/// so that the "tests per ray" numbers stay comparable between backends.
/// Per primitive counters (kTriTest, kSphereTest...) are counted by the
/// primitive itself and need no work in the backend.
///
/// The hasCollision() family is not used by the renderer. Only its calls
/// are counted (kAccelHasCollision); primitive tests done by it are not.
///
enum class Stat : unsigned
{
	// Rays, counted where the ray is created.
	kRayPrimary,
	kRayReflection,
	kRayGI,
	kRayShadow,
	kRayEnvLight,
	// Scene::throwRay entry, and its final outcome (planes included).
	kAccelCast,
	kCastHit,
	kCastMiss,
	kAccelHasCollision,
	// Acceleration structure internals.
	kAccelNodeVisit,
	kAccelLightSkip,
	kAccelBoxTest,
	kAccelBoxReject,
	kAccelPrimTest,
	kAccelPrimHit,
	// Primitives.
	kTriTest,
	kTriBackfaceReject,
	kSphereTest,
	kCylinderTest,
	kPlaneTest,
	kGridCast,
	kGridVoxelStep,
	kGridVoxelBoxTest,
	kGridSkipL1, // empty 8^3 cells stepped over by the grid traversal
	kGridSkipL2, // empty 64^3 cells stepped over (grids of 128 or more)
	kMeshCast,
	kMeshBucketLookup,
	kMeshBvhNodeVisit,
	kDomeLookup,
	kCount
};

enum class Phase : unsigned
{
	kSceneParse,
	kSceneBuild,   // includes kAccelBuild
	kAccelBuild,   // AccelStructure::updateCache only
	kRenderWall,   // first thread start to last thread end
	kRenderExecute, // RenderProcess::execute, includes its 1s+ polling sleep
	kImageConvert, // float buffer to RGBA8
	kImageEncode,  // JPEG encode and write
	kTotal,
	kCount
};

using StatArray = std::array<std::uint64_t, static_cast<std::size_t>(Stat::kCount)>;

/// Everything the report needs that PerfStats cannot know by itself.
struct PerfRunInfo
{
	std::string scenePath;
	std::string accel;
	std::string imagePath;
	unsigned width{0};
	unsigned height{0};
	unsigned pixelSamples{0};
	unsigned rayDepth{0};
	unsigned threads{0};
	unsigned hardwareThreads{0};
	bool seeded{false};
	std::uint64_t seed{0};
	std::uint64_t imageRgbaHash{0};
	std::uint64_t jpegHash{0};
	std::uint64_t jpegBytes{0};
};

class PerfStats
{
	static inline bool s_enabled{false};
	static inline bool s_quiet{false};

public:
	/// Set once, before any render thread is created.
	static void enable(bool on) { s_enabled = on; }
	static bool enabled() { return s_enabled; }

	/// Silences the per ray debug print in DirectLight::acummulationLight.
	static void setQuiet(bool on) { s_quiet = on; }
	static bool quiet() { return s_quiet; }

	static bool compiledIn();

	/// This thread's counters.
	static StatArray &local()
	{
		thread_local StatArray counters{};
		return counters;
	}

	/// Adds this thread's counters to the totals and zeroes them, so a
	/// second call cannot count anything twice.
	static void flushThread();
	/// True when this thread holds counts that were never flushed.
	static bool hasUnflushed();

	/// Clears totals and flush bookkeeping, keeps the phase timers.
	static void resetCounters();
	/// Clears everything.
	static void reset();

	static const StatArray &totals();
	static std::uint64_t total(Stat s);
	static const char *name(Stat s);

	static void setPhase(Phase p, double seconds);
	static double phase(Phase p);
	static const char *name(Phase p);

	/// Consistency checks, empty when everything adds up.
	static std::vector<std::string> validate(const PerfRunInfo &info);

	static void printSummary(std::ostream &os, const PerfRunInfo &info);
	/// Hand written JSON, one object. Returns false when it cannot write.
	static bool writeJson(const std::string &path, const PerfRunInfo &info);

	static std::uint64_t fnv1a64(const void *data, std::size_t size);
	static std::uint64_t fnv1aFile(const std::string &path,
								   std::uint64_t *bytes = nullptr);
};

/// Writes the elapsed wall clock time into a phase when it goes out of scope.
class ScopedPhase
{
	Phase m_phase;
	std::chrono::steady_clock::time_point m_start{std::chrono::steady_clock::now()};

public:
	explicit ScopedPhase(Phase phase) : m_phase(phase) {}
	~ScopedPhase()
	{
		PerfStats::setPhase(
			m_phase, std::chrono::duration<double>(std::chrono::steady_clock::now() -
													m_start)
						 .count());
	}
};

} // namespace vxCore

#ifdef VXR_NO_PERF_STATS
#define PERF_COUNT(stat) ((void)0)
#else
// An expression, so it can sit inside a condition with the comma operator.
#define PERF_COUNT(stat)                                                       \
	((void)(vxCore::PerfStats::enabled() &&                                    \
			(++vxCore::PerfStats::local()[static_cast<std::size_t>(            \
				 vxCore::Stat::stat)],                                         \
			 true)))
#endif

#endif // VXPERFSTATS_H
