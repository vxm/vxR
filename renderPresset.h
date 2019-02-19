#ifndef RENDERPRESSET_H
#define RENDERPRESSET_H

#include "ImageProperties.h"
namespace vxCore
{

using prop = ImageProperties;

class RenderPresset
{
public:
	RenderPresset();

	enum class imageQuality
	{
		kLow,
		kScreen,
		k1k,
		kFullHD,
		k2k,
		k3k,
		k4k,
		k8k,
		k16k,
		k32k
	};

	static vxCore::ImageProperties quality(imageQuality qlt)
	{
		switch (qlt)
		{
		case imageQuality::kLow:
			return prop(100, 100);
			break;
		case imageQuality::k1k:
			return prop(1000, 1000);
			break;
		case imageQuality::k2k:
			return prop(2000, 2000);
			break;
		case imageQuality::k3k:
			return prop(3000, 3000);
			break;
		case imageQuality::k4k:
			return prop(4000, 4000);
			break;
		case imageQuality::k8k:
			return prop(8000, 8000);
			break;
		case imageQuality::k16k:
			return prop(16000, 16000);
			break;
		case imageQuality::k32k:
			return prop(32000, 32000);
			break;
		case imageQuality::kFullHD:
			return prop(1920, 1080);
			break;
		case imageQuality::kScreen:
			return prop(750, 750);
			break;
		}

		return prop(100, 100);
	}
};

} // namespace vxCore
#endif // RENDERPRESSET_H
