#include "vxBitMap2d.h"
#include <QImage>

using namespace vxCore;

vxBitMap2d::vxBitMap2d()
{
	//TODO:Constructor very populated.
	//TODO:stolen QImage object, replace with non QT functions. Please soon.
	QImage img;
	img.load("/home/john/code/vxR/Barcelona_Rooftops/Barce_Rooftop_C_8k.jpg", "jpg");
	m_prop = std::make_shared<ImageProperties>(img.width(), img.height());
	
	m_prop->setFormat(ImageProperties::ImgFormat::k8);
	m_data.setProperties(m_prop);
	m_data.initialise();
	m_data.copyData( img.bits() );
}

vxColor vxBitMap2d::compute(const vxCollision &collision) const
{
	auto remap = vxVector2d( 1.0-collision.v(), collision.u()); 
	unsigned char *px = m_data.get(remap);
	
	int r = (unsigned char)(*px);
	int g = (unsigned char)(*(px+1));
	int b = (unsigned char)(*(px+2));
	
	vxColor outColor(vxColor::lookup256(r,g,b));

	return outColor;
}


