#ifndef VXBUCKETLIST_H
#define VXBUCKETLIST_H

#include <imageproperties.h>
#include <vxpxbuffer.h>


namespace vxStorage {

class vxBucket
{
	// size in pixexl of size
	unsigned int m_sidePixels = {10u};
	
	// links to neighbour buckets
	std::unique_ptr<vxBucket> m_up;
	std::unique_ptr<vxBucket> m_down;
	std::unique_ptr<vxBucket> m_left;
	std::unique_ptr<vxBucket> m_right;
	
	std::unique_ptr <vxPxBuffer> m_pb = nullptr;

public:

	// creates this bucket initializing the buffer with a size
	// of sidePixels (num side pxs) ^ 2
	vxBucket(unsigned int sidePixels)
		: m_sidePixels(sidePixels) 
	{
		m_pb.reset(new vxPxBuffer(sidePixels*sidePixels));
	}
};



class vxBucketList : public vxObject
{
private:
	
	unsigned int m_numBuckets = {1u};
	std::vector<vxBucket> m_buckets;
	
public:
	vxBucketList();
	vxBucketList(const ImageProperties &prop, unsigned int sidePixels)
	{
		m_numBuckets = getNumBuckets(prop, sidePixels);
		createBuckets();
	}

	void createBuckets()
	{
		m_buckets.resize(m_numBuckets);
	}
	
	// simple utility to get correct number of buckets in an image
	static const unsigned int getNumBuckets(const ImageProperties &prop,
											unsigned int sidePixels)
	{
		return (prop.rx() * prop.ry()) / sidePixels;
	}
	
};

}
#endif // VXBUCKETLIST_H
