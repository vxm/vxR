#include <chrono>
#include <sstream>
#include <QApplication>
#include <QImage>

#include "FileUtils.h"
#include "vxRenderProcess.h"
#include "ImageProperties.h"
#include "vxRenderMain.h"
#include "renderPresset.h"
#include "TimeUtils.h"
#include "MathUtils.h"
#include "MathUtilsUnitTest.h"
#include "vxVector.h"
#include "vxGrid.h"
#include "vxGridUnitTest.h"
#include "vxSceneParser.h"

static const std::string baseName("image.0000001.tif");
using namespace std::string_literals;
using timePoint = std::chrono::time_point<std::chrono::system_clock>;
using render = vxCompute::vxRenderProcess;
using namespace vxCore;

int executeRenderProcess(int argc, char *argv[])
{
	timePoint start = std::chrono::system_clock::now();
	std::cout << "Start program" << std::endl;

	std::string scenePath;
	// Look for a scene argument.
	bool sceneParam{false};
	for(int i=1; i<argc; i++)
	{
		if(sceneParam)
		{
			scenePath = argv[i];
		}
		
		if("-scene"s==std::string(argv[i]))
		{
			sceneParam = true;
		}
	}
	std::cout << "\t Captured scene file argument: " << scenePath << std::endl;
	
	//TODO:find a home for this next two lines of code.
	auto sceneParser = std::make_shared<vxSceneParser>(scenePath);
	sceneParser->procesScene();
	for(auto&& node: sceneParser->getNodesByType("vxRenderSettings"))
	{
		const auto resolution = node->getVector2dAttribute("resolution");
		const auto samples = node->getIntAttribute("pixelSamples");
		const auto reflectionSamples = node->getIntAttribute("reflectionSamples");
		
		// Img properties for render.
		auto renderProperties = std::make_shared<ImageProperties>(resolution.x(),
																  resolution.y());
		// create the render process
		render rp(renderProperties, samples);
		rp.setDatabase(sceneParser);
		rp.setVisSamples(samples);
		rp.setReflectionSamples(reflectionSamples);
		
		// creates the bucket list (vector) using the img 
		// description.
		//rp.createBucketList(samples);
		
		std::cout << "Render process: " << TimeUtils::decorateTime(start,2) << std::endl;
		
		// executes the render.
		if(rp.execute()==vxStatus::code::kError)
		{
			return 1;
		}
		
		std::cout << "Finished render : " 
				<< TimeUtils::decorateTime(start,2) << std::endl;

		// generates an image buffer and fills it 
		// with the render results. Buffer properties
		// are based on ImageProperties stored in the 
		// render process object.
		auto bff = rp.generateImage();
	
		std::cout << "Ended creation of the image: " 
				<< TimeUtils::decorateTime(start,2) << std::endl;
	
		// storing an image from the buffer obtained.
		if (bff!=nullptr)
		{
			QImage img( bff,
						renderProperties->rx(),
						renderProperties->ry(),
						QImage::Format_RGBA8888);

			std::string fileName = 
					FileUtils::makeUnique(baseName);

			std::cout << "Opening file: " 
					  << fileName 
					  << " to write." 
					  << std::endl;

			//TODO: investigate other formats and resolution limits
			img.save(QString(fileName.c_str()),"TIFF",100);
			
			std::cout << "File saved : " 
						<< fileName 
						<< std::endl;
			
			std::cout << "Finished image generation : " 
					<< TimeUtils::decorateTime(start,2) << std::endl;
		}
	}
	//return a.exec();
	std::cout << "done!" << std::endl;
	return 0;
}

int printHelp()
{
	//!TODO: this help could be written in external file
	//!and be read in compilation time.
	std::cout << "vxR 0.2.0" << std::endl << std::endl
				<< "\t Non realtime render for solid voxels" << std::endl
				<< "\t vxR [-at] | [-allTests] to perform unit tests." << std::endl
				<< "\t vxR [-help] | [--help] for this help" << std::endl;

	return 0;
}


int main(int argc, char *argv[])
{
	auto program{0u};
	
	// Depending on arguments the execution will be different.
	// Here we extract the elements on the input argument array
	// and record the behaviour for the program.
	if(argc>1)
	{
		std::cout << "Extra arguments detected " << std::endl;
		
		for(int i=1; i<argc; i++)
		{
			std::cout << "\t arc   " << argv[i] << std::endl;
			if("-r"s==argv[i])
			{
				program = 3;
				break;
			}
			
			if(memcmp(argv[i],"-allTests", 9)==0
				|| memcmp(argv[i],"-at", 3)==0)
			{
				program = 2;
			}
			
			if(memcmp(argv[i],"--help", 6)==0
				|| memcmp(argv[i],"-help", 5)==0)
			{
				program = 1;
			}

			// If there are more arguments and are not recognized
			// printing help should be executed.
			if(program==0)
			{
				program = 1;
			}
		}
	}
	
	switch(program)
	{
		case 1:
			printHelp();
		break;
		case 2:
			vxGridUnitTest::testGrid();
			MUUnitTest::testMU();
		break;
		default:
			executeRenderProcess(argc, argv);
		break;
	}

	return 1;
}


