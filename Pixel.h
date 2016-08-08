#ifndef VXPIXEL_H
#define VXPIXEL_H

#include "Color.h"


namespace vxCore {

class Pixel final:public Color
{
private:
	unsigned int m_comp{0u};

public:
	///
	/// \brief vxPixel
	///
	Pixel();
	///
	/// \brief vxPixel
	/// \param r
	/// \param g
	/// \param b
	/// \param a
	///
	Pixel(const scalar r, const scalar g, const scalar b, const scalar a);
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
	Color getResult() const;
	///
	/// \brief setResult
	///
	void setResult();
	///
	/// \brief add
	/// \param color
	///
	void add(const Color &color);
	///
	/// \brief add
	/// \param color
	///
	void add(const Pixel &color);
	///
	/// \brief operator =
	/// \param otro
	/// \return 
	///
	Pixel operator=(const Pixel &other);
	///
	/// \brief operator =
	/// \param otro
	/// \return 
	///
	Pixel operator=(const Color &other);
};

}
#endif // VXPIXEL_H

