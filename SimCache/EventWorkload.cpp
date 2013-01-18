#include "stdafx.h"
#include "EventWorkload.h"

CEventWorkload::CEventWorkload(
	CCallback*		callback,
	unsigned int	index
	)
{
	assert(callback);
	this->callback = callback;
	this->index = index;
}

void CEventWorkload::Process()
{
	this->callback->Workload(this->index);
}