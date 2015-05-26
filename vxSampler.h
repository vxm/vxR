#ifndef VXSAMPLER_H
#define VXSAMPLER_H

#include <vector>
#include <math.h>
#include <cstdlib>
#include "vxObject.h"

namespace vxCore {


class vxSampler
{
	std::vector<double> m_x = {0.5};
	std::vector<double> m_y = {0.5};

	unsigned int m_iter {0};
	unsigned int m_nSamples {1};

	bool m_random {true};
	
public:
	
	vxSampler()
	{
	};
	
	~vxSampler()
	{
	}
	

	void next()
	{
		m_iter++;

		if(m_iter == m_nSamples)
		{
			m_iter=0;
		}
	};
	
	bool isDone() const
	{
		return m_iter+1==m_nSamples;
	}

	double x() const
		{return m_x[m_iter];}

	double y() const
		{return m_y[m_iter];}

	void setSamples(unsigned int samples)
	{
		m_nSamples = samples;
		
		m_x.resize(samples);
		m_y.resize(samples);
		
		if(m_random)
		{
			for(unsigned int i=0;i<m_nSamples;i++)
			{
				m_x[i]=(rand()/(double)RAND_MAX);
				m_y[i]=(rand()/(double)RAND_MAX);
			}
		}

		resetIterator();
	}
	
	void resetIterator()
	{
		m_iter=0;
	}
	
	bool random() const
	{
		return m_random;
	}
	
	void setRandom(bool random)
	{
		m_random = random;
	}
};

}
#endif // VXSAMPLER_H
