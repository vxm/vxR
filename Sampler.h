#ifndef VXSAMPLER_H
#define VXSAMPLER_H

#include <vector>
#include <math.h>
#include <cstdlib>
#include "Object.h"
#include "MathUtils.h"

namespace vxCore {

static std::vector<v2s> uniformScatterFree;

class Sampler
{
	std::vector<scalar> m_x = {0.5};
	std::vector<scalar> m_y = {0.5};

	unsigned int m_iter {0};
	unsigned int m_nSamples {1};

	bool m_random {true};
	unsigned int m_k{0u};
	void populateFreeSamples();
	
public:
	
	Sampler(unsigned int nSamples);
	~Sampler();
	void next();
	bool isDone() const;

	scalar x() const;
	scalar y() const;
	
	v2s xy(const scalar jitter = 0.0);

	void setSamples(unsigned int samples);
	void resetIterator();

	bool random() const;
	void setRandom(bool random);
};

}
#endif // VXSAMPLER_H
