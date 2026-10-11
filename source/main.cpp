#include <cerrno>
#include <cstdio>
#include <chrono>
#include <cstdlib>
#include <sstream>
#include <thread>

#include "AccelStructure.h"
#include "AccelUnitTest.h"
#include "Clock.h"
#include "FileUtils.h"
#include "Grid.h"
#include "GridUnitTest.h"
#include "ImageProperties.h"
#include "MathUtils.h"
#include "MathUtilsUnitTest.h"
#include "NonSequentalPool.h"
#include "PerfStats.h"
#include "RenderProcess.h"
#include "SceneParser.h"
#include "TimeUtils.h"
#include "Vector.h"
#include "renderPresset.h"
#include "stb_image_write.h"

static const std::string baseName("image.0000001.jpg");
using namespace std::string_literals;
using timePoint = std::chrono::time_point<std::chrono::system_clock>;
using render = vxCompute::RenderProcess;
using namespace vxCore;

// Options of the render mode (-r). They can go anywhere on the command line.
struct RunOptions
{
	std::string accel{AccelStructure::defaultName()};
	bool seeded{false};
	std::uint64_t seed{0};
	unsigned threads{0}; // 0 keeps the renderer default.
	bool stats{false};
	std::string statsOut;
	bool quiet{false};
	std::string out;
	unsigned resX{0}; // 0 keeps the scene value.
	unsigned resY{0};
	unsigned samples{0};
};

// Number of arguments taken by a render option (itself included) or 0 when
// arg is not one. It does not check that the value exists.
static int renderOptionArgs(const char *arg)
{
	for (const auto *name : {"--accel", "--seed", "--threads", "--stats-out",
							 "--out", "--res", "--samples"})
	{
		if (name == std::string(arg))
		{
			return 2;
		}
	}
	for (const auto *name : {"--stats", "--quiet"})
	{
		if (name == std::string(arg))
		{
			return 1;
		}
	}
	return 0;
}

static bool parseUnsigned(const std::string &text, std::uint64_t &value)
{
	if (text.empty() || text.find_first_not_of("0123456789") != std::string::npos)
	{
		return false;
	}
	errno = 0;
	value = std::strtoull(text.c_str(), nullptr, 10);
	return errno == 0;
}

static bool parseRunOptions(int argc, char *argv[], RunOptions &opt)
{
	for (int i = 1; i < argc; i++)
	{
		const std::string arg(argv[i]);
		const auto taken = renderOptionArgs(argv[i]);

		if (taken == 0)
		{
			if (arg.rfind("--", 0) == 0 && arg != "--help")
			{
				std::cerr << "Unknown option " << arg << ", see --help" << std::endl;
				return false;
			}
			continue;
		}

		if (taken == 1)
		{
			(arg == "--stats" ? opt.stats : opt.quiet) = true;
			continue;
		}

		if (i + 1 >= argc)
		{
			std::cerr << "Option " << arg << " needs a value" << std::endl;
			return false;
		}

		const std::string value(argv[++i]);
		std::uint64_t number = 0;
		bool ok = true;

		if (arg == "--accel")
		{
			opt.accel = value;
		}
		else if (arg == "--stats-out")
		{
			opt.statsOut = value;
		}
		else if (arg == "--out")
		{
			opt.out = value;
		}
		else if (arg == "--seed")
		{
			ok = parseUnsigned(value, number);
			opt.seeded = true;
			opt.seed = number;
		}
		else if (arg == "--threads")
		{
			ok = parseUnsigned(value, number) && number > 0 && number <= 1024;
			opt.threads = static_cast<unsigned>(number);
		}
		else if (arg == "--samples")
		{
			ok = parseUnsigned(value, number) && number > 0 && number <= 1000000;
			opt.samples = static_cast<unsigned>(number);
		}
		else if (arg == "--res")
		{
			const auto x = value.find('x');
			std::uint64_t w = 0;
			std::uint64_t h = 0;
			ok = x != std::string::npos && parseUnsigned(value.substr(0, x), w) &&
				 parseUnsigned(value.substr(x + 1), h) && w > 0 && h > 0 &&
				 w <= 65536 && h <= 65536;
			opt.resX = static_cast<unsigned>(w);
			opt.resY = static_cast<unsigned>(h);
		}

		if (!ok)
		{
			std::cerr << "Bad value '" << value << "' for " << arg << std::endl;
			return false;
		}
	}

	if (!AccelStructure::create(opt.accel))
	{
		std::cerr << "Unknown acceleration structure '" << opt.accel
				  << "'. Available:";
		for (const auto &name : AccelStructure::names())
		{
			std::cerr << " " << name;
		}
		std::cerr << std::endl;
		return false;
	}

	return true;
}

