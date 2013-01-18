#ifndef __Info_h__
#define __Info_h__

#include "SimHandler.h"
#include "ModelNet.h"
#include "ModelWorkload.h"
#include "ModelCache.h"

#define MESSAGE_BITMAP_REQUEST				0
#define MESSAGE_BITMAP_RESPONSE				0
#define MESSAGE_BOOT_REQUEST				0
#define MESSAGE_BOOT_RESPONSE				0
#define MESSAGE_REGISTER_REQUEST			0
#define MESSAGE_DEREGISTER_REQUEST			0
#define MESSAGE_SEGMENT_REQUEST				0
#define MESSAGE_SEGMENT_RESPONSE			0
#define MESSAGE_STREAM_REQUEST				0
#define MESSAGE_STREAM_RESPONSE				0

class CInfo
{
private:
	CSimHandler*	handler;

	CModelNet*		modelNet;
	CModelWorkload*	modelWorkload;
	CModelCache*	modelCache;

	__time			startTime;
	__time			endTime;
	unsigned int	numSenders;
	unsigned int	cacheSize;
	double			indicationThreshold;
	double			pvcShortSmoothFactor;
	double			pvcLongSmoothFactor;
	double			pvcTrendThreshold;

public:
	CInfo(
		CModelNet*		modelNet,
		CModelWorkload*	modelWorkload,
		CModelCache*	modelCache,
		__time			startTime,
		__time			endTime,
		unsigned int	numSenders,
		unsigned int	cacheSize,
		double			indicationThreshold,
		double			pvcShortSmoothFactor,
		double			pvcLongSmoothFactor,
		double			pvcTrendThreshold
		);
	~CInfo() {}

	void					SetHandler(CSimHandler* handler);

	inline CModelNet*		Net() { return this->modelNet; }
	inline CModelWorkload*	Workload() { return this->modelWorkload; }
	inline CModelCache*		Cache() { return this->modelCache; }

	inline __time			StartTime() { return this->startTime; }
	inline __time			EndTime() { return this->endTime; }
	inline unsigned int		NumSenders() { return this->numSenders; }
	inline unsigned int		CacheSize() { return this->cacheSize; }
	//inline __time			SessionStartDelay
	inline double			IndicationThreshold() { return this->indicationThreshold; }
	inline double			PvcShortSmoothFactor() { return this->pvcShortSmoothFactor; }
	inline double			PvcLongSmoothFactor() { return this->pvcLongSmoothFactor; }
	inline double			PvcTrendThreshold() { return this->pvcTrendThreshold; }

	inline CSimHandler*		Handler() { return this->handler; }
};

#endif