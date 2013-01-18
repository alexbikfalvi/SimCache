#include "stdafx.h"
#include "ModelCachePvcP.h"

CModelCachePvcP::CModelCachePvcP(
	CModelWorkload*	workload,
	unsigned int	cacheSize,
	double			shortSmoothFactor,
	double			longSmoothFactor,
	double			trendThreshold
	)
{
	assert(workload);
	assert(workload->NumPeers());
	assert(cacheSize);

	this->workload = workload;
	this->cacheSize = cacheSize;

	this->shortSmoothFactor = shortSmoothFactor;
	this->longSmoothFactor = longSmoothFactor;
	this->trendThreshold = trendThreshold;

	// Cache
	this->cache = new SModelCachePvcPPeer[this->workload->NumPeers()];
	assert(this->cache);

	for(unsigned int peer = 0; peer < this->workload->NumPeers(); peer++)
	{
		this->cache[peer].videos = new SModelCachePvcPVideo[this->workload->NumFiles()];
		assert(this->cache[peer].videos);

		this->cache[peer].size = 0;
		for(unsigned int video = 0; video < this->workload->NumFiles(); video++)
		{
			this->cache[peer].videos[video].sewma = 0;
			this->cache[peer].videos[video].lewma = 0;
			this->cache[peer].videos[video].freq = 0;		// Negative frequency for videos with no hits
			this->cache[peer].videos[video].timestamp = 0;
			this->cache[peer].videos[video].cache = 1;		// Cache (default)
		}
	}

	// Mapping
	this->mapping = new TModelCacheVideo*[this->workload->NumFiles()];
	assert(this->mapping);

	for(unsigned int index = 0; index < this->workload->NumFiles(); index++)
		this->mapping[index] = NULL;
}

CModelCachePvcP::~CModelCachePvcP()
{
	for(unsigned int peer = 0; peer < this->workload->NumPeers(); peer++)
		delete[] this->cache[peer].videos;
	delete[] this->cache;
	
	for(unsigned int index = 0; index < this->workload->NumFiles(); index++)
		if(this->mapping[index])
			delete[] this->mapping[index];
	delete[] this->mapping;
}

CIndication CModelCachePvcP::SegmentRequest(
	__time			timestamp,
	unsigned int	peer,
	unsigned int	video,
	CBitmap*		bitmap
	)
{
	assert(bitmap);
	assert(peer < this->workload->NumPeers());
	assert(video < this->workload->NumFiles());

	for(list<unsigned int>::iterator iter = this->cache[peer].videos[video].segments.begin();
		iter != this->cache[peer].videos[video].segments.end(); iter++)
		bitmap->StoreConf(*iter);

	// Calculate cache indication based on video popularity
		// If video frequency is for 1 or less requests, return no cache
	if(this->cache[peer].videos[video].freq <= 0) return 0;	
	else
	{
		assert(this->cache[peer].videos[video].sewma);
		assert(this->cache[peer].videos[video].lewma);

		double trend = this->cache[peer].videos[video].lewma / this->cache[peer].videos[video].sewma;

		return (trend <= this->trendThreshold); // Return cache if the trend is increasing
	}
}

void CModelCachePvcP::SegmentResponse(
	unsigned int		peer,
	unsigned int		video,
	CIndication			indication
	)
{
	assert(peer < this->workload->NumPeers());
	assert(video < this->workload->NumFiles());

	// Save the indication for this video
	this->cache[peer].videos[video].cache = indication.Cache();
}

void CModelCachePvcP::ClearReceiver(
	unsigned int		peer
	)
{
	// Do nothing (there is no indication state to clear)
}

