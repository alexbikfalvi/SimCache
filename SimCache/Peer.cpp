#include "stdafx.h"
#include "Peer.h"
#include "Console.h"
#include "Rand.h"
#include "Shuffle.h"

#include "EventStream.h"
#include "EventFinish.h"

#include "MessageBootRequest.h"
#include "MessageSegmentRequest.h"
#include "MessageSegmentResponse.h"
#include "MessageStreamRequest.h"
#include "MessageStreamResponse.h"

CPeer::CPeer(
	unsigned int	id,
	CInfo*			info,
	CCallback*		callback,
	CBoot*			boot,
	CServer*		server,
	CData*			data
	)
{
	assert(info);
	assert(callback);
	assert(boot);
	assert(server);
	assert(data);

	this->id = id;
	this->info = info;
	this->callback = callback;
	this->boot = boot;
	this->server = server;
	this->data = data;
	this->videomap = NULL;

	// State information
	this->state = STATE_IDLE;

	// Stream variables

//		// Senders: numSenders
//	this->senders = new CNode*[this->info->NumSenders()];
//	assert(this->senders);
//
//		// Senders info: numSenders
//	this->sendersInfo = new SSenderInfo[this->info->NumSenders()];
//	assert(this->sendersInfo);
//	
//		// Senders ID
//	this->sendersId = new int[this->info->NumSenders()];
//	assert(this->sendersId);
//
//	// Sender variables
//
//	this->recvRequests = 0;
//	this->recvRequestsSuccess = 0;
//	this->recvRequestsFailPeer = 0;
//	this->recvRequestsFailCache = 0;
//	
//	// Bandwidth variables
//	this->uplink = 0;
//	this->lastUplink = 0;
//	this->avgUplink = 0;
//
//	// Metadata variables
//#ifdef _META
//	this->metaSegTotal = 0;
//	this->metaSegAdd = 0;
//	this->metaSegReplace = 0;
//	this->metaServedRequests = 0;
//	this->metaRecvStreams = 0;
//	this->metaMaxUplink = 0;
//#endif
}

CPeer::~CPeer()
{
//	// Update average bandwidth
//	this->avgUplink += this->uplink * (this->info->Handler()->Time() - this->lastUplink);
//
//	this->avgUplink /= this->info->Handler()->Time();
//
//	// Record statistical info
//	this->data->RecordSender(
//		this->id,
//		this->recvRequests,
//		this->recvRequestsSuccess,
//		this->recvRequestsFailPeer,
//		this->recvRequestsFailCache,
//		this->avgUplink);
//
//#ifdef _META
//	// Record metadata info
//	this->data->RecordMetadata(
//		this->id,
//		this->metaSegTotal,
//		this->metaSegAdd,
//		this->metaSegReplace,
//		this->metaServedRequests,
//		this->metaRecvStreams,
//		this->metaMaxUplink,
//		this->avgUplink);
//#endif
//
//	delete[] this->senders;
//	delete[] this->sendersInfo;
//	delete[] this->sendersId;
//
//	if(this->videomap) delete this->videomap;
}

void CPeer::Arrive()
{
//	printf("%I64u peer %u ARRIVE\n", this->info->Handler()->Time(), this->id);
}

void CPeer::Leave()
{
//	printf("%I64u peer %u LEAVE\n", this->info->Handler()->Time(), this->id);
}

