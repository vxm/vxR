#ifndef VXSAMPLER_H
#define VXSAMPLER_H

#include <vector>
#include <math.h>
#include <cstdlib>
#include "vxObject.h"
#include "MathUtils.h"

namespace vxCore {

static std::vector<vxVector2d> uniformScatterFree;

class vxSampler
{
	std::vector<double> m_x = {0.5};
	std::vector<double> m_y = {0.5};

	unsigned int m_iter {0};
	unsigned int m_nSamples {1};

	bool m_random {true};
	unsigned int m_k{0u};
	void populateFreeSamples();
	
public:
	
	vxSampler(unsigned int nSamples);
	~vxSampler();
	void next();
	bool isDone() const;

	double x() const;
	double y() const;
	
	vxVector2d& xy();
	void setSamples(unsigned int samples);
	void resetIterator();
	bool random() const;
	void setRandom(bool random);
};

}
#endif // VXSAMPLER_H
