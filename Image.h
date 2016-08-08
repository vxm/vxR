#ifndef vxImage_H
#define vxImage_H
#include "Vector.h"
#include "Map2d.h"
#include "ImageProperties.h"
#include "MathUtils.h"
#include "imageData.h"

namespace vxCore
{

class Image : public Map2d
{
	ImageData m_data;
	std::string m_path;
	
	scalar m_gain{0.0};
	scalar m_gamma{1.0};

public:
	
	Image(const std::string path);
	///
	/// \brief compute
	/// \param collision
	/// \return 
	///
	virtual Color compute(const Collision &collision) const override;
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
	bool operator==(const Image& other) const;
};

using vxImageHandle = std::shared_ptr<Image>;

}

#endif // vxImage_H