void CPeer::Session(unsigned int file, double duration, double start, double prefix)
{
//	// Connect to the a new video
//#ifdef _OUTPUT
//	CConsole::SetColor(CConsole::LIGHT_CYAN);
//	printf("\n%016I64u Peer %4u WATCH video %u (%u - %u)",
//		this->info->Handler()->Time(), this->id, watch.Video()->Id(), watch.Start(), watch.Duration());
//	CConsole::SetColor(CConsole::LIGHT_GRAY);
//#endif
//
//	// State check
//	if(STATE_IDLE != this->state)//throw "Invalid peer state";
//	{
//		if(112 == id) printf("%I64u peer %u BUSY video %u ptr %u first %u last %u : %u\n", this->info->Handler()->Time(), this->id,
//			this->video,
//			this->pointer,
//			this->firstSegment,
//			this->lastSegment,
//			this->numResponses);
//		return;
//	}
//
//	if(112 == id) printf("%I64u peer %u START\n", this->info->Handler()->Time(), this->id);
//
//	// Change the state
//	this->state = STATE_BOOT;
//
//	// Do some checks
//	assert(start + prefix <= duration);										// Check the duration
//
//	// Save session information
//	this->video = file;
//	this->start = start;
//	this->prefix = prefix;
//
//	this->firstSegment = this->info->Workload()->Segment(this->video, this->start);
//	this->lastSegment = this->info->Workload()->Segment(this->video, this->start+this->prefix);
//
//	assert(this->firstSegment <= this->lastSegment);
//
//	// Reset statistical info
//	this->segTotal = 0;
//	this->segServer = 0;
//	this->segPeerCache = 0;
//	this->segLocalCache = 0;
//	
//	this->numRejectCache = 0;
//	this->numRejectPeer = 0;
//
//	// Prepare the number of requested senders
//	this->numSenders = this->info->NumSenders();
//
//	// Clear the list of senders
//	for(unsigned int index = 0; index < this->info->NumSenders(); index++)
//		this->senders[index] = NULL;
//
//	// Create the bootstrap request
//	CMessageBootRequest* evt = new CMessageBootRequest(
//		this,
//		this->boot,
//		this->video,
//		&this->numSenders,
//		this->senders,
//		&this->numCopies
//		);
//	assert(evt);
//
//	// Send the request
//	this->info->Handler()->ScheduleEventAfter(
//		this->info->Net()->DelayPeerToBoot(this->info->Handler()->Time(), this->id, MESSAGE_BOOT_REQUEST),
//		evt);
//
//#ifdef _META
//	this->metaRecvStreams++;
//#endif
}

void CPeer::EventStream(unsigned int pointer)
{
	//// Check the state
	//if(STATE_STREAM != this->state) throw "Invalid peer state";

	//// Check the pointer
	//assert(this->pointer == pointer);

	//// Call stream request function
	//this->StreamRequest();
}

void CPeer::EventFinish(unsigned int file)
{
	//assert(this->uplink >= bandwidth);
	//
	//this->avgUplink += this->uplink * (this->info->Handler()->Time() - this->lastUplink); 
	//
	//this->uplink -= bandwidth;

	//this->lastUplink = this->info->Handler()->Time();
}

void CPeer::EventBootResponse(CNode* src, unsigned int video, unsigned int* numSenders, CNode** senders, double* numCopies)
{
	//// Check the state
	//if(STATE_BOOT != this->state) throw "Invalid peer state";

	//// Change the state
	//this->state = STATE_SEGMENT;

	//// Do some checks on the received data
	//assert(&this->numSenders == numSenders);
	//assert(this->senders == senders);
	//assert(&this->numCopies == numCopies);
	//assert(this->video == video);
	//assert(this->boot == src);
	//assert(this->numSenders <= this->info->NumSenders());

	//// Reset number of bitmap request / response
	//this->numRequests = 0;

	//// If the number of senders is greater than zero, generate segment requests
	//if(this->numSenders)
	//{
	//	this->numRequests = this->numSenders;
	//	this->numResponses = 0;

	//	// Randomize the senders
	//	CShuffle shuffle(this->numSenders);
	//	CNode*	aux;
	//	for(unsigned int index = 0; index < this->numSenders; index++)
	//	{
	//		aux = this->senders[shuffle[index]];
	//		this->senders[shuffle[index]] = this->senders[index];
	//		this->senders[index] = aux;
	//	}

	//	// Create the request bitmaps
	//	for(unsigned int index = 0; index < this->numSenders; index++)
	//	{
	//		assert(this->senders[index]);

	//		// Request all segments between start and end for this layer (the sender will confirm whether these segments are available)
	//		this->sendersInfo[index].bitmap = new CBitmap(
	//			this->info->Workload()->FileSizeSegments(this->video),
	//			this->firstSegment,
	//			this->lastSegment);
	//		assert(this->sendersInfo[index].bitmap);

	//		// Create the request
	//		CMessageSegmentRequest* evt = new CMessageSegmentRequest(
	//			this,
	//			this->senders[index],
	//			this->video,
	//			this->sendersInfo[index].bitmap,
	//			index
	//			);
	//		assert(evt);

	//		// Send the request
	//		this->info->Handler()->ScheduleEventAfter(
	//			this->info->Net()->DelayPeerToPeer(
	//			this->info->Handler()->Time(),
	//			this->id,
	//			this->senders[index]->Id(),
	//			MESSAGE_SEGMENT_REQUEST),
	//			evt);
	//	}
	//}
	//else
	//{
	//	// Go directly to streaming

	//	// Change the state
	//	this->state = STATE_STREAM;
	//	
	//	// Call segment request function
	//	this->Stream();
	//}
}

