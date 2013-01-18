#include "stdafx.h"
#include "Boot.h"
#include "Shuffle.h"

#include "MessageBootResponse.h"

typedef map<CNode*, unsigned int> TBootTablePeers;

CBoot::CBoot(
	CInfo*		info,
	CNode**		peers
	)
{
	assert(info);
	assert(peers);

	this->info = info;
	this->peers = peers;
}

CBoot::~CBoot()
{
}

void CBoot::EventBootRequest(
	CNode*			src,
	unsigned int	video,
	unsigned int*	numSenders,
	CNode**			senders,
	double*			numCopies
	)
{
	// Bootstrap request for a video
	// Look in the peer table and return maximum {numBoostrap} neighbors for the requested thinning layers
	
	// Do some checks
	assert(src);
	assert(numSenders);
	assert(senders);
	assert(numCopies);
	assert(video < this->info->Workload()->NumFiles());

	// Check the number of requested neighbors is less that the allowed maximum
	assert((*numSenders) <= this->info->NumSenders());

	// Choose a random set of neighbors for that layer

	// First, enumarate from the cache all peers that have at least one segment of that layer
	TBootTablePeers peers;
	
	// Variables to calculate the number of copies for this layer
	double			sumCopies = 0;
	unsigned int	numSegs = 0;

	for(unsigned int seg = 0; seg < this->info->Workload()->FileSizeSegments(video); seg++)
	{
		TModelCacheVideo* mapp = this->info->Cache()->GetPeers(video, seg);

		sumCopies += mapp->size();
		numSegs++;

		for(TModelCacheVideo::iterator iter = mapp->begin(); iter != mapp->end(); iter++)
		{
			assert((*iter) < this->info->Workload()->NumPeers());

			peers.insert(pair<CNode*, unsigned int>(this->peers[*iter], *iter));
		}
	}

	*numCopies = (numSegs == 0)?0:(sumCopies/numSegs);

	if(peers.size() <= (*numSenders))
	{
		// If the number of stored peers is less than or equal to the requested number, return all peers
		unsigned int index = 0;
		for(TBootTablePeers::iterator iter = peers.begin(); iter != peers.end(); iter++, index++)
			senders[index] = iter->first;

		// Save the number of returned neighbors
		*numSenders = peers.size();
	}
	else
	{
		// If the number of stored peers is greater than the requested number, select a random subset
		CShuffle shuffle(peers.size());

		unsigned int idxi = 0;
		unsigned int idxo = 0;
		for(TBootTablePeers::iterator iter = peers.begin(); (iter != peers.end()) && (idxo < (*numSenders)); iter++, idxi++)
		{
			if(shuffle[idxi] < (*numSenders))
			{
				senders[idxo] = iter->first;
				idxo++;
			}
		}

		assert(idxo == (*numSenders));
	}

	// Create the response message event
	CMessageBootResponse* evt = new CMessageBootResponse(
		this,
		src,
		video,
		numSenders,
		senders,
		numCopies);
	assert(evt);

	this->info->Handler()->ScheduleEventAfter(
		this->info->Net()->DelayBootToPeer(this->info->Handler()->Time(), src->Id(), MESSAGE_BOOT_RESPONSE),
		evt);
}
