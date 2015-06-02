#include "vxCamera.h"

using namespace vxCore;


vxCamera::vxCamera(std::shared_ptr<const ImageProperties> prop) 
	: m_prop{prop}
{
	srand(time(NULL));
}


vxCamera::vxCamera(const vxVector3d &position, vxVector3d orientation, double focusD, double apertureH, double apertureV)
{
	m_orientation=orientation;
	m_position=position;
	m_focusDistance=focusD;
	m_horizontalAperture=apertureH;
	m_verticalAperture=apertureV;
	
	srand(time(NULL));
}

void vxCamera::setPixelSamples(const unsigned int numSamples)
{
	m_nSamples=numSamples;
	m_sampler.setSamples(m_nSamples);
}

unsigned int vxCamera::getPixelSamples() const
{
	return m_nSamples;
}

vxVector2d vxCamera::coords() const
{ 
	return vxVector2d(xCoord(), yCoord());
}

double vxCamera::xCoord() const
{
	return (m_iteratorPosX) / (double)m_prop->rx();
}

double vxCamera::yCoord() const
{
	return (m_iteratorPosY) / (double)m_prop->ry();
}

void vxCamera::set(const vxVector3d &position, vxVector3d orientation, double focusD, double apertureH, double apertureV) 
{
	m_orientation=orientation;
	m_position=position;
	m_focusDistance=focusD;
	
	if(std::max(apertureH+apertureV,0.0)==0.0)
	{
		m_horizontalAperture = 1.0;
		m_verticalAperture = m_prop->aspectRatio();
	}
	else
	{	
		m_horizontalAperture = apertureH;
		m_verticalAperture = apertureV;
	}
	
	m_iteratorPosX=0;
	m_iteratorPosY=0;
}

vxRayXYZ vxCamera::givemeRay(double x, double y) const
{
	double compX = tan(-m_horizontalAperture/2.0) * (( x * 2.0) -1.0) - 1.0/(double)(2.0 * m_prop->rx()) + m_sampler.x()/(double)(m_prop->rx());
	double compY = tan(-m_verticalAperture/2.0) * (( y * 2.0) -1.0) - 1.0/(double)(2.0 * m_prop->ry()) + m_sampler.y()/(double)(m_prop->ry());
	return vxRayXYZ(compY, compX, m_focusDistance);
}

vxRayXYZ vxCamera::givemeRandRay(double x, double y)
{
	double compX = tan(-m_horizontalAperture/2.0) * (( x * 2.0) -1.0 ) - 1.0/(double)(2.0 * m_prop->rx()) + ((rand()/(double)RAND_MAX))/(double)(m_prop->rx());
	double compY = tan(-m_verticalAperture/2.0) * (( y * 2.0) -1.0 ) - 1.0/(double)(2.0 * m_prop->ry()) + ((rand()/(double)RAND_MAX))/(double)(m_prop->ry());
	return vxRayXYZ( compY , compX , m_focusDistance );
}

void vxCamera::resetRay()
{ 
	m_iteratorPosX=0; 
	m_iteratorPosY=0;
	m_nSamples=1;
}

bool vxCamera::pixIsDone()
{
	return m_sampler.isDone();
}

void vxCamera::resetSampler()
{
	m_sampler.resetIterator();
}

void vxCamera::next(unsigned int skip)
{
	m_iteratorPosX+=skip;
	
	if(m_iteratorPosX >= m_prop->rx())
	{
		m_iteratorPosY++;
		m_iteratorPosX %= m_prop->rx();
	}
}

vxRayXYZ vxCamera::nextSampleRay()
{
	vxRayXYZ ret = givemeRay( xCoord(), yCoord() );
	//todo:remove hardcoded value.
	ret.rotateX(.321);
	m_sampler.next();
	return ret;
}

bool vxCamera::rayIsDone()
{
	return m_iteratorPosY>=(m_prop->ry());
}

vxRayXYZ vxCamera::givemeNextRay(const vxContactBuffer &imagen, double ang)
{
	vxRayXYZ ret;
	ret.set(tan(m_verticalAperture/2.0) * ((imagen.getScanYd()*2)-1) , tan(m_horizontalAperture/2.0)*(( imagen.getScanXd() *2)-1), m_focusDistance);
	ret=ret.unit();
	ret=ret.rotateY(ang);
	return ret;
}

vxRayXYZ vxCamera::givemeRandomRay(double x, double y)
{
	double yrv,xrv;
	xrv=((rand()/double(RAND_MAX)))/m_prop->rx();
	yrv=((rand()/double(RAND_MAX)))/m_prop->ry();
	return vxVector3d(tan(m_horizontalAperture/2.0) * (((y+yrv)*2)-1) ,tan(m_verticalAperture/2)*(((x+xrv)*2.0)-1), m_focusDistance);
}

std::shared_ptr<const ImageProperties> vxCamera::prop() const
{
	return m_prop;
}

void vxCamera::setProp(std::shared_ptr<const ImageProperties> prop)
{
	m_prop = prop;
}
