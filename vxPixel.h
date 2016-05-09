#ifndef VXPIXEL_H
#define VXPIXEL_H

#include "vxColor.h"


namespace vxCore {

class vxPixel:public vxColor
{
private:
	unsigned int m_comp{0u};

public:
	///
	/// \brief vxPixel
	///
	vxPixel();
	///
	/// \brief vxPixel
	/// \param r
	/// \param g
	/// \param b
	/// \param a
	///
	vxPixel(const scalar r, const scalar g, const scalar b, const scalar a);
	///
	/// \brief add
	/// \param r
	/// \param g
	/// \param b
	///
	void add(const scalar r, const scalar g, const scalar b, const scalar a);
	///
	/// \brief reset
	///
	void reset();
	///
	/// \brief getResult
	/// \return 
	///
	vxColor getResult() const;
	///
	/// \brief setResult
	///
	void setResult();
	///
	/// \brief add
	/// \param color
	///
	void add(const vxColor &color);
	///
	/// \brief add
	/// \param color
	///
	void add(const vxPixel &color);
	///
	/// \brief operator =
	/// \param otro
	/// \return 
	///
	vxPixel operator=(const vxPixel &other);
	///
	/// \brief operator =
	/// \param otro
	/// \return 
	///
	vxPixel operator=(const vxColor &other);
};

}
#endif // VXPIXEL_H

