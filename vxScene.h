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
		m_shader = std::make_shared<vxLambert>();
		m_light = std::make_shared<vxPointLight>();
		m_light->setPosition(4, 8, -3);
		
		createCamera(vxMatrix(), 2);
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
		m_camera = std::make_shared<vxCamera>(m_prop);
		m_camera->set(	vxVector3d(0,0,0),
						vxVector3d(0,0,1),
									1.23);
		m_camera->setPixelSamples(samples);
		return m_camera;
	}

	std::shared_ptr<vxGrid>
	createGrid()
	{
		// this is the grid object
		const double resl = 15.0;

		vxVector3d p{resl/2.0, -1.0, resl*2.20};
		
		// this is a hardcode program to test the rays. 
		//TODO:get rid of this hard-coded values.
		m_grids.push_back(std::make_shared<vxGrid>(p.x(), p.y(), p.z(), resl));
		m_grids[0]->setResolution(resl);
		m_grids[0]->createSphere(p.x(), p.y(), p.z(),  (resl/3.0)); // Position, radius		
		m_grids[0]->createSphere(p.x()+(resl/3.0), p.y(), p.z(),  (resl/6.0)); // Position, radius
		m_grids[0]->createSphere(p.x()-(resl/3.0), p.y(), p.z(),  (resl/6.0)); // Position, radius
		m_grids[0]->createSphere(p.x(), p.y()+(resl/3.0), p.z(),  (resl/6.0)); // Position, radius
		m_grids[0]->createSphere(p.x(), p.y()-(resl/3.0), p.z(),  (resl/6.0)); // Position, radius
		m_grids[0]->createSphere(p.x(), p.y(), p.z()+(resl/3.0),  (resl/6.0)); // Position, radius
		m_grids[0]->createSphere(p.x(), p.y(), p.z()-(resl/3.0),  (resl/6.0)); // Position, radius
		//m_grids[0]->createRandom(0.0007);

		m_grids[0]->createEdges(); // of the grid
	#ifdef _DEBUG
		auto na = m_grids[0]->numActiveVoxels();
		auto totals = m_grids[0]->getNumberOfVoxels();
		std::cout << "Number of active voxels " << na << " of " << totals << std::endl;
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
	int throwRay(const vxVector3d &ray, vxCollision &collide);
	
	std::shared_ptr<vxLight> defaultLight() const;
	void setLight(const std::shared_ptr<vxLight> &defaultLight);
	
	std::shared_ptr<vxShader> defaultShader() const;
	void setShader(const std::shared_ptr<vxShader> &defaultShader);
	
	std::shared_ptr<vxCamera> defaultCamera() const;
	void setCamera(const std::shared_ptr<vxCamera> &defaultCamera);
};

}
#endif
