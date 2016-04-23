#include "vxSampler.h"

using namespace vxCore;

vxSampler::~vxSampler()
{
}

static std::vector<v2s> uniformScatter1
{
	v2s(0.5, 0.5)
};

static std::vector<v2s> uniformScatter2
{
	v2s(0.30, 0.70),
	v2s(0.70, 0.30),
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

static std::vector<v2s> uniformScatter5
{
	v2s(0.5, 0.5),
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

v2s vxSampler::xy(const scalar jitter)
{
	m_k>=m_nSamples ? m_k=0 : m_k++;
	
	v2s ret;
	switch(m_nSamples)
	{
	case 1:
		ret = uniformScatter1[m_k] + MU::getRand(jitter);
		break;
	case 2:
		ret = uniformScatter2[m_k] + MU::getRand(jitter);
		break;
	case 3:
		ret = uniformScatter3[m_k] + MU::getRand(jitter);
		break;
	case 4:
		ret = uniformScatter4[m_k] + MU::getRand(jitter);
		break;
	default:
		ret.set(MU::getRand(1.0+jitter),MU::getRand(1.0+jitter));
		break;
	}
	
	return ret;
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



