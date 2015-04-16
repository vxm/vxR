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
		double resl = 40.0;
		
		// this is a hardcode program to test the rays. 
		//TODO:get rid of this hard-coded values.
		m_grids.push_back(std::make_shared<vxGrid>(resl/2.0, -10.0, (resl*2.20), resl));
		m_grids[0]->setResolution(resl);
		m_grids[0]->createSphere(resl-3, -20.0, (resl*2.20),  (resl/3.0)); // Position, radius
		m_grids[0]->createSphere(16.0, -2.0, (resl*2.20),  (resl/5.0)); // Position, radius
/*
		m_grids[0]->createSphere(55, 4, 13,  17); // Position, radius
		m_grids[0]->createSphere(-15, 4, 79,  17); // Position, radius
		m_grids[0]->createSphere(25, -4, 9,  17); // Position, radius
		m_grids[0]->createSphere(25, -4, 24,  17); // Position, radius
	*/	
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
