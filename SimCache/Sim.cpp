#include "stdafx.h"
#include "Sim.h"
#include "Console.h"

CSim::CSim(CSimModel* model)
{
	assert(model);
	this->model = model;

	// Set the handler to the model
	this->model->SetHandler(this);

	// Create the event list
	this->events = new CSimEventList(this->model);
	assert(this->events);
}

CSim::~CSim()
{
	delete this->events;
}

unsigned int CSim::Run(time_t& duration)
{
	this->time = 0;
	
	unsigned int oldPercent = 1;
	unsigned int newPercent;
	__time div = 100000;
	time_t startTime;
	time_t currentTime;
	time_t endTime;

	::time(&startTime);

	printf("\nStart simulation (max time = %lf)\n", this->model->MaxTime());

	while((this->events->HasEvents()) && (time <= this->model->MaxTime()))
	{
		newPercent = (unsigned int)(100*time/this->model->MaxTime());
		if(newPercent != oldPercent)
		{
			::time(&currentTime);

			endTime = startTime + (time_t)(time?((currentTime-startTime)*this->model->MaxTime()/time):(currentTime-startTime));

#ifndef _OUTPUT
//			printf("=");
//			printf("%u percent complete (time is %I64u of %I64u, %I64u seconds passed, %I64u seconds remaining)\t\t", newPercent, time, this->model->MaxTime(), currentTime-startTime, endTime - currentTime);
//			CConsole::MoveCursorBeginLine();
//			printf("\n");
#else
			printf("\n=================================================================================================================");
			printf("\n%u percent complete (time is %I64u of %I64u, %I64u seconds passed, %I64u seconds remaining)", newPercent, time, this->model->MaxTime(), currentTime-startTime, endTime - currentTime);
			printf("\n=================================================================================================================");
#endif
			oldPercent = newPercent;
		}


		CSimEvent* evt = this->events->GetEvent(time);
		assert(evt);

		this->model->Run(time, evt);

		delete evt;
	}

	::time(&endTime);

	duration = endTime - startTime;

	return this->events->Size();
}

__time CSim::Time()
{
	return this->time;
}

void CSim::ScheduleEventNow(CSimEvent* evt)
{
	assert(evt);
	this->events->Add(this->time, evt);
}

void CSim::ScheduleEventAt(__time time, CSimEvent* evt)
{
	assert(evt);
	assert(time >= this->time);
	this->events->Add(time, evt);
}

void CSim::ScheduleEventAfter(__time delay, CSimEvent* evt)
{
	assert(evt);
	assert(this->time + delay >= this->time);
	this->events->Add(this->time + delay, evt);
}

void CSim::CancelEvent(__time time, CSimEvent* evt)
{
	assert(evt);
	if(time < this->time) return;	// If event is in the past, do nothing

	this->events->Cancel(time, evt);
	delete evt;
}