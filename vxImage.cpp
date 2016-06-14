#include "vxImage.h"
#include <QImage>

using namespace vxCore;

scalar vxImage::gain() const
{
	return m_gain;
}

void vxImage::setGain(const scalar &gain)
{
	m_gain = gain;
}

scalar vxImage::gamma() const
{
	return m_gamma;
}

void vxImage::setGamma(const scalar &gamma)
{
	m_gamma = gamma;
}

bool vxImage::operator==(const vxImage &other) const
{
	return m_path==other.path() 
			&& m_gain == other.m_gain
			&& m_gamma == other.m_gamma;
}

vxImage::vxImage(const std::string path)
	:m_path(path)
{
	//TODO:Constructor very populated.
	//TODO:stolen QImage object, replace with non QT functions. Please soon.
	load();
}

bool vxImage::load()
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

vxColor vxImage::compute(const vxCollision &collision) const
{
	auto px = m_data.get({ 1.0-collision.v(), collision.u()});
	
	return {px[2]/255.0,
			px[1]/255.0,
			px[0]/255.0,
			px[3]/255.0};
}
std::string vxImage::path() const
{
	return m_path;
}

void vxImage::setPath(const std::string &path)
{
	m_path = path;
}



