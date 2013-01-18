#ifndef __ModelCache_h__
#define __ModelCache_h__

#include "Bitmap.h"
#include "Indication.h"

typedef set<unsigned int>			TModelCacheVideo;

#define CACHE_ADD_NO			0
#define CACHE_ADD_YES_REP_NO	1
#define CACHE_ADD_YES_REP_YES	2

enum ECacheResult
{
	CACHE_SUCCESS = 0,
	CACHE_FAIL = 1
};

class CModelCache
{
protected:
	TModelCacheVideo	emptyMapping;

public:
	CModelCache() {}
	virtual ~CModelCache() {}


	// This function is called upone a received segment request
	// The cache should return the cache hits in the bitmap response, eventually locking/marking those segments
	virtual CIndication			SegmentRequest(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		CBitmap*			bitmap
		) = 0;

	virtual void				SegmentResponse(
		unsigned int		peer,
		unsigned int		video,
		CIndication			indication
		) = 0;

	virtual void				ClearReceiver(
		unsigned int		peer
		) = 0;

	// This function adds a new segment to the cache, according to the admission policy and set indication
	virtual unsigned char		Add(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		unsigned int		segment
		) = 0;

	// This function retrieves a segment from the cache, and, according to the policy - on whether the segment exists
	// - marks a cache hit or miss
	virtual bool				Get(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		unsigned int		segment
		) = 0;

	// - same as get but without marking cache hit/miss
	virtual bool				Has(
		unsigned int		peer,
		unsigned int		video,
		unsigned int		segment
		) = 0;

	// Reverse cache mapping: returns a list of peers for a video/segment pair
	virtual TModelCacheVideo*	GetPeers(
		unsigned int		video,
		unsigned int		segment
		) = 0;

	// Returns the cache indication for a specified peer/video
	virtual CIndication			Indication(
		unsigned int		peer,
		unsigned int		video
		) = 0;

	// Returns the cache size
	virtual unsigned int		Size(
		unsigned int		peer
		) = 0;
	virtual unsigned int		MaxSize(
		unsigned int		peer
		) = 0;
};

#endif