#include "stdafx.h"
#include "ModelCacheLru.h"

CModelCacheLru::CModelCacheLru(
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
	this->cache = new TModelCacheLruPeer[this->workload->NumPeers()];
	assert(this->cache);
	
	// Mapping
	this->mapping = new TModelCacheVideo*[this->workload->NumFiles()];
	assert(this->mapping);

	for(unsigned int index = 0; index < this->workload->NumFiles(); index++)
		this->mapping[index] = NULL;
}

CModelCacheLru::~CModelCacheLru()
{
	delete[] this->cache;
	for(unsigned int index = 0; index < this->workload->NumFiles(); index++)
		if(this->mapping[index])
			delete[] this->mapping[index];
	delete[] this->mapping;
}

CIndication CModelCacheLru::SegmentRequest(
	__time			timestamp,
	unsigned int	peer,
	unsigned int	video,
	CBitmap*		bitmap
	)
{
	assert(bitmap);
	assert(peer < this->workload->NumPeers());
	assert(video < this->workload->NumFiles());

	for(TModelCacheLruPeer::iterator iter = this->cache[peer].begin(); iter != this->cache[peer].end(); iter++)
		if(iter->video == video)
			bitmap->StoreConf(iter->segment);

	return 1; // Always cache
}

void CModelCacheLru::SegmentResponse(
	unsigned int		peer,
	unsigned int		video,
	CIndication			indication
	)
{
	assert(peer < this->workload->NumPeers());
	assert(video < this->workload->NumFiles());
	assert(1 == indication.Cache());						// This cache model does not support indication

	// Do nothing: ignore indication
}

void CModelCacheLru::ClearReceiver(
	unsigned int		peer
	)
{
	// Do nothing (there is no indication state to clear)
}

unsigned char CModelCacheLru::Add(
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
		SModelCacheLruEntry entry;

		entry.timestamp = timestamp;
		entry.video = video;
		entry.segment = segment;

		// Push the entry at the front of the list
		this->cache[peer].push_front(entry);

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

		// Remove the least recent entry (first) from the cache

		// Check the timestamp of the last entry is less than the current timestamp
		assert(this->cache[peer].back().timestamp <= timestamp);

		// Remove the mapping
		assert(this->mapping[this->cache[peer].back().video][this->cache[peer].back().segment].find(peer) !=
			this->mapping[this->cache[peer].back().video][this->cache[peer].back().segment].end());
		this->mapping[this->cache[peer].back().video][this->cache[peer].back().segment].erase(peer);

		// Remove the last entry from the cache
		this->cache[peer].pop_back();

		// Check the cache size
		assert(this->cache[peer].size() < this->cacheSize);

		// Add the new entry
		SModelCacheLruEntry entry;

		entry.timestamp = timestamp;
		entry.video = video;
		entry.segment = segment;

		this->cache[peer].push_front(entry);

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

bool CModelCacheLru::Get(
	__time				timestamp,
	unsigned int		peer,
	unsigned int		video,
	unsigned int		segment
	)
{
	for(TModelCacheLruPeer::iterator iter = this->cache[peer].begin(); iter != this->cache[peer].end(); iter++)
		if((iter->video == video) && (iter->segment == segment))
		{
			// Mark the segment as hit, by changing its timestamp
			// Remove old entry
			this->cache[peer].erase(iter);
			
			// Add the new entry
			SModelCacheLruEntry entry;

			entry.timestamp = timestamp;
			entry.video = video;
			entry.segment = segment;

			this->cache[peer].push_front(entry);

			return CACHE_SUCCESS;
		}
	return CACHE_FAIL;
}

bool CModelCacheLru::Has(
	unsigned int		peer,
	unsigned int		video,
	unsigned int		segment
	)
{
	for(TModelCacheLruPeer::iterator iter = this->cache[peer].begin(); iter != this->cache[peer].end(); iter++)
		if((iter->video == video) && (iter->segment == segment))
			return CACHE_SUCCESS;
	return CACHE_FAIL;
}

TModelCacheVideo* CModelCacheLru::GetPeers(
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
