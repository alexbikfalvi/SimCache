#ifndef __EventWorkload_h__
#define __EventWorkload_h__

#include "SimEvent.h"
#include "Callback.h"

class CEventWorkload : public CSimEvent
{
private:
	CCallback*		callback;
	unsigned int	index;

public:
	CEventWorkload(
		CCallback*		callback,
		unsigned int	index
		);
	virtual ~CEventWorkload() {}

	virtual void	Process();
};

#endif