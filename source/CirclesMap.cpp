#include "CirclesMap.h"

using namespace vxCore;

Color CirclesMap::compute(const Collision &collision, const Color &innerColor,
						  const Color &outerColor) const
{
	Color outColor(outerColor);

	const auto &uv = collision.uv();
	auto dst = sqrtf(powf(0.5 - uv.x(), 2.0) +
					 powf(0.5 - uv.y(), 2.0)); // collision.uv().distance(v2s(0.5,
											   // 0.5));

	if (dst < .35)
	{
		outColor = innerColor;
	}

	return outColor;
}

Color vxCore::CirclesMap::compute(const Collision &collision) const {
    return Color::white;
}
