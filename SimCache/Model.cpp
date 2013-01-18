#include "stdafx.h"
#include "Model.h"

#include "PeerSimple.h"
#include "EventWorkload.h"

CModel::CModel(
	CModelNet*		modelNet,
	CModelWorkload*	modelWorkload,
	CModelCache*	modelCache,
	CData*			data,
	__time			startTime,
	__time			endTime,
	unsigned int	numSenders,
	unsigned int	cacheSize,
	double			indicationThreshold,
	double			pvcShortSmoothFactor,
	double			pvcLongSmoothFactor,
	double			pvcTrendThreshold
	)
{
	// Data
	assert(data);
	this->data = data;

	// Create info
	this->info = new CInfo(
		modelNet,
		modelWorkload,
		modelCache,
		startTime,
		endTime,
		numSenders,
		cacheSize,
		indicationThreshold,
		pvcShortSmoothFactor,
		pvcLongSmoothFactor,
		pvcTrendThreshold
		);
	assert(this->info);

	// Create the peers (this list of peers should only be available to the model)
	// Once a peer becomes active, it should activate itself in the list of peers in the CInfo variable
	// Only that list of peers should be available to other peers, bootstrapper, cache, server etc
	this->peers = new CNode*[this->info->Workload()->NumPeers()];
	assert(this->peers);

	// Create the bootstrap node
	this->boot = new CBoot(
		this->info,
		this->peers
		);
	assert(this->boot);

	// Create the server node
	this->server = new CServer(
		this->info,
		this->data
		);
	assert(this->server);

	// Create the network
	this->network = new CNetwork(
		this->info
		);
	assert(this->network);

	for(unsigned int index = 0; index < this->info->Workload()->NumPeers(); index++)
	{
		this->peers[index] = new CPeerSimple(index, this->info, this, this->boot, this->server, this->network, this->data);
		assert(this->peers[index]);
	}
}

CModel::~CModel()
{
	for(unsigned int index = 0; index < this->info->Workload()->NumPeers(); index++)
		delete this->peers[index];
	delete[] this->peers;

	delete this->boot;
	delete this->server;
	delete this->network;

	delete this->info;
}

__time CModel::MaxTime()
{
	return this->info->EndTime();
}

unsigned int CModel::Events()
{
	// The number of initial events = the number of workload events
	return (this->info->Workload()->NumEvents() > 0)?1:0;
}

CSimEvent* CModel::Event(unsigned int index, __time& time)
{
	// The initial event from the workload
	assert(this->info->Workload()->NumEvents() > 0);

	time =(__time)(TIME_UNIT*this->info->Workload()->Event(0).time);
	return new CEventWorkload(this, 0);
}

void CModel::Run(__time time, CSimEvent* evt)
{
	// Process the event
	assert(evt);
	evt->Process();
}

void CModel::SetHandler(CSimHandler* handler)
{
	this->info->SetHandler(handler);
}

void CModel::Workload(unsigned int index)
{
	// Process workload event
	assert(index < this->info->Workload()->NumEvents());

	// Get the event
	SEvent evt = this->info->Workload()->Event(index);
	bool next = 0;
	do
	{
		// Verify the event time
		assert(this->info->Handler()->Time() == (__time)(TIME_UNIT*evt.time));

		// Get the peer
		CNode* peer = this->peers[evt.peer];
		assert(peer);

		// Execute the workload event
		switch(evt.op)
		{
		case ARRIVE:
			peer->Arrive();
			break;
		case LEAVE:
			peer->Leave();
			break;
		case SESSION:
			peer->Session(evt.file, evt.duration, evt.start, evt.prefix);
			break;
		default: assert(0);
		}

		// Increase the event index
		index++;
		// If there are events left
		if(index < this->info->Workload()->NumEvents())
		{
			// Get the next event
			evt = this->info->Workload()->Event(index);

			// If the event time is the current time
			if(this->info->Handler()->Time() >= (__time)(TIME_UNIT*evt.time))
				// Execute the event without putting it in the event list
				next = 1;
			else
			{
				// Put the event in the event list and exit the loop
				next = 0;
				
				CEventWorkload* newEvent = new CEventWorkload(this, index);
				assert(newEvent);

				this->info->Handler()->ScheduleEventAt((__time)(TIME_UNIT*evt.time), newEvent);
			}
		}
	}
	while(next);
}