void CPeer::EventSegmentRequest(CNode* src, unsigned int video, CBitmap* bitmap, unsigned int sender)
{
	//// FUTURE TO DO: Check if the peer is turned off
	//
	//// Request for a set of segments

	//assert(src);
	//assert(bitmap);

	//// Get the bitmap response from the cache
	//// The function also returns a cache indication for the receiver (such as cache/not-cache)
	//CIndication indication = this->info->Cache()->SegmentRequest(
	//	this->info->Handler()->Time(),
	//	this->id,
	//	video,
	//	bitmap);

	//// FUTURE TO DO: Based on the request, the cache model may or may-not lock/mark the segments as requested by this receiver

	//// Determine if the video is related or not
	//bool relaying = 0;
	//if(STATE_STREAM == this->state) relaying = (this->video == video);

	//// Return the bitmap and the indication
	//CMessageSegmentResponse* evt = new CMessageSegmentResponse(
	//	this,
	//	src,
	//	video,
	//	bitmap,
	//	sender,
	//	indication,
	//	relaying);
	//assert(evt);

	//this->info->Handler()->ScheduleEventAfter(
	//	this->info->Net()->DelayPeerToPeer(
	//	this->info->Handler()->Time(),
	//	this->id,
	//	src->Id(),
	//	MESSAGE_SEGMENT_RESPONSE),
	//	evt);
}

void CPeer::EventSegmentResponse(CNode* src, unsigned int video, CBitmap* bitmap, unsigned int sender, CIndication indication, bool relaying)
{
	//// Check the state
	//if(STATE_SEGMENT != this->state) throw "Invalid peer state";

	//// Do some checks
	//assert(src);
	//assert(bitmap);
	//assert(sender < this->numSenders);
	//assert(this->senders[sender]);
	//assert(bitmap == this->sendersInfo[sender].bitmap);

	//// Increment the number of responses and do nothing until all answers have been received
	//this->numResponses++;

	//// Save the sender information
	//this->sendersInfo[sender].relay = relaying;
	//this->sendersInfo[sender].indication = indication;

	//// If the number of requests equals the number of responses, all segment requests have returned
	//// Can start stream the video at the playback rate
	//if(this->numRequests == this->numResponses)
	//{
	//	// Change the state to stream
	//	this->state = STATE_STREAM;

	//	// Call the stream function
	//	this->Stream();
	//}
}

