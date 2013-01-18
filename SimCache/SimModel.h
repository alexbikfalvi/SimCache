#ifndef __SimModel_h__
#define __SimModel_h__

#include "SimEvent.h"
#include "SimHandler.h"

class CSimModel
{
public:
	virtual __time			MaxTime() = 0;
	virtual unsigned int	Events() = 0;
	virtual CSimEvent*		Event(unsigned int index, __time& time) = 0;
	virtual void			Run(__time time, CSimEvent* evt) = 0;
	virtual void			SetHandler(CSimHandler* handler) = 0;
};

#endif