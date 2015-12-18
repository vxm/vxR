#ifndef VXGEOMETRY_H
#define VXGEOMETRY_H

#include <typeinfo>
#include <vector>
#include <atomic>
#include "vxVector.h"
#include "vxRay.h"
//#include "vxShader.h"
#include "vxCollision.h"
#include "vxTriRef.h"
#include "vxMatrix.h"
#include "vxBoundingBox.h"
#include "vxGeoGrid.h"
#include "vxColor.h"
#include <set>

namespace vxCore {

class vxGeometry;
//class vxShader;

using vxGeometryHandle = std::shared_ptr<vxGeometry>;

class vxGeometry
{
protected:
	std::shared_ptr<vxBoundingBox> m_bb;
	vxMatrix m_transform;
	
	vxColor m_baseColor;
	//std::shared_ptr<vxShader> m_shader;
	
public:

	vxGeometry();
	vxMatrix transform() const;
	vxMatrix& transform();

	void setTransform(const vxMatrix &transform);

	std::shared_ptr<vxBoundingBox> boundingBox();

	vxColor baseColor() const;
	void setBaseColor(const vxColor &baseColor);
//	std::shared_ptr<vxShader> shader() const;
//	void setShader(const std::shared_ptr<vxShader> &shader);
	
	//renderable interface
	virtual bool throwRay(const vxRay &ray) const;
	virtual int throwRay(const vxRay &ray, vxCollision &collide) const;
	virtual bool hasCollision(const vxRay &ray) const;
};

}
#endif // VXGEOMETRY_H
