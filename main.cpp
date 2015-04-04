#include <chrono>
#include <sstream>
#include <QApplication>
#include <QImage>

#include <ImageProperties.h>
#include <vxfilemanager.h>
#include <vxRenderProcess.h>
#include "vxRenderMain.h"
#include <vxtimeutilities.h>

static const std::string baseName("image.1.bmp");

int main(int argc, char *argv[])
{
	using timePoint = std::chrono::time_point<std::chrono::system_clock>;
	using render = vxCompute::vxRenderProcess;
	timePoint start = std::chrono::system_clock::now();
	
	std::cout << "Start program" << std::endl;

 	QApplication a(argc, argv);
	vxRenderMain w;
	//w.show();
	
	// if buffer is created it will then be used to store 
	// the render while rendering.
	std::shared_ptr<vxStorage::ImageProperties> 
			imgDesc(new vxStorage::ImageProperties(78, 78));

	// create the render process
	render rp(imgDesc);
	
	// creates the bucket list (vector) using the img 
	// description.
	rp.createBucketList();
	
	
	std::cout << "Starting render : " << vxTimeUtilities::decorateTime(start) << std::endl;
	
	// executes the render.
	rp.execute();
	
	std::cout << "Finished render : " << vxTimeUtilities::decorateTime(start) << std::endl;

	// generates an image buffer and fills it 
	// with the render results. Buffer properties
	// are based on ImageProperties stored in the 
	// render process object.
	auto bff = rp.generateImage();
	
	std::cout << "Ended creation of the image: " << vxTimeUtilities::decorateTime(start) << std::endl;

	// storing an image from the buffer obtained.
	if (bff!=nullptr)
	{
		QImage img( bff,
					imgDesc->rx(),
					imgDesc->ry(),
					QImage::Format_RGBA8888);

		std::string fileName = 
				vxFileManager::makeUnique(baseName);

		img.save(QString(fileName.c_str()),"BMP");
		
		std::cout << "Finished image generation : " << vxTimeUtilities::decorateTime(start) << std::endl;
	}
	
	//return a.exec();
	std::cout << "done!" << std::endl;
	return 0;
}
