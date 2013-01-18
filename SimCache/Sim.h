#ifndef __Sim_h__
#define __Sim_h__

#include "SimHandler.h"
#include "SimModel.h"
#include "SimEventList.h"

class CSim : public CSimHandler
{
private:
	CSimModel*		model;
	CSimEventList*	events;
	__time			time;

public:
	CSim(CSimModel* model);
	~CSim();

	unsigned int	Run(time_t& duration);

	__time			Time();
	void			ScheduleEventNow(CSimEvent* evt);
	void			ScheduleEventAt(__time time, CSimEvent* evt);
	void			ScheduleEventAfter(__time delay, CSimEvent* evt);
	void			CancelEvent(__time time, CSimEvent* evt);
};

#endif