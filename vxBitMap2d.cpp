#include "vxBitMap2d.h"
#include <QImage>

using namespace vxCore;

vxBitMap2d::vxBitMap2d(const std::__cxx11::string path)
	:m_path(path)
{
	//TODO:Constructor very populated.
	//TODO:stolen QImage object, replace with non QT functions. Please soon.
	loadImage();
}

bool vxBitMap2d::loadImage()
{
	if (!m_path.length())
	{
		return false;
	}
	
	QImage img;
	bool envSuccess = img.load(m_path.c_str());
	if(!envSuccess)
	{
		std::cout << "Could not load image '" 
				  << m_path 
				  << "' for environment" 
				  << std::endl;
		return false;
	}
	else
	{
		std::cout << "Loaded image '" 
				  << m_path 
				  << "' for environment" 
				  << std::endl;
	}
	
	m_prop = std::make_shared<ImageProperties>(img.width(), img.height());
	
	m_prop->setFormat(ImageProperties::ImgFormat::k8);
	m_data.setProperties(m_prop);
	m_data.initialise();
	m_data.copyData( img.bits() );
	return true;
}

vxColor vxBitMap2d::compute(const vxCollision &collision) const
{
	auto remap = vxVector2d{ 1.0-collision.v(), collision.u()}; 
	unsigned char *px = m_data.get(remap);
	
	int b = (unsigned char)(*px);
	px++;
	
	int g = (unsigned char)(*px);
	px++;
	
	int r = (unsigned char)(*px);
	
	vxColor outColor(0.2*(r/255.0),
					 0.2*(g/255.0),
					 0.2*(b/255.0));

	return outColor;
}
std::string vxBitMap2d::path() const
{
	return m_path;
}

void vxBitMap2d::setPath(const std::string &path)
{
	m_path = path;
}



