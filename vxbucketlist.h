#ifndef VXBUCKETLIST_H
#define VXBUCKETLIST_H

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

	vxBucket(sidePixels)
		: m_sidePixels(sidePixels)
	{
		m_pb.reset(new vxPxBuffer(sidePixele*sidePixele));
	}
};



class vxBucketList : public vxObject
{
private:
	
	unsigned int m_numBuckets = {1u};
	std::vector<vxBucket> m_buckets;
	
public:
	vxBucketList();
	vxBucketList(int resX, int rexY, pixelsSide)
	{
		
	}

	void createBuckets()
	{
		m_buckets.resize(m_numBuckets);
	}
	
};

}
#endif // VXBUCKETLIST_H
