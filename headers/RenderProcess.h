#ifndef VXRENDERPROCESS_H
#define VXRENDERPROCESS_H

#include <atomic>
#include <memory>

#include "ImageProperties.h"
#include "Process.h"
#include "Scene.h"
#include "SceneParser.h"
#include "Status.h"
#include "imageData.h"
namespace vxCompute
{

using namespace vxCore;

/* This class performs the render.
 * it takes a scene, with at least one light, one camera
 * a geometry and performs the render, storing the results
 * in a vxContactBuffer
 */

class RenderProcess : public Process
{
private:
	///
	/// \brief m_properties
	///
	std::shared_ptr<const ImageProperties> m_properties;
	///
	/// \brief m_scene
	///
	Scene *m_scene = nullptr;
	///
	/// \brief m_finished
	///
	std::atomic_bool m_finished;
	///
	/// \brief m_nThreads
	///
	unsigned int m_nThreads;

	///
	/// \brief m_giMultiplier
	///
	scalar m_giMultiplier = 1.0;
	///
	/// \brief m_progress
	///
	std::atomic<scalar> m_progress{0.0};
	///
	/// \brief m_imageData
	///
	ImageData m_imageData;
	///
	/// \brief m_contactBuffer
	///
	ContactBuffer m_contactBuffer;
	///
	unsigned int m_samples = 1u;
	///
	/// \brief m_c_invSamples
	///
	scalar m_c_invSamples = scalar(1.0) / scalar(m_samples);
	///
	/// \brief m_rayDepth
	///
	unsigned int m_rayDepth = 0u;

public:
	// constructor with imageproperties propagation
	RenderProcess(ImagePropertiesHandle &prop, unsigned int samples);

	// constructor with imageproperties propagation
	virtual ~RenderProcess() {}

	virtual Status::code execute();

	///
	/// \brief render
	/// \param by
	/// \param offset
	/// \return
	///
	Status::code render(unsigned int by = 1u, unsigned int offset = 0u);

	// void createBucketList(unsigned int samples);
	const unsigned char *generateImage();
	///
	/// \brief setDatabase
	/// \return
	///
	Status setDatabase(std::shared_ptr<SceneParser>);
	///
	/// \brief setImageProperties
	/// \param imageProperties
	///
	void
	setImageProperties(std::shared_ptr<const ImageProperties> imageProperties);
	///
	/// \brief imageProperties
	/// \return
	///
	std::shared_ptr<const ImageProperties> imageProperties() const;
	///
	/// \brief setScene
	/// \param scene
	///
	void setScene(Scene *scene);
	///
	/// \brief scene
	/// \return
	///
	Scene *scene() const;
	///
	/// \brief setNMaxThreads
	/// \param nMaxThreads
	///
	void setNMaxThreads(unsigned int nMaxThreads);
	///
	/// \brief nMaxThreads
	/// \return
	///
	unsigned int nMaxThreads() const;
	///
	/// \brief progress
	/// \return
	///
	scalar progress() const;
	///
	/// \brief visSamples
	/// \return
	///
	unsigned int visSamples() const;
	///
	/// \brief setVisSamples
	/// \param visSamples
	///
	void setVisSamples(unsigned int visSamples);
	///
	/// \brief rayDepth
	/// \return
	///
	unsigned int rayDepth() const;
	///
	/// \brief setRayDepth
	/// \param rayDepth
	///
	void setRayDepth(unsigned int rayDepth);
	///
	/// \brief giMultiplier
	/// \return
	///
	scalar giMultiplier() const;
	///
	/// \brief setGIMultiplier
	/// \param giMultiplier
	///
	void setGIMultiplier(const scalar &giMultiplier);
	///
	/// \brief samples
	/// \return
	///
	unsigned int samples() const;
	///
	/// \brief setSamples
	/// \param samples
	///
	void setSamples(unsigned int samples);
	///
	/// \brief properties
	/// \return
	///
	std::shared_ptr<const ImageProperties> properties() const;
	///
	/// \brief setProperties
	/// \param properties
	///
	void setProperties(const std::shared_ptr<const ImageProperties> &properties);
	///
	/// \brief computeEnergyAndColor
	/// \param ray
	/// \param col
	/// \return
	///
	Color computeEnergyAndColor(const Ray &ray, Collision &col,
								unsigned int bounces);
	///
	/// \brief computeReflection
	/// \param iter
	/// \param ray
	/// \param col
	/// \return
	///
	Color computeReflection(const Ray &ray, const Collision &col,
							unsigned int bounces);
	///
	/// \brief computeGI
	/// \param iter
	/// \param ray
	/// \param col
	/// \return
	///
	Color computeGI(const Collision &col, unsigned int bounces);
	///
	/// \brief getShader
	/// \param col
	/// \return
	/// Gets the proper shader, either stored in geometry
	/// or scene default.
	Shader *getShader(const Collision &col);
};

#endif // VXRENDERPROCESS_H
}