void CPeer::EventStreamRequest(CNode* src, unsigned int video, unsigned int segment)
{
//	assert(src);
//
//	// Increase the number of requests
//	this->recvRequests++;
//
//	unsigned char result;
//	
//	// Check if there is bandwidth to serve the current request
//	if(this->uplink + this->info->Workload()->File(video).bitrate > this->info->Workload()->Peer(this->id).bandwidth)
//	{
//		// If the request arrived before the completion of the last request
//		result = RESULT_STREAM_FAIL_PEER;
//		this->recvRequestsFailPeer++;
//	}
//	else
//	{
//		// Check if the segment is in the cache (this will mark the segment as hit)
//		result = this->info->Cache()->Get(
//					this->info->Handler()->Time(),
//					this->id,
//					video,
//					segment);
//
//		if(CACHE_SUCCESS != result)
//		{
//			result = RESULT_STREAM_FAIL_CACHE;
//			this->recvRequestsFailCache++;
//		}
//	}
//
//	// If the peer will stream the segment, allocate bandwidth
//	if(CACHE_SUCCESS == result)
//	{
//		this->recvRequestsSuccess++;
//
//		// Calculate average utilized bandwidth
//		this->avgUplink += this->uplink * (this->info->Handler()->Time() - this->lastUplink); 
//	
//		this->lastUplink = this->info->Handler()->Time();
//
//		// Update the uplink bandwidth
//		this->uplink += this->info->Workload()->File(video).bitrate;
//
//#ifdef _META
//		this->metaMaxUplink = (this->metaMaxUplink < this->uplink)?this->uplink:this->metaMaxUplink;
//#endif
//
//		// Schedule to free the uplink bandwidth after the stream has finished
//		CEventFinish* evtDone = new CEventFinish(this, this->info->Workload()->File(video).bitrate);
//		assert(evtDone);
//
//		this->info->Handler()->ScheduleEventAfter(
//			this->info->Workload()->DefaultSegmentDuration(video),
//			evtDone);
//	}
//
//#ifdef _META
//	this->metaServedRequests++;
//#endif
//
//	// Return the response
//	CMessageStreamResponse* evt = new CMessageStreamResponse(
//		this,
//		src,
//		video,
//		segment,
//		result);
//	assert(evt);
//
//	this->info->Handler()->ScheduleEventAfter(
//		this->info->Net()->DelayPeerToPeer(
//		this->info->Handler()->Time(),
//		this->id,
//		src->Id(),
//		MESSAGE_STREAM_RESPONSE),
//		evt);
}

void CPeer::EventStreamResponse(CNode* src, unsigned int video, unsigned int segment, EStreamResponseResult result)
{
//	// Check the state
//	if(STATE_STREAM != this->state) throw "Invalid peer state";
//
//	assert(src);
//	assert(video == this->video);
//	assert(this->pointer <= this->info->Workload()->FileSizeSegments(this->video)+1);
//	assert(segment < this->pointer);
//
//	// Check the number of responses is less than the video duration (not possible to have finished yet)
//	// assert(this->numResponses < this->watch.Duration());
//
//	// Increment the number of responses
//	if(result == CACHE_SUCCESS)
//	{
//		// If the response is successful (the sender returned the segment)
//
//		// Increment the number of responses
//		this->numResponses++;
//
//		// Increment the statistical info
//		if(src == this->server) this->segServer++;
//		else this->segPeerCache++;
//
//		// Add the segment to the cache (according to the cache admission policy and previously set cache indication)
//		switch(this->info->Cache()->Add(
//			this->info->Handler()->Time(),
//			this->id,
//			this->video,
//			segment))
//		{
//		case CACHE_ADD_YES_REP_NO:
//#ifdef _META
//			this->metaSegAdd++;
//#endif
//			break;
//		case CACHE_ADD_YES_REP_YES:
//#ifdef _META
//			this->metaSegAdd++;
//			this->metaSegReplace++;
//#endif
//			break;
//		}
//#ifdef _META
//		this->metaSegTotal++;
//#endif
//	}
//	else
//	{
//		// If the response is unsuccessful (the sender no longer has the segment)
//
//		// Increment the statistical info
//		switch(result)
//		{
//		case RESULT_STREAM_FAIL_PEER: this->numRejectPeer++; break;
//		case RESULT_STREAM_FAIL_CACHE: this->numRejectCache++; break;
//		default: throw "Not supported here";
//		}
//
//		// Get the sender for the current segment
//		CNode* sender = this->videomap->GetSender(segment, this->lastSenderIndex);
//		if(NULL == sender) sender = this->server;
//
//		// Send a request directly to the next sender
//		CMessageStreamRequest* evt = new CMessageStreamRequest(
//			this,
//			sender,
//			this->video,
//			segment);
//		assert(evt);
//
//		// Calculate delay
//		__time delay;
//		if(sender == server)
//			delay = this->info->Net()->DelayPeerToServer(this->info->Handler()->Time(), this->id, MESSAGE_STREAM_REQUEST);
//		else
//			delay = this->info->Net()->DelayPeerToPeer(this->info->Handler()->Time(), this->id, sender->Id(), MESSAGE_STREAM_REQUEST);
//
//		this->info->Handler()->ScheduleEventAfter(delay, evt);
//	}
//
//	// If the number of responses coincides with the video duration
//	if(this->numResponses == this->lastSegment-this->firstSegment+1)
//	{
//		// Check the pointer is at the end
////		throw "To do";
////		assert(this->pointer == this->watch.Start() + this->watch.Duration());
//
//		// The video has finished
//		// Change the state to idle
//		this->state = STATE_IDLE;
//
//		// Call the finish function
//		this->Finish();
//	}
}

