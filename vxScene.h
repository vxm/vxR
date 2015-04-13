#ifndef _VXSCENEMC_
#define _VXSCENEMC_

#include<memory>
#include<vector>

#include<vxObject.h>
#include<vxMatrix.h>
#include<vxShader.h>
#include<vxLight.h>
#include<vxGrid.h>
#include<vxCamera.h>
#include<ImageProperties.h>

namespace vxStorage {

class vxScene:public vxObject
{
protected:

	bool m_defaultLight = {true};
	bool m_defaultShader = {true};

	std::shared_ptr<vxLight> m_light = {nullptr};
	std::shared_ptr<vxShader> m_shader = {nullptr};
	std::shared_ptr<vxCamera> m_camera = {nullptr};

	std::vector<std::shared_ptr<vxGrid>> m_grids;
	std::vector<vxGrid> m_cameras;
	std::shared_ptr<ImageProperties> m_prop = {nullptr};

public:

	vxScene(std::shared_ptr<ImageProperties> prop)
		: m_prop(prop)
	{
		m_light.reset(new vxPointLight);
		m_light->setPosition( 5 , 15 , 0 );
		
		m_shader.reset(new vxLambert);
		
		createCamera(vxMatrix());
		createGrid();
	}
	
	~vxScene()
	{
	}

	double getLight(vxCollision &collide)
	{
		return m_light->luminance(collide);
	}

	std::shared_ptr<vxCamera>
	createCamera(const vxMatrix &transform,
					unsigned int samples = 1,
					double hAperture = 0.0,
					double vAperture = 0.0)
	{
		vxCamera cam(m_prop);
	
		cam.set(vxVector3d(0,0,0),
				vxVector3d(0,0,1),
							1.23);

		cam.setPixelSamples(1);
		
		m_camera = std::make_shared<vxCamera>(cam);
	
		return m_camera;
	}

	std::shared_ptr<vxGrid>
	createGrid()
	{
		// this is the grid object
		m_grids.push_back(std::make_shared<vxGrid>(13.0, -5.0, 60.0, 25.0));
		m_grids[0]->setResolution(25);
		m_grids[0]->createSphere(13.0, -5.0, 60.0,  7.2); // Position, radius
		m_grids[0]->createSphere(48.0, -5.0, 60.0,  4.2); // Position, radius
		
		m_grids[0]->createEdges(); // of the grid
	
	#ifdef _DEBUG
		auto na = m_grids[0]->numActiveVoxels();
		std::cout << "Number of active voxels " << na << std::endl;
	#endif
		
		
		return m_grids[0];
	}
	
	std::shared_ptr<vxStorage::ImageProperties> prop() const
	{
		return m_prop;
	}
	
	void setProp(const std::shared_ptr<vxStorage::ImageProperties> &prop)
	{
		m_prop = prop;
	}

	
	// devuelve 0 si no le da a la caja
	// 1 si da y 2 y el resultado es optimo
	int throwRay(const vxVector3d &ray, vxCollision &collide)
	{ 
		if(!m_grids.size())
			return 0;
		
		return m_grids[0]->throwRay(ray,collide);
	}
	
	std::shared_ptr<vxLight> defaultLight() const;
	void setLight(const std::shared_ptr<vxLight> &defaultLight);
	
	std::shared_ptr<vxShader> defaultShader() const;
	void setShader(const std::shared_ptr<vxShader> &defaultShader);
	
	std::shared_ptr<vxCamera> defaultCamera() const;
	void setCamera(const std::shared_ptr<vxCamera> &defaultCamera);
};

}
#endif
