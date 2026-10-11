#include "PerfStats.h"

#include <algorithm>
#include <cstdio>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <thread>

#include <sys/resource.h>
#include <unistd.h>

#ifndef VXR_SOURCE_DIR
#define VXR_SOURCE_DIR "."
#endif
#ifndef VXR_BUILD_TYPE
#define VXR_BUILD_TYPE "unknown"
#endif

namespace vxCore
{

namespace
{

constexpr std::size_t kNumStats = static_cast<std::size_t>(Stat::kCount);
constexpr std::size_t kNumPhases = static_cast<std::size_t>(Phase::kCount);

const char *const kStatNames[kNumStats] = {
	"ray_primary",         "ray_reflection",      "ray_gi",
	"ray_shadow",          "ray_env_light",       "accel_casts",
	"cast_hits",           "cast_misses",         "accel_has_collision_calls",
	"accel_node_visits",   "accel_light_skips",   "accel_box_tests",
	"accel_box_rejects",   "accel_prim_tests",    "accel_prim_hits",
	"tri_tests",           "tri_backface_rejects", "sphere_tests",
	"cylinder_tests",      "plane_tests",         "grid_casts",
	"grid_voxel_steps",    "grid_voxel_box_tests", "grid_skips_l1",
	"grid_skips_l2",       	"mesh_casts",
	"mesh_bucket_lookups", "mesh_bvh_node_visits", "dome_lookups"};

const char *const kPhaseNames[kNumPhases] = {
	"scene_parse",    "scene_build",  "accel_build", "render_wall",
	"render_execute", "image_convert", "image_encode", "total"};

std::mutex g_mutex;
StatArray g_totals{};
std::vector<std::thread::id> g_flushed;
std::array<double, kNumPhases> g_phases{};

std::uint64_t at(const StatArray &a, Stat s)
{
	return a[static_cast<std::size_t>(s)];
}

double ratio(double num, double den) { return den > 0.0 ? num / den : 0.0; }

/// Ray counts and "tests per ray" style numbers shared by summary and JSON.
struct Derived
{
	std::uint64_t rays;
	std::uint64_t secondaryRays;
	std::uint64_t tests;
	double renderSec;
	double raysPerSec;
	double primaryRaysPerSec;
	double testsPerRay;
	double boxTestsPerRay;
	double primTestsPerRay;
	double nodeVisitsPerRay;
	double voxelStepsPerRay;
	double bucketLookupsPerRay;
	double hitRatio;
};

Derived derive()
{
	const auto &t = g_totals;
	Derived d{};
	d.rays = at(t, Stat::kAccelCast);
	d.secondaryRays = at(t, Stat::kRayReflection) + at(t, Stat::kRayGI) +
					  at(t, Stat::kRayShadow) + at(t, Stat::kRayEnvLight);
	const auto prims = at(t, Stat::kTriTest) + at(t, Stat::kSphereTest) +
					   at(t, Stat::kCylinderTest) + at(t, Stat::kPlaneTest);
	d.tests = at(t, Stat::kAccelBoxTest) + prims + at(t, Stat::kGridVoxelBoxTest);
	d.renderSec = g_phases[static_cast<std::size_t>(Phase::kRenderWall)];
	d.raysPerSec = ratio(double(d.rays), d.renderSec);
	d.primaryRaysPerSec = ratio(double(at(t, Stat::kRayPrimary)), d.renderSec);
	d.testsPerRay = ratio(double(d.tests), double(d.rays));
	d.boxTestsPerRay = ratio(double(at(t, Stat::kAccelBoxTest)), double(d.rays));
	d.primTestsPerRay = ratio(double(prims), double(d.rays));
	d.nodeVisitsPerRay =
		ratio(double(at(t, Stat::kAccelNodeVisit)), double(d.rays));
	d.voxelStepsPerRay =
		ratio(double(at(t, Stat::kGridVoxelStep)), double(d.rays));
	d.bucketLookupsPerRay =
		ratio(double(at(t, Stat::kMeshBucketLookup)), double(d.rays));
	d.hitRatio = ratio(double(at(t, Stat::kCastHit)), double(d.rays));
	return d;
}

std::string shellQuote(const std::string &s)
{
	std::string out = "'";
	for (const char c : s)
	{
		if (c == '\'')
		{
			out += "'\\''";
		}
		else
		{
			out += c;
		}
	}
	return out + "'";
}

/// Runs a command, returns its stdout. ok tells if it exited with 0.
std::string runCommand(const std::string &cmd, bool &ok)
{
	std::string out;
	ok = false;
	FILE *pipe = popen(cmd.c_str(), "r");
	if (!pipe)
	{
		return out;
	}
	char buf[4096];
	std::size_t n;
	while ((n = fread(buf, 1, sizeof(buf), pipe)) > 0)
	{
		out.append(buf, n);
	}
	ok = pclose(pipe) == 0;
	return out;
}

struct GitInfo
{
	bool available{false};
	std::string commit;
	bool dirty{false};
	std::uint64_t diffHash{0};
};

GitInfo gitInfo()
{
	GitInfo g;
	const std::string git = "git -C " + shellQuote(VXR_SOURCE_DIR) + " ";
	bool ok = false;
	auto head = runCommand(git + "rev-parse HEAD 2>/dev/null", ok);
	if (!ok)
	{
		return g;
	}
	while (!head.empty() && (head.back() == '\n' || head.back() == '\r'))
	{
		head.pop_back();
	}
	g.commit = head;

	// Untracked files (JPEGs, logs) do not change the code that was built.
	const auto status =
		runCommand(git + "status --porcelain --untracked-files=no 2>/dev/null", ok);
	if (ok)
	{
		g.available = true;
		g.dirty = !status.empty();
		if (g.dirty)
		{
			const auto diff = runCommand(git + "diff HEAD 2>/dev/null", ok);
			g.diffHash = PerfStats::fnv1a64(diff.data(), diff.size());
		}
	}
	return g;
}

std::string escapeJson(const std::string &s)
{
	std::string out;
	for (const char c : s)
	{
		switch (c)
		{
		case '"':
			out += "\\\"";
			break;
		case '\\':
			out += "\\\\";
			break;
		case '\n':
			out += "\\n";
			break;
		case '\r':
			out += "\\r";
			break;
		case '\t':
			out += "\\t";
			break;
		default:
			if (static_cast<unsigned char>(c) < 0x20)
			{
				char b[8];
				snprintf(b, sizeof(b), "\\u%04x", c);
				out += b;
			}
			else
			{
				out += c;
			}
		}
	}
	return out;
}

std::string hex64(std::uint64_t v)
{
	std::ostringstream ss;
	ss << std::hex << std::setfill('0') << std::setw(16) << v;
	return ss.str();
}

std::string utcTimestamp(std::time_t t, bool compact)
{
	std::tm tm{};
	gmtime_r(&t, &tm);
	char buf[32];
	strftime(buf, sizeof(buf), compact ? "%Y%m%dT%H%M%SZ" : "%Y-%m-%dT%H:%M:%SZ",
			 &tm);
	return buf;
}

double timevalSec(const timeval &tv)
{
	return double(tv.tv_sec) + double(tv.tv_usec) * 1e-6;
}

/// ru_maxrss is bytes on macOS and kilobytes elsewhere.
std::uint64_t peakRssBytes(rusage &ru)
{
	getrusage(RUSAGE_SELF, &ru);
#ifdef __APPLE__
	return static_cast<std::uint64_t>(ru.ru_maxrss);
#else
	return static_cast<std::uint64_t>(ru.ru_maxrss) * 1024ull;
#endif
}

const char *osName()
{
#if defined(__APPLE__)
	return "darwin";
#elif defined(__linux__)
	return "linux";
#else
	return "unknown";
#endif
}

const char *archName()
{
#if defined(__aarch64__) || defined(__arm64__)
	return "arm64";
#elif defined(__x86_64__)
	return "x86_64";
#else
	return "unknown";
#endif
}

/// Small helper that writes comma separated JSON members.
class Json
{
	std::ostringstream m_out;
	std::vector<bool> m_first{true};

