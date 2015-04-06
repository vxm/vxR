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
	std::unique_ptr<vxBucket> m_up = nullptr;
	std::unique_ptr<vxBucket> m_down = nullptr;
	std::unique_ptr<vxBucket> m_left = nullptr;
	std::unique_ptr<vxBucket> m_right = nullptr;

public:

	vxContactBuffer m_pb;

	// creates this bucket initializing the buffer with a size
	// of sidePixels (num side pxs) ^ 2
	vxBucket()
		: m_pb(100)
	{
	}
	
	vxBucket(vxBucket&& other)
		: m_pb(std::move(m_pb))
	{
		m_up.reset( other.m_up.release() );
		m_down.reset( other.m_down.release() );
		m_left.reset( other.m_left.release() );
		m_right.reset( other.m_right.release() );
	}

	vxBucket(const vxBucket& other)
		: m_pb(other.m_pb)
	{
	}

	~vxBucket()
	{
	}
	
	void append(const vxColor &px, double hx, double hy)
	{
		m_pb.append(px, hx, hy);
	}
};


class vxBucketList : public vxObject
{
private:
	unsigned int m_nBucketsInX = {20u};
	std::vector <vxBucket> m_buckets;
	std::shared_ptr<const ImageProperties> m_prop;
	
public:
	vxBucketList(std::shared_ptr<const ImageProperties> prop,
					unsigned int bucketsInX)
		: m_nBucketsInX(bucketsInX)
		, m_prop(prop)
	{
		createBuckets();
	}
	
	void reset(std::shared_ptr<const ImageProperties> prop,
			   unsigned int bucketsInX)
	{
		m_nBucketsInX = bucketsInX;
		m_prop = prop;
		createBuckets();
	}

	void createBuckets()
	{
		if(!m_buckets.size())
		{
			unsigned int totalBuckets = m_nBucketsInX*m_nBucketsInX;
			m_buckets.resize(totalBuckets);
		}
	}
	
	// simple utility to get correct number of buckets in an image
	unsigned int getNumBuckets(unsigned int sidePixels) const
	{
		return (m_prop->rx() * m_prop->ry()) / (sidePixels * sidePixels);
	}
	
	unsigned int getIndex(double x, double y) const
	{
		int r1 = (x*m_nBucketsInX);
		int r2 = (m_nBucketsInX * y);
		return  r1 + r2*m_nBucketsInX;
	}
	

	vxBucket* getBucket(double x, double y)
	{
		auto id = getIndex(x,y);
		
		return &m_buckets[id];
	}
	
	vxBucket& operator[](unsigned int i)
	{
		return m_buckets[i];
	}
	
	std::shared_ptr<const ImageProperties> prop() const;
	
	void setProp(const std::shared_ptr<ImageProperties> &prop);

	size_t size() const
	{
		return m_buckets.size();
	}
};

}
#endif // VXBUCKETLIST_H
