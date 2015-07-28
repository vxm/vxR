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
#include "vxVector.h"
#include "vxGrid.h"
#include "vxGridUnitTest.h"

static const std::string baseName("image.0000001.tif");
using timePoint = std::chrono::time_point<std::chrono::system_clock>;
using render = vxCompute::vxRenderProcess;
using namespace vxCore;

int executeRenderProcess(int argc, char *argv[])
{
	timePoint start = std::chrono::system_clock::now();

	std::cout << "Start program" << std::endl;

	QApplication a(argc, argv);
	vxRenderMain w;
	//w.show();
	
	// Img properties for render.
	auto imgDesc = std::make_shared<ImageProperties>(700,700);
	
	// create the render process
	render rp(imgDesc);
	
	// creates the bucket list (vector) using the img 
	// description.
	rp.createBucketList();
	
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
					imgDesc->rx(),
					imgDesc->ry(),
					QImage::Format_RGBA8888);

		std::string fileName = 
				FileUtils::makeUnique(baseName);

		//TODO: investigate other formats and resolution limits
		img.save(QString(fileName.c_str()),"TIFF",100);
		
		std::cout << "File saved : " 
					<< fileName 
					<< std::endl;
		
		std::cout << "Finished image generation : " 
				<< TimeUtils::decorateTime(start,2) << std::endl;
	}
	//return a.exec();
	std::cout << "done!" << std::endl;
	return 0;
}

int main(int argc, char *argv[])
{
	unsigned int program{0};
	if(argc>1)
	{
		std::cout << "Extra arguments detected " << std::endl;
		
		for(int i=1; i<argc; i++)
		{
			std::cout << "\t arc   " << argv[i] << std::endl;
			if(memcmp(argv[i],"-allTests", 9)
				|| memcmp(argv[i],"-at", 3))
			{
				program = 1;
			}
		}
	}
	
	switch(program)
	{
		case 1:
			vxGridUnitTest::testGrid();
			MathUtilsUnitTest::testMathUtils();
		break;
		default:
			executeRenderProcess(argc, argv);
		break;
	}

	return 1;
}


