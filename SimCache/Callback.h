#ifndef __Callback_h__
#define __Callback_h__

#include "Node.h"

class CCallback
{
public:
	CCallback() {}
	virtual ~CCallback() {}

	virtual void	Workload(unsigned int index) = 0;
};

#endif