int executeRenderProcess(int argc, char *argv[])
{
	timePoint start = std::chrono::system_clock::now();
	const auto totalStart = std::chrono::steady_clock::now();

	RunOptions opt;
	if (!parseRunOptions(argc, argv, opt))
	{
		return 2;
	}

	const bool statsOn = opt.stats || !opt.statsOut.empty();
	if (statsOn && !PerfStats::compiledIn())
	{
		std::cerr << "--stats needs a build without VXR_PERF_STATS=OFF" << std::endl;
		return 2;
	}
	PerfStats::reset();
	PerfStats::enable(statsOn);
	PerfStats::setQuiet(opt.quiet);

	// The scene build draws random numbers too (Grid::createRandom).
	if (opt.seeded)
	{
		MU::seedRand(opt.seed, ~std::uint64_t(0));
	}

	std::cout << "Start program" << std::endl;

	std::string scenePath;
	// Look for a scene argument.
	bool sceneParam{false};
	for (int i = 1; i < argc && scenePath.empty(); i++)
	{
		if (sceneParam)
		{
			scenePath = argv[i];
		}

		if ("-scene"s == std::string(argv[i]))
		{
			sceneParam = true;
		}
	}

	FileUtils::path = FileUtils::removeLastFileOnRoute(scenePath);
	std::cout << "\t Captured scene file argument: " << scenePath << std::endl;

	// TODO:find a home for this next two lines of code.
	std::shared_ptr<SceneParser> sceneParser;
	{
		ScopedPhase parsePhase(Phase::kSceneParse);
		sceneParser = std::make_shared<SceneParser>(scenePath);
		sceneParser->procesScene();
	}
	std::shared_ptr<Scene> m_scene;

	int frameStart = 0;
	int frameEnd = 1;
	for (const auto &timeOption : sceneParser->getNodesByType("vxClock"))
	{
		frameStart = timeOption->getScalar("start");
		frameEnd = timeOption->getScalar("end");
	}

	for (const auto &renderSettings :
		 sceneParser->getNodesByType("vxRenderSettings"))
	{
		for (int i = frameStart; i < frameEnd; i++)
		{
			const auto resolution = renderSettings->getVector2d("resolution");
			const unsigned width = opt.resX ? opt.resX : resolution.x();
			const unsigned height = opt.resY ? opt.resY : resolution.y();
			const unsigned samples =
				opt.samples ? opt.samples : renderSettings->getInt("pixelSamples");
			const auto rayDepth = renderSettings->getInt("rayDepth");
			const auto giMultiplier = renderSettings->getScalar("giMultiplier");

			//			const auto numThreads = node->getInt("numThreads");

			// Img properties for render.
			auto renderProperties =
				std::make_shared<ImageProperties>(width, height);
			render rp(renderProperties, samples);
			if (opt.threads)
			{
				rp.setNMaxThreads(opt.threads);
			}
			if (opt.seeded)
			{
				rp.setSeed(opt.seed);
			}

			if (m_scene == nullptr)
			{
				ScopedPhase buildPhase(Phase::kSceneBuild);
				m_scene = std::make_shared<Scene>(renderProperties, opt.accel);
				m_scene->build(sceneParser);
			}
			else
			{
				m_scene->setProperties(renderProperties);
				m_scene->camera()->setProperties(renderProperties);

				std::cout << "\t\tRender process: " << TimeUtils::decorateTime(start, 2)
						  << std::endl;
			}

			// call time callbacks.

			rp.setScene(m_scene.get());
			rp.setDatabase(sceneParser);
			rp.setGIMultiplier(giMultiplier);
			rp.setVisSamples(samples);
			rp.setRayDepth(rayDepth);
			rp.setSamples(samples);

			// create the render process

			// creates the bucket list (vector) using the img
			// description.
			// rp.createBucketList(samples);

			// executes the render.
			PerfStats::resetCounters();
			if (rp.execute() == Status::code::kError)
			{
				return 1;
			}
			PerfStats::setPhase(Phase::kRenderWall, rp.renderSeconds());
			{
				// RenderProcess::execute leaves cout at precision 2.
				char seconds[32];
				snprintf(seconds, sizeof(seconds), "%.4f", rp.renderSeconds());
				std::cout << "\t\tRender time, threads only: " << seconds << " s"
						  << std::endl;
			}

			std::cout << "\t\tFinished render : " << TimeUtils::decorateTime(start, 2)
					  << std::endl;

			// generates an image buffer and fills it
			// with the render results. Buffer properties
			// are based on ImageProperties stored in the
			// render process object.
			const unsigned char *bff = nullptr;
			{
				ScopedPhase convertPhase(Phase::kImageConvert);
				bff = rp.generateImage();
			}

			std::cout << "\t\tEnded creation of the image: "
					  << TimeUtils::decorateTime(start, 2) << std::endl;

			// storing an image from the buffer obtained.
			if (bff != nullptr)
			{
				std::string fileName =
					opt.out.empty() ? FileUtils::makeUnique(baseName, 0.0) : opt.out;

				std::cout << "Opening file: "s << fileName << " to write."s
						  << std::endl;

				// TODO: investigate other formats and resolution limits
				int written = 0;
				{
					ScopedPhase encodePhase(Phase::kImageEncode);
					written =
						stbi_write_jpg(fileName.c_str(), renderProperties->rx(),
									   renderProperties->ry(), 4, bff, 100);
				}
				if (written == 0)
				{
					std::cout << "Could not write file: "s << fileName << std::endl;
					return 1;
				}

				std::cout << "\t\tFile saved :"s << fileName << std::endl;

				std::cout << "\t\tFinished image generation : "
						  << TimeUtils::decorateTime(start, 2) << std::endl;

				if (statsOn)
				{
					PerfRunInfo info;
					info.scenePath = scenePath;
					info.accel = m_scene->accelName();
					info.imagePath = fileName;
					info.width = renderProperties->rx();
					info.height = renderProperties->ry();
					info.pixelSamples = samples;
					info.rayDepth = rayDepth;
					info.threads = rp.activeThreads();
					info.hardwareThreads = std::thread::hardware_concurrency();
					info.seeded = opt.seeded;
					info.seed = opt.seed;
					info.imageRgbaHash = PerfStats::fnv1a64(
						bff, std::size_t(info.width) * info.height * 4);
					info.jpegHash = PerfStats::fnv1aFile(fileName, &info.jpegBytes);

					PerfStats::setPhase(
						Phase::kTotal,
						std::chrono::duration<double>(
							std::chrono::steady_clock::now() - totalStart)
							.count());
					PerfStats::printSummary(std::cout, info);

					if (!opt.statsOut.empty())
					{
						auto path = opt.statsOut;
						if (frameEnd - frameStart > 1)
						{
							path += "." + std::to_string(i);
						}
						if (!PerfStats::writeJson(path, info))
						{
							std::cerr << "Could not write stats file " << path
									  << std::endl;
							return 1;
						}
						std::cout << "Stats written to " << path << std::endl;
					}
				}
			}

			std::cout << "\tEnded frame " << std::endl;

			if (!m_scene->grids().empty())
			{
				m_scene->grids().front()->playGameOfLife();
				// The voxels changed, so did the occupancy used to skip space.
				m_scene->grids().front()->rebuildOccupancy();
			}
			auto tr = m_scene->camera()->transform();
			tr.setOrigin(tr.origin() - v3s(.001, -0.001, .0011));

			m_scene->camera()->setTransform(tr);
		}
	}
	// return a.exec();
	std::cout << "done!" << std::endl;
	return 0;
}

