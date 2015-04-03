#include <QApplication>
#include <QImage>

#include <ImageProperties.h>
#include <chrono>
#include <vxfilemanager.h>
#include <vxRenderProcess.h>
#include "vxRenderMain.h"



int main(int argc, char *argv[])
{
	using timePoint = std::chrono::time_point<std::chrono::system_clock>;
	
	timePoint start, now;
	start = std::chrono::system_clock::now();

	std::cout << "Start program " << std::endl;

	using render = vxCompute::vxRenderProcess;

 	QApplication a(argc, argv);
	vxRenderMain w;
	//w.show();
	
	// if buffer is created it will then be used to store 
	// the render while rendering.
	std::shared_ptr<vxStorage::ImageProperties> 
			imgDesc(new vxStorage::ImageProperties(540, 540));

	// create the render process
	render rp(imgDesc);
	
	// creates the bucket list (vector) using the img 
	// description.
	rp.createBucketList();
	
	now = std::chrono::system_clock::now();
	auto elapsed_seconds = std::chrono::duration<double>(now-start);
	std::cout << "Starting render : " << elapsed_seconds.count() << std::endl;
	
	// executes the render.
	rp.execute();
	
	now = std::chrono::system_clock::now();
	elapsed_seconds = now-start;
	std::cout << "finished render : " << elapsed_seconds.count() << std::endl;

	// generates an image buffer and fills it 
	// with the render results. Buffer properties
	// are based on ImageProperties stored in the 
	// render process object.
	auto bff = rp.generateImage();
	
	
	now = std::chrono::system_clock::now();
	elapsed_seconds = now-start;
	std::cout << "finished image generation : " << elapsed_seconds.count() << std::endl;

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
		
		now = std::chrono::system_clock::now();
		elapsed_seconds = now-start;
		std::cout << "finished image generation : " << elapsed_seconds.count() << std::endl;
	}
	
	//return a.exec();
	std::cout << "done!" << std::endl;
	return 0;
}
