#ifndef __ModelCachePvcR_h__
#define __ModelCachePvcR_h__

#include "ModelCache.h"
#include "Info.h"

/*
 * Cache model : p2p video caching reactive
 */

struct SModelCachePvcRVideo
{
	double							sewma;
	double							lewma;
	double							freq;
	__time							timestamp;

	list<unsigned int>				segments;
};

struct SModelCachePvcRPeer
{
	SModelCachePvcRVideo*			videos;
	unsigned int					size;
};

class CModelCachePvcR : public CModelCache
{
private:
	CModelWorkload*			workload;
	unsigned int			cacheSize;

	double					shortSmoothFactor;
	double					longSmoothFactor;
	double					trendThreshold;

	SModelCachePvcRPeer*	cache;
	TModelCacheVideo**		mapping;

public:
	CModelCachePvcR(
		CModelWorkload*		workload,
		unsigned int		cacheSize,
		double				shortSmoothFactor,
		double				longSmoothFactor,
		double				trendThreshold
		);
	virtual ~CModelCachePvcR();

	virtual CIndication				SegmentRequest(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		CBitmap*			bitmap
		);

	virtual void					SegmentResponse(
		unsigned int		peer,
		unsigned int		video,
		CIndication	indication
		);

	virtual void					ClearReceiver(
		unsigned int		peer
		);

	virtual unsigned char			Add(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		unsigned int		segment
		);

	virtual bool					Get(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		unsigned int		segment
		);

	virtual bool					Has(
		unsigned int		peer,
		unsigned int		video,
		unsigned int		segment
		);

	virtual TModelCacheVideo*		GetPeers(
		unsigned int		video,
		unsigned int		segment
		);

	virtual CIndication				Indication(
		unsigned int		peer,
		unsigned int		video
		) { return 1; } // Always cache

	virtual unsigned int			Size(
		unsigned int		peer
		);

	inline virtual unsigned int		MaxSize(
		unsigned int		peer
		) { return this->cacheSize; }
};

#endif