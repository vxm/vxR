#ifndef VXSAMPLER_H
#define VXSAMPLER_H

#include <vector>
#include <vxObject.h>
#include <math.h>
#include <cstdlib>

namespace vxStorage {


class vxSampler:public vxObject
{
	std::vector<double> m_x;
	std::vector<double> m_y;

	int m_f {0};
	int m_samples {1};

public:
	vxSampler() 
	{
		setSamples(1);
	};
	
	~vxSampler() 
	{
	}

	void next() 
	{
		if(m_f >= m_samples-1)
		{
			m_f=0;
		}
		else
		{
			m_f++;
		}
	};

	double getX() {return m_x[m_f];}
	double getY() {return m_y[m_f];}

	void setSamples(int sampless)
	{
		m_samples=sampless;
		
		m_x.clear();
		m_x.resize(sampless);
		
		m_y.clear();
		m_y.resize(sampless);
		//m_y=(double*)malloc(sizeof(double)*m_samples);

		for(int i=0;i<m_samples;i++)
		{
			m_x[i]=(rand()/(double)RAND_MAX);
			m_y[i]=(rand()/(double)RAND_MAX);
		}
		m_f=0;
	}
};

}
#endif // VXSAMPLER_H
