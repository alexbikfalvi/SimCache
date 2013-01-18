#ifndef __SimEvent_h__
#define __SimEvent_h__

#include "SimEvent.h"

class CSimEvent
{
public:
	CSimEvent() {};
	virtual ~CSimEvent() {};

	virtual void	Process() = 0;
};

#endif