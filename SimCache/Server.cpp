#include "stdafx.h"
#include "Server.h"
#include "MessageStreamResponse.h"

CServer::CServer(
	CInfo*		info,
	CData*		data
	)
{
	assert(info);
	assert(data);

	this->info = info;
	this->data = data;
}

void CServer::EventStreamRequest(CNode* src, unsigned int video, unsigned int segment)
{
	// Stream request to the server

	assert(src);

	// Always reply success

	CMessageStreamResponse* evt = new CMessageStreamResponse(
		this,
		src,
		video,
		segment,
		STREAM_RESULT_SUCCESS);
	assert(evt);

	this->info->Handler()->ScheduleEventAfter(
		this->info->Net()->DelayServerToPeer(this->info->Handler()->Time(), src->Id(), MESSAGE_STREAM_RESPONSE),
		evt);

	// Record data
	//this->data->ServerStream(this->info->Handler()->Time(), src->Id(), video, segment);
}