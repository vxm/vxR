#ifndef vxImage_H
#define vxImage_H
#include "vxVector.h"
#include "vxMap2d.h"
#include "ImageProperties.h"
#include "MathUtils.h"
#include "imageData.h"

namespace vxCore
{

class vxImage : public vxMap2d
{
	ImageData m_data;
	std::string m_path;
	
	scalar m_gain{0.0};
	scalar m_gamma{1.0};

public:
	
	vxImage(const std::string path);
	///
	/// \brief compute
	/// \param collision
	/// \return 
	///
	virtual vxColor compute(const vxCollision &collision) const override;
	///
	/// \brief m_prop
	///
	ImagePropertiesHandle m_prop;
	///
	/// \brief loadImage
	/// \return 
	///
	bool load();
	///
	/// \brief path
	/// \return 
	///
	std::string path() const;
	///
	/// \brief setPath
	/// \param path
	///
	void setPath(const std::string &path);
	///
	/// \brief gain
	/// \return 
	///
	scalar gain() const;
	///
	/// \brief setGain
	/// \param gain
	///
	void setGain(const scalar &gain);
	///
	/// \brief gamma
	/// \return 
	///
	scalar gamma() const;
	///
	/// \brief setGamma
	/// \param gamma
	///
	void setGamma(const scalar &gamma);
	///
	/// \brief operator ==
	/// \param other
	/// \return 
	/// Compares the input attributes
	bool operator==(const vxImage& other) const;
};

using vxImageHandle = std::shared_ptr<vxImage>;

}

#endif // vxImage_H