int printHelp()
{
	//! TODO: this help could be written in external file
	//! and be read in compilation time.
	std::cout << "vxR 0.2.0" << std::endl
			  << std::endl
			  << "\t Non realtime render for solid voxels" << std::endl
			  << "\t vxR [-at] | [-allTests] to perform unit tests." << std::endl
			  << "\t vxR [-help] | [--help] for this help" << std::endl
			  << "\t vxR -r -scene <file.vx> [options] to render" << std::endl
			  << std::endl
			  << "\t render options:" << std::endl
			  << "\t   --accel <name>      acceleration structure (default: "
			  << AccelStructure::defaultName() << "; available:";
	for (const auto &name : AccelStructure::names())
	{
		std::cout << " " << name;
	}
	std::cout << ")" << std::endl
			  << "\t   --seed <n>          reproducible render, same image for any thread count" << std::endl
			  << "\t   --threads <n>       render threads (default: min(cores, 6))" << std::endl
			  << "\t   --res <W>x<H>       override scene resolution" << std::endl
			  << "\t   --samples <n>       override scene pixelSamples" << std::endl
			  << "\t   --out <file.jpg>    output image (default: image.NNNNNNN.jpg)" << std::endl
			  << "\t   --stats             print counters, timings and sanity checks" << std::endl
			  << "\t   --stats-out <file>  write them as JSON (implies --stats)" << std::endl
			  << "\t   --quiet             skip the per ray debug print" << std::endl
			  << "\t env VXR_MESH=ref      old GeoGrid walk"
			  << std::endl
			  << "\t env VXR_MESH=csr      CSR GeoGrid + DDA (default)"
			  << std::endl
			  << "\t env VXR_MESH=bvh      per-mesh triangle BVH (closest-hit)"
			  << std::endl;

	return 0;
}