	void sep()
	{
		if (!m_first.back())
		{
			m_out << ",";
		}
		m_first.back() = false;
		m_out << "\n" << std::string((m_first.size() - 1) * 2, ' ');
	}

public:
	Json() { m_out << std::setprecision(10); }

	void open(const std::string &key = "")
	{
		if (m_out.tellp() > 0)
		{
			sep();
		}
		if (!key.empty())
		{
			m_out << "\"" << key << "\": ";
		}
		m_out << "{";
		m_first.push_back(true);
	}
	void close()
	{
		m_first.pop_back();
		m_out << "\n" << std::string(m_first.size() * 2 - 2, ' ') << "}";
	}
	void str(const std::string &key, const std::string &v)
	{
		sep();
		m_out << "\"" << key << "\": \"" << escapeJson(v) << "\"";
	}
	void num(const std::string &key, double v)
	{
		sep();
		m_out << "\"" << key << "\": " << v;
	}
	void uint(const std::string &key, std::uint64_t v)
	{
		sep();
		m_out << "\"" << key << "\": " << v;
	}
	void boolean(const std::string &key, bool v)
	{
		sep();
		m_out << "\"" << key << "\": " << (v ? "true" : "false");
	}
	void null(const std::string &key)
	{
		sep();
		m_out << "\"" << key << "\": null";
	}
	void raw(const std::string &key, const std::string &v)
	{
		sep();
		m_out << "\"" << key << "\": " << v;
	}
	std::string text() const { return m_out.str() + "\n"; }
};

} // namespace

bool PerfStats::compiledIn()
{
#ifdef VXR_NO_PERF_STATS
	return false;
#else
	return true;
#endif
}

void PerfStats::flushThread()
{
	auto &mine = local();
	std::lock_guard<std::mutex> lock(g_mutex);
	for (std::size_t i = 0; i < kNumStats; i++)
	{
		g_totals[i] += mine[i];
		mine[i] = 0;
	}
	g_flushed.push_back(std::this_thread::get_id());
}

bool PerfStats::hasUnflushed()
{
	const auto &mine = local();
	return std::any_of(mine.begin(), mine.end(),
					   [](std::uint64_t v) { return v != 0; });
}

void PerfStats::resetCounters()
{
	std::lock_guard<std::mutex> lock(g_mutex);
	g_totals.fill(0);
	g_flushed.clear();
	local().fill(0);
}

void PerfStats::reset()
{
	resetCounters();
	g_phases.fill(0.0);
}

const StatArray &PerfStats::totals() { return g_totals; }

std::uint64_t PerfStats::total(Stat s) { return at(g_totals, s); }

const char *PerfStats::name(Stat s) { return kStatNames[static_cast<std::size_t>(s)]; }

void PerfStats::setPhase(Phase p, double seconds)
{
	g_phases[static_cast<std::size_t>(p)] = seconds;
}

double PerfStats::phase(Phase p) { return g_phases[static_cast<std::size_t>(p)]; }

const char *PerfStats::name(Phase p)
{
	return kPhaseNames[static_cast<std::size_t>(p)];
}

std::vector<std::string> PerfStats::validate(const PerfRunInfo &info)
{
	std::vector<std::string> bad;
	const auto &t = g_totals;
	auto fail = [&bad](const std::string &msg) { bad.push_back(msg); };

	// Every render thread merged once, and nobody merged twice.
	if (g_flushed.size() != info.threads)
	{
		fail("threads merged " + std::to_string(g_flushed.size()) + " != threads " +
			 std::to_string(info.threads));
	}
	auto ids = g_flushed;
	std::sort(ids.begin(), ids.end());
	if (std::adjacent_find(ids.begin(), ids.end()) != ids.end())
	{
		fail("a thread merged its counters more than once");
	}
	// Nothing left behind on the thread that runs the checks.
	if (hasUnflushed())
	{
		fail("the main thread holds counts that were never merged");
	}

	const std::uint64_t expectedPrimary = std::uint64_t(info.width) *
										  std::uint64_t(info.height) *
										  std::uint64_t(info.pixelSamples);
	if (at(t, Stat::kRayPrimary) != expectedPrimary)
	{
		fail("primary rays " + std::to_string(at(t, Stat::kRayPrimary)) +
			 " != width*height*samples " + std::to_string(expectedPrimary));
	}

	const auto kinds = at(t, Stat::kRayPrimary) + at(t, Stat::kRayReflection) +
					   at(t, Stat::kRayGI) + at(t, Stat::kRayShadow) +
					   at(t, Stat::kRayEnvLight);
	if (kinds != at(t, Stat::kAccelCast))
	{
		fail("ray kinds sum " + std::to_string(kinds) + " != accel casts " +
			 std::to_string(at(t, Stat::kAccelCast)));
	}
	if (at(t, Stat::kCastHit) + at(t, Stat::kCastMiss) != at(t, Stat::kAccelCast))
	{
		fail("hits + misses != accel casts");
	}
	if (at(t, Stat::kAccelBoxReject) > at(t, Stat::kAccelBoxTest))
	{
		fail("accel box rejects > accel box tests");
	}
	if (at(t, Stat::kAccelPrimHit) > at(t, Stat::kAccelPrimTest))
	{
		fail("accel primitive hits > accel primitive tests");
	}
	if (at(t, Stat::kTriBackfaceReject) > at(t, Stat::kTriTest))
	{
		fail("triangle backface rejects > triangle tests");
	}
	return bad;
}

std::uint64_t PerfStats::fnv1a64(const void *data, std::size_t size)
{
	std::uint64_t h = 1469598103934665603ull;
	const auto *p = static_cast<const unsigned char *>(data);
	for (std::size_t i = 0; i < size; i++)
	{
		h ^= p[i];
		h *= 1099511628211ull;
	}
	return h;
}

std::uint64_t PerfStats::fnv1aFile(const std::string &path, std::uint64_t *bytes)
{
	std::ifstream in(path, std::ios::binary);
	std::string content((std::istreambuf_iterator<char>(in)),
						std::istreambuf_iterator<char>());
	if (bytes)
	{
		*bytes = content.size();
	}
	return fnv1a64(content.data(), content.size());
}

void PerfStats::printSummary(std::ostream &os, const PerfRunInfo &info)
{
	const auto d = derive();
	const auto &t = g_totals;
	const auto problems = validate(info);

	auto line = [&os](const std::string &k, const std::string &v) {
		os << "  " << std::left << std::setw(26) << k << v << "\n";
	};
	auto count = [&](Stat s) {
		return std::to_string(at(t, s));
	};
	auto sec = [](double v) {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(4) << v << " s";
		return ss.str();
	};
	auto fix = [](double v, int p) {
		std::ostringstream ss;
		ss << std::fixed << std::setprecision(p) << v;
		return ss.str();
	};

	os << "\n==================== vxR perf summary ====================\n";
	line("scene", info.scenePath);
	line("accel backend", info.accel);
	line("image", std::to_string(info.width) + "x" + std::to_string(info.height) +
					  "  samples " + std::to_string(info.pixelSamples) +
					  "  depth " + std::to_string(info.rayDepth));
	line("threads / seed", std::to_string(info.threads) + " / " +
							   (info.seeded ? std::to_string(info.seed) : "none"));
	os << "  -- time\n";
	for (unsigned p = 0; p < kNumPhases; p++)
	{
		line(std::string("  ") + kPhaseNames[p], sec(g_phases[p]));
	}
	os << "  -- rays\n";
	line("  primary", count(Stat::kRayPrimary));
	line("  reflection", count(Stat::kRayReflection));
	line("  gi", count(Stat::kRayGI));
	line("  shadow", count(Stat::kRayShadow));
	line("  env light", count(Stat::kRayEnvLight));
	line("  total casts", count(Stat::kAccelCast) + "  (hit " +
							 count(Stat::kCastHit) + ", miss " +
							 count(Stat::kCastMiss) + ")");
	os << "  -- tests\n";
	line("  accel node visits", count(Stat::kAccelNodeVisit));
	line("  accel box tests", count(Stat::kAccelBoxTest) + "  (rejected " +
								  count(Stat::kAccelBoxReject) + ")");
	line("  accel prim tests", count(Stat::kAccelPrimTest) + "  (hit " +
								  count(Stat::kAccelPrimHit) + ")");
	line("  light skips", count(Stat::kAccelLightSkip));
	line("  triangle", count(Stat::kTriTest) + "  (backface " +
						   count(Stat::kTriBackfaceReject) + ")");
	line("  sphere / cylinder", count(Stat::kSphereTest) + " / " +
									count(Stat::kCylinderTest));
	line("  plane", count(Stat::kPlaneTest));
	line("  grid casts", count(Stat::kGridCast));
	line("  grid voxel steps", count(Stat::kGridVoxelStep));
	line("  grid voxel box tests", count(Stat::kGridVoxelBoxTest));
	line("  grid skips L1 / L2", count(Stat::kGridSkipL1) + " / " +
								   count(Stat::kGridSkipL2));
	line("  mesh casts", count(Stat::kMeshCast));
	line("  mesh bucket lookups", count(Stat::kMeshBucketLookup));
	line("  mesh bvh node visits", count(Stat::kMeshBvhNodeVisit));
	line("  dome lookups", count(Stat::kDomeLookup));
	os << "  -- derived\n";
	line("  rays / s", fix(d.raysPerSec, 0));
	line("  tests / ray", fix(d.testsPerRay, 3) + "  (box " +
							 fix(d.boxTestsPerRay, 3) + ", prim " +
							 fix(d.primTestsPerRay, 3) + ")");
	line("  voxel steps / ray", fix(d.voxelStepsPerRay, 3));
	line("  bucket lookups / ray", fix(d.bucketLookupsPerRay, 3));
	line("  hit ratio", fix(d.hitRatio, 4));
	os << "  -- image\n";
	line("  rgba8 fnv1a64", hex64(info.imageRgbaHash));
	line("  jpeg fnv1a64", hex64(info.jpegHash));
	os << "  -- checks: ";
	if (problems.empty())
	{
		os << "OK\n";
	}
	else
	{
		os << "FAILED\n";
		for (const auto &p : problems)
		{
			os << "     ! " << p << "\n";
		}
	}
	os << "==========================================================\n"
	   << std::flush;
}

bool PerfStats::writeJson(const std::string &path, const PerfRunInfo &info)
{
	const auto d = derive();
	const auto problems = validate(info);
	const auto git = gitInfo();
	const auto now = std::time(nullptr);

	rusage ru{};
	const auto rss = peakRssBytes(ru);

	std::uint64_t sceneBytes = 0;
	const auto sceneHash = fnv1aFile(info.scenePath, &sceneBytes);

	Json j;
	j.open();
	j.str("schema", "vxr-perf/1");
	j.str("run_id", utcTimestamp(now, true) + "-" + std::to_string(getpid()));
	j.str("timestamp_utc", utcTimestamp(now, false));
	j.uint("unix_time", static_cast<std::uint64_t>(now));

	j.open("git");
	j.boolean("available", git.available);
	j.str("commit", git.commit);
	j.boolean("dirty", git.dirty);
	j.str("dirty_diff_fnv1a64", git.dirty ? hex64(git.diffHash) : "");
	j.str("note", "read when the run started, not when the binary was built");
	j.close();

	j.open("build");
	j.str("type", VXR_BUILD_TYPE);
	j.str("compiler", __VERSION__);
	j.boolean("fast_math",
#ifdef __FAST_MATH__
			  true
#else
			  false
#endif
	);
	j.boolean("ndebug",
#ifdef NDEBUG
			  true
#else
			  false
#endif
	);
	j.boolean("stats_compiled_in", compiledIn());
	j.str("os", osName());
	j.str("arch", archName());
	j.close();

	j.open("scene");
	j.str("path", info.scenePath);
	j.uint("bytes", sceneBytes);
	j.str("fnv1a64", hex64(sceneHash));
	j.close();

	j.open("config");
	j.uint("width", info.width);
	j.uint("height", info.height);
	j.uint("pixel_samples", info.pixelSamples);
	j.uint("ray_depth", info.rayDepth);
	if (info.seeded)
	{
		j.uint("seed", info.seed);
	}
	else
	{
		j.null("seed");
	}
	j.uint("threads", info.threads);
	j.uint("hardware_threads", info.hardwareThreads);
	j.str("accel", info.accel);
	j.boolean("quiet", quiet());
	j.close();

	j.open("timings_sec");
	for (unsigned p = 0; p < kNumPhases; p++)
	{
		j.num(kPhaseNames[p], g_phases[p]);
	}
	j.close();

	j.open("counters");
	for (std::size_t s = 0; s < kNumStats; s++)
	{
		j.uint(kStatNames[s], g_totals[s]);
	}
	j.close();

	j.open("derived");
	j.uint("rays_total", d.rays);
	j.uint("rays_secondary", d.secondaryRays);
	j.uint("tests_total", d.tests);
	j.num("rays_per_sec", d.raysPerSec);
	j.num("primary_rays_per_sec", d.primaryRaysPerSec);
	j.num("tests_per_ray", d.testsPerRay);
	j.num("box_tests_per_ray", d.boxTestsPerRay);
	j.num("prim_tests_per_ray", d.primTestsPerRay);
	j.num("node_visits_per_ray", d.nodeVisitsPerRay);
	j.num("voxel_steps_per_ray", d.voxelStepsPerRay);
	j.num("bucket_lookups_per_ray", d.bucketLookupsPerRay);
	j.num("hit_ratio", d.hitRatio);
	j.close();

	j.open("checks");
	j.uint("threads_merged", g_flushed.size());
	j.uint("threads_expected", info.threads);
	j.uint("primary_rays_expected", std::uint64_t(info.width) * info.height *
										info.pixelSamples);
	j.boolean("all_ok", problems.empty());
	std::string failures = "[";
	for (std::size_t i = 0; i < problems.size(); i++)
	{
		failures += (i ? ", \"" : "\"") + escapeJson(problems[i]) + "\"";
	}
	failures += "]";
	j.raw("failures", failures);
	j.close();

	j.open("process");
	j.uint("peak_rss_bytes", rss);
	j.num("user_cpu_sec", timevalSec(ru.ru_utime));
	j.num("sys_cpu_sec", timevalSec(ru.ru_stime));
	j.close();

	j.open("image");
	j.str("path", info.imagePath);
	j.str("rgba8_fnv1a64", hex64(info.imageRgbaHash));
	j.str("jpeg_fnv1a64", hex64(info.jpegHash));
	j.uint("jpeg_bytes", info.jpegBytes);
	j.close();
	j.close();

	std::ofstream out(path);
	if (!out)
	{
		return false;
	}
	out << j.text();
	return static_cast<bool>(out);
}

} // namespace vxCore
