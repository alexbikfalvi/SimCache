#ifndef __Node_h__
#define __Node_h__

#include "Bitmap.h"
#include "Indication.h"

enum EStreamResponseResult
{
	STREAM_RESULT_SUCCESS = 0,
	STREAM_RESULT_FAIL_INACTIVE = 1,
	STREAM_RESULT_FAIL_BANDWIDTH = 2,
	STREAM_RESULT_FAIL_CACHE = 3
};

class CNode
{
public:
	CNode() {}
	virtual ~CNode() {}

	virtual unsigned int	Id() = 0;
	virtual __bitrate		Bandwidth() = 0;

	virtual void			Arrive() {}
	virtual void			Leave() {}
	virtual void			Session(unsigned int file, double duration, double start, double prefix) {}

	virtual void			EventStream(unsigned int file, unsigned int segment) {}
	virtual void			EventPlay(unsigned int file, unsigned int segment) {}
	virtual void			EventFinish(unsigned int file) {}

	virtual void			EventBootRequest(CNode* src, unsigned int video, unsigned int* numSenders, CNode** senders, double* numCopies) {}
	virtual void			EventBootResponse(CNode* src, unsigned int video, unsigned int* numSenders, CNode** senders, double* numCopies) {}

	virtual void			EventSegmentRequest(CNode* src, unsigned int video, CBitmap* bitmap, unsigned int sender) {}
	virtual void			EventSegmentResponse(CNode* src, unsigned int video, CBitmap* bitmap, unsigned int sender, CIndication indication, bool relaying) {}

	virtual void			EventStreamRequest(CNode* src, unsigned int video, unsigned int segment) {}
	virtual void			EventStreamResponse(CNode* src, unsigned int video, unsigned int segment, EStreamResponseResult result) {}
};

#endif