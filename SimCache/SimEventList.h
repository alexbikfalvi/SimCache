#ifndef __SimEventList_h__
#define __SimEventList_h__

#include "SimEvent.h"
#include "SimModel.h"

class CSimEventList
{
private:
	multimap<__time, CSimEvent*>*	events;

public:
	CSimEventList();
	CSimEventList(CSimModel* model);
	~CSimEventList();

	bool			HasEvents();
	CSimEvent*		GetEvent(__time &time);
	unsigned int	Size();

	void			Add(__time time, CSimEvent* evt);
	void			Cancel(__time time, CSimEvent* evt);
};

#endif