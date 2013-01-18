#include "stdafx.h"
#include "Videomap.h"

CVideomap::	CVideomap(
	unsigned int	begin,
	unsigned int	end,
	unsigned int	numSenders
	)
{
	assert(begin <= end);

	this->begin = begin;
	this->end = end;
	this->numSenders = numSenders;

	this->length = this->end - this->begin + 1;

	this->senders = new CNode*[this->length * this->numSenders];
	assert(this->senders);

	for(unsigned int index = 0; index < this->length * this->numSenders; index++) this->senders[index] = NULL;
}

CVideomap::~CVideomap()
{
	delete[] this->senders;
}

void CVideomap::AddSender(
	CBitmap*		bitmap,
	CNode*			sender,
	unsigned int	senderIndex
	)
{
	// Set the sender for all requested segments in the bitmap and marked by the sender as available
	// In addition, check the segments are only for the layer

	assert(bitmap);
	assert(sender);
	assert(senderIndex < this->numSenders);

	for(unsigned int index = this->begin; index <= this->end; index++)
		if(bitmap->HasConf(index))
		{

			assert(NULL == this->senders[(index - this->begin) * this->numSenders + senderIndex]);

			this->senders[(index - this->begin) * this->numSenders + senderIndex] = sender;
		}
}

//void CVideomap::AddServer(
//	CNode*			server
//	)
//{
//	// Add the server for all segments that don't have a sender
//	for(unsigned int index = this->begin; index < this->end; index++)
//		if(NULL == this->senders[index])
//			this->senders[index] = server;
//}

CNode* CVideomap::GetSender(
	unsigned int	pointer,
	unsigned int&	lastIndex
	)
{

	assert(pointer >= this->begin);
	assert(pointer <= this->end);

	for(unsigned int index = lastIndex; index < this->numSenders; index++)
	{
		if(this->senders[(pointer - this->begin) * this->numSenders + index])
		{
			lastIndex = index+1;
			return this->senders[(pointer - this->begin) * this->numSenders + index];
		}
	}
	lastIndex = this->numSenders;
	return NULL;
}
