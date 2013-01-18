#ifndef __ModelCacheNone_h__
#define __ModelCacheNone_h__

#include "ModelCache.h"

class CModelCacheNone : public CModelCache
{
public:
	CModelCacheNone() {}
	virtual ~CModelCacheNone() {}

	inline virtual CIndication			SegmentRequest(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		CBitmap*			bitmap
		) { return 0; } // Always not cache

	inline virtual void					SegmentResponse(
		unsigned int		peer,
		unsigned int		video,
		CIndication			indication
		) {}

	inline virtual void					ClearReceiver(
		unsigned int		peer
		) {}

	inline virtual unsigned char		Add(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		unsigned int		segment
		) { return CACHE_ADD_NO; }

	inline virtual bool					Get(
		__time				timestamp,
		unsigned int		peer,
		unsigned int		video,
		unsigned int		segment
		) { return CACHE_FAIL; }

	inline virtual bool					Has(
		unsigned int		peer,
		unsigned int		video,
		unsigned int		segment
		) { return CACHE_FAIL; }

	inline virtual TModelCacheVideo*	GetPeers(
		unsigned int		video,
		unsigned int		segment
		) { return &this->emptyMapping; }

	inline virtual CIndication			Indication(
		unsigned int		peer,
		unsigned int		video
		) { return 0; } // Always not cache

	inline virtual unsigned int			Size(
		unsigned int		peer
		) { return 0; }

	inline virtual unsigned int			MaxSize(
		unsigned int		peer
		) { return 0; }
};

#endif