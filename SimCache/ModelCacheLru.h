#ifndef __ModelCacheLru_h__
#define __ModelCacheLru_h__

#include "ModelCache.h"
#include "ModelWorkload.h"

/*
 * Cache model : least recently used
 */

// peer -> timestamp -> (video, segment)
struct SModelCacheLruEntry
{
	__time			timestamp;
	unsigned int	video;
	unsigned int	segment;
};

typedef list<SModelCacheLruEntry>	TModelCacheLruPeer;

class CModelCacheLru : public CModelCache
{
private:
	CModelWorkload*			workload;
	unsigned int			cacheSize;

	TModelCacheLruPeer*		cache;
	TModelCacheVideo**		mapping;

public:
	CModelCacheLru(
		CModelWorkload*		workload,
		unsigned int		cacheSize
		);
	virtual ~CModelCacheLru();

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