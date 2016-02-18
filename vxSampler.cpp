#include "vxSampler.h"

using namespace vxCore;

static std::vector<v2s> uniformScatter1
{
	v2s(0.5, 0.5)
};

static std::vector<v2s> uniformScatter2
{
	v2s(0.25, 0.75),
	v2s(0.75, 0.25),
};

static std::vector<v2s> uniformScatter3
{
	v2s(0.5, 0.33),
	v2s(0.31, 0.69),
	v2s(0.66, 0.62)
};

static std::vector<v2s> uniformScatter4
{
	v2s(0.25, 0.25),
	v2s(0.75, 0.75),
	v2s(0.25, 0.75),
	v2s(0.75, 0.25),
};

void vxSampler::populateFreeSamples()
{
	if(uniformScatterFree.size())
		return;
	
	for(int i=0;i<50;i++)
	{
		uniformScatterFree.emplace_back(v2s(
										 MU::getRand(1.0),
										 MU::getRand(1.0)));
	}
}

vxSampler::vxSampler(unsigned int nSamples)
{
	setSamples(nSamples);
	populateFreeSamples();
}

vxSampler::~vxSampler()
{
}

void vxSampler::next()
{
	m_iter++;
	
	if(m_iter == m_nSamples)
	{
		m_iter=0;
	}
}

bool vxSampler::isDone() const
{
	return m_iter+1==m_nSamples;
}

scalar vxSampler::x() const
{
	return MU::getRand(1.0);
}

scalar vxSampler::y() const
{
	return MU::getRand(1.0);
}

v2s &vxSampler::xy()
{
	m_k>=m_nSamples ? m_k=0 : m_k++;
	switch(m_nSamples)
	{
	case 1:
		return uniformScatter1[m_k];
		break;
	case 2:
		return uniformScatter2[m_k];
		break;
	case 3:
		return uniformScatter3[m_k];
		break;
	case 4:
		return uniformScatter4[m_k];
		break;
	default:
		return uniformScatterFree[m_k];
		break;
	}
}

void vxSampler::setSamples(unsigned int samples)
{
	m_nSamples = samples;
	
	m_x.resize(samples);
	m_y.resize(samples);
	
	if(m_random)
	{
		for(unsigned int i=0;i<m_nSamples;i++)
		{
			m_x[i]=MU::getRand(1.0);
			m_y[i]=MU::getRand(1.0);
		}
	}
	
	resetIterator();
}

void vxSampler::resetIterator()
{
	m_iter=0;
}

bool vxSampler::random() const
{
	return m_random;
}

void vxSampler::setRandom(bool random)
{
	m_random = random;
}



