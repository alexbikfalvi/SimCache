#include "stdafx.h"
#include "SimEventList.h"

CSimEventList::CSimEventList()
{
	this->events = new multimap<__time, CSimEvent*>();
	assert(this->events);
}

CSimEventList::CSimEventList(CSimModel* model)
{
	this->events = new multimap<__time, CSimEvent*>();
	assert(this->events);

	if(NULL == model) return;

	__time time;
	for(unsigned int index = 0; index < model->Events(); index++)
	{
		try {
			CSimEvent* evt = model->Event(index, time);
			this->Add(time, evt);
		} catch(...) { }
	}
}

CSimEventList::~CSimEventList()
{
	for(multimap<__time, CSimEvent*>::iterator iter = this->events->begin(); iter != this->events->end(); iter++)
		delete iter->second;
	delete this->events;
}

bool CSimEventList::HasEvents()
{
	return this->events->begin() != this->events->end();
}

CSimEvent* CSimEventList::GetEvent(__time &time)
{
	if(this->events->begin() == this->events->end()) return NULL;

	multimap<__time, CSimEvent*>::iterator iter = this->events->begin();
	CSimEvent* evt = iter->second;
	time = iter->first;
	this->events->erase(iter);
	return evt;
}

unsigned int CSimEventList::Size()
{
	return this->events->size();
}

void CSimEventList::Add(__time time, CSimEvent *evt)
{
	assert(evt);
	this->events->insert(pair<__time, CSimEvent*>(time, evt));
}

void CSimEventList::Cancel(__time time, CSimEvent* evt)
{
	assert(this->events->count(time));
	
	pair<multimap<__time, CSimEvent*>::iterator, multimap<__time, CSimEvent*>::iterator> bounds = 
		this->events->equal_range(time);

	for(multimap<__time, CSimEvent*>::iterator iter = bounds.first; iter != bounds.second; iter++)
		if(iter->second == evt)
		{
			this->events->erase(iter);
			return;
		}
}