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
#include "Matrix44.h"
#include "vxBoundingBox.h"
#include "vxGeoGrid.h"
#include "vxColor.h"
#include <set>

namespace vxCore {

class vxGeometry;
class vxShader;

using vxGeometryHandle = std::shared_ptr<vxGeometry>;

class vxGeometry
{
protected:
	std::shared_ptr<vxBoundingBox> m_bb;
	Matrix44 m_transform;
	
	vxColor m_baseColor;
	vxShader* m_shader;
	
public:

	vxGeometry();
	Matrix44 transform() const;
	Matrix44& transform();

	void setTransform(const Matrix44 &transform);

	std::shared_ptr<vxBoundingBox> boundingBox();

	vxColor baseColor() const;
	void setBaseColor(const vxColor &baseColor);
//	vxShaderHandle shader() const;
//	void setShader(const vxShaderHandle &shader);
	
	//renderable interface
	virtual bool throwRay(const vxRay &ray) const;
	virtual int throwRay(const vxRay &ray, vxCollision &collide) const;
	virtual bool hasCollision(const vxRay &ray) const;
	vxShader *shader() const;
	void setShader(vxShader *shader);
};

}
#endif // VXGEOMETRY_H
