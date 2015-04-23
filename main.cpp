#include <chrono>
#include <sstream>
#include <QApplication>
#include <QImage>

#include <ImageProperties.h>
#include <FileUtils.h>
#include <vxRenderProcess.h>
#include <vxRenderMain.h>
#include <TimeUtils.h>
#include <MathUtils.h>
#include <vxVector.h>
#include <vxGrid.h>

static const std::string baseName("image.1.bmp");
using timePoint = std::chrono::time_point<std::chrono::system_clock>;
using render = vxCompute::vxRenderProcess;

int main(int argc, char *argv[])
{
	timePoint start = std::chrono::system_clock::now();
	
	std::cout << "Start program" << std::endl;

	QApplication a(argc, argv);
	vxRenderMain w;
	//w.show();
	
	// Img properties for render.
	auto imgDesc = 
			std::make_shared<vxStorage::ImageProperties>
			(512, 512);
	
	// create the render process
	render rp(imgDesc);
	
	// creates the bucket list (vector) using the img 
	// description.
	rp.createBucketList();
	
	std::cout << "Starting render : " << TimeUtils::decorateTime(start) << std::endl;
	
	// executes the render.
	rp.execute();
	
	std::cout << "Finished render : " 
			<< TimeUtils::decorateTime(start) << std::endl;

	// generates an image buffer and fills it 
	// with the render results. Buffer properties
	// are based on ImageProperties stored in the 
	// render process object.
	auto bff = rp.generateImage();

	std::cout << "Ended creation of the image: " 
			<< TimeUtils::decorateTime(start) << std::endl;

	// storing an image from the buffer obtained.
	if (bff!=nullptr)
	{
		QImage img( bff,
					imgDesc->rx(),
					imgDesc->ry(),
					QImage::Format_RGBA8888);

		std::string fileName = 
				FileUtils::makeUnique(baseName);

		img.save(QString(fileName.c_str()),"BMP");
		
		std::cout << "Finished image generation : " 
				<< TimeUtils::decorateTime(start) << std::endl;
	}

	//return a.exec();
	std::cout << "done!" << std::endl;
	return 0;
}

