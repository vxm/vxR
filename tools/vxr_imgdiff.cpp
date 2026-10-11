// Compares two images and prints how far apart they are.
//
//   vxr_imgdiff a.jpg b.jpg [--max-mae <x>] [--json]
//
// Both images are loaded as 8 bit RGB, the alpha channel is ignored. Errors
// are in 0..255 units. Exit status: 0 when the mean absolute error is not above
// --max-mae (default 0, so any difference fails), 1 when it is, 2 when an image
// cannot be loaded or the sizes differ.

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

int main(int argc, char *argv[])
{
	std::string pathA;
	std::string pathB;
	double maxMae = 0.0;
	bool json = false;

	for (int i = 1; i < argc; i++)
	{
		const std::string arg(argv[i]);
		if (arg == "--max-mae" && i + 1 < argc)
		{
			maxMae = std::atof(argv[++i]);
		}
		else if (arg == "--json")
		{
			json = true;
		}
		else if (pathA.empty())
		{
			pathA = arg;
		}
		else if (pathB.empty())
		{
			pathB = arg;
		}
		else
		{
			pathA.clear();
			break;
		}
	}

	if (pathA.empty() || pathB.empty())
	{
		std::cerr << "usage: vxr_imgdiff a.jpg b.jpg [--max-mae <x>] [--json]"
				  << std::endl;
		return 2;
	}

	int wa, ha, ca, wb, hb, cb;
	unsigned char *a = stbi_load(pathA.c_str(), &wa, &ha, &ca, 3);
	unsigned char *b = stbi_load(pathB.c_str(), &wb, &hb, &cb, 3);
	if (!a || !b)
	{
		std::cerr << "cannot load " << (a ? pathB : pathA) << std::endl;
		stbi_image_free(a);
		stbi_image_free(b);
		return 2;
	}
	if (wa != wb || ha != hb)
	{
		std::cerr << "size differs: " << wa << "x" << ha << " vs " << wb << "x"
				  << hb << std::endl;
		stbi_image_free(a);
		stbi_image_free(b);
		return 2;
	}

	const size_t pixels = size_t(wa) * size_t(ha);
	double sumAbs = 0.0;
	double sumSq = 0.0;
	int maxDiff = 0;
	size_t differing = 0;

	for (size_t p = 0; p < pixels; p++)
	{
		bool different = false;
		for (int c = 0; c < 3; c++)
		{
			const int d = std::abs(int(a[p * 3 + c]) - int(b[p * 3 + c]));
			sumAbs += d;
			sumSq += double(d) * d;
			maxDiff = std::max(maxDiff, d);
			different |= d != 0;
		}
		differing += different ? 1 : 0;
	}
	stbi_image_free(a);
	stbi_image_free(b);

	const double samples = double(pixels) * 3.0;
	const double mae = sumAbs / samples;
	const double mse = sumSq / samples;
	const double rmse = std::sqrt(mse);
	const bool identical = sumSq == 0.0;
	const double psnr = identical ? INFINITY : 10.0 * std::log10(255.0 * 255.0 / mse);
	const bool pass = mae <= maxMae;

	if (json)
	{
		std::printf("{\"width\": %d, \"height\": %d, \"mae\": %.9g, \"rmse\": %.9g, "
					"\"psnr\": %s, \"max_diff\": %d, \"differing_pixels\": %zu, "
					"\"pixels\": %zu, \"identical\": %s, \"pass\": %s}\n",
					wa, ha, mae, rmse,
					identical ? "null" : std::to_string(psnr).c_str(), maxDiff,
					differing, pixels, identical ? "true" : "false",
					pass ? "true" : "false");
	}
	else
	{
		std::printf("size              %dx%d\n", wa, ha);
		std::printf("mean abs error    %.6f\n", mae);
		std::printf("rmse              %.6f\n", rmse);
		if (identical)
		{
			std::printf("psnr              inf (identical)\n");
		}
		else
		{
			std::printf("psnr              %.3f dB\n", psnr);
		}
		std::printf("max channel diff  %d\n", maxDiff);
		std::printf("differing pixels  %zu of %zu\n", differing, pixels);
		std::printf("%s (mean abs error %s %.6f)\n", pass ? "PASS" : "FAIL",
					pass ? "<=" : ">", maxMae);
	}

	return pass ? 0 : 1;
}
