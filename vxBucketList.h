#ifndef VXBUCKETLIST_H
#define VXBUCKETLIST_H

#include <ImageProperties.h>
#include <vxContactBuffer.h>


namespace vxStorage {

// vxBucket will be built from vxBucketList
// which is a factory of this class.
class vxBucket
{
	// links to neighbour buckets
	std::unique_ptr<vxBucket> m_up;
	std::unique_ptr<vxBucket> m_down;
	std::unique_ptr<vxBucket> m_left;
	std::unique_ptr<vxBucket> m_right;
	
	std::unique_ptr <vxContactBuffer> m_pb = nullptr;

public:

	// creates this bucket initializing the buffer with a size
	// of sidePixels (num side pxs) ^ 2
	vxBucket()
	{
		m_pb.reset(new vxContactBuffer(10));
	}
};



class vxBucketList : public vxObject
{
private:
	
	unsigned int m_nBucketsInX = {10u};
	std::vector<vxBucket> m_buckets;
	std::shared_ptr<const ImageProperties> m_prop;
	
public:
	vxBucketList(const ImageProperties &prop,
					unsigned int bucketsInX)
		: m_nBucketsInX(bucketsInX)
	{
		m_prop.reset(&prop);
		createBuckets();
	}

	void createBuckets()
	{
	//	m_buckets.resize(m_numBuckets);
	}
	
	// simple utility to get correct number of buckets in an image
	const unsigned int getNumBuckets(unsigned int sidePixels)
	{
		return (m_prop->rx() * m_prop->ry()) / (sidePixels * sidePixels);
	}
	
	inline unsigned int getIndex(double x, double y)
	{
		return 100u;
	}
	
	std::shared_ptr<vxBucket> getBucket(double x, double y)
	{
		auto id = getIndex(x,y);
	}
	
	std::shared_ptr<const ImageProperties> prop() const;
	void setProp(const std::shared_ptr<ImageProperties> &prop);
};

}
#endif // VXBUCKETLIST_H
