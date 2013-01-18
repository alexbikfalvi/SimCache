#ifndef __ModelCacheLfu2_h__
#define __ModelCacheLfu2_h__

#include "ModelCache.h"
#include "ModelWorkload.h"
#include "ModelCacheLfu.h"

typedef multimap<unsigned __int64, SModelCacheLfuEntry>	TModelCacheLfuPeer2;

class CModelCacheLfu2 : public CModelCache
{
private:
	CModelWorkload*			workload;
	unsigned int			cacheSize;

	TModelCacheLfuPeer2*	cache;
	TModelCacheVideo**		mapping;

public:
	CModelCacheLfu2(
		CModelWorkload*		workload,
		unsigned int		cacheSize
		);
	virtual ~CModelCacheLfu2();

	virtual CIndication			SegmentRequest(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		CBitmap*			bitmap
		);

	virtual void				SegmentResponse(
		unsigned int		peer,
		unsigned int		video,
		CIndication			indication
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