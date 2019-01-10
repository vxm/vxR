#include "Image.h"
#include <QImage>

using namespace vxCore;

scalar Image::gain() const { return m_gain; }

void Image::setGain(const scalar &gain) { m_gain = gain; }

scalar Image::gamma() const { return m_gamma; }

void Image::setGamma(const scalar &gamma) { m_gamma = gamma; }

bool Image::operator==(const Image &other) const
{
	return m_path == other.path() && m_gain == other.m_gain &&
		   m_gamma == other.m_gamma;
}

Image::Image(const std::string &path) : m_path(path)
{
	// TODO:Constructor very populated.
	// TODO:stolen QImage object, replace with non QT functions. Please soon.
	load();
}

bool Image::load()
{
	// TODO:I am reading this image as many times as it is invoked.
	if (m_path.empty())
	{
		return false;
	}

	QImage img;
	bool envSuccess = img.load(m_path.c_str());
	if (!envSuccess)
	{
		std::cout << "Could not load image '" << m_path << "' for environment"
				  << std::endl;
		return false;
	}

	std::cout << "Loaded image '" << m_path << "' for environment" << std::endl;

	m_prop = std::make_shared<ImageProperties>(img.width(), img.height());

	m_prop->setFormat(ImageProperties::ImgFormat::k8);
	m_data.setProperties(m_prop);
	m_data.initialise();
	m_data.copyData(img.bits());
	return true;
}

Color Image::compute(const Collision &collision) const
{
	if (m_data.hasData())
	{
		auto px = m_data.get(v2s(1.0 - collision.v(), collision.u()));

		Color c = {px[2] / scalar(255.0), px[1] / scalar(255.0),
				   px[0] / scalar(255.0), px[3] / scalar(255.0)};
		c.applyCurve(gamma(), gain());

		return c;
	}

	auto r = pow(collision.v() + m_gain, m_gamma);
	return {r, r, r, r};
}
std::string Image::path() const { return m_path; }

void Image::setPath(const std::string &path) { m_path = path; }
