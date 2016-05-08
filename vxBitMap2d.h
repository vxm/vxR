#ifndef VXBITMAP2D_H
#define VXBITMAP2D_H
#include "vxVector.h"
#include "vxMap2d.h"
#include "ImageProperties.h"
#include "MathUtils.h"
#include "imageData.h"

namespace vxCore
{

class vxBitMap2d : public vxMap2d
{
	ImageData m_data;
	std::string m_path;
	scalar m_gain{0.0};
	scalar m_gamma{1.0};

public:
	vxBitMap2d(const std::string path);
	///
	/// \brief compute
	/// \param collision
	/// \return 
	///
	virtual vxColor compute(const vxCollision &collision) const override;
	///
	/// \brief m_prop
	///
	std::shared_ptr<ImageProperties> m_prop;
	///
	/// \brief loadImage
	/// \return 
	///
	bool loadImage();
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
};


}

#endif // VXBITMAP2D_H