int main(int argc, char *argv[])
{
	MU::fillRand();
	auto program(0u);
	bool optionSeen{false};
	int exitCode{0};

	// Depending on arguments the execution will be different.
	// Here we extract the elements on the input argument array
	// and record the behaviour for the program.
	if (argc > 1)
	{
		std::cout << "Extra arguments detected " << std::endl;

		for (int i = 1; i < argc; i++)
		{
			std::cout << "\t arc   " << argv[i] << std::endl;
			if (argv[i] == "-r"s)
			{
				program = 3;
				break;
			}

			// Render options are valid anywhere, with or without a value.
			if (const auto taken = renderOptionArgs(argv[i]))
			{
				optionSeen = true;
				i += taken - 1;
				continue;
			}

			if (memcmp(argv[i], "-allTests", 9) == 0 ||
				memcmp(argv[i], "-at", 3) == 0)
			{
				program = 2;
			}

			if (memcmp(argv[i], "--help", 6) == 0 || memcmp(argv[i], "-help", 5) == 0)
			{
				program = 1;
			}

			// If there are more arguments and are not recognized
			// printing help should be executed.
			if (program == 0)
			{
				program = 1;
			}
		}
	}

	// Only render options, without -r: nothing to do but explain.
	if (program == 0 && optionSeen)
	{
		program = 1;
	}

	switch (program)
	{
	case 1:
		printHelp();
		break;
	case 2:
		GridUnitTest::testGrid();
		MUUnitTest::testMU();
		// The differential tests decide the exit code.
		exitCode = AccelUnitTest::run() == 0 ? 0 : 1;
		break;
	default:
		exitCode = executeRenderProcess(argc, argv);
		break;
	}
	return exitCode;
}
