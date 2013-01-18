#ifndef __ModelCacheLfu_h__
#define __ModelCacheLfu_h__

#include "ModelCache.h"
#include "ModelWorkload.h"

/*
 * Cache model : least frequently used
 */

// peer -> timestamp -> (video, segment)
struct SModelCacheLfuEntry
{
	__time				timestamp;
	unsigned __int64	counter;
	unsigned int		video;
	unsigned int		segment;
};

typedef multimap<double, SModelCacheLfuEntry>	TModelCacheLfuPeer;

class CModelCacheLfu : public CModelCache
{
private:
	CModelWorkload*			workload;
	unsigned int			cacheSize;

	TModelCacheLfuPeer*		cache;
	TModelCacheVideo**		mapping;

public:
	CModelCacheLfu(
		CModelWorkload*		workload,
		unsigned int		cacheSize
		);
	virtual ~CModelCacheLfu();

	virtual CIndication			SegmentRequest(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		CBitmap*			bitmap
		);

	virtual void				SegmentResponse(
		unsigned int		peer,
		unsigned int		video,
		CIndication	indication
		);

	virtual void				ClearReceiver(
		unsigned int		peer
		);

	virtual unsigned char		Add(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		unsigned int		segment
		);

	virtual bool				Get(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		unsigned int		segment
		);

	virtual bool				Has(
		unsigned int		peer,
		unsigned int		video,
		unsigned int		segment
		);

	virtual TModelCacheVideo*	GetPeers(
		unsigned int		video,
		unsigned int		segment
		);

	virtual CIndication			Indication(
		unsigned int		peer,
		unsigned int		video
		) { return 1; } // Always cache

	virtual unsigned int		Size(
		unsigned int		peer
		) { assert(peer < this->workload->NumPeers()); return this->cache[peer].size(); }
	
	virtual unsigned int		MaxSize(
		unsigned int		peer
		) { return this->cacheSize; }
};

#endif