void CPeer::Stream()
{
	//// Check the state is stream
	//if(STATE_STREAM != this->state) throw "Invalid peer state";

	//// Create the videomap for the current video
	//assert(NULL == this->videomap);
	//this->videomap = new CVideomap(
	//	this->firstSegment,
	//	this->lastSegment,
	//	this->numSenders);
	//assert(this->videomap);

	//// Clear cache receiver state (such as cache indications)
	//this->info->Cache()->ClearReceiver(this->id);
	//
	//// Add the peers to the videomap and calculate indication

	//double indSum = 0;

	//for(unsigned int index = 0; index < this->numSenders; index++)
	//{
	//	assert(this->senders[index]);
	//	assert(this->sendersInfo[index].bitmap);

	//	// Add the bitmap to the videomap
	//	this->videomap->AddSender(this->sendersInfo[index].bitmap, this->senders[index], index);

	//	// Calculate indication
	//	indSum += this->sendersInfo[index].indication.Cache()?1:0;

	//	// Delete the bitmap
	//	delete this->sendersInfo[index].bitmap;
	//	this->sendersInfo[index].bitmap = NULL;
	//}

	//bool ind = this->numSenders?(indSum/this->numSenders >= this->info->IndicationThreshold()):1;

	//// Save the indication in the cache model
	//this->info->Cache()->SegmentResponse(
	//	this->id,
	//	this->video,
	//	ind);

	//// Set the pointer to start
	//this->pointer = this->firstSegment;

	//// Reset the number of requests / responses
	//this->numRequests = 0;
	//this->numResponses = 0;

	//// Call the stream request function
	//this->StreamRequest();
}

void CPeer::StreamRequest()
{
//	// Check the state is stream
//	if(STATE_STREAM != this->state) throw "Invalid peer state";
//
//	assert(this->videomap);
//	assert(this->pointer >= this->firstSegment);
//	assert(this->pointer <= this->lastSegment);
//
//	this->segTotal++;
//
//	this->lastSenderIndex = 0;
//
//	// Check if the peer has the segment in the local cache
//	if(CACHE_SUCCESS == this->info->Cache()->Has(this->id, this->video, this->pointer))
//	{
//		this->segLocalCache++;
//
//		// Advance the video pointer
//		this->pointer++;
//
//		// Increment the number of requests and number of response
//		this->numRequests++;
//		this->numResponses++;
//		
//		// If the pointer is not at the end of the video duration, schedule a stream event
//		if(this->pointer < this->lastSegment)
//		{
//			CEventStream* evts = new CEventStream(this, this->pointer);
//			assert(evts);
//
////			throw "To update delay";
//			this->info->Handler()->ScheduleEventAfter(
//				this->info->Workload()->DefaultSegmentDuration(this->video),			// The delay is the duration of one segment
//				evts);
//		}
//		else
//		// If the number of responses coincides with the video duration
////		throw "To do if";
//		if(this->numResponses == this->lastSegment-this->firstSegment+1)
//		{
//			// Check the pointer is at the end
////			throw "To do";
//			//assert(this->pointer == this->watch.Start() + this->watch.Duration());
//
//			// The video has finished
//			// Change the state to idle
//			this->state = STATE_IDLE;
//
//			// Call the finish function
//			this->Finish();
//		}
//	}
//	else
//	{
//		// Get the sender for the current segment
//		CNode* sender = this->videomap->GetSender(this->pointer, this->lastSenderIndex);
//		if(NULL == sender) sender = this->server;
//
//		// Send a request for the current pointer to the sender
//		CMessageStreamRequest* evt = new CMessageStreamRequest(
//			this,
//			sender,
//			this->video,
//			pointer);
//		assert(evt);
//
//		// Calculate delay
//		__time delay;
//		if(sender == server)
//			delay = this->info->Net()->DelayPeerToServer(this->info->Handler()->Time(), this->id, MESSAGE_STREAM_REQUEST);
//		else
//			delay = this->info->Net()->DelayPeerToPeer(this->info->Handler()->Time(), this->id, sender->Id(), MESSAGE_STREAM_REQUEST);
//
//		this->info->Handler()->ScheduleEventAfter(delay, evt);
//
//		// Advance the video pointer
//		this->pointer++;
//
//		// Increment the number of requests
//		this->numRequests++;
//
//		// If the pointer is not at the end of the video duration, schedule a stream event
//		if(this->pointer <= this->lastSegment)
//		{
//			CEventStream* evts = new CEventStream(this, this->pointer);
//			assert(evts);
//
////			throw "To update delay";
//			this->info->Handler()->ScheduleEventAfter(
//				this->info->Workload()->DefaultSegmentDuration(this->video),			// The delay is the duration of one segment
//				evts);
//		}
//	}
}

