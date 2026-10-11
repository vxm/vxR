#include "Image.h"
#include "stb_image.h"
#include "tinyexr.h"

#include <algorithm>
#include <cstring>

using namespace vxCore;

namespace
{

bool hasExtension(const std::string &path, const char *extension)
{
	const auto extLength = std::strlen(extension);
	if (path.size() < extLength)
	{
		return false;
	}

	const auto start = path.size() - extLength;
	for (size_t i = 0; i < extLength; ++i)
	{
		char left = path[start + i];
		char right = extension[i];
		if (left >= 'A' && left <= 'Z')
		{
			left = static_cast<char>(left - 'A' + 'a');
		}
		if (right >= 'A' && right <= 'Z')
		{
			right = static_cast<char>(right - 'A' + 'a');
		}
		if (left != right)
		{
			return false;
		}
	}

	return true;
}

bool loadExr(const std::string &path, ImageData &data, ImagePropertiesHandle &prop)
{
	float *rgba = nullptr;
	int width = 0;
	int height = 0;
	const char *error = nullptr;
	const int status = LoadEXR(&rgba, &width, &height, path.c_str(), &error);
	if (status != TINYEXR_SUCCESS)
	{
		std::cout << "Could not load EXR '" << path << "'";
		if (error != nullptr)
		{
			std::cout << ": " << error;
			FreeEXRErrorMessage(error);
		}
		std::cout << std::endl;
		return false;
	}

	prop = std::make_shared<ImageProperties>(width, height);
	prop->setFormat(ImageProperties::ImgFormat::k32);
	prop->setChannels(ImageProperties::ImgChannels::kRGBA);
	data.setProperties(prop);
	data.adoptFloatRGBA(rgba);

	float lowest = rgba[0];
	float highest = rgba[0];
	const size_t sampleCount = static_cast<size_t>(width) * static_cast<size_t>(height) * 4;
	for (size_t i = 0; i < sampleCount; i += 4)
	{
		lowest = std::min(lowest, std::min(rgba[i], std::min(rgba[i + 1], rgba[i + 2])));
		highest = std::max(highest, std::max(rgba[i], std::max(rgba[i + 1], rgba[i + 2])));
	}

	std::cout << "Loaded HDR EXR '" << path << "' " << width << "x" << height
			  << " range " << lowest << " .. " << highest << std::endl;
	return true;
}

} // namespace

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
	load();
}

bool Image::load()
{
	// TODO:I am reading this image as many times as it is invoked.
	if (m_path.empty())
	{
		return false;
	}

	if (m_data.hasData())
	{
		return true;
	}

	if (hasExtension(m_path, ".exr"))
	{
		return loadExr(m_path, m_data, m_prop);
	}

	int width = 0;
	int height = 0;
	int channels = 0;
	unsigned char *pixels =
		stbi_load(m_path.c_str(), &width, &height, &channels, 4);
	if (pixels == nullptr)
	{
		std::cout << "Could not load image '" << m_path << "' for environment"
				  << std::endl;
		return false;
	}

	std::cout << "Loaded image '" << m_path << "' for environment" << std::endl;

	m_prop = std::make_shared<ImageProperties>(width, height);

	m_prop->setFormat(ImageProperties::ImgFormat::k8);
	m_data.setProperties(m_prop);
	m_data.initialise();
	m_data.copyData(pixels);
	stbi_image_free(pixels);
	return true;
}

Color Image::compute(const Collision &collision) const
{
	if (m_data.hasData())
	{
		const auto px = m_data.sample(v2s(1.0 - collision.v(), collision.u()));

		Color c = {px[0], px[1], px[2], px[3]};
		c.applyCurve(gamma(), gain());

		return c;
	}

	auto r = pow(collision.v() + m_gain, m_gamma);
	return {r, r, r, r};
}
std::string Image::path() const { return m_path; }

void Image::setPath(const std::string &path) { m_path = path; }
