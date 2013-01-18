#include "stdafx.h"
#include "Info.h"

CInfo::CInfo(
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
	)
{
	assert(modelNet);
	assert(modelWorkload);
	assert(modelCache);

	this->modelNet = modelNet;
	this->modelWorkload = modelWorkload;
	this->modelCache = modelCache;

	this->startTime = startTime;
	this->endTime = endTime;
	this->numSenders = numSenders;
	this->cacheSize = cacheSize;
	this->indicationThreshold = indicationThreshold;
	this->pvcShortSmoothFactor = pvcShortSmoothFactor;
	this->pvcLongSmoothFactor = pvcLongSmoothFactor;
	this->pvcTrendThreshold = pvcTrendThreshold;

	this->handler = NULL;
}

void CInfo::SetHandler(CSimHandler* handler)
{
	assert(handler);
	this->handler = handler;
}
