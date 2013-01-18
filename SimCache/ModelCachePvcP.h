#ifndef __ModelCachePvcP_h__
#define __ModelCachePvcP_h__

#include "ModelCache.h"
#include "ModelWorkload.h"

/*
 * Cache model : p2p video caching proactive
 */

struct SModelCachePvcPVideo
{
	double							sewma;
	double							lewma;
	double							freq;
	__time							timestamp;
	bool							cache;

	list<unsigned int>				segments;
};

struct SModelCachePvcPPeer
{
	SModelCachePvcPVideo*			videos;
	unsigned int					size;
};

class CModelCachePvcP : public CModelCache
{
private:
	CModelWorkload*			workload;
	unsigned int			cacheSize;

	double					shortSmoothFactor;
	double					longSmoothFactor;
	double					trendThreshold;

	SModelCachePvcPPeer*	cache;
	TModelCacheVideo**		mapping;

public:
	CModelCachePvcP(
		CModelWorkload*	workload,
		unsigned int	cacheSize,
		double			shortSmoothFactor,
		double			longSmoothFactor,
		double			trendThreshold
		);
	virtual ~CModelCachePvcP();

	virtual CIndication				SegmentRequest(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		CBitmap*			bitmap
		);

	virtual void					SegmentResponse(
		unsigned int		peer,
		unsigned int		video,
		CIndication			indication
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
		);

	virtual unsigned int			Size(
		unsigned int		peer
		);

	inline virtual unsigned int		MaxSize(
		unsigned int		peer
		) { return this->cacheSize; }
};

#endif