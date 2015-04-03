#ifndef VXSAMPLER_H
#define VXSAMPLER_H

#include <vector>
#include <vxObject.h>
#include <math.h>
#include <cstdlib>

namespace vxStorage {


class vxSampler:public vxObject
{
	std::vector<double> m_x = {0.5};
	std::vector<double> m_y = {0.5};

	unsigned int m_iter {0};
	unsigned int m_nSamples {1};

	bool m_random {false};
	
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

	double getX() const 
		{return m_x[m_iter];}
	double getY() const
		{return m_y[m_iter];}

	void setSamples(unsigned int samples)
	{
		m_nSamples = samples;
		
		m_x.resize(samples);
		
		m_y.resize(samples);
		
		if(m_random)
		{
			for(uint i=0;i<m_nSamples;i++)
			{
				m_x[i]=(rand()/(double)RAND_MAX);
				m_y[i]=(rand()/(double)RAND_MAX);
			}
		}

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
