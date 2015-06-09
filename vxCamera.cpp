#include "vxCamera.h"

using namespace vxCore;


vxCamera::vxCamera(std::shared_ptr<const ImageProperties> prop) 
	: m_prop{prop}
{
	srand(time(NULL));
}


vxCamera::vxCamera(const vxVector3d &position, 
					const vxVector3d &orientation, 
					double focusD, 
					double apertureH, 
					double apertureV)
{
	m_orientation=orientation;
	m_position=position;
	m_focusDistance=focusD;
	m_horizontalAperture=apertureH;
	m_verticalAperture=apertureV;
	
	m_hApTan = tan(-m_horizontalAperture/2.0);
	m_vApTan = tan(-m_verticalAperture/2.0);
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
	return (m_itX) / ((double)m_prop->rx());
}

double vxCamera::yCoord() const
{
	return (m_itY) / ((double)m_prop->ry());
}

void vxCamera::set(const vxVector3d& position, 
					const vxVector3d& orientation, 
					double focusD, 
					double apertureH, 
					double apertureV) 
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
	
	m_itX=0;
	m_itY=0;
	
	m_hApTan = tan(-m_horizontalAperture/2.0);
	m_vApTan = tan(-m_verticalAperture/2.0);
}

vxRayXYZ vxCamera::ray(double x, double y) const
{
	double rx = (double)m_prop->rx();
	double ry = (double)m_prop->ry();

	double compX = m_hApTan * (( x * 2.0)-1.0) 
				   - 1.0/(double)(2.0 * rx)
				   + m_sampler.x()/rx;
	
	double compY = m_vApTan * (( y * 2.0)-1.0)
				   - 1.0/(double)(2.0 * ry)
				   + m_sampler.y()/ry;
	
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
	m_itX=0; 
	m_itY=0;
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
	if(!skip)
		return;

	m_itX+=skip;
	
	if(m_itX >= m_prop->rx())
	{
		m_itY+=(1+(skip/m_prop->rx()));
		m_itX %= m_prop->rx();
	}
}

vxRayXYZ vxCamera::nextSampleRay()
{
	vxRayXYZ ret = ray( xCoord(), yCoord() );
	//todo:remove hardcoded value.
	ret.rotateX(.333);
	m_sampler.next();
	return ret;
}

bool vxCamera::rayIsDone()
{
	return m_itY>=(m_prop->ry());
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