void CPeer::Finish()
{
//	// Check the state
//	if(STATE_IDLE != this->state) throw "Invalid peer state";
//
//	if(112 == id) printf("%I64u peer %u FINISH\n", this->info->Handler()->Time(), this->id);
//
//	// Record data
//	double quality = (double)100*(this->segLocalCache + this->segPeerCache) / this->segTotal;
//	bool indication = this->info->Cache()->Indication(this->id, this->video).Cache();
//
//	// Save senders ID
//	for(unsigned int index = 0; index < this->numSenders; index++)
//		this->sendersId[index] = this->senders[index]->Id();
//	for(unsigned int index = this->numSenders; index < this->info->NumSenders(); index++)
//		this->sendersId[index] = -1;
//
//#ifdef _OUTPUT
//	CConsole::SetColor(CConsole::LIGHT_RED);
//	printf("\n%016I64u Peer %4u FINISHED video %u (%u - %u) \t%u\t%u\t%u\t%u\t(%3.2lf %%)",
//		this->info->Handler()->Time(), this->id, watch.Video()->Id(), watch.Start(), watch.Duration(),
//		this->segTotal, this->segServer, this->segLocalCache, this->segPeerCache, quality);
//	CConsole::SetColor(CConsole::LIGHT_YELLOW);
//	printf("\t%u %u ",
//		this->numRejectCache,
//		this->numRejectPeer
//		);
//	CConsole::SetColor(CConsole::WHITE);
//	printf("%u %u %u %.02lf ",
//		indication,
//		this->info->Cache()->Size(this->id),
//		this->info->Cache()->MaxSize(this->id),
//		this->numCopies);
//	CConsole::SetColor(CConsole::LIGHT_GREEN);
//	printf("%u [ ", this->numSenders);
//	for(unsigned int index = 0; index < this->numSenders; index++)
//	{
//		assert(this->senders[index]);
//		printf("%u ", this->sendersId[index]);
//	}
//	printf("]");
//	CConsole::SetColor(CConsole::LIGHT_GRAY);
//#endif
//
////	throw "To update";
//	this->data->RecordReceiver(
//		this->info->Handler()->Time(),
//		this->id,
//		this->video,
//		this->info->Workload()->FileSizeSegments(this->video),
//		this->info->Workload()->File(this->video).freq,
//		this->firstSegment,
//		this->lastSegment - this->firstSegment + 1,
//		this->segTotal,
//		this->segServer,
//		this->segLocalCache,
//		this->segPeerCache,
//		quality,
//		this->numRejectCache,
//		this->numRejectPeer,
//		indication,
//		this->info->Cache()->Size(this->id),
//		this->info->Cache()->MaxSize(this->id),
//		this->numCopies,
//		this->numSenders,
//		this->info->NumSenders(),
//		this->sendersId
//		);
//
//	// Delete the videomap
//	assert(this->videomap);
//	delete this->videomap;
//	this->videomap = NULL;
//
//	assert(this->state == STATE_IDLE);

	// Call the callback
//	this->callback->Finished(this);
}
