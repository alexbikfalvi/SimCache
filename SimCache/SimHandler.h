#ifndef __SimHandler_h__
#define __SimHandler_h__

#include "SimEvent.h"

class CSimHandler
{
public:
	virtual __time	Time() = 0;
	virtual void	ScheduleEventNow(CSimEvent* evt) = 0;
	virtual void	ScheduleEventAt(__time time, CSimEvent* evt) = 0;
	virtual void	ScheduleEventAfter(__time delay, CSimEvent* evt) = 0;
	virtual void	CancelEvent(__time time, CSimEvent* evt) = 0;
};

#endif