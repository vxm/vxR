#include <chrono>
#include <sstream>
#include <QApplication>
#include <QImage>

#include <ImageProperties.h>
#include <vxfilemanager.h>
#include <vxRenderProcess.h>
#include "vxRenderMain.h"

using timePoint = std::chrono::time_point<std::chrono::system_clock>;
using timeDuration = std::chrono::duration<double>;

std::string decorateTime(const timePoint &start)
{
	std::stringstream sst;
	timePoint now = std::chrono::system_clock::now();
	timeDuration elapsed_time = timeDuration(now-start);
	double seconds = elapsed_time.count();
	
	if (seconds<60)
	{
		sst << "elapsed time is " << int(seconds)  << " seconds";
	}
	else if(seconds<3600)
	{
		sst << "elapsed time is " << int(seconds/60.0)  << " minutes";
	}
	else if(seconds<(3600*24))
	{
		sst << "elapsed time is " << int(seconds/3600.0)  << " hours";
	}
	
	sst << std::endl;

	return sst.str();
}


int main(int argc, char *argv[])
{
	using render = vxCompute::vxRenderProcess;

	timePoint start = std::chrono::system_clock::now();
	std::cout << "Start program " << std::endl;

 	QApplication a(argc, argv);
	vxRenderMain w;
	//w.show();
	
	// if buffer is created it will then be used to store 
	// the render while rendering.
	std::shared_ptr<vxStorage::ImageProperties> 
			imgDesc(new vxStorage::ImageProperties(480, 480));

	// create the render process
	render rp(imgDesc);
	
	// creates the bucket list (vector) using the img 
	// description.
	rp.createBucketList();
	
	
	std::cout << "Starting render : " << decorateTime(start) << std::endl;
	
	// executes the render.
	rp.execute();
	
	std::cout << "Finished render : " << decorateTime(start) << std::endl;

	// generates an image buffer and fills it 
	// with the render results. Buffer properties
	// are based on ImageProperties stored in the 
	// render process object.
	auto bff = rp.generateImage();
	
	std::cout << "Ended creation of the image: " << decorateTime(start) << std::endl;

	// storing an image from the buffer obtained.
	if (bff!=nullptr)
	{
		QImage img( bff,
					imgDesc->rx(),
					imgDesc->ry(),
					QImage::Format_RGBA8888);

		std::string baseName("image.1.bmp");

		std::string fileName = 
				vxFileManager::makeUnique(baseName);

		img.save(QString(fileName.c_str()),"BMP");
		
		std::cout << "Finished image generation : " << decorateTime(start) << std::endl;
	}
	
	//return a.exec();
	std::cout << "done!" << std::endl;
	return 0;
}
