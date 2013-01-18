#include "stdafx.h"
#include "ModelCacheLfu.h"

CModelCacheLfu::CModelCacheLfu(
	CModelWorkload*	workload,
	unsigned int	cacheSize
	)
{
	assert(workload);
	assert(workload->NumPeers());
	assert(cacheSize);

	this->workload = workload;
	this->cacheSize = cacheSize;

	// Cache
	this->cache = new TModelCacheLfuPeer[this->workload->NumPeers()];
	assert(this->cache);
	
	// Mapping
	this->mapping = new TModelCacheVideo*[this->workload->NumFiles()];
	assert(this->mapping);

	for(unsigned int index = 0; index < this->workload->NumFiles(); index++)
		this->mapping[index] = NULL;
}

CModelCacheLfu::~CModelCacheLfu()
{
	delete[] this->cache;
	for(unsigned int index = 0; index < this->workload->NumFiles(); index++)
		if(this->mapping[index])
			delete[] this->mapping[index];
	delete[] this->mapping;
}

CIndication CModelCacheLfu::SegmentRequest(
	__time			timestamp,
	unsigned int	peer,
	unsigned int	video,
	CBitmap*		bitmap
	)
{
	assert(bitmap);
	assert(peer < this->workload->NumPeers());
	assert(video < this->workload->NumFiles());

	for(TModelCacheLfuPeer::iterator iter = this->cache[peer].begin(); iter != this->cache[peer].end(); iter++)
		if(iter->second.video == video)
			bitmap->StoreConf(iter->second.segment);

	return 1; // Always cache (although ignored)
}

void CModelCacheLfu::SegmentResponse(
	unsigned int		peer,
	unsigned int		video,
	CIndication	indication
	)
{
	assert(peer < this->workload->NumPeers());
	assert(video < this->workload->NumFiles());
	assert(1 == indication.Cache());						// This cache model does not support indication

	// Do nothing: ignore indication
}

void CModelCacheLfu::ClearReceiver(
	unsigned int		peer
	)
{
	// Do nothing (there is no indication state to clear)
}

unsigned char CModelCacheLfu::Add(
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

	// In the debug version check, the peer does not already store the segment
	assert(CACHE_FAIL == this->Has(peer, video, segment));


	if(this->cache[peer].size() < this->cacheSize)
	{
		// If the cache is not full
		SModelCacheLfuEntry entry;

		entry.counter = 0;
		entry.timestamp = timestamp;
		entry.video = video;
		entry.segment = segment;

		this->cache[peer].insert(pair<double, SModelCacheLfuEntry>(0, entry));

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

		// Remove the least frequently used entry (first) from the cache

		// Remove the mapping
		assert(this->mapping[this->cache[peer].begin()->second.video][this->cache[peer].begin()->second.segment].find(peer) !=
			this->mapping[this->cache[peer].begin()->second.video][this->cache[peer].begin()->second.segment].end());
		this->mapping[this->cache[peer].begin()->second.video][this->cache[peer].begin()->second.segment].erase(peer);

		// Remove the first entry from the cache
		this->cache[peer].erase(this->cache[peer].begin());

		// Check the cache size
		assert(this->cache[peer].size() < this->cacheSize);

		// Add the new entry
		SModelCacheLfuEntry entry;

		entry.counter = 0;
		entry.timestamp = timestamp;
		entry.video = video;
		entry.segment = segment;

		this->cache[peer].insert(pair<double, SModelCacheLfuEntry>(0, entry));

		// Allocate mapping, if necessary
		if(NULL == this->mapping[video])
			this->mapping[video] = new TModelCacheVideo[this->workload->FileSizeSegments(video)];
		assert(this->mapping[video]);

		// Add the mapping
		this->mapping[video][segment].insert(peer);

		// Check the cache size
		assert(this->cache[peer].size() == this->cacheSize);

		return CACHE_ADD_YES_REP_YES;
	}
}

bool CModelCacheLfu::Get(
	__time				timestamp,
	unsigned int		peer,
	unsigned int		video,
	unsigned int		segment
	)
{
	for(TModelCacheLfuPeer::iterator iter = this->cache[peer].begin(); iter != this->cache[peer].end(); iter++)
		if((iter->second.video == video) && (iter->second.segment == segment))
		{
			// Mark the segment as hit, by changing its hit count

			// Create new entry
			SModelCacheLfuEntry entry;

			entry.timestamp = iter->second.timestamp;
			entry.counter = iter->second.counter + 1;
			entry.video = video;
			entry.segment = segment;
			
			// Calculate new frequency
			double freq = ((double)entry.counter) / (timestamp - entry.timestamp);
			
			// Remove old entry
			this->cache[peer].erase(iter);

			// Add new entry
			this->cache[peer].insert(pair<double, SModelCacheLfuEntry>(freq, entry));

			return CACHE_SUCCESS;
		}
	return CACHE_FAIL;
}

bool CModelCacheLfu::Has(
	unsigned int		peer,
	unsigned int		video,
	unsigned int		segment
	)
{
	for(TModelCacheLfuPeer::iterator iter = this->cache[peer].begin(); iter != this->cache[peer].end(); iter++)
		if((iter->second.video == video) && (iter->second.segment == segment))
			return CACHE_SUCCESS;
	return CACHE_FAIL;
}

TModelCacheVideo* CModelCacheLfu::GetPeers(
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