unsigned char CModelCachePvcP::Add(
	__time				timestamp,
	unsigned int		peer,
	unsigned int		video,
	unsigned int		segment
	)
{
	// Add a video-segment pair to the peer cache
	assert(peer < this->workload->NumPeers());
	assert(video < this->workload->NumFiles());
	assert(segment < this->workload->FileSizeSegments(video));

	// If the video must not be cached, return
	if(!this->cache[peer].videos[video].cache) return CACHE_ADD_NO;

	// In the debug version check, the peer does not already store the segment
	assert(CACHE_FAIL == this->Has(peer, video, segment));

	if(this->cache[peer].size < this->cacheSize)
	{
		// If the cache is not full

		// Push the segment at the front of the list
		this->cache[peer].videos[video].segments.push_front(segment);

		// Increase the cache size
		this->cache[peer].size++;

		// Allocate mapping, if necessary
		if(NULL == this->mapping[video])
			this->mapping[video] = new TModelCacheVideo[this->workload->FileSizeSegments(video)];
		assert(this->mapping[video]);

		// Add the mapping
		this->mapping[video][segment].insert(peer);

		return CACHE_ADD_YES_REP_NO;
	}
	else
	{
		// If the cache is full

		// Apply removal policy

		// The victim video
		unsigned int victim;
		double popularity = 1E300; // maximum popularity
		bool selected = 0;

		// First, check to see if there are videos with a popularity trend below the threshold
		for(unsigned int index = 0; index < this->workload->NumFiles(); index++)
		{
			// For all videos...
			if(this->cache[peer].videos[index].segments.size())
			{
				// ... that have segments in the local cache

				// Assume a hit now
				
				// Calculate a temporary frequency amd EWMA
				double freq = 1.0/(timestamp - this->cache[peer].videos[index].timestamp);
				double lewma = this->longSmoothFactor * this->cache[peer].videos[video].lewma + (1-this->longSmoothFactor) * freq;
				double sewma = this->shortSmoothFactor * this->cache[peer].videos[video].sewma + (1-this->shortSmoothFactor) * freq;

				// If the trend is above the threshold
				if(lewma / sewma > this->trendThreshold)
				{
					// Choose this video
					if(sewma < popularity)
					{
						// If its popularity is lower that what might have been selected, choose that video
						victim = index;
						popularity = sewma;
						selected = 1;
					}
				}
			}
		}

		// If a video was not selected (no videos with decreasing popularity trend)
		if(!selected)
		{
			popularity = 0;
			// Choose the video with the lowest popularity regardless of the trend
			for(unsigned int index = 0; index < this->workload->NumFiles(); index++)
			{
				if(this->cache[peer].videos[index].segments.size())
				{
					// Assume a hit now
					// Calculate a temporary frequency amd SEWMA
					double freq = 1.0/(timestamp - this->cache[peer].videos[index].timestamp);
					double sewma = this->shortSmoothFactor * this->cache[peer].videos[video].sewma + (1-this->shortSmoothFactor) * freq;

					// Choose this video
					if(sewma > popularity)
					{
						// If its popularity is higher that what might have been selected, choose that video
						victim = index;
						popularity = sewma;
						selected = 1;
					}
				}
			}
		}

		assert(selected); // The cache is full, something must have been selected

		// Remove the last segment of the selected video
			// First, remove its mapping
		assert(this->mapping[victim][this->cache[peer].videos[victim].segments.back()].find(peer) !=
			this->mapping[victim][this->cache[peer].videos[victim].segments.back()].end());
		this->mapping[victim][this->cache[peer].videos[victim].segments.back()].erase(peer);

			// Remove the segment
		this->cache[peer].videos[victim].segments.pop_back();

		// Add the new segment
		// Push the segment at the front of the list
		this->cache[peer].videos[video].segments.push_front(segment);

		// Allocate mapping, if necessary
		if(NULL == this->mapping[video])
			this->mapping[video] = new TModelCacheVideo[this->workload->FileSizeSegments(video)];
		assert(this->mapping[video]);

		// Add the mapping
		this->mapping[video][segment].insert(peer);

		return CACHE_ADD_YES_REP_YES;
	}
}

bool CModelCachePvcP::Get(
	__time				timestamp,
	unsigned int		peer,
	unsigned int		video,
	unsigned int		segment
	)
{
	bool found = 0;
	for(list<unsigned int>::iterator iter = this->cache[peer].videos[video].segments.begin();
		iter != this->cache[peer].videos[video].segments.end(); iter++)
	{
		if((*iter) == segment)
		{
			found = 1;
			break;
		}
	}

	// Update the video popularity - regardless of whether the segment was found or not
		// Update the frequency
	assert(timestamp != this->cache[peer].videos[video].timestamp);
	this->cache[peer].videos[video].freq = (this->cache[peer].videos[video].freq == -1)?0:1.0/(timestamp - this->cache[peer].videos[video].timestamp);
		// Update the timestamp
	this->cache[peer].videos[video].timestamp = timestamp;
		// Update the long-term moving average
	this->cache[peer].videos[video].lewma = this->longSmoothFactor * this->cache[peer].videos[video].lewma + (1-this->longSmoothFactor) * this->cache[peer].videos[video].freq;
		// Update the short-term moving average
	this->cache[peer].videos[video].sewma = this->shortSmoothFactor * this->cache[peer].videos[video].sewma + (1-this->shortSmoothFactor) * this->cache[peer].videos[video].freq;

	return found?CACHE_SUCCESS:CACHE_FAIL;
}

bool CModelCachePvcP::Has(
	unsigned int		peer,
	unsigned int		video,
	unsigned int		segment
	)
{
	for(list<unsigned int>::iterator iter = this->cache[peer].videos[video].segments.begin(); iter != this->cache[peer].videos[video].segments.end(); iter++)
		if((*iter) == segment)
			return CACHE_SUCCESS;
	return CACHE_FAIL;
}

TModelCacheVideo* CModelCachePvcP::GetPeers(
	unsigned int		video,
	unsigned int		segment
	)
{
	assert(video < this->workload->NumFiles());
	assert(segment < this->workload->FileSizeSegments(video));

	if(this->mapping[video])
		return &this->mapping[video][segment];
	else
		return &this->emptyMapping;
}

CIndication CModelCachePvcP::Indication(
	unsigned int		peer,
	unsigned int		video
	)
{
	assert(video < this->workload->NumFiles());
	assert(peer < this->workload->NumPeers());

	return this->cache[peer].videos[video].cache;
}

unsigned int CModelCachePvcP::Size(
	unsigned int		peer
	)
{
	assert(peer < this->workload->NumPeers());

	return this->cache[peer].size;
}
