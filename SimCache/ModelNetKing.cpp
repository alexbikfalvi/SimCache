#include "stdafx.h"
#include "ModelNetKing.h"
#include "Shuffle.h"

#pragma warning(disable : 4996)

unsigned int king[NUM_NODES][NUM_NODES];
double delay[NUM_NODES][NUM_NODES];

CModelNetKing::CModelNetKing(
	bool			rnd,
	char*			kingFile
	)
{
	// Load the kind data set
	FILE* in = fopen(kingFile, "rb");
	if(NULL == in)
	{
		printf("\nCannot open the KING delay file: king.dat");
		throw -1;
	}

	if(fread(king, sizeof(king), 1, in) != 1)
	{
		printf("\nCannot open the KING delay file: king.dat");
		throw -1;
	}

	fclose(in);

	this->nodeMap = new unsigned int[NUM_NODES];
	assert(this->nodeMap);

	for(unsigned int idx = 0; idx < NUM_NODES; idx++)
		this->nodeMap[idx] = idx;

	for(unsigned int idx1 = 0; idx1 < NUM_NODES; idx1++)
		for(unsigned int idx2 = 0; idx2 < NUM_NODES; idx2++)
			delay[idx1][idx2] = ((double)king[idx1][idx2])/2000000.0;

	if(rnd)
	{
		CShuffle shuffle(NUM_NODES);

		for(unsigned int idx = 0; idx < NUM_NODES; idx++)
			this->nodeMap[idx] = shuffle[idx];
	}
}

CModelNetKing::~CModelNetKing()
{
	delete[] this->nodeMap;
}

__time CModelNetKing::Delay(
	__time			time,
	unsigned int	src,
	unsigned int	dst,
	unsigned int	data
	)
{
	return delay[this->nodeMap[src % NUM_NODES]][this->nodeMap[dst % NUM_NODES]];
}

__time CModelNetKing::DelayPeerToPeer(
	__time			time,
	unsigned int	src,
	unsigned int	dst,
	unsigned int	data
	)
{
	return delay[this->nodeMap[src % NUM_PEERS]][this->nodeMap[dst % NUM_PEERS]];
}

__time CModelNetKing::DelayPeerToBoot(
	__time			time,
	unsigned int	src,
	unsigned int	data
	)
{
	assert(src < NUM_PEERS);
	return delay[this->nodeMap[src % NUM_PEERS]][this->nodeMap[INDEX_BOOT]];
}

__time CModelNetKing::DelayBootToPeer(
	__time			time,
	unsigned int	dst,
	unsigned int	data
	)
{
	assert(dst < NUM_PEERS);
	return delay[this->nodeMap[INDEX_BOOT]][this->nodeMap[dst % NUM_PEERS]];
}

__time CModelNetKing::DelayPeerToServer(
	__time			time,
	unsigned int	src,
	unsigned int	data
	)
{
	assert(src < NUM_PEERS);
	return delay[this->nodeMap[src % NUM_PEERS]][this->nodeMap[INDEX_SERVER]];
}

__time CModelNetKing::DelayServerToPeer(
	__time			time,
	unsigned int	dst,
	unsigned int	data
	)
{
	assert(dst < NUM_PEERS);
	return delay[this->nodeMap[INDEX_SERVER]][this->nodeMap[dst % NUM_PEERS]];
